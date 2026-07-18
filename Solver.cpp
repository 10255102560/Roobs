#include "Solver.h"
#include "MinHeap.h"
#include "CircularQueue.h"
#include <filesystem>

// ===== 定义全局变量（分配实际内存） =====
int n, m;
vector<vector<Edge>> graph;
vector<NodeInfo> nodes;
vector<Package> packages;
double speed, car_weight, capacity;
vector<double> dist;
vector<int> prev_node;

struct ShortestPathCache {
    vector<double> dist;
    vector<int> prev;
};
static unordered_map<int, ShortestPathCache> sp_cache;
static const vector<double>& getDistancesFromZero();
static vector<int> buildRoutePath(int start, const vector<int>& stops);

enum class DeliveryMode {
    T2,
    T3
};

enum class StrategyKind {
    Baseline,
    Improved
};

struct PlanStats {
    double total_unsat = 0;
    double total_cost = 0;
    int timeout_count = 0;
    double finish_time = 0;
    int trip_count = 0;
};

struct DualCarPlan {
    vector<Package> car1;
    vector<Package> car2;
    PlanStats stats1;
    PlanStats stats2;
    double total_cost = 0;
    int timeout_count = 0;
    double finish_time = 0;
};

// 本文件内用于缓存可视化输出的全局 JSON 字符串（在 exportVisualizationData 中填充）
static string vis_json_output;

// ===== 使用循环队列管理驿站待配送包裹 =====
// 模拟"包裹到站 → 入队等待 → 装车出发"的流水线
static vector<int> getAvailablePackages(const vector<Package>& pkgs,
                                         const vector<bool>& delivered,
                                         double current_time,
                                         DeliveryMode mode) {
    // 使用循环队列收集当前可派送的包裹索引
    CircularQueue<int> ready_queue(static_cast<int>(pkgs.size()) + 10);
    for (int i = 0; i < static_cast<int>(pkgs.size()); i++) {
        if (delivered[i]) continue;
        if (mode == DeliveryMode::T3) {
            // T3: 所有包裹到站时间为 0，始终可派送
            ready_queue.enqueue(i);
        } else {
            // T2: 只有在到站时间之后的包裹才可派送
            if (pkgs[i].arrive <= current_time + 1e-9) {
                ready_queue.enqueue(i);
            }
        }
    }

    vector<int> result;
    while (!ready_queue.empty()) {
        result.push_back(ready_queue.dequeue());
    }
    return result;
}

// 获取下一个包裹到站时间（T2 模式下用于时间跳跃）
static double getNextArriveTime(const vector<Package>& pkgs,
                                 const vector<bool>& delivered) {
    double next_time = 1e18;
    for (int i = 0; i < static_cast<int>(pkgs.size()); i++) {
        if (!delivered[i] && pkgs[i].arrive < next_time) {
            next_time = pkgs[i].arrive;
        }
    }
    return next_time;
}

static vector<int> selectTripPackages(
    const vector<Package>& pkgs,
    const vector<int>& candidates,
    double current_time,
    DeliveryMode mode,
    StrategyKind strategy,
    const vector<double>& dist_from_station
) {
    vector<int> ordered = candidates;
    auto priority = [&](int idx) {
        const Package& p = pkgs[idx];
        double station_dist = dist_from_station[p.dest] / max(1e-9, speed);
        if (mode == DeliveryMode::T2) {
            if (strategy == StrategyKind::Baseline) {
                // 基线：按到站时间排序，同时间按距离
                return make_pair(p.arrive, station_dist);
            }
            // 改进：综合考虑等待时间和距离
            double wait_time = max(0.0, current_time - p.arrive);
            return make_pair(station_dist - 0.35 * wait_time, p.arrive);
        }
        // T3 模式
        if (strategy == StrategyKind::Baseline) {
            // 基线：按截止时间排序
            return make_pair(p.deadline, station_dist);
        }
        // 改进：综合考虑截止时间和距离
        return make_pair(p.deadline - 0.35 * station_dist, p.arrive);
    };

    sort(ordered.begin(), ordered.end(), [&](int a, int b) {
        auto pa = priority(a);
        auto pb = priority(b);
        if (pa.first != pb.first) return pa.first < pb.first;
        if (pa.second != pb.second) return pa.second < pb.second;
        return pkgs[a].id < pkgs[b].id;
    });
    return ordered;
}

static vector<int> orderTripStops(
    const vector<Package>& pkgs,
    const vector<int>& trip_packages,
    StrategyKind strategy,
    const vector<double>& dist_from_station
) {
    vector<int> order;
    vector<bool> used(trip_packages.size(), false);
    int current_node = 0;

    while (order.size() < trip_packages.size()) {
        int best_pos = -1;
        double best_score = 1e18;
        for (size_t i = 0; i < trip_packages.size(); i++) {
            if (used[i]) continue;
            int idx = trip_packages[i];
            int dest = pkgs[idx].dest;
            double travel = dijkstra(current_node, dest).first;
            double score = travel;
            if (strategy == StrategyKind::Improved) {
                // 改进策略：偏向离驿站更远的目的地（先远后近）
                score -= 0.12 * dist_from_station[dest];
            }
            if (best_pos == -1 || score < best_score ||
                (abs(score - best_score) < 1e-9 && pkgs[idx].id < pkgs[trip_packages[best_pos]].id)) {
                best_score = score;
                best_pos = static_cast<int>(i);
            }
        }
        used[best_pos] = true;
        order.push_back(trip_packages[best_pos]);
        current_node = pkgs[trip_packages[best_pos]].dest;
    }
    return order;
}

