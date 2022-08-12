# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-08-11 15:43:35
# Name:p.py
# Version:V1.0
"""
A_star 2D
@author: huiming zhou
"""

import os
import sys
import math
import heapq
import itertools
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import re


def import_excel_matrix0(path):
    data = pd.read_excel(path)
    data = data.values
    return data


def coordinate_change(x, y):
    x = x - 2000
    y = y - 1384
    return x, y


def coordinate_back(x, y):
    x = x + 2000
    y = y + 1384
    return x, y


data_file = u'Downloads/summar/附件1：栅格地图.xlsx'  # Excel文件存储位置
a = import_excel_matrix0(data_file)

print(a.shape)  # (606,397)
map = np.array(a)


class Plotting:

    def __init__(self, xI, xG):
        self.xI, self.xG = xI, xG

    def animation(self, path, visited, name):
        self.plot_grid(name)
        #self.plot_visited(visited)
        self.plot_path(path)
        plt.show()
        plt.savefig('2')

    def plot_grid(self, name):
        tem_map = np.flipud(map)
        plt.imshow(tem_map,
                   cmap=plt.cm.gray,
                   interpolation='nearest',
                   vmin=0,
                   vmax=1)
        plt.colorbar()
        plt.xlim(0, 397)  # 设置x轴范围
        plt.ylim(0, 606)  # 设置y轴范围
        # x = np.arange(0, 607, 1)
        # y = np.arange(0, 398, 1)
        # plt.xticks(x)  # 去刻度
        # plt.yticks(y)
        plt.grid(True)  # 开启
        # plt.show()

        # obs_x = [x[0] for x in self.obs]
        # obs_y = [x[1] for x in self.obs]

        # plt.plot(self.xI[0], self.xI[1], "bs", linewidth=1)
        # plt.plot(self.xG[0], self.xG[1], "gs", linewidth=1)
        # plt.plot(obs_x, obs_y, "sk", linewidth=1)
        plt.title(name)
        # plt.axis("equal")

    def plot_path(self, path, cl='r', flag=False):
        path_x = [path[i][0] for i in range(len(path))]
        path_y = [path[i][1] for i in range(len(path))]

        if not flag:
            plt.plot(path_x, path_y, linewidth='1', color='r')
        else:
            plt.plot(path_x, path_y, linewidth='1', color=cl)

        plt.plot(self.xI[0], self.xI[1], "bs", linewidth=1)
        if isinstance(self.xG[0], int):
            plt.plot(self.xG[0], self.xG[1], "gs", linewidth=1)
        else:
            for i in self.xG:
                x = i[0]
                y = i[1]
                plt.plot(x, y, "gs", linewidth=1)

        plt.pause(0.01)


