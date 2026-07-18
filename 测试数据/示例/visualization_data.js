// 菜鸟驿站配送系统 - 可视化数据
// 由程序自动生成，供 visualization.html 加载

const VIS_DATA = {
  "map": {
    "nodes": [
      { "id": 0, "x": 100, "y": 300, "is_station": 1 },
      { "id": 1, "x": 300, "y": 120, "is_station": 0 },
      { "id": 2, "x": 300, "y": 480, "is_station": 0 },
      { "id": 3, "x": 520, "y": 300, "is_station": 0 },
      { "id": 4, "x": 720, "y": 150, "is_station": 0 },
      { "id": 5, "x": 720, "y": 450, "is_station": 0 }
    ],
    "edges": [
      { "u": 0, "v": 1, "w": 10 },
      { "u": 0, "v": 2, "w": 12 },
      { "u": 1, "v": 3, "w": 8 },
      { "u": 1, "v": 2, "w": 15 },
      { "u": 2, "v": 3, "w": 9 },
      { "u": 3, "v": 4, "w": 7 },
      { "u": 3, "v": 5, "w": 6 },
      { "u": 4, "v": 5, "w": 5 }
    ]
  },
  "car": { "speed": 5, "car_weight": 20, "capacity": 20 },
  "packages": [
    { "id": 1, "weight": 5, "dest": 3, "arrive": 0, "deadline": 100 },
    { "id": 2, "weight": 8, "dest": 4, "arrive": 0, "deadline": 100 },
    { "id": 3, "weight": 6, "dest": 5, "arrive": 0, "deadline": 100 },
    { "id": 4, "weight": 4, "dest": 1, "arrive": 0, "deadline": 100 },
    { "id": 5, "weight": 7, "dest": 2, "arrive": 0, "deadline": 100 }
  ],
  "t1": {
    "from_station": [
      { "dest": 0, "distance": 0, "path": "0" },
      { "dest": 1, "distance": 10, "path": "0 -> 1" },
      { "dest": 2, "distance": 12, "path": "0 -> 2" },
      { "dest": 3, "distance": 18, "path": "0 -> 1 -> 3" },
      { "dest": 4, "distance": 25, "path": "0 -> 1 -> 3 -> 4" },
      { "dest": 5, "distance": 24, "path": "0 -> 1 -> 3 -> 5" }
    ]
  },
  "t2": {
    "baseline_unsat": 37.2,
    "baseline_trips": 2,
    "improved_unsat": 37.2,
    "improved_trips": 2,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 7.8, "load": 16, "package_ids": [4, 1, 5], "route_nodes": [1, 3, 2], "arrival_times": [2, 3.6, 5.4], "full_route": [0, 1, 3, 2, 0] },
      { "trip_id": 2, "depart": 7.8, "return": 18.6, "load": 14, "package_ids": [3, 2], "route_nodes": [5, 4], "arrival_times": [12.6, 13.6], "full_route": [0, 1, 3, 5, 4, 3, 1, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 7.8, "load": 16, "package_ids": [4, 1, 5], "route_nodes": [1, 3, 2], "arrival_times": [2, 3.6, 5.4], "full_route": [0, 1, 3, 2, 0] },
      { "trip_id": 2, "depart": 7.8, "return": 18.6, "load": 14, "package_ids": [3, 2], "route_nodes": [5, 4], "arrival_times": [12.6, 13.6], "full_route": [0, 1, 3, 5, 4, 3, 1, 0] }
    ]
  },
  "t3": {
    "baseline_cost": 2555,
    "baseline_timeout": 0,
    "baseline_trips": 2,
    "improved_cost": 2501,
    "improved_timeout": 0,
    "improved_trips": 2,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 7.8, "load": 16, "package_ids": [4, 1, 5], "route_nodes": [1, 3, 2], "arrival_times": [2, 3.6, 5.4], "full_route": [0, 1, 3, 2, 0] },
      { "trip_id": 2, "depart": 7.8, "return": 18.6, "load": 14, "package_ids": [3, 2], "route_nodes": [5, 4], "arrival_times": [12.6, 13.6], "full_route": [0, 1, 3, 5, 4, 3, 1, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 10.8, "load": 19, "package_ids": [1, 3, 2], "route_nodes": [3, 5, 4], "arrival_times": [3.6, 4.8, 5.8], "full_route": [0, 1, 3, 5, 4, 3, 1, 0] },
      { "trip_id": 2, "depart": 10.8, "return": 18.2, "load": 11, "package_ids": [4, 5], "route_nodes": [1, 2], "arrival_times": [12.8, 15.8], "full_route": [0, 1, 2, 0] }
    ]
  },
  "t4": {
    "return_stops": [1, 4, 5],
    "visit_order": [5, 4, 1],
    "total_distance": 54,
    "total_time": 10.8,
    "full_route": [0, 1, 3, 5, 4, 3, 1, 0]
  },
  "t5": {
    "total_cost": 2465,
    "timeout_count": 0,
    "finish_time": 10.8,
    "car1_package_count": 2,
    "car2_package_count": 3,
    "car1_trips": [
      { "trip_id": 1, "depart": 0, "return": 7.8, "package_ids": [5, 1], "full_route": [0, 2, 3, 1, 0] }
    ],
    "car2_trips": [
      { "trip_id": 1, "depart": 0, "return": 10.8, "package_ids": [4, 3, 2], "full_route": [0, 1, 3, 5, 4, 3, 1, 0] }
    ]
  },
  "t6": {
    "scenarios": [
      { "name": "原始数据", "t2_baseline_unsat": 37.2, "t2_improved_unsat": 37.2, "t3_baseline_cost": 2555, "t3_improved_cost": 2501, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "等待压力场景", "t2_baseline_unsat": 30.6, "t2_improved_unsat": 30.6, "t3_baseline_cost": 2555, "t3_improved_cost": 2501, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "截止时间压力场景", "t2_baseline_unsat": 37.2, "t2_improved_unsat": 37.2, "t3_baseline_cost": 2555, "t3_improved_cost": 2555, "t3_baseline_timeout": 2, "t3_improved_timeout": 2 }
    ]
  }
}
;
