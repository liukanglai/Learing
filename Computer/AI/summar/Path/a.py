# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-08-08 21:49:21
# Name:a.py
# Version:V1.0

import queue


def heuristic(a, b):
    # Manhattan distance on a square grid
    return abs(a.x - b.x) + abs(a.y - b.y)


def a_star_search(graph, start, goal):
    frontier = PriorityQueue()  # 第一轮方块，优先排序
    frontier.put(start, 0)  # 放起点
    came_from = dict()  # 前一方块
    cost_so_far = dict()  # 当前代价
    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
        current = frontier.get()  # 提取出代价最低方块

        if current == goal:
            break

        for next in graph.neighbors(current):
            new_cost = cost_so_far[current] + graph.cost(current, next)
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost
                priority = new_cost + heuristic(goal, next)
                frontier.put(next, priority)
                came_from[next] = current
    return came_from, cost_so_far