def turn():
    print("请输入观测点，以空格隔开（仪表1-26，火灾检验1，2为35，36；地面塌陷1，2为37,38）")
    a = input()
    b = re.findall(r"\d+", a)
    statue = []
    for i in b:
        a = int(i)
        if a == 1:
            statue.append(coordinate_change(2235, 1949))
            continue
        if a == 2:
            statue.append(coordinate_change(2235, 1913))
            continue
        if a == 3:
            statue.append(coordinate_change(2274, 1969))
            continue
        if a == 4:
            statue.append(coordinate_change(2277, 1930))
            continue
        if a == 5:
            statue.append(coordinate_change(2277, 1888))
            continue
        if a == 6:
            statue.append(coordinate_change(2102, 1819))
            continue
        if a == 7:
            statue.append(coordinate_change(2122, 1764))
            continue
        if a == 8:
            statue.append(coordinate_change(2162, 1670))
            continue
        if a == 9:
            statue.append(coordinate_change(2218, 1841))
            continue
        if a == 10:
            statue.append(coordinate_change(2198, 1793))
            continue
        if a == 11:
            statue.append(coordinate_change(2198, 1741))
            continue
        if a == 12:
            statue.append(coordinate_change(2198, 1720))
            continue
        if a == 13:
            statue.append(coordinate_change(2166, 1650))
            continue
        if a == 14:
            statue.append(coordinate_change(2173, 1640))
            continue
        if a == 15:
            statue.append(coordinate_change(2245, 1538))
            continue
        if a == 16:
            statue.append(coordinate_change(2244, 1548))
            continue
        if a == 17:
            statue.append(coordinate_change(2282, 1491))
            continue
        if a == 18:
            statue.append(coordinate_change(2262, 1669))
            continue
        if a == 19:
            statue.append(coordinate_change(2315, 1705))
            continue
        if a == 20:
            statue.append(coordinate_change(2315, 1727))
            continue
        if a == 21:
            statue.append(coordinate_change(2315, 1747))
            continue
        if a == 22:
            statue.append(coordinate_change(2315, 1787))
            continue
        if a == 23:
            statue.append(coordinate_change(2315, 1808))
            continue
        if a == 24:
            statue.append(coordinate_change(2343, 1751))
            continue
        if a == 25:
            statue.append(coordinate_change(2394, 1754))
            continue
        if a == 26:
            statue.append(coordinate_change(2374, 1846))
            continue
        if a == 35:
            statue.append(coordinate_change(2339, 1731))
            continue
        if a == 36:
            statue.append(coordinate_change(2194, 1930))
            continue
        if a == 37:
            statue.append(coordinate_change(2269, 1940))
            continue
        if a == 38:
            statue.append(coordinate_change(2273, 1835))
            continue

    return statue


def back(statue):
    original = []
    for i in statue:
        x, y = coordinate_back(i[0], i[1])
        print(x, y)
        if x == 2235 and y == 1949:
            original.append(1)
            continue
        if x == 2235 and y == 1913:
            original.append(2)
            continue
        if x == 2274 and y == 1969:
            original.append(3)
            continue
        if x == 2277 and y == 1930:
            original.append(4)
            continue
        if x == 2277 and y == 1888:
            original.append(5)
            continue
        if x == 2102 and y == 1819:
            original.append(6)
            continue
        if x == 2122 and y == 1764:
            original.append(7)
            continue
        if x == 2162 and y == 1670:
            original.append(8)
            continue
        if x == 2218 and y == 1841:
            original.append(9)
            continue
        if x == 2198 and y == 1793:
            original.append(10)
            continue
        if x == 2198 and y == 1741:
            original.append(11)
            continue
        if x == 2198 and y == 1720:
            original.append(12)
            continue
        if x == 2166 and y == 1650:
            original.append(13)
            continue
        if x == 2173 and y == 1640:
            original.append(14)
            continue
        if x == 2245 and y == 1538:
            original.append(15)
            continue
        if x == 2244 and y == 1548:
            original.append(16)
            continue
        if x == 2282 and y == 1491:
            original.append(17)
            continue
        if x == 2262 and y == 1669:
            original.append(18)
            continue
        if x == 2315 and y == 1705:
            original.append(19)
            continue
        if x == 2315 and y == 1727:
            original.append(20)
            continue
        if x == 2315 and y == 1747:
            original.append(21)
            continue
        if x == 2315 and y == 1787:
            original.append(22)
            continue
        if x == 2315 and y == 1808:
            original.append(23)
            continue
        if x == 2343 and y == 1751:
            original.append(24)
            continue
        if x == 2394 and y == 1754:
            original.append(25)
            continue
        if x == 2374 and y == 1846:
            original.append(26)
            continue
        if x == 2339 and y == 1731:
            original.append(35)
            continue
        if x == 2194 and y == 1930:
            original.append(36)
            continue
        if x == 2269 and y == 1940:
            original.append(37)
            continue
        if x == 2273 and y == 1835:
            original.append(38)
            continue
    return original


