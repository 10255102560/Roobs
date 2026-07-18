#pragma once
#include "DataDef.h"
#include <filesystem>

pair<double, vector<int>> dijkstra(int src, int dst);
string pathToString(const vector<int>& path);
void solve_T2();
void solve_T3();
void solve_T4(const vector<int>& return_stops);
void solve_T5();
void solve_T6();
void loadData(const std::filesystem::path& data_dir);

// 可视化：输出地图数据与各任务结果到 JSON 文件
void exportVisualizationData(const std::filesystem::path& data_dir,
                              const vector<int>& return_stops);