static PlanStats simulateDeliveryPlan(
    const vector<Package>& pkgs,
    DeliveryMode mode,
    StrategyKind strategy,
    const string& title,
    bool print_plan
) {
    PlanStats stats;
    if (pkgs.empty()) {
        if (print_plan) cout << title << "\n  no packages" << endl;
        return stats;
    }

    vector<bool> delivered(pkgs.size(), false);
    double cur_time = 0;
    int delivered_count = 0;
    int trip_no = 1;
    const auto& dist_from_station = getDistancesFromZero();

    if (print_plan) cout << title << endl;

    while (delivered_count < static_cast<int>(pkgs.size())) {
        // 使用循环队列管理待配送包裹
        vector<int> candidates = getAvailablePackages(pkgs, delivered, cur_time, mode);

        if (candidates.empty()) {
            if (mode == DeliveryMode::T2) {
                // 跳到下一个包裹到站时间
                double next_arrive = getNextArriveTime(pkgs, delivered);
                cur_time = next_arrive;
                continue;
            }
            break;
        }

        vector<int> trip_candidates = selectTripPackages(pkgs, candidates, cur_time, mode, strategy, dist_from_station);

        // 在容量限制内装载包裹
        vector<int> trip_packages;
        double load = 0;
        for (int idx : trip_candidates) {
            if (load + pkgs[idx].weight <= capacity + 1e-9) {
                trip_packages.push_back(idx);
                load += pkgs[idx].weight;
            }
        }
        if (trip_packages.empty()) {
            trip_packages.push_back(trip_candidates.front());
            load = pkgs[trip_candidates.front()].weight;
        }
        if (load > capacity + 1e-9) {
            cerr << "Trip load exceeds capacity: " << load << " > " << capacity << endl;
            exit(1);
        }

        vector<int> visit_order = orderTripStops(pkgs, trip_packages, strategy, dist_from_station);

        int cur_node = 0;
        double trip_start = cur_time;
        double elapsed = 0;
        double current_load_on_car = load;
        vector<int> route_nodes;
        vector<double> arrival_times;
        vector<int> route_stops;

        for (int idx : visit_order) {
            int dest = pkgs[idx].dest;
            double seg_len = dijkstra(cur_node, dest).first;
            if (mode == DeliveryMode::T3) {
                stats.total_cost += seg_len * (car_weight + current_load_on_car);
            }
            elapsed += seg_len / speed;
            double deliver_time = trip_start + elapsed;
            if (mode == DeliveryMode::T2) {
                stats.total_unsat += (deliver_time - pkgs[idx].arrive);
            } else if (deliver_time > pkgs[idx].deadline + 1e-9) {
                stats.timeout_count++;
            }

            route_nodes.push_back(dest);
            arrival_times.push_back(deliver_time);
            route_stops.push_back(dest);
            current_load_on_car -= pkgs[idx].weight;
            cur_node = dest;
            delivered[idx] = true;
            delivered_count++;
        }

        double return_len = dijkstra(cur_node, 0).first;
        if (mode == DeliveryMode::T3) {
            stats.total_cost += return_len * car_weight;
        }
        double trip_end = trip_start + elapsed + (return_len / speed);
        cur_time = trip_end;
        stats.finish_time = trip_end;
        stats.trip_count++;

        if (print_plan) {
            cout << "  Trip " << trip_no++ << ": depart=" << trip_start << ", load=[";
            for (size_t i = 0; i < trip_packages.size(); i++) {
                if (i) cout << ", ";
                int idx = trip_packages[i];
                cout << pkgs[idx].id << "(w=" << pkgs[idx].weight;
                if (mode == DeliveryMode::T2) {
                    cout << ",S=" << pkgs[idx].arrive;
                } else {
                    cout << ",T=" << pkgs[idx].deadline;
                }
                cout << ",D=" << pkgs[idx].dest << ")";
            }
            route_stops.push_back(0);
            vector<int> full_route = buildRoutePath(0, route_stops);
            cout << "], visit=";
            for (size_t i = 0; i < route_nodes.size(); i++) {
                if (i) cout << " -> ";
                cout << route_nodes[i] << "@" << arrival_times[i];
            }
            cout << ", route=" << pathToString(full_route) << ", return@" << trip_end << endl;
        }
    }

    return stats;
}

static vector<Package> makeScenarioWaitPressure(const vector<Package>& source) {
    vector<Package> scenario = source;
    for (size_t i = 0; i < scenario.size(); i++) {
        scenario[i].arrive = static_cast<double>((i % 4) * 2);
        scenario[i].deadline = 1e9;
    }
    return scenario;
}

static vector<Package> makeScenarioDeadlinePressure(const vector<Package>& source) {
    vector<Package> scenario = source;
    const auto& dist_from_station = getDistancesFromZero();
    for (size_t i = 0; i < scenario.size(); i++) {
        scenario[i].arrive = 0;
        scenario[i].deadline = dist_from_station[scenario[i].dest] / max(1e-9, speed) + 6.0 + 0.4 * (i % 3);
    }
    return scenario;
}

static vector<int> normalizeStops(vector<int> stops) {
    vector<int> filtered;
    unordered_set<int> seen;
    for (int node : stops) {
        if (node <= 0 || node >= n) continue;
        if (seen.insert(node).second) filtered.push_back(node);
    }
    return filtered;
}

static DualCarPlan evaluateDualPlan(const vector<Package>& car1, const vector<Package>& car2) {
    DualCarPlan result;
    result.car1 = car1;
    result.car2 = car2;
    result.stats1 = simulateDeliveryPlan(result.car1, DeliveryMode::T3, StrategyKind::Improved, "", false);
    result.stats2 = simulateDeliveryPlan(result.car2, DeliveryMode::T3, StrategyKind::Improved, "", false);
    result.total_cost = result.stats1.total_cost + result.stats2.total_cost;
    result.timeout_count = result.stats1.timeout_count + result.stats2.timeout_count;
    result.finish_time = max(result.stats1.finish_time, result.stats2.finish_time);
    return result;
}

static void assignGreedyByScore(const vector<Package>& ordered, vector<Package>& car1, vector<Package>& car2, int score_mode) {
    car1.clear();
    car2.clear();
    double load1 = 0, load2 = 0;
    double score1 = 0, score2 = 0;
    const auto& dist_from_station = getDistancesFromZero();

    auto packageScore = [&](const Package& p) {
        switch (score_mode) {
        case 0: return p.deadline;
        case 1: return -dist_from_station[p.dest] * p.weight;
        case 2: return -p.weight;
        default: return p.deadline - dist_from_station[p.dest] / max(1e-9, speed);
        }
    };

    vector<Package> sorted = ordered;
    sort(sorted.begin(), sorted.end(), [&](const Package& a, const Package& b) {
        double sa = packageScore(a);
        double sb = packageScore(b);
        if (sa != sb) return sa < sb;
        return a.id < b.id;
    });

    for (const auto& p : sorted) {
        double delta = dist_from_station[p.dest] * p.weight;
        bool fit1 = load1 + p.weight <= capacity + 1e-9;
        bool fit2 = load2 + p.weight <= capacity + 1e-9;
        if (fit1 && (!fit2 || score1 <= score2)) {
            car1.push_back(p);
            load1 += p.weight;
            score1 += delta;
        } else if (fit2) {
            car2.push_back(p);
            load2 += p.weight;
            score2 += delta;
        } else {
            if (load1 <= load2) {
                car1.push_back(p);
                load1 += p.weight;
                score1 += delta;
            } else {
                car2.push_back(p);
                load2 += p.weight;
                score2 += delta;
            }
        }
    }
}