class AStar:
    """AStar set the cost + heuristics as the priority
    """

    def __init__(self, s_start, s_goal, heuristic_type):
        self.s_start = s_start
        self.s_goal = s_goal
        self.heuristic_type = heuristic_type

        # self.Env = env1.Env()  # class Env
        # self.map = env1.map

        # feasible input set
        self.u_set = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1),
                      (0, -1), (-1, -1)]  # 可提高速度
        # self.obs = self.Env.obs  # position of obstacles

        self.OPEN = []  # priority queue / OPEN set
        self.CLOSED = []  # CLOSED set / VISITED order
        self.PARENT = dict()  # recorded parent
        self.g = dict()  # cost to come

    def searching(self):
        """
        A_star Searching.
        :return: path, visited order
        """

        self.PARENT[self.s_start] = self.s_start
        self.g[self.s_start] = 0
        if (self.s_start != self.s_goal):  # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            self.g[self.s_goal] = math.inf
        heapq.heappush(self.OPEN, (self.f_value(self.s_start), self.s_start))

        while self.OPEN:
            _, s = heapq.heappop(self.OPEN)
            self.CLOSED.append(s)

            if s == self.s_goal:  # stop condition
                break

            for s_n in self.get_neighbor(s):
                new_cost = self.g[s] + self.cost(s, s_n)

                if s_n not in self.g:
                    self.g[s_n] = math.inf

                if new_cost < self.g[s_n]:  # conditions for updating Cost
                    self.g[s_n] = new_cost
                    self.PARENT[s_n] = s
                    heapq.heappush(self.OPEN, (self.f_value(s_n), s_n))

        return self.extract_path(self.PARENT), self.CLOSED

    def searching_repeated_astar(self, e):
        """
        repeated A*.
        :param e: weight of A*
        :return: path and visited order
        """

        path, visited = [], []

        while e >= 1:
            p_k, v_k = self.repeated_searching(self.s_start, self.s_goal, e)
            path.append(p_k)
            visited.append(v_k)
            e -= 0.5

        return path, visited

    def repeated_searching(self, s_start, s_goal, e):
        """
        run A* with weight e.
        :param s_start: starting state
        :param s_goal: goal state
        :param e: weight of a*
        :return: path and visited order.
        """

        g = {s_start: 0, s_goal: float("inf")}
        PARENT = {s_start: s_start}
        OPEN = []
        CLOSED = []
        heapq.heappush(OPEN,
                       (g[s_start] + e * self.heuristic(s_start), s_start))

        while OPEN:
            _, s = heapq.heappop(OPEN)
            CLOSED.append(s)

            if s == s_goal:
                break

            for s_n in self.get_neighbor(s):
                new_cost = g[s] + self.cost(s, s_n)

                if s_n not in g:
                    g[s_n] = math.inf

                if new_cost < g[s_n]:  # conditions for updating Cost
                    g[s_n] = new_cost
                    PARENT[s_n] = s
                    heapq.heappush(OPEN,
                                   (g[s_n] + e * self.heuristic(s_n), s_n))

        return self.extract_path(PARENT), CLOSED

    def get_neighbor(self, s):
        """
        find neighbors of state s that not in obstacles.
        :param s: state
        :return: neighbors
        """

        return [(s[0] + u[0], s[1] + u[1]) for u in self.u_set]

    def cost(self, s_start, s_goal):
        """
        Calculate Cost for this motion
        :param s_start: starting node
        :param s_goal: end node
        :return:  Cost for this motion
        :note: Cost function could be more complicate!
        """

        if self.is_collision(s_start, s_goal):
            return math.inf

        return math.hypot(s_goal[0] - s_start[0], s_goal[1] - s_start[1])

    def is_collision(self, s_start, s_end):
        """
        check if the line segment (s_start, s_end) is collision.
        :param s_start: start node
        :param s_end: end node
        :return: True: is collision / False: not collision
        """

        # if s_start in self.obs or s_end in self.obs:
        # return True

        tem_start = 606 - s_start[1], s_start[0]
        tem_end = 606 - s_end[1], s_end[0]
        if map[tem_start] == 0 or map[tem_end] == 0:
            return True

        if s_start[0] != s_end[0] and s_start[1] != s_end[1]:
            if s_end[0] - s_start[0] == s_start[1] - s_end[1]:
                s1 = (min(s_start[0], s_end[0]), min(s_start[1], s_end[1]))
                s2 = (max(s_start[0], s_end[0]), max(s_start[1], s_end[1]))
            else:
                s1 = (min(s_start[0], s_end[0]), max(s_start[1], s_end[1]))
                s2 = (max(s_start[0], s_end[0]), min(s_start[1], s_end[1]))

            # if s1 in self.obs or s2 in self.obs:
            # return True
            tem_s1 = 606 - s1[1], s1[0]
            tem_s2 = 606 - s2[1], s2[0]
            if map[tem_s1] == 0 or map[tem_s2] == 0:
                return True

        return False

    def f_value(self, s):
        """
        f = g + h. (g: Cost to come, h: heuristic value)
        :param s: current state
        :return: f
        """

        return self.g[s] + self.heuristic(s)

    def extract_path(self, PARENT):
        """
        Extract the path based on the PARENT set.
        :return: The planning path
        """

        path = [self.s_goal]
        s = self.s_goal

        while True:
            s = PARENT[s]
            path.append(s)

            if s == self.s_start:
                break

        return list(path)

    def get_value(self):
        return self.g[self.s_goal]

    def heuristic(self, s):
        """
        Calculate heuristic.
        :param s: current node (state)
        :return: heuristic function value
        """

        heuristic_type = self.heuristic_type  # heuristic type
        goal = self.s_goal  # goal node
        if heuristic_type == "manhattan":
            return abs(goal[0] - s[0]) + abs(goal[1] - s[1])
        else:
            return math.hypot(goal[0] - s[0], goal[1] - s[1])


