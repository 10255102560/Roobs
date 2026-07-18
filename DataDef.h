#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <limits>
using namespace std;

struct Edge {
    int to;
    double w;
};

struct NodeInfo {
    int id;
    double x;
    double y;
    int is_station;
};

struct Package {
    int id;
    double weight;
    int dest;
    double arrive;
    double deadline;
};

// 全局变量声明（加 extern，告诉编译器"这些变量在其他cpp里定义"）
extern int n, m;
extern vector<vector<Edge>> graph;
extern vector<NodeInfo> nodes;
extern vector<Package> packages;
extern double speed, car_weight, capacity;
extern vector<double> dist;
extern vector<int> prev_node;