static DualCarPlan optimizeDualPlan(vector<Package> car1, vector<Package> car2) {
    DualCarPlan best = evaluateDualPlan(car1, car2);
    bool improved = true;
    int rounds = 0;

    while (improved && rounds++ < 12) {
        improved = false;
        // 尝试交换两车之间的包裹
        for (size_t i = 0; i < car1.size(); i++) {
            for (size_t j = 0; j < car2.size(); j++) {
                vector<Package> next1 = car1;
                vector<Package> next2 = car2;
                swap(next1[i], next2[j]);
                double w1 = 0, w2 = 0;
                for (const auto& p : next1) w1 += p.weight;
                for (const auto& p : next2) w2 += p.weight;
                if (w1 > capacity + 1e-9 || w2 > capacity + 1e-9) continue;
                DualCarPlan cand = evaluateDualPlan(next1, next2);
                if (cand.total_cost + 1e-9 < best.total_cost ||
                    (abs(cand.total_cost - best.total_cost) < 1e-9 && cand.timeout_count < best.timeout_count)) {
                    best = cand;
                    car1 = next1;
                    car2 = next2;
                    improved = true;
                }
            }
        }
        // 尝试从 car1 移动包裹到 car2
        for (size_t i = 0; i < car1.size() && !improved; i++) {
            vector<Package> next1;
            vector<Package> next2 = car2;
            for (size_t a = 0; a < car1.size(); a++) if (a != i) next1.push_back(car1[a]);
            next2.push_back(car1[i]);
            double w1 = 0, w2 = 0;
            for (const auto& p : next1) w1 += p.weight;
            for (const auto& p : next2) w2 += p.weight;
            if (w1 > capacity + 1e-9 || w2 > capacity + 1e-9) continue;
            DualCarPlan cand = evaluateDualPlan(next1, next2);
            if (cand.total_cost + 1e-9 < best.total_cost ||
                (abs(cand.total_cost - best.total_cost) < 1e-9 && cand.timeout_count < best.timeout_count)) {
                best = cand;
                car1 = next1;
                car2 = next2;
                improved = true;
                break;
            }
        }
        if (improved) continue;
        // 尝试从 car2 移动包裹到 car1
        for (size_t i = 0; i < car2.size(); i++) {
            vector<Package> next1 = car1;
            vector<Package> next2;
            for (size_t a = 0; a < car2.size(); a++) if (a != i) next2.push_back(car2[a]);
            next1.push_back(car2[i]);
            double w1 = 0, w2 = 0;
            for (const auto& p : next1) w1 += p.weight;
            for (const auto& p : next2) w2 += p.weight;
            if (w1 > capacity + 1e-9 || w2 > capacity + 1e-9) continue;
            DualCarPlan cand = evaluateDualPlan(next1, next2);
            if (cand.total_cost + 1e-9 < best.total_cost ||
                (abs(cand.total_cost - best.total_cost) < 1e-9 && cand.timeout_count < best.timeout_count)) {
                best = cand;
                car1 = next1;
                car2 = next2;
                improved = true;
                break;
            }
        }
    }

    return best;
}

static vector<int> buildRoutePath(int start, const vector<int>& stops) {
    vector<int> route;
    int current = start;
    route.push_back(current);
    for (int next_stop : stops) {
        auto segment = dijkstra(current, next_stop).second;
        for (size_t i = 1; i < segment.size(); i++) {
            route.push_back(segment[i]);
        }
        current = next_stop;
    }
    return route;
}

string pathToString(const vector<int>& path) {
    ostringstream out;
    for (size_t i = 0; i < path.size(); i++) {
        if (i) out << " -> ";
        out << path[i];
    }
    return out.str();
}

static const ShortestPathCache& getShortestPaths(int src) {
    auto it = sp_cache.find(src);
    if (it != sp_cache.end()) return it->second;

    ShortestPathCache cache;
    cache.dist.assign(n, 1e18);
    cache.prev.assign(n, -1);

    MinHeap heap;
    cache.dist[src] = 0;
    heap.push(0, src);

    while (!heap.empty()) {
        HeapNode cur = heap.pop();
        double d = cur.dist;
        int u = cur.node;
        if (d != cache.dist[u]) continue;
        for (auto &e : graph[u]) {
            int v = e.to;
            double new_d = d + e.w;
            if (new_d < cache.dist[v]) {
                cache.dist[v] = new_d;
                cache.prev[v] = u;
                heap.push(new_d, v);
            }
        }
    }

    auto [inserted_it, ok] = sp_cache.emplace(src, std::move(cache));
    return inserted_it->second;
}

static const vector<double>& getDistancesFromZero() {
    return getShortestPaths(0).dist;
}

void loadData(const std::filesystem::path& data_dir) {
    const auto base_dir = data_dir;

    ifstream fin1(base_dir / "map.txt");
    if (!fin1) { wcerr << L"无法打开 " << (base_dir / L"map.txt").wstring() << endl; exit(1); }
    fin1 >> n >> m;
    graph.assign(n, {});
    nodes.resize(n);

    for (int i = 0; i < n; i++) {
        NodeInfo node;
        fin1 >> node.id >> node.x >> node.y >> node.is_station;
        nodes[node.id] = node;
    }

    for (int i = 0; i < m; i++) {
        int u, v; double w;
        fin1 >> u >> v >> w;
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cerr << "错误: 边端点 " << u << " 或 " << v << " 超出范围 (0~" << n-1 << ")" << endl;
            exit(1);
        }
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }
    fin1.close();

    ifstream fin2(base_dir / "packages.txt");
    if (!fin2) { wcerr << L"无法打开 " << (base_dir / L"packages.txt").wstring() << endl; exit(1); }
    int k;
    fin2 >> k;
    packages.assign(k, {});
    for (int i = 0; i < k; i++) {
        Package p;
        fin2 >> p.id >> p.weight >> p.dest >> p.arrive >> p.deadline;
        packages[i] = p;
    }
    fin2.close();

    ifstream fin3(base_dir / "car.txt");
    if (!fin3) { wcerr << L"无法打开 " << (base_dir / L"car.txt").wstring() << endl; exit(1); }
    fin3 >> speed >> car_weight >> capacity;
    fin3.close();

    sp_cache.clear();
}

