// 菜鸟驿站配送系统 - 可视化数据
// 由程序自动生成，供 visualization.html 加载

const VIS_DATA = {
  "map": {
    "nodes": [
      { "id": 0, "x": 881, "y": 539.3, "is_station": 1 },
      { "id": 1, "x": 898.1, "y": 38.9, "is_station": 0 },
      { "id": 2, "x": 366.8, "y": 47.4, "is_station": 0 },
      { "id": 3, "x": 641, "y": 305.3, "is_station": 0 },
      { "id": 4, "x": 683.7, "y": 527.3, "is_station": 0 },
      { "id": 5, "x": 402.5, "y": 514.7, "is_station": 0 },
      { "id": 6, "x": 941.8, "y": 265.8, "is_station": 0 },
      { "id": 7, "x": 329.1, "y": 336.8, "is_station": 0 },
      { "id": 8, "x": 105.7, "y": 565.9, "is_station": 0 },
      { "id": 9, "x": 100.3, "y": 32.2, "is_station": 0 },
      { "id": 10, "x": 108.4, "y": 298.4, "is_station": 0 },
      { "id": 11, "x": 607.2, "y": 45.3, "is_station": 0 }
    ],
    "edges": [
      { "u": 0, "v": 7, "w": 39 },
      { "u": 0, "v": 2, "w": 47 },
      { "u": 0, "v": 1, "w": 33 },
      { "u": 0, "v": 6, "w": 19 },
      { "u": 1, "v": 11, "w": 19 },
      { "u": 1, "v": 6, "w": 15 },
      { "u": 1, "v": 2, "w": 35 },
      { "u": 2, "v": 9, "w": 18 },
      { "u": 2, "v": 7, "w": 19 },
      { "u": 2, "v": 3, "w": 25 },
      { "u": 3, "v": 4, "w": 15 },
      { "u": 3, "v": 11, "w": 17 },
      { "u": 4, "v": 7, "w": 27 },
      { "u": 4, "v": 10, "w": 41 },
      { "u": 5, "v": 6, "w": 40 },
      { "u": 6, "v": 10, "w": 56 },
      { "u": 8, "v": 10, "w": 18 }
    ]
  },
  "car": { "speed": 4, "car_weight": 22, "capacity": 61 },
  "packages": [
    { "id": 1, "weight": 11.8, "dest": 9, "arrive": 0, "deadline": 100 },
    { "id": 2, "weight": 8.8, "dest": 5, "arrive": 10, "deadline": 70 },
    { "id": 3, "weight": 5.1, "dest": 7, "arrive": 0, "deadline": 60 },
    { "id": 4, "weight": 13.4, "dest": 3, "arrive": 30, "deadline": 210 },
    { "id": 5, "weight": 12, "dest": 8, "arrive": 5, "deadline": 225 },
    { "id": 6, "weight": 12, "dest": 11, "arrive": 10, "deadline": 130 },
    { "id": 7, "weight": 6.7, "dest": 9, "arrive": 0, "deadline": 220 },
    { "id": 8, "weight": 5.9, "dest": 9, "arrive": 5, "deadline": 155 },
    { "id": 9, "weight": 14.8, "dest": 1, "arrive": 10, "deadline": 160 },
    { "id": 10, "weight": 7.6, "dest": 5, "arrive": 20, "deadline": 140 },
    { "id": 11, "weight": 2.1, "dest": 9, "arrive": 20, "deadline": 80 },
    { "id": 12, "weight": 5.1, "dest": 5, "arrive": 20, "deadline": 100 },
    { "id": 13, "weight": 13.2, "dest": 11, "arrive": 0, "deadline": 150 },
    { "id": 14, "weight": 3.2, "dest": 5, "arrive": 0, "deadline": 220 }
  ],
  "t1": {
    "from_station": [
      { "dest": 0, "distance": 0, "path": "0" },
      { "dest": 1, "distance": 33, "path": "0 -> 1" },
      { "dest": 2, "distance": 47, "path": "0 -> 2" },
      { "dest": 3, "distance": 69, "path": "0 -> 1 -> 11 -> 3" },
      { "dest": 4, "distance": 66, "path": "0 -> 7 -> 4" },
      { "dest": 5, "distance": 59, "path": "0 -> 6 -> 5" },
      { "dest": 6, "distance": 19, "path": "0 -> 6" },
      { "dest": 7, "distance": 39, "path": "0 -> 7" },
      { "dest": 8, "distance": 93, "path": "0 -> 6 -> 10 -> 8" },
      { "dest": 9, "distance": 65, "path": "0 -> 2 -> 9" },
      { "dest": 10, "distance": 75, "path": "0 -> 6 -> 10" },
      { "dest": 11, "distance": 52, "path": "0 -> 1 -> 11" }
    ]
  },
  "t2": {
    "baseline_unsat": 1146,
    "baseline_trips": 3,
    "improved_unsat": 1044.25,
    "improved_trips": 3,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 67.25, "load": 40, "package_ids": [3, 1, 7, 13, 14], "route_nodes": [7, 9, 9, 11, 5], "arrival_times": [9.75, 19, 19, 34, 52.5], "full_route": [0, 7, 2, 9, 2, 3, 11, 1, 6, 5, 6, 0] },
      { "trip_id": 2, "depart": 67.25, "return": 174, "load": 60.7, "package_ids": [9, 6, 8, 11, 2, 12, 5], "route_nodes": [1, 11, 9, 9, 5, 5, 8], "arrival_times": [75.5, 80.25, 95.25, 95.25, 122.25, 122.25, 150.75], "full_route": [0, 1, 11, 3, 2, 9, 2, 1, 6, 5, 6, 10, 8, 10, 6, 0] },
      { "trip_id": 3, "depart": 174, "return": 228.75, "load": 21, "package_ids": [10, 4], "route_nodes": [5, 3], "arrival_times": [188.75, 211.5], "full_route": [0, 6, 5, 6, 1, 11, 3, 11, 1, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 67.25, "load": 40, "package_ids": [3, 1, 7, 13, 14], "route_nodes": [7, 9, 9, 11, 5], "arrival_times": [9.75, 19, 19, 34, 52.5], "full_route": [0, 7, 2, 9, 2, 3, 11, 1, 6, 5, 6, 0] },
      { "trip_id": 2, "depart": 67.25, "return": 137, "load": 56.3, "package_ids": [9, 6, 8, 11, 2, 10, 12], "route_nodes": [1, 11, 9, 9, 5, 5, 5], "arrival_times": [75.5, 80.25, 95.25, 95.25, 122.25, 122.25, 122.25], "full_route": [0, 1, 11, 3, 2, 9, 2, 1, 6, 5, 6, 0] },
      { "trip_id": 3, "depart": 137, "return": 196, "load": 25.4, "package_ids": [4, 5], "route_nodes": [3, 8], "arrival_times": [154.25, 172.75], "full_route": [0, 1, 11, 3, 4, 10, 8, 10, 6, 0] }
    ]
  },
  "t3": {
    "baseline_cost": 28668.5,
    "baseline_timeout": 0,
    "baseline_trips": 3,
    "improved_cost": 28668.5,
    "improved_timeout": 0,
    "improved_trips": 3,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 67.25, "load": 58.4, "package_ids": [3, 1, 8, 11, 6, 2, 10, 12], "route_nodes": [7, 9, 9, 9, 11, 5, 5, 5], "arrival_times": [9.75, 19, 19, 19, 34, 52.5, 52.5, 52.5], "full_route": [0, 7, 2, 9, 2, 3, 11, 1, 6, 5, 6, 0] },
      { "trip_id": 2, "depart": 67.25, "return": 137, "load": 51.3, "package_ids": [9, 13, 4, 7, 14], "route_nodes": [1, 11, 3, 9, 5], "arrival_times": [75.5, 80.25, 84.5, 95.25, 122.25], "full_route": [0, 1, 11, 3, 2, 9, 2, 1, 6, 5, 6, 0] },
      { "trip_id": 3, "depart": 137, "return": 183.5, "load": 12, "package_ids": [5], "route_nodes": [8], "arrival_times": [160.25], "full_route": [0, 6, 10, 8, 10, 6, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 67.25, "load": 58.4, "package_ids": [3, 1, 8, 11, 6, 2, 10, 12], "route_nodes": [7, 9, 9, 9, 11, 5, 5, 5], "arrival_times": [9.75, 19, 19, 19, 34, 52.5, 52.5, 52.5], "full_route": [0, 7, 2, 9, 2, 3, 11, 1, 6, 5, 6, 0] },
      { "trip_id": 2, "depart": 67.25, "return": 137, "load": 51.3, "package_ids": [9, 13, 4, 7, 14], "route_nodes": [1, 11, 3, 9, 5], "arrival_times": [75.5, 80.25, 84.5, 95.25, 122.25], "full_route": [0, 1, 11, 3, 2, 9, 2, 1, 6, 5, 6, 0] },
      { "trip_id": 3, "depart": 137, "return": 183.5, "load": 12, "package_ids": [5], "route_nodes": [8], "arrival_times": [160.25], "full_route": [0, 6, 10, 8, 10, 6, 0] }
    ]
  },
  "t4": {
    "return_stops": [9, 5, 7, 3, 8, 11, 1],
    "visit_order": [1, 11, 3, 9, 7, 8, 5],
    "total_distance": 408,
    "total_time": 102,
    "full_route": [0, 1, 11, 3, 2, 9, 2, 7, 4, 10, 8, 10, 6, 5, 6, 0]
  },
  "t5": {
    "total_cost": 26426.5,
    "timeout_count": 0,
    "finish_time": 101.25,
    "car1_package_count": 8,
    "car2_package_count": 6,
    "car1_trips": [
      { "trip_id": 1, "depart": 0, "return": 70, "package_ids": [9, 1, 7, 8, 11, 3, 10, 14], "full_route": [0, 1, 2, 9, 2, 7, 0, 6, 5, 6, 0] }
    ],
    "car2_trips": [
      { "trip_id": 1, "depart": 0, "return": 54.75, "package_ids": [6, 13, 4, 2, 12], "full_route": [0, 1, 11, 3, 11, 1, 6, 5, 6, 0] },
      { "trip_id": 2, "depart": 54.75, "return": 101.25, "package_ids": [5], "full_route": [0, 6, 10, 8, 10, 6, 0] }
    ]
  },
  "t6": {
    "scenarios": [
      { "name": "原始数据", "t2_baseline_unsat": 1146, "t2_improved_unsat": 1044.25, "t3_baseline_cost": 28668.5, "t3_improved_cost": 28668.5, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "等待压力场景", "t2_baseline_unsat": 1268.75, "t2_improved_unsat": 1268.75, "t3_baseline_cost": 29811.7, "t3_improved_cost": 31813.7, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "截止时间压力场景", "t2_baseline_unsat": 1050.25, "t2_improved_unsat": 1050.25, "t3_baseline_cost": 31813.7, "t3_improved_cost": 31813.7, "t3_baseline_timeout": 11, "t3_improved_timeout": 11 }
    ]
  }
}
;