class Must:

    def __init__(self, statue):
        self.statue = statue
        # self.Env = env1.Env()  # class Env

    def permutation(self):
        items = itertools.permutations(self.statue)
        distance = []
        items1 = []
        for item in items:
            items1.append(item)
            len_item = len(item)
            tem_distance = 0
            i = 0
            while i < len_item - 1:
                s_start = item[i][0], item[i][1]
                s_goal = item[i + 1][0], item[i + 1][1]
                # print(s_goal, s_start)
                astar = AStar(s_start, s_goal, "euclidean")
                astar.searching()
                tem_distance += astar.get_value()
                # print(tem_distance)
                i = i + 1

            distance.append(tem_distance)
        tmp = min(distance)
        index = distance.index(tmp)

        return list(items1[index])


def main():
    s_start = coordinate_change(2261, 1661)
    start = coordinate_change(2261, 1661)
    statue = turn()
    if len(statue) > 1:
        must = Must(statue)
        new_statue = must.permutation()
        new_statue.insert(0, s_start)
        new_statue.insert(len(new_statue), s_start)
        print(new_statue)
        i = 0
        path = []
        while i < len(new_statue) - 1:
            s_start = new_statue[i][0], new_statue[i][1]
            s_goal = new_statue[i + 1][0], new_statue[i + 1][1]
            # print(s_goal, s_start)
            astar = AStar(s_start, s_goal, "euclidean")

            tem_path, visited = astar.searching()
            path.extend(reversed(tem_path))

            i = i + 1

        new_statue.pop(0)
        new_statue.pop(len(new_statue) - 1)
        print(back(new_statue))
        plot = Plotting(start, new_statue)
        plot.animation(path, visited, "A*")  # animation

    else:
        s_goal = statue[0][0], statue[0][1]
        astar = AStar(s_start, s_goal, "euclidean")
        plot = Plotting(s_start, s_goal)

        path, visited = astar.searching()
        plot.animation(path, visited, "A*")  # animation


# path, visited = astar.searching_repeated_astar(2.5)               # initial weight e = 2.5
# plot.animation_ara_star(path, visited, "Repeated A*")

if __name__ == '__main__':
    main()