// ===== dijkstra 函数 =====
pair<double, vector<int>> dijkstra(int src, int dst) {
    const auto& sp = getShortestPaths(src);

    vector<int> path;
    int cur = dst;
    while (cur != -1) {
        path.push_back(cur);
        cur = sp.prev[cur];
    }
    reverse(path.begin(), path.end());
    return {sp.dist[dst], path};
}

// ===== T2：同时输出基线策略和改进策略 =====
void solve_T2() {
    cout << "===== T2: 最小化不满意度之和 =====" << endl;

    PlanStats stats_base = simulateDeliveryPlan(packages, DeliveryMode::T2, StrategyKind::Baseline, "T2 Baseline (按到站时间):", true);
    cout << endl;
    PlanStats stats_impr = simulateDeliveryPlan(packages, DeliveryMode::T2, StrategyKind::Improved, "T2 Improved (综合等待+距离):", true);

    cout << endl << "=== T2 结果对比 ===" << endl;
    cout << "  基线策略 不满意度之和: " << stats_base.total_unsat << ", 趟数: " << stats_base.trip_count << endl;
    cout << "  改进策略 不满意度之和: " << stats_impr.total_unsat << ", 趟数: " << stats_impr.trip_count << endl;
    cout << "  改进幅度: " << (stats_base.total_unsat - stats_impr.total_unsat) << " ("
         << (stats_base.total_unsat > 0 ? (stats_base.total_unsat - stats_impr.total_unsat) / stats_base.total_unsat * 100 : 0) << "%)" << endl;
}

// ===== T3：同时输出基线策略和改进策略 =====
void solve_T3() {
    cout << "===== T3: 带容量的运送成本 =====" << endl;

    PlanStats stats_base = simulateDeliveryPlan(packages, DeliveryMode::T3, StrategyKind::Baseline, "T3 Baseline (按截止时间):", true);
    cout << endl;
    PlanStats stats_impr = simulateDeliveryPlan(packages, DeliveryMode::T3, StrategyKind::Improved, "T3 Improved (综合截止时间+距离):", true);

    cout << endl << "=== T3 结果对比 ===" << endl;
    cout << "  基线策略 总成本: " << stats_base.total_cost << ", 超时数: " << stats_base.timeout_count << ", 趟数: " << stats_base.trip_count << endl;
    cout << "  改进策略 总成本: " << stats_impr.total_cost << ", 超时数: " << stats_impr.timeout_count << ", 趟数: " << stats_impr.trip_count << endl;
    cout << "  成本改进: " << (stats_base.total_cost - stats_impr.total_cost) << " ("
         << (stats_base.total_cost > 0 ? (stats_base.total_cost - stats_impr.total_cost) / stats_base.total_cost * 100 : 0) << "%)" << endl;
}

// ===== T4：退货回收 =====
void solve_T4(const vector<int>& return_stops_input) {
    vector<int> stops = normalizeStops(return_stops_input);
    if (stops.empty()) {
        for (const auto& p : packages) {
            if (p.dest != 0) stops.push_back(p.dest);
        }
        stops = normalizeStops(stops);
    }

    cout << "===== T4 退货回收 =====" << endl;
    if (stops.empty()) {
        cout << "  无退货地点" << endl;
        return;
    }

    cout << "  退货点 (" << stops.size() << " 个): " << pathToString(stops) << endl;

    vector<int> visit_order;
    double total_distance = 0;

    if (stops.size() <= 15) {
        // 状态压缩 DP 求精确最优
        vector<int> all_nodes;
        all_nodes.push_back(0);
        all_nodes.insert(all_nodes.end(), stops.begin(), stops.end());
        int k = static_cast<int>(stops.size());
        vector<vector<double>> dist_mat(k + 1, vector<double>(k + 1, 1e18));
        for (int i = 0; i <= k; i++) {
            for (int j = 0; j <= k; j++) {
                dist_mat[i][j] = dijkstra(all_nodes[i], all_nodes[j]).first;
            }
        }

        int full = 1 << k;
        vector<vector<double>> dp(full, vector<double>(k, 1e18));
        vector<vector<int>> parent(full, vector<int>(k, -1));
        for (int i = 0; i < k; i++) {
            dp[1 << i][i] = dist_mat[0][i + 1];
        }
        for (int mask = 1; mask < full; mask++) {
            for (int last = 0; last < k; last++) {
                if (!(mask & (1 << last))) continue;
                double cur = dp[mask][last];
                if (cur >= 1e18 / 2) continue;
                for (int nxt = 0; nxt < k; nxt++) {
                    if (mask & (1 << nxt)) continue;
                    int next_mask = mask | (1 << nxt);
                    double cand = cur + dist_mat[last + 1][nxt + 1];
                    if (cand < dp[next_mask][nxt]) {
                        dp[next_mask][nxt] = cand;
                        parent[next_mask][nxt] = last;
                    }
                }
            }
        }

        double best = 1e18;
        int best_last = -1;
        int all_mask = full - 1;
        for (int last = 0; last < k; last++) {
            double cand = dp[all_mask][last] + dist_mat[last + 1][0];
            if (cand < best) {
                best = cand;
                best_last = last;
            }
        }

        vector<int> order_index;
        int mask = all_mask;
        int cur = best_last;
        while (cur != -1) {
            order_index.push_back(cur);
            int prev = parent[mask][cur];
            mask ^= (1 << cur);
            cur = prev;
        }
        reverse(order_index.begin(), order_index.end());
        for (int idx : order_index) visit_order.push_back(stops[idx]);
        total_distance = best;
        cout << "  (使用状态压缩DP精确求解)" << endl;
    } else {
        // 最近邻启发式
        vector<bool> used(stops.size(), false);
        int cur = 0;
        while (visit_order.size() < stops.size()) {
            int best_pos = -1;
            double best_dist = 1e18;
            for (size_t i = 0; i < stops.size(); i++) {
                if (used[i]) continue;
                double d = dijkstra(cur, stops[i]).first;
                if (d < best_dist) {
                    best_dist = d;
                    best_pos = static_cast<int>(i);
                }
            }
            used[best_pos] = true;
            visit_order.push_back(stops[best_pos]);
            total_distance += best_dist;
            cur = stops[best_pos];
        }
        total_distance += dijkstra(cur, 0).first;
        cout << "  (使用最近邻启发式求解，点数=" << stops.size() << ")" << endl;
    }

    vector<int> route_stops = visit_order;
    route_stops.push_back(0);
    vector<int> route = buildRoutePath(0, route_stops);

    cout << "  访问顺序: " << pathToString(visit_order) << endl;
    cout << "  完整路径: " << pathToString(route) << endl;
    cout << "  总距离: " << total_distance << ", 总耗时: " << total_distance / speed << endl;
}

