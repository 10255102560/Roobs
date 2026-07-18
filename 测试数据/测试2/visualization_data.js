// 菜鸟驿站配送系统 - 可视化数据
// 由程序自动生成，供 visualization.html 加载

const VIS_DATA = {
  "map": {
    "nodes": [
      { "id": 0, "x": 381.9, "y": 349.5, "is_station": 1 },
      { "id": 1, "x": 329.8, "y": 220.9, "is_station": 0 },
      { "id": 2, "x": 389.4, "y": 95.2, "is_station": 0 },
      { "id": 3, "x": 596.4, "y": 57.5, "is_station": 0 },
      { "id": 4, "x": 615.7, "y": 231.4, "is_station": 0 },
      { "id": 5, "x": 974, "y": 235, "is_station": 0 },
      { "id": 6, "x": 943.2, "y": 91.3, "is_station": 0 },
      { "id": 7, "x": 679.3, "y": 503.4, "is_station": 0 },
      { "id": 8, "x": 665.3, "y": 394.1, "is_station": 0 },
      { "id": 9, "x": 73, "y": 526.1, "is_station": 0 },
      { "id": 10, "x": 91.5, "y": 223.4, "is_station": 0 },
      { "id": 11, "x": 44.9, "y": 73.8, "is_station": 0 },
      { "id": 12, "x": 911.7, "y": 555.1, "is_station": 0 },
      { "id": 13, "x": 976.3, "y": 374.3, "is_station": 0 },
      { "id": 14, "x": 372.7, "y": 541.7, "is_station": 0 },
      { "id": 15, "x": 121.1, "y": 398.2, "is_station": 0 }
    ],
    "edges": [
      { "u": 0, "v": 5, "w": 40 },
      { "u": 0, "v": 7, "w": 22 },
      { "u": 0, "v": 15, "w": 18 },
      { "u": 0, "v": 3, "w": 24 },
      { "u": 1, "v": 11, "w": 21 },
      { "u": 1, "v": 15, "w": 18 },
      { "u": 1, "v": 9, "w": 27 },
      { "u": 1, "v": 8, "w": 25 },
      { "u": 2, "v": 13, "w": 43 },
      { "u": 2, "v": 6, "w": 37 },
      { "u": 2, "v": 15, "w": 27 },
      { "u": 3, "v": 4, "w": 12 },
      { "u": 4, "v": 10, "w": 35 },
      { "u": 5, "v": 8, "w": 23 },
      { "u": 6, "v": 13, "w": 19 },
      { "u": 7, "v": 10, "w": 43 },
      { "u": 8, "v": 11, "w": 47 },
      { "u": 8, "v": 9, "w": 40 },
      { "u": 10, "v": 11, "w": 10 },
      { "u": 10, "v": 14, "w": 28 },
      { "u": 11, "v": 13, "w": 65 },
      { "u": 11, "v": 12, "w": 66 },
      { "u": 11, "v": 15, "w": 22 }
    ]
  },
  "car": { "speed": 4, "car_weight": 15, "capacity": 95 },
  "packages": [
    { "id": 1, "weight": 7.7, "dest": 5, "arrive": 0, "deadline": 220 },
    { "id": 2, "weight": 7.8, "dest": 11, "arrive": 15, "deadline": 135 },
    { "id": 3, "weight": 12.7, "dest": 5, "arrive": 30, "deadline": 250 },
    { "id": 4, "weight": 4, "dest": 14, "arrive": 15, "deadline": 95 },
    { "id": 5, "weight": 12.6, "dest": 2, "arrive": 5, "deadline": 85 },
    { "id": 6, "weight": 13.4, "dest": 4, "arrive": 15, "deadline": 115 },
    { "id": 7, "weight": 7, "dest": 8, "arrive": 30, "deadline": 130 },
    { "id": 8, "weight": 8, "dest": 7, "arrive": 5, "deadline": 125 },
    { "id": 9, "weight": 14.7, "dest": 1, "arrive": 30, "deadline": 210 },
    { "id": 10, "weight": 12.7, "dest": 13, "arrive": 5, "deadline": 225 },
    { "id": 11, "weight": 2.5, "dest": 13, "arrive": 5, "deadline": 155 },
    { "id": 12, "weight": 8.7, "dest": 14, "arrive": 20, "deadline": 120 },
    { "id": 13, "weight": 14.1, "dest": 2, "arrive": 0, "deadline": 100 },
    { "id": 14, "weight": 8.2, "dest": 9, "arrive": 5, "deadline": 85 },
    { "id": 15, "weight": 12.4, "dest": 6, "arrive": 15, "deadline": 165 },
    { "id": 16, "weight": 10.5, "dest": 7, "arrive": 0, "deadline": 60 },
    { "id": 17, "weight": 2.4, "dest": 11, "arrive": 15, "deadline": 165 },
    { "id": 18, "weight": 12.9, "dest": 14, "arrive": 10, "deadline": 110 },
    { "id": 19, "weight": 8.8, "dest": 9, "arrive": 20, "deadline": 120 },
    { "id": 20, "weight": 8.4, "dest": 9, "arrive": 20, "deadline": 140 }
  ],
  "t1": {
    "from_station": [
      { "dest": 0, "distance": 0, "path": "0" },
      { "dest": 1, "distance": 36, "path": "0 -> 15 -> 1" },
      { "dest": 2, "distance": 45, "path": "0 -> 15 -> 2" },
      { "dest": 3, "distance": 24, "path": "0 -> 3" },
      { "dest": 4, "distance": 36, "path": "0 -> 3 -> 4" },
      { "dest": 5, "distance": 40, "path": "0 -> 5" },
      { "dest": 6, "distance": 82, "path": "0 -> 15 -> 2 -> 6" },
      { "dest": 7, "distance": 22, "path": "0 -> 7" },
      { "dest": 8, "distance": 61, "path": "0 -> 15 -> 1 -> 8" },
      { "dest": 9, "distance": 63, "path": "0 -> 15 -> 1 -> 9" },
      { "dest": 10, "distance": 50, "path": "0 -> 15 -> 11 -> 10" },
      { "dest": 11, "distance": 40, "path": "0 -> 15 -> 11" },
      { "dest": 12, "distance": 106, "path": "0 -> 15 -> 11 -> 12" },
      { "dest": 13, "distance": 88, "path": "0 -> 15 -> 2 -> 13" },
      { "dest": 14, "distance": 78, "path": "0 -> 15 -> 11 -> 10 -> 14" },
      { "dest": 15, "distance": 18, "path": "0 -> 15" }
    ]
  },
  "t2": {
    "baseline_unsat": 2225.5,
    "baseline_trips": 3,
    "improved_unsat": 2301,
    "improved_trips": 3,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 53.5, "load": 32.3, "package_ids": [16, 1, 13], "route_nodes": [7, 5, 2], "arrival_times": [5.5, 21, 42.25], "full_route": [0, 7, 0, 5, 0, 15, 2, 15, 0] },
      { "trip_id": 2, "depart": 53.5, "return": 172.5, "load": 93.3, "package_ids": [8, 2, 17, 4, 18, 6, 5, 10, 11, 14, 19], "route_nodes": [7, 11, 11, 14, 14, 4, 2, 13, 13, 9, 9], "arrival_times": [59, 72.25, 72.25, 81.75, 81.75, 97.5, 117.75, 128.5, 128.5, 156.75, 156.75], "full_route": [0, 7, 10, 11, 10, 14, 10, 4, 3, 0, 15, 2, 13, 11, 1, 9, 1, 15, 0] },
      { "trip_id": 3, "depart": 172.5, "return": 281.75, "load": 63.9, "package_ids": [9, 7, 3, 20, 12, 15], "route_nodes": [1, 8, 5, 9, 14, 6], "arrival_times": [181.5, 187.75, 193.5, 209.25, 230.75, 261.25], "full_route": [0, 15, 1, 8, 5, 8, 9, 1, 11, 10, 14, 10, 11, 13, 6, 2, 15, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 53.5, "load": 32.3, "package_ids": [16, 1, 13], "route_nodes": [7, 5, 2], "arrival_times": [5.5, 21, 42.25], "full_route": [0, 7, 0, 5, 0, 15, 2, 15, 0] },
      { "trip_id": 2, "depart": 53.5, "return": 172, "load": 91.1, "package_ids": [8, 2, 17, 9, 14, 19, 3, 6, 5, 11], "route_nodes": [7, 11, 11, 1, 9, 9, 5, 4, 2, 13], "arrival_times": [59, 72.25, 72.25, 77.5, 84.25, 84.25, 100, 119, 139.25, 150], "full_route": [0, 7, 10, 11, 1, 9, 8, 5, 0, 3, 4, 3, 0, 15, 2, 13, 2, 15, 0] },
      { "trip_id": 3, "depart": 172, "return": 269.75, "load": 66.1, "package_ids": [7, 20, 4, 12, 18, 10, 15], "route_nodes": [8, 9, 14, 14, 14, 13, 6], "arrival_times": [187.25, 197.25, 218.75, 218.75, 218.75, 244.5, 249.25], "full_route": [0, 15, 1, 8, 9, 1, 11, 10, 14, 10, 11, 13, 6, 2, 15, 0] }
    ]
  },
  "t3": {
    "baseline_cost": 45193.7,
    "baseline_timeout": 2,
    "baseline_trips": 3,
    "improved_cost": 45193.7,
    "improved_timeout": 2,
    "improved_trips": 3,
    "baseline_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 86.5, "load": 93.2, "package_ids": [16, 6, 4, 12, 18, 14, 19, 5, 13], "route_nodes": [7, 4, 14, 14, 14, 9, 9, 2, 2], "arrival_times": [5.5, 20, 35.75, 35.75, 35.75, 57.25, 57.25, 75.25, 75.25], "full_route": [0, 7, 0, 3, 4, 10, 14, 10, 11, 1, 9, 1, 15, 2, 15, 0] },
      { "trip_id": 2, "depart": 86.5, "return": 192.25, "load": 83.6, "package_ids": [8, 2, 17, 9, 7, 1, 20, 15, 10, 11], "route_nodes": [7, 11, 11, 1, 8, 5, 9, 6, 13, 13], "arrival_times": [92, 105.25, 105.25, 110.5, 116.75, 122.5, 138.25, 165.5, 170.25, 170.25], "full_route": [0, 7, 10, 11, 1, 8, 5, 8, 9, 1, 15, 2, 6, 13, 2, 15, 0] },
      { "trip_id": 3, "depart": 192.25, "return": 212.25, "load": 12.7, "package_ids": [3], "route_nodes": [5], "arrival_times": [202.25], "full_route": [0, 5, 0] }
    ],
    "improved_trip_details": [
      { "trip_id": 1, "depart": 0, "return": 86.5, "load": 93.2, "package_ids": [16, 6, 4, 12, 18, 14, 19, 5, 13], "route_nodes": [7, 4, 14, 14, 14, 9, 9, 2, 2], "arrival_times": [5.5, 20, 35.75, 35.75, 35.75, 57.25, 57.25, 75.25, 75.25], "full_route": [0, 7, 0, 3, 4, 10, 14, 10, 11, 1, 9, 1, 15, 2, 15, 0] },
      { "trip_id": 2, "depart": 86.5, "return": 192.25, "load": 83.6, "package_ids": [8, 2, 17, 9, 7, 1, 20, 15, 10, 11], "route_nodes": [7, 11, 11, 1, 8, 5, 9, 6, 13, 13], "arrival_times": [92, 105.25, 105.25, 110.5, 116.75, 122.5, 138.25, 165.5, 170.25, 170.25], "full_route": [0, 7, 10, 11, 1, 8, 5, 8, 9, 1, 15, 2, 6, 13, 2, 15, 0] },
      { "trip_id": 3, "depart": 192.25, "return": 212.25, "load": 12.7, "package_ids": [3], "route_nodes": [5], "arrival_times": [202.25], "full_route": [0, 5, 0] }
    ]
  },
  "t4": {
    "return_stops": [5, 11, 14, 2, 4, 8, 7, 1, 13, 9, 6],
    "visit_order": [7, 4, 14, 11, 13, 6, 2, 1, 9, 8, 5],
    "total_distance": 477,
    "total_time": 119.25,
    "full_route": [0, 7, 0, 3, 4, 10, 14, 10, 11, 13, 6, 2, 15, 1, 9, 8, 5, 0]
  },
  "t5": {
    "total_cost": 38388.2,
    "timeout_count": 0,
    "finish_time": 99.25,
    "car1_package_count": 9,
    "car2_package_count": 11,
    "car1_trips": [
      { "trip_id": 1, "depart": 0, "return": 99.25, "package_ids": [8, 16, 5, 13, 15, 10, 11, 18, 19], "full_route": [0, 7, 0, 15, 2, 6, 13, 11, 10, 14, 10, 11, 1, 9, 1, 15, 0] }
    ],
    "car2_trips": [
      { "trip_id": 1, "depart": 0, "return": 94.25, "package_ids": [6, 2, 17, 9, 7, 1, 3, 14, 20, 4, 12], "full_route": [0, 3, 4, 10, 11, 1, 8, 5, 8, 9, 1, 11, 10, 14, 10, 11, 15, 0] }
    ]
  },
  "t6": {
    "scenarios": [
      { "name": "原始数据", "t2_baseline_unsat": 2225.5, "t2_improved_unsat": 2301, "t3_baseline_cost": 45193.7, "t3_improved_cost": 45193.7, "t3_baseline_timeout": 2, "t3_improved_timeout": 2 },
      { "name": "等待压力场景", "t2_baseline_unsat": 2343.75, "t2_improved_unsat": 2157.5, "t3_baseline_cost": 49364, "t3_improved_cost": 41230.7, "t3_baseline_timeout": 0, "t3_improved_timeout": 0 },
      { "name": "截止时间压力场景", "t2_baseline_unsat": 2107.25, "t2_improved_unsat": 2107.25, "t3_baseline_cost": 49108.9, "t3_improved_cost": 49108.9, "t3_baseline_timeout": 18, "t3_improved_timeout": 18 }
    ]
  }
}
;
