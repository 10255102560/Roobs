// 菜鸟驿站配送系统 - 可视化数据
// 由程序自动生成，供 visualization.html 加载

const VIS_DATA = {
  "map": {
    "nodes": [
      { "id": 0, "x": 0, "y": 0, "is_station": 1 },
      { "id": 1, "x": 8, "y": 0, "is_station": 0 },
      { "id": 2, "x": 16, "y": 0, "is_station": 0 },
      { "id": 3, "x": 8, "y": 8, "is_station": 0 },
      { "id": 4, "x": 16, "y": 8, "is_station": 0 },
      { "id": 5, "x": 24, "y": 8, "is_station": 0 },
      { "id": 6, "x": 24, "y": 0, "is_station": 0 },
      { "id": 7, "x": 32, "y": 0, "is_station": 0 }
    ],
    "edges": [
      { "u": 0, "v": 1, "w": 5 },
      { "u": 0, "v": 3, "w": 6 },
      { "u": 1, "v": 2, "w": 5 },
      { "u": 1, "v": 3, "w": 4 },
      { "u": 2, "v": 4, "w": 4 },
      { "u": 2, "v": 6, "w": 7 },
      { "u": 3, "v": 4, "w": 3 },
      { "u": 3, "v": 6, "w": 8 },
      { "u": 4, "v": 5, "w": 4 },
      { "u": 5, "v": 6, "w": 4 },
      { "u": 6, "v": 7, "w": 5 }
    ]
  },
  "car": { "speed": 5, "car_weight": 20, "capacity": 14 },
  "packages": [
    { "id": 1, "weight": 2, "dest": 1, "arrive": 0, "deadline": 12 },
    { "id": 2, "weight": 3, "dest": 2, "arrive": 0, "deadline": 14 },
    { "id": 3, "weight": 4, "dest": 3, "arrive": 0, "deadline": 16 },
    { "id": 4, "weight": 3, "dest": 4, "arrive": 0, "deadline": 15 },
    { "id": 5, "weight": 2, "dest": 5, "arrive": 2, "deadline": 13 },
    { "id": 6, "weight": 4, "dest": 6, "arrive": 4, "deadline": 18 },
    { "id": 7, "weight": 3, "dest": 7, "arrive": 6, "deadline": 20 }
  ],
  "t1": {
    "from_station": [
      { "dest": 0, "distance": 0, "path": "0" },
      { "dest": 1, "distance": 5, "path": "0 -> 1" },
      { "dest": 2, "distance": 10, "path": "0 -> 1 -> 2" },
      { "dest": 3, "distance": 6, "path": "0 -> 3" },
      { "dest": 4, "distance": 9, "path": "0 -> 3 -> 4" },
      { "dest": 5, "distance": 13, "path": "0 -> 3 -> 4 -> 5" },
      { "dest": 6, "distance": 14, "path": "0 -> 3 -> 6" },
      { "dest": 7, "distance": 19, "path": "0 -> 3 -> 6 -> 7" }
    ]
  },
  "t2": {
    "baseline_unsat": 28,
    "baseline_trips": 3,
    "improved_unsat": 28,
    "improved_trips": 3,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 5.2, "load": 12, "package_ids": [1, 3, 4, 2], "route_nodes": [1, 3, 4, 2], "arrival_times": [1, 1.8, 2.4, 3.2], "full_route": [0, 1, 3, 4, 2, 1, 0] },
      { "trip_id": 2, "depart": 5.2, "return": 11.4, "load": 6, "package_ids": [5, 6], "route_nodes": [5, 6], "arrival_times": [7.8, 8.6], "full_route": [0, 3, 4, 5, 6, 3, 0] },
      { "trip_id": 3, "depart": 11.4, "return": 19, "load": 3, "package_ids": [7], "route_nodes": [7], "arrival_times": [15.2], "full_route": [0, 3, 6, 7, 6, 3, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 5.2, "load": 12, "package_ids": [1, 3, 4, 2], "route_nodes": [1, 3, 4, 2], "arrival_times": [1, 1.8, 2.4, 3.2], "full_route": [0, 1, 3, 4, 2, 1, 0] },
      { "trip_id": 2, "depart": 5.2, "return": 11.4, "load": 6, "package_ids": [5, 6], "route_nodes": [5, 6], "arrival_times": [7.8, 8.6], "full_route": [0, 3, 4, 5, 6, 3, 0] },
      { "trip_id": 3, "depart": 11.4, "return": 19, "load": 3, "package_ids": [7], "route_nodes": [7], "arrival_times": [15.2], "full_route": [0, 3, 6, 7, 6, 3, 0] }
    ]
  },
  "t3": {
    "baseline_cost": 1791,
    "baseline_timeout": 0,
    "baseline_trips": 2,
    "improved_cost": 1739,
    "improved_timeout": 0,
    "improved_trips": 2,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 7.4, "load": 14, "package_ids": [1, 3, 4, 2, 5], "route_nodes": [1, 3, 4, 2, 5], "arrival_times": [1, 1.8, 2.4, 3.2, 4.8], "full_route": [0, 1, 3, 4, 2, 4, 5, 4, 3, 0] },
      { "trip_id": 2, "depart": 7.4, "return": 15, "load": 7, "package_ids": [6, 7], "route_nodes": [6, 7], "arrival_times": [10.2, 11.2], "full_route": [0, 3, 6, 7, 6, 3, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 6.8, "load": 14, "package_ids": [1, 3, 4, 5, 2], "route_nodes": [1, 3, 4, 5, 2], "arrival_times": [1, 1.8, 2.4, 3.2, 4.8], "full_route": [0, 1, 3, 4, 5, 4, 2, 1, 0] },
      { "trip_id": 2, "depart": 6.8, "return": 14.4, "load": 7, "package_ids": [6, 7], "route_nodes": [6, 7], "arrival_times": [9.6, 10.6], "full_route": [0, 3, 6, 7, 6, 3, 0] }
    ]
  },
  "t4": {
    "return_stops": [1, 2, 3, 4, 5, 6, 7],
    "visit_order": [3, 4, 5, 7, 6, 2, 1],
    "total_distance": 44,
    "total_time": 8.8,
    "full_route": [0, 3, 4, 5, 6, 7, 6, 2, 1, 0]
  },
  "t5": {
    "total_cost": 1546,
    "timeout_count": 0,
    "finish_time": 8.2,
    "car1_package_count": 4,
    "car2_package_count": 3,
    "car1_trips": [
      { "trip_id": 1, "depart": 0, "return": 8.2, "package_ids": [3, 5, 6, 7], "full_route": [0, 3, 4, 5, 6, 7, 6, 3, 0] }
    ],
    "car2_trips": [
      { "trip_id": 1, "depart": 0, "return": 4.6, "package_ids": [1, 2, 4], "full_route": [0, 1, 2, 4, 3, 0] }
    ]
  },
  "t6": {
    "scenarios": [
      { "name": "原始数据", "t2_baseline_unsat": 28, "t2_improved_unsat": 28, "t3_baseline_cost": 1791, "t3_improved_cost": 1739, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "等待压力场景", "t2_baseline_unsat": 38.6, "t2_improved_unsat": 38.6, "t3_baseline_cost": 1791, "t3_improved_cost": 1631, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "截止时间压力场景", "t2_baseline_unsat": 34.6, "t2_improved_unsat": 33.4, "t3_baseline_cost": 1791, "t3_improved_cost": 1739, "t3_baseline_timeout": 2, "t3_improved_timeout": 1 }
    ]
  }
}
;