// ===== T5：双车协同 =====
void solve_T5() {
    cout << "===== T5 双车协同配送 =====" << endl;

    vector<DualCarPlan> candidates;
    for (int mode = 0; mode < 4; mode++) {
        vector<Package> car1, car2;
        assignGreedyByScore(packages, car1, car2, mode);
        candidates.push_back(optimizeDualPlan(car1, car2));
    }

    DualCarPlan best = candidates.front();
    for (const auto& cand : candidates) {
        if (cand.total_cost + 1e-9 < best.total_cost ||
            (abs(cand.total_cost - best.total_cost) < 1e-9 && cand.timeout_count < best.timeout_count)) {
            best = cand;
        }
    }

    cout << "  车辆1 包裹数: " << best.car1.size() << endl;
    cout << "  车辆2 包裹数: " << best.car2.size() << endl;
    cout << endl;

    cout << "  --- 车辆1 配送方案 ---" << endl;
    simulateDeliveryPlan(best.car1, DeliveryMode::T3, StrategyKind::Improved, "", true);
    cout << endl;

    cout << "  --- 车辆2 配送方案 ---" << endl;
    simulateDeliveryPlan(best.car2, DeliveryMode::T3, StrategyKind::Improved, "", true);

    cout << endl << "=== T5 结果汇总 ===" << endl;
    cout << "  双车总成本: " << best.total_cost << endl;
    cout << "  双车总超时数: " << best.timeout_count << endl;
    cout << "  完成时间 (max): " << best.finish_time << endl;
}

// ===== T6：策略对比实验 =====
void solve_T6() {
    cout << "===== T6 策略对比实验 =====" << endl;

    vector<pair<string, vector<Package>>> scenarios;
    scenarios.push_back({"原始数据", packages});
    scenarios.push_back({"等待压力场景 (错开到站时间)", makeScenarioWaitPressure(packages)});
    scenarios.push_back({"截止时间压力场景 (紧缩deadline)", makeScenarioDeadlinePressure(packages)});

    for (const auto& scenario : scenarios) {
        cout << "  --- " << scenario.first << " ---" << endl;

        PlanStats t2_base = simulateDeliveryPlan(scenario.second, DeliveryMode::T2, StrategyKind::Baseline, "", false);
        PlanStats t2_impr = simulateDeliveryPlan(scenario.second, DeliveryMode::T2, StrategyKind::Improved, "", false);
        PlanStats t3_base = simulateDeliveryPlan(scenario.second, DeliveryMode::T3, StrategyKind::Baseline, "", false);
        PlanStats t3_impr = simulateDeliveryPlan(scenario.second, DeliveryMode::T3, StrategyKind::Improved, "", false);

        cout << "    T2 不满意度 : 基线=" << t2_base.total_unsat << ", 改进=" << t2_impr.total_unsat
             << " (Δ=" << (t2_base.total_unsat - t2_impr.total_unsat) << ")" << endl;
        cout << "    T3 总成本   : 基线=" << t3_base.total_cost << ", 改进=" << t3_impr.total_cost
             << " (Δ=" << (t3_base.total_cost - t3_impr.total_cost) << ")" << endl;
        cout << "    T3 超时数   : 基线=" << t3_base.timeout_count << ", 改进=" << t3_impr.timeout_count << endl;
    }
}

// ===== JSON 转义辅助函数 =====
static string jsonEscape(const string& s) {
    string result;
    for (char c : s) {
        if (c == '"') result += "\\\"";
        else if (c == '\\') result += "\\\\";
        else if (c == '\n') result += "\\n";
        else result += c;
    }
    return result;
}

// ===== 收集一趟配送的详细信息（用于可视化） =====
struct TripDetail {
    vector<int> package_ids;
    vector<int> route_nodes;
    vector<double> arrival_times;
    vector<int> full_route;
    double depart_time;
    double return_time;
    double total_load;
};

static TripDetail collectOneTrip(
    const vector<Package>& pkgs,
    const vector<int>& trip_packages,
    const vector<int>& visit_order,
    const vector<bool>& delivered_before,
    double trip_start,
    double load
) {
    TripDetail detail;
    detail.depart_time = trip_start;
    detail.total_load = load;
    double elapsed = 0;
    int cur_node = 0;
    double current_load_on_car = load;
    vector<int> route_stops;

    for (int idx : visit_order) {
        int dest = pkgs[idx].dest;
        double seg_len = dijkstra(cur_node, dest).first;
        elapsed += seg_len / speed;
        double deliver_time = trip_start + elapsed;
        detail.package_ids.push_back(pkgs[idx].id);
        detail.route_nodes.push_back(dest);
        detail.arrival_times.push_back(deliver_time);
        route_stops.push_back(dest);
        current_load_on_car -= pkgs[idx].weight;
        cur_node = dest;
    }

    double return_len = dijkstra(cur_node, 0).first;
    detail.return_time = trip_start + elapsed + (return_len / speed);
    route_stops.push_back(0);
    detail.full_route = buildRoutePath(0, route_stops);

    return detail;
}

// ===== 为可视化收集一趟行程 =====
static vector<TripDetail> collectTrips(
    const vector<Package>& pkgs,
    DeliveryMode mode,
    StrategyKind strategy
) {
    vector<TripDetail> trips;
    if (pkgs.empty()) return trips;

    vector<bool> delivered(pkgs.size(), false);
    double cur_time = 0;
    int delivered_count = 0;
    const auto& dist_from_station = getDistancesFromZero();

    while (delivered_count < static_cast<int>(pkgs.size())) {
        vector<int> candidates = getAvailablePackages(pkgs, delivered, cur_time, mode);
        if (candidates.empty()) {
            if (mode == DeliveryMode::T2) {
                cur_time = getNextArriveTime(pkgs, delivered);
                continue;
            }
            break;
        }

        vector<int> trip_candidates = selectTripPackages(pkgs, candidates, cur_time, mode, strategy, dist_from_station);
        vector<int> trip_packages;
        double load = 0;
        for (int idx : trip_candidates) {
            if (load + pkgs[idx].weight <= capacity + 1e-9) {
                trip_packages.push_back(idx);
                load += pkgs[idx].weight;
            }
        }
        if (trip_packages.empty()) {
            trip_packages.push_back(trip_candidates.front());
            load = pkgs[trip_candidates.front()].weight;
        }

        vector<int> visit_order = orderTripStops(pkgs, trip_packages, strategy, dist_from_station);
        TripDetail detail = collectOneTrip(pkgs, trip_packages, visit_order, delivered, cur_time, load);

        // 标记已送达
        for (int idx : visit_order) {
            delivered[idx] = true;
            delivered_count++;
        }

        cur_time = detail.return_time;
        trips.push_back(detail);
    }

    return trips;
}

