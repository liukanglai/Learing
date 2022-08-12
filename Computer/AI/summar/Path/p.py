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

# sys.path.append(
# os.path.dirname(os.path.abspath(__file__)) +
# "/../../Search_based_Planning/")

import plotting, env, coord


class AStar:
    """AStar set the cost + heuristics as the priority
    """

    def __init__(self, s_start, s_goal, heuristic_type):
        self.s_start = s_start
        self.s_goal = s_goal
        self.heuristic_type = heuristic_type

        self.Env = env.Env()  # class Env

        self.u_set = self.Env.motions  # feasible input set
        self.obs = self.Env.obs  # position of obstacles

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

        if s_start in self.obs or s_end in self.obs:
            return True

        if s_start[0] != s_end[0] and s_start[1] != s_end[1]:
            if s_end[0] - s_start[0] == s_start[1] - s_end[1]:
                s1 = (min(s_start[0], s_end[0]), min(s_start[1], s_end[1]))
                s2 = (max(s_start[0], s_end[0]), max(s_start[1], s_end[1]))
            else:
                s1 = (min(s_start[0], s_end[0]), max(s_start[1], s_end[1]))
                s2 = (max(s_start[0], s_end[0]), min(s_start[1], s_end[1]))

            if s1 in self.obs or s2 in self.obs:
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
        self.Env = env.Env()  # class Env

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
                path, visited = astar.searching()
                tem_distance += astar.get_value()
                # print(tem_distance)
                i = i + 1

            distance.append(tem_distance)
        tmp = min(distance)
        index = distance.index(tmp)

        return list(items1[index])


def main():
    s_start = (env.coordinate_change(2261, 1661))
    start = (env.coordinate_change(2261, 1661))
    # s_start = (env.coordinate_change(2004, 1988))
    # s_goal = (env.coordinate_change(2218, 1841))
    statue = coord.turn()
    # x = statue[0][0]
    # y = statue[0][1]
    # s_goal = env.coordinate_change(x, y)
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
        print(coord.back(new_statue))
        plot = plotting.Plotting(start, start)
        plot.animation(path, visited, "A*")  # animation

    else:
        s_goal = statue[0][0], statue[0][1]
        astar = AStar(s_start, s_goal, "euclidean")
        plot = plotting.Plotting(s_start, s_goal)

        path, visited = astar.searching()
        plot.animation(path, visited, "A*")  # animation


# path, visited = astar.searching_repeated_astar(2.5)               # initial weight e = 2.5
# plot.animation_ara_star(path, visited, "Repeated A*")

if __name__ == '__main__':
    main()
