# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-08-11 15:45:06
# Name:env.py
# Version:V1.0
"""
Env 2D
@author: huiming zhou
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def import_excel_matrix0(path):
    data = pd.read_excel(path)
    data = data.values
    return data


data_file = u'Downloads/summar/附件1：栅格地图.xlsx'  # Excel文件存储位置
a = import_excel_matrix0(data_file)

print(a.shape)  # (606,397)
map = np.array(a)


def coordinate_change(x, y):
    t = x
    x = 606 - (y - 1384)
    y = t - 2000
    return x, y


def coordinate_back(x, y):
    t = x
    x = y + 2000
    y = 606 - t + 1384
    return x, y


class Env:

    def __init__(self):
        # self.x_range = 397  # size of background
        # self.y_range = 606
        self.x_range = 606  # size of background
        self.y_range = 397
        self.motions = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1),
                        (0, -1), (-1, -1)]
        self.obs = self.obs_map()

    def update_obs(self, obs):
        self.obs = obs

    def obs_map(self):
        """
        Initialize obstacles' positions
        :return: map of obstacles
        """

        x = self.x_range
        y = self.y_range
        obs = set()

        obs_np = np.where(map == 0)
        # obs_x = [x for x in obs_np[0]]
        # obs_y = [x for x in obs_np[1]]
        i = 0
        while i < len(obs_np[0]):
            obs.add((obs_np[0][i], obs_np[1][i]))
            i = i + 1
        # obs.add((obs_x, obs_y))
        '''
        for i in range(x):
            obs.add((i, 0))
        for i in range(x):
            obs.add((i, y - 1))

        for i in range(y):
            obs.add((0, i))
        for i in range(y):
            obs.add((x - 1, i))
            '''

        return obs

    def plot_grid(self, name):
        obs_x = [x[0] for x in self.obs]
        obs_y = [x[1] for x in self.obs]

        xI = (coordinate_change(2004, 1988))
        xG = (coordinate_change(2218, 1841))
        plt.plot(xI[0], xI[1], "bs")
        plt.plot(xG[0], xG[1], "gs")
        plt.plot(obs_x, obs_y, "sk")
        plt.title(name)
        plt.axis("equal")
        plt.xlim(0, 606)  # 设置x轴范围
        plt.ylim(0, 397)  # 设置y轴范围
        plt.show()


# a = Env()
# Env.plot_grid(a, 'h')