// ===== 导出可视化数据为 JSON =====
void exportVisualizationData(const std::filesystem::path& data_dir,
                              const vector<int>& return_stops_input) {
    const auto base_dir = data_dir;
    std::filesystem::path out_path = base_dir / "visualization_data.js";

    ofstream fout(out_path);
    if (!fout) {
        cerr << "Warning: cannot write visualization data to " << out_path.string() << endl;
        return;
    }

    // 写入 JS 变量（方便 HTML 直接用 <script> 加载，避免 CORS 限制）
    fout << "// 菜鸟驿站配送系统 - 可视化数据" << endl;
    fout << "// 由程序自动生成，供 visualization.html 加载" << endl << endl;
    fout << "const VIS_DATA = ";
    fout.precision(12);

    // === JSON 开始 ===
    ostringstream json;
    json.precision(12);
    json << "{" << endl;

    // 地图数据
    json << "  \"map\": {" << endl;
    json << "    \"nodes\": [" << endl;
    for (int i = 0; i < n; i++) {
        json << "      { \"id\": " << nodes[i].id
             << ", \"x\": " << nodes[i].x
             << ", \"y\": " << nodes[i].y
             << ", \"is_station\": " << nodes[i].is_station << " }";
        if (i < n - 1) json << ",";
        json << endl;
    }
    json << "    ]," << endl;
    json << "    \"edges\": [" << endl;
    int edge_count = 0;
    for (int u = 0; u < n; u++) {
        for (const auto& e : graph[u]) {
            if (u < e.to) {  // 无向边只输出一次
                if (edge_count > 0) json << "," << endl;
                json << "      { \"u\": " << u << ", \"v\": " << e.to << ", \"w\": " << e.w << " }";
                edge_count++;
            }
        }
    }
    json << endl << "    ]" << endl;
    json << "  }," << endl;

    // 小车参数
    json << "  \"car\": { \"speed\": " << speed
         << ", \"car_weight\": " << car_weight
         << ", \"capacity\": " << capacity << " }," << endl;

    // 包裹列表
    json << "  \"packages\": [" << endl;
    for (size_t i = 0; i < packages.size(); i++) {
        json << "    { \"id\": " << packages[i].id
             << ", \"weight\": " << packages[i].weight
             << ", \"dest\": " << packages[i].dest
             << ", \"arrive\": " << packages[i].arrive
             << ", \"deadline\": " << packages[i].deadline << " }";
        if (i < packages.size() - 1) json << ",";
        json << endl;
    }
    json << "  ]," << endl;

    // === T1 最短路 ===
    json << "  \"t1\": {" << endl;
    json << "    \"from_station\": [" << endl;
    for (int i = 0; i < n; i++) {
        auto res = dijkstra(0, i);
        json << "      { \"dest\": " << i
             << ", \"distance\": " << res.first
             << ", \"path\": \"" << jsonEscape(pathToString(res.second)) << "\" }";
        if (i < n - 1) json << ",";
        json << endl;
    }
    json << "    ]" << endl;
    json << "  }," << endl;

    // === T2 不满意度 ===
    {
        PlanStats s_base = simulateDeliveryPlan(packages, DeliveryMode::T2, StrategyKind::Baseline, "", false);
        PlanStats s_impr = simulateDeliveryPlan(packages, DeliveryMode::T2, StrategyKind::Improved, "", false);
        auto trips_base = collectTrips(packages, DeliveryMode::T2, StrategyKind::Baseline);
        auto trips_impr = collectTrips(packages, DeliveryMode::T2, StrategyKind::Improved);

        json << "  \"t2\": {" << endl;
        json << "    \"baseline_unsat\": " << s_base.total_unsat << "," << endl;
        json << "    \"baseline_trips\": " << s_base.trip_count << "," << endl;
        json << "    \"improved_unsat\": " << s_impr.total_unsat << "," << endl;
        json << "    \"improved_trips\": " << s_impr.trip_count << "," << endl;

        // 基线方案详情
        json << "    \"baseline_trip_details\": [" << endl;
        for (size_t ti = 0; ti < trips_base.size(); ti++) {
            const auto& t = trips_base[ti];
            json << "      { \"trip_id\": " << (ti + 1)
                 << ", \"depart\": " << t.depart_time
                 << ", \"return\": " << t.return_time
                 << ", \"load\": " << t.total_load
                 << ", \"package_ids\": [";
            for (size_t j = 0; j < t.package_ids.size(); j++) {
                if (j) json << ", ";
                json << t.package_ids[j];
            }
            json << "], \"route_nodes\": [";
            for (size_t j = 0; j < t.route_nodes.size(); j++) {
                if (j) json << ", ";
                json << t.route_nodes[j];
            }
            json << "], \"arrival_times\": [";
            for (size_t j = 0; j < t.arrival_times.size(); j++) {
                if (j) json << ", ";
                json << t.arrival_times[j];
            }
            json << "], \"full_route\": [";
            for (size_t j = 0; j < t.full_route.size(); j++) {
                if (j) json << ", ";
                json << t.full_route[j];
            }
            json << "] }";
            if (ti < trips_base.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]," << endl;

        // 改进方案详情
        json << "    \"improved_trip_details\": [" << endl;
        for (size_t ti = 0; ti < trips_impr.size(); ti++) {
            const auto& t = trips_impr[ti];
            json << "      { \"trip_id\": " << (ti + 1)
                 << ", \"depart\": " << t.depart_time
                 << ", \"return\": " << t.return_time
                 << ", \"load\": " << t.total_load
                 << ", \"package_ids\": [";
            for (size_t j = 0; j < t.package_ids.size(); j++) {
                if (j) json << ", ";
                json << t.package_ids[j];
            }
            json << "], \"route_nodes\": [";
            for (size_t j = 0; j < t.route_nodes.size(); j++) {
                if (j) json << ", ";
                json << t.route_nodes[j];
            }
            json << "], \"arrival_times\": [";
            for (size_t j = 0; j < t.arrival_times.size(); j++) {
                if (j) json << ", ";
                json << t.arrival_times[j];
            }
            json << "], \"full_route\": [";
            for (size_t j = 0; j < t.full_route.size(); j++) {
                if (j) json << ", ";
                json << t.full_route[j];
            }
            json << "] }";
            if (ti < trips_impr.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]" << endl;
        json << "  }," << endl;
    }

    // === T3 运送成本 ===
    {
        PlanStats s_base = simulateDeliveryPlan(packages, DeliveryMode::T3, StrategyKind::Baseline, "", false);
        PlanStats s_impr = simulateDeliveryPlan(packages, DeliveryMode::T3, StrategyKind::Improved, "", false);
        auto trips_base = collectTrips(packages, DeliveryMode::T3, StrategyKind::Baseline);
        auto trips_impr = collectTrips(packages, DeliveryMode::T3, StrategyKind::Improved);

        json << "  \"t3\": {" << endl;
        json << "    \"baseline_cost\": " << s_base.total_cost << "," << endl;
        json << "    \"baseline_timeout\": " << s_base.timeout_count << "," << endl;
        json << "    \"baseline_trips\": " << s_base.trip_count << "," << endl;
        json << "    \"improved_cost\": " << s_impr.total_cost << "," << endl;
        json << "    \"improved_timeout\": " << s_impr.timeout_count << "," << endl;
        json << "    \"improved_trips\": " << s_impr.trip_count << "," << endl;

        json << "    \"baseline_trip_details\": [" << endl;
        for (size_t ti = 0; ti < trips_base.size(); ti++) {
            const auto& t = trips_base[ti];
            json << "      { \"trip_id\": " << (ti + 1)
                 << ", \"depart\": " << t.depart_time
                 << ", \"return\": " << t.return_time
                 << ", \"load\": " << t.total_load
                 << ", \"package_ids\": [";
            for (size_t j = 0; j < t.package_ids.size(); j++) {
                if (j) json << ", ";
                json << t.package_ids[j];
            }
            json << "], \"route_nodes\": [";
            for (size_t j = 0; j < t.route_nodes.size(); j++) {
                if (j) json << ", ";
                json << t.route_nodes[j];
            }
            json << "], \"arrival_times\": [";
            for (size_t j = 0; j < t.arrival_times.size(); j++) {
                if (j) json << ", ";
                json << t.arrival_times[j];
            }
            json << "], \"full_route\": [";
            for (size_t j = 0; j < t.full_route.size(); j++) {
                if (j) json << ", ";
                json << t.full_route[j];
            }
            json << "] }";
            if (ti < trips_base.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]," << endl;

        json << "    \"improved_trip_details\": [" << endl;
        for (size_t ti = 0; ti < trips_impr.size(); ti++) {
            const auto& t = trips_impr[ti];
            json << "      { \"trip_id\": " << (ti + 1)
                 << ", \"depart\": " << t.depart_time
                 << ", \"return\": " << t.return_time
                 << ", \"load\": " << t.total_load
                 << ", \"package_ids\": [";
            for (size_t j = 0; j < t.package_ids.size(); j++) {
                if (j) json << ", ";
                json << t.package_ids[j];
            }
            json << "], \"route_nodes\": [";
            for (size_t j = 0; j < t.route_nodes.size(); j++) {
                if (j) json << ", ";
                json << t.route_nodes[j];
            }
            json << "], \"arrival_times\": [";
            for (size_t j = 0; j < t.arrival_times.size(); j++) {
                if (j) json << ", ";
                json << t.arrival_times[j];
            }
            json << "], \"full_route\": [";
            for (size_t j = 0; j < t.full_route.size(); j++) {
                if (j) json << ", ";
                json << t.full_route[j];
            }
            json << "] }";
            if (ti < trips_impr.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]" << endl;
        json << "  }," << endl;
    }

    // === T4 退货回收 ===
    {
        vector<int> stops = normalizeStops(return_stops_input);
        if (stops.empty()) {
            for (const auto& p : packages) {
                if (p.dest != 0) stops.push_back(p.dest);
            }
            stops = normalizeStops(stops);
        }

        json << "  \"t4\": {" << endl;
        json << "    \"return_stops\": [";
        for (size_t i = 0; i < stops.size(); i++) {
            if (i) json << ", ";
            json << stops[i];
        }
        json << "]," << endl;

        if (!stops.empty()) {
            // 计算 T4 路线
            vector<int> visit_order;
            double total_distance = 0;
            if (stops.size() <= 15) {
                vector<int> all_nodes;
                all_nodes.push_back(0);
                all_nodes.insert(all_nodes.end(), stops.begin(), stops.end());
                int k = static_cast<int>(stops.size());
                vector<vector<double>> dist_mat(k + 1, vector<double>(k + 1, 1e18));
                for (int i = 0; i <= k; i++)
                    for (int j = 0; j <= k; j++)
                        dist_mat[i][j] = dijkstra(all_nodes[i], all_nodes[j]).first;

                int full = 1 << k;
                vector<vector<double>> dp(full, vector<double>(k, 1e18));
                vector<vector<int>> parent(full, vector<int>(k, -1));
                for (int i = 0; i < k; i++) dp[1 << i][i] = dist_mat[0][i + 1];
                for (int mask = 1; mask < full; mask++) {
                    for (int last = 0; last < k; last++) {
                        if (!(mask & (1 << last))) continue;
                        if (dp[mask][last] >= 1e18 / 2) continue;
                        for (int nxt = 0; nxt < k; nxt++) {
                            if (mask & (1 << nxt)) continue;
                            int nm = mask | (1 << nxt);
                            double cand = dp[mask][last] + dist_mat[last + 1][nxt + 1];
                            if (cand < dp[nm][nxt]) { dp[nm][nxt] = cand; parent[nm][nxt] = last; }
                        }
                    }
                }
                double best = 1e18;
                int best_last = -1;
                int all_mask = full - 1;
                for (int last = 0; last < k; last++) {
                    double cand = dp[all_mask][last] + dist_mat[last + 1][0];
                    if (cand < best) { best = cand; best_last = last; }
                }
                vector<int> oi;
                int mask = all_mask, cur = best_last;
                while (cur != -1) { oi.push_back(cur); int p = parent[mask][cur]; mask ^= (1 << cur); cur = p; }
                reverse(oi.begin(), oi.end());
                for (int idx : oi) visit_order.push_back(stops[idx]);
                total_distance = best;
            } else {
                vector<bool> used(stops.size(), false);
                int cur = 0;
                while (visit_order.size() < stops.size()) {
                    int bp = -1; double bd = 1e18;
                    for (size_t i = 0; i < stops.size(); i++) {
                        if (used[i]) continue;
                        double d = dijkstra(cur, stops[i]).first;
                        if (d < bd) { bd = d; bp = static_cast<int>(i); }
                    }
                    used[bp] = true; visit_order.push_back(stops[bp]); total_distance += bd; cur = stops[bp];
                }
                total_distance += dijkstra(cur, 0).first;
            }

            json << "    \"visit_order\": [";
            for (size_t i = 0; i < visit_order.size(); i++) {
                if (i) json << ", ";
                json << visit_order[i];
            }
            json << "]," << endl;
            json << "    \"total_distance\": " << total_distance << "," << endl;
            json << "    \"total_time\": " << (total_distance / speed) << "," << endl;
            vector<int> rs = visit_order; rs.push_back(0);
            json << "    \"full_route\": [";
            vector<int> fr = buildRoutePath(0, rs);
            for (size_t i = 0; i < fr.size(); i++) {
                if (i) json << ", ";
                json << fr[i];
            }
            json << "]" << endl;
        } else {
            json << "    \"visit_order\": []," << endl;
            json << "    \"total_distance\": 0," << endl;
            json << "    \"total_time\": 0," << endl;
            json << "    \"full_route\": []" << endl;
        }
        json << "  }," << endl;
    }

    // === T5 双车协同 ===
    {
        vector<DualCarPlan> candidates;
        for (int mode = 0; mode < 4; mode++) {
            vector<Package> car1, car2;
            assignGreedyByScore(packages, car1, car2, mode);
            candidates.push_back(optimizeDualPlan(car1, car2));
        }
        DualCarPlan best = candidates.front();
        for (const auto& cand : candidates) {
            if (cand.total_cost + 1e-9 < best.total_cost ||
                (abs(cand.total_cost - best.total_cost) < 1e-9 && cand.timeout_count < best.timeout_count)) {
                best = cand;
            }
        }

        json << "  \"t5\": {" << endl;
        json << "    \"total_cost\": " << best.total_cost << "," << endl;
        json << "    \"timeout_count\": " << best.timeout_count << "," << endl;
        json << "    \"finish_time\": " << best.finish_time << "," << endl;
        json << "    \"car1_package_count\": " << best.car1.size() << "," << endl;
        json << "    \"car2_package_count\": " << best.car2.size() << "," << endl;

        // Car1 行程
        auto trips1 = collectTrips(best.car1, DeliveryMode::T3, StrategyKind::Improved);
        json << "    \"car1_trips\": [" << endl;
        for (size_t ti = 0; ti < trips1.size(); ti++) {
            const auto& t = trips1[ti];
            json << "      { \"trip_id\": " << (ti + 1)
                 << ", \"depart\": " << t.depart_time
                 << ", \"return\": " << t.return_time
                 << ", \"package_ids\": [";
            for (size_t j = 0; j < t.package_ids.size(); j++) {
                if (j) json << ", ";
                json << t.package_ids[j];
            }
            json << "], \"full_route\": [";
            for (size_t j = 0; j < t.full_route.size(); j++) {
                if (j) json << ", ";
                json << t.full_route[j];
            }
            json << "] }";
            if (ti < trips1.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]," << endl;

        // Car2 行程
        auto trips2 = collectTrips(best.car2, DeliveryMode::T3, StrategyKind::Improved);
        json << "    \"car2_trips\": [" << endl;
        for (size_t ti = 0; ti < trips2.size(); ti++) {
            const auto& t = trips2[ti];
            json << "      { \"trip_id\": " << (ti + 1)
                 << ", \"depart\": " << t.depart_time
                 << ", \"return\": " << t.return_time
                 << ", \"package_ids\": [";
            for (size_t j = 0; j < t.package_ids.size(); j++) {
                if (j) json << ", ";
                json << t.package_ids[j];
            }
            json << "], \"full_route\": [";
            for (size_t j = 0; j < t.full_route.size(); j++) {
                if (j) json << ", ";
                json << t.full_route[j];
            }
            json << "] }";
            if (ti < trips2.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]" << endl;
        json << "  }," << endl;
    }

    // === T6 策略对比 ===
    {
        json << "  \"t6\": {" << endl;
        json << "    \"scenarios\": [" << endl;
        vector<pair<string, vector<Package>>> scenarios;
        scenarios.push_back({"原始数据", packages});
        scenarios.push_back({"等待压力场景", makeScenarioWaitPressure(packages)});
        scenarios.push_back({"截止时间压力场景", makeScenarioDeadlinePressure(packages)});

        for (size_t si = 0; si < scenarios.size(); si++) {
            const auto& sc = scenarios[si];
            PlanStats t2b = simulateDeliveryPlan(sc.second, DeliveryMode::T2, StrategyKind::Baseline, "", false);
            PlanStats t2i = simulateDeliveryPlan(sc.second, DeliveryMode::T2, StrategyKind::Improved, "", false);
            PlanStats t3b = simulateDeliveryPlan(sc.second, DeliveryMode::T3, StrategyKind::Baseline, "", false);
            PlanStats t3i = simulateDeliveryPlan(sc.second, DeliveryMode::T3, StrategyKind::Improved, "", false);

            json << "      { \"name\": \"" << jsonEscape(sc.first) << "\","
                 << " \"t2_baseline_unsat\": " << t2b.total_unsat << ","
                 << " \"t2_improved_unsat\": " << t2i.total_unsat << ","
                 << " \"t3_baseline_cost\": " << t3b.total_cost << ","
                 << " \"t3_improved_cost\": " << t3i.total_cost << ","
                 << " \"t3_baseline_timeout\": " << t3b.timeout_count << ","
                 << " \"t3_improved_timeout\": " << t3i.timeout_count << " }";
            if (si < scenarios.size() - 1) json << ",";
            json << endl;
        }
        json << "    ]" << endl;
        json << "  }" << endl;
    }

    json << "}" << endl;

    fout << json.str() << ";" << endl;
    fout.close();

    cout << "可视化数据已导出到: " << out_path.string() << endl;
}
