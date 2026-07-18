#include "Solver.h"

int wmain(int argc, wchar_t* argv[]) {
    // 设置控制台输出 UTF-8（Windows 中文兼容）
    system("chcp 65001 > nul");

    std::filesystem::path data_dir = (argc >= 2) ? std::filesystem::path(argv[1]) : std::filesystem::path(L"测试数据/示例");
    int src = 0;
    int dst = -1;

    if (argc >= 4) {
        src = std::stoi(std::wstring(argv[2]));
        dst = std::stoi(std::wstring(argv[3]));
    } else if (argc >= 3) {
        dst = std::stoi(std::wstring(argv[2]));
    }

    cout << "========================================" << endl;
    cout << "  菜鸟驿站配送调度系统" << endl;
    cout << "  数据目录: " << data_dir.string() << endl;
    cout << "========================================" << endl << endl;

    loadData(data_dir);

    cout << "[地图] " << n << " 个节点, " << m << " 条边" << endl;
    cout << "[包裹] " << packages.size() << " 个包裹" << endl;
    cout << "[小车] 速度=" << speed << ", 自重=" << car_weight << ", 容量=" << capacity << endl << endl;

    // 读取退货点（如果存在）
    vector<int> return_stops;
    std::filesystem::path return_file = data_dir / "returns.txt";
    if (std::filesystem::exists(return_file)) {
        ifstream fin(return_file);
        if (fin) {
            int count = 0;
            fin >> count;
            for (int i = 0; i < count; i++) {
                int node;
                fin >> node;
                return_stops.push_back(node);
            }
        }
    }

    // ===== T1: 最短路查询 =====
    cout << "===== T1: 最短路查询 =====" << endl;
    if (dst >= 0) {
        auto res = dijkstra(src, dst);
        cout << "  " << src << " -> " << dst << ": distance=" << res.first << ", path=" << pathToString(res.second) << endl;
    } else {
        cout << "  从驿站(0)到各节点的最短距离:" << endl;
        for (int node = 0; node < n; node++) {
            auto res = dijkstra(0, node);
            cout << "    0 -> " << node << ": distance=" << res.first << ", path=" << pathToString(res.second) << endl;
        }
    }
    cout << endl;

    // ===== T2: 不满意度最小化 =====
    solve_T2();
    cout << endl;

    // ===== T3: 运送成本 =====
    solve_T3();
    cout << endl;

    // ===== T4: 退货回收 =====
    solve_T4(return_stops);
    cout << endl;

    // ===== T5: 双车协同 =====
    solve_T5();
    cout << endl;

    // ===== T6: 策略对比实验 =====
    solve_T6();
    cout << endl;

    // ===== 导出可视化数据 =====
    cout << "----------------------------------------" << endl;
    exportVisualizationData(data_dir, return_stops);

    cout << endl << "========================================" << endl;
    cout << "  所有任务完成！" << endl;
    cout << "  打开 visualization.html 查看可视化结果" << endl;
    cout << "========================================" << endl;

    return 0;
}
