// 菜鸟驿站配送系统 - 可视化数据
// 由程序自动生成，供 visualization.html 加载

const VIS_DATA = {
  "map": {
    "nodes": [
      { "id": 0, "x": 0, "y": 0, "is_station": 1 },
      { "id": 1, "x": 10, "y": 0, "is_station": 0 },
      { "id": 2, "x": 20, "y": 0, "is_station": 0 },
      { "id": 3, "x": 10, "y": 10, "is_station": 0 },
      { "id": 4, "x": 20, "y": 10, "is_station": 0 },
      { "id": 5, "x": 30, "y": 10, "is_station": 0 },
      { "id": 6, "x": 30, "y": 0, "is_station": 0 }
    ],
    "edges": [
      { "u": 0, "v": 1, "w": 6 },
      { "u": 0, "v": 3, "w": 7 },
      { "u": 1, "v": 2, "w": 6 },
      { "u": 1, "v": 3, "w": 4 },
      { "u": 2, "v": 4, "w": 5 },
      { "u": 2, "v": 6, "w": 7 },
      { "u": 3, "v": 4, "w": 3 },
      { "u": 3, "v": 6, "w": 9 },
      { "u": 4, "v": 5, "w": 4 },
      { "u": 5, "v": 6, "w": 5 }
    ]
  },
  "car": { "speed": 5, "car_weight": 20, "capacity": 15 },
  "packages": [
    { "id": 1, "weight": 3, "dest": 1, "arrive": 0, "deadline": 1000000000 },
    { "id": 2, "weight": 4, "dest": 2, "arrive": 2, "deadline": 1000000000 },
    { "id": 3, "weight": 2, "dest": 3, "arrive": 4, "deadline": 1000000000 },
    { "id": 4, "weight": 5, "dest": 4, "arrive": 6, "deadline": 1000000000 },
    { "id": 5, "weight": 3, "dest": 5, "arrive": 8, "deadline": 1000000000 },
    { "id": 6, "weight": 4, "dest": 6, "arrive": 10, "deadline": 1000000000 }
  ],
  "t1": {
    "from_station": [
      { "dest": 0, "distance": 0, "path": "0" },
      { "dest": 1, "distance": 6, "path": "0 -> 1" },
      { "dest": 2, "distance": 12, "path": "0 -> 1 -> 2" },
      { "dest": 3, "distance": 7, "path": "0 -> 3" },
      { "dest": 4, "distance": 10, "path": "0 -> 3 -> 4" },
      { "dest": 5, "distance": 14, "path": "0 -> 3 -> 4 -> 5" },
      { "dest": 6, "distance": 16, "path": "0 -> 3 -> 6" }
    ]
  },
  "t2": {
    "baseline_unsat": 22.8,
    "baseline_trips": 4,
    "improved_unsat": 22.8,
    "improved_trips": 4,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 2.4, "load": 3, "package_ids": [1], "route_nodes": [1], "arrival_times": [1.2], "full_route": [0, 1, 0] },
      { "trip_id": 2, "depart": 2.4, "return": 7.2, "load": 4, "package_ids": [2], "route_nodes": [2], "arrival_times": [4.8], "full_route": [0, 1, 2, 1, 0] },
      { "trip_id": 3, "depart": 7.2, "return": 11.2, "load": 7, "package_ids": [3, 4], "route_nodes": [3, 4], "arrival_times": [8.6, 9.2], "full_route": [0, 3, 4, 3, 0] },
      { "trip_id": 4, "depart": 11.2, "return": 18.2, "load": 7, "package_ids": [5, 6], "route_nodes": [5, 6], "arrival_times": [14, 15], "full_route": [0, 3, 4, 5, 6, 3, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 2.4, "load": 3, "package_ids": [1], "route_nodes": [1], "arrival_times": [1.2], "full_route": [0, 1, 0] },
      { "trip_id": 2, "depart": 2.4, "return": 7.2, "load": 4, "package_ids": [2], "route_nodes": [2], "arrival_times": [4.8], "full_route": [0, 1, 2, 1, 0] },
      { "trip_id": 3, "depart": 7.2, "return": 11.2, "load": 7, "package_ids": [3, 4], "route_nodes": [3, 4], "arrival_times": [8.6, 9.2], "full_route": [0, 3, 4, 3, 0] },
      { "trip_id": 4, "depart": 11.2, "return": 18.2, "load": 7, "package_ids": [5, 6], "route_nodes": [5, 6], "arrival_times": [14, 15], "full_route": [0, 3, 4, 5, 6, 3, 0] }
    ]
  },
  "t3": {
    "baseline_cost": 1593,
    "baseline_timeout": 0,
    "baseline_trips": 2,
    "improved_cost": 1539,
    "improved_timeout": 0,
    "improved_trips": 2,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 6, "load": 14, "package_ids": [1, 3, 4, 2], "route_nodes": [1, 3, 4, 2], "arrival_times": [1.2, 2, 2.6, 3.6], "full_route": [0, 1, 3, 4, 2, 1, 0] },
      { "trip_id": 2, "depart": 6, "return": 13, "load": 7, "package_ids": [5, 6], "route_nodes": [5, 6], "arrival_times": [8.8, 9.8], "full_route": [0, 3, 4, 5, 6, 3, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 7.6, "load": 13, "package_ids": [3, 5, 6, 2], "route_nodes": [3, 5, 6, 2], "arrival_times": [1.4, 2.8, 3.8, 5.2], "full_route": [0, 3, 4, 5, 6, 2, 1, 0] },
      { "trip_id": 2, "depart": 7.6, "return": 12.2, "load": 8, "package_ids": [1, 4], "route_nodes": [1, 4], "arrival_times": [8.8, 10.2], "full_route": [0, 1, 3, 4, 3, 0] }
    ]
  },
  "t4": {
    "return_stops": [1, 2, 3, 4, 5, 6],
    "visit_order": [3, 4, 5, 6, 2, 1],
    "total_distance": 38,
    "total_time": 7.6,
    "full_route": [0, 3, 4, 5, 6, 2, 1, 0]
  },
  "t5": {
    "total_cost": 1428,
    "timeout_count": 0,
    "finish_time": 7,
    "car1_package_count": 4,
    "car2_package_count": 2,
    "car1_trips": [
      { "trip_id": 1, "depart": 0, "return": 7, "package_ids": [3, 4, 5, 6], "full_route": [0, 3, 4, 5, 6, 3, 0] }
    ],
    "car2_trips": [
      { "trip_id": 1, "depart": 0, "return": 4.8, "package_ids": [1, 2], "full_route": [0, 1, 2, 1, 0] }
    ]
  },
  "t6": {
    "scenarios": [
      { "name": "原始数据", "t2_baseline_unsat": 22.8, "t2_improved_unsat": 22.8, "t3_baseline_cost": 1593, "t3_improved_cost": 1539, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "等待压力场景", "t2_baseline_unsat": 26.2, "t2_improved_unsat": 26.2, "t3_baseline_cost": 1593, "t3_improved_cost": 1539, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "截止时间压力场景", "t2_baseline_unsat": 28, "t2_improved_unsat": 28, "t3_baseline_cost": 1593, "t3_improved_cost": 1593, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 }
    ]
  }
}
;
