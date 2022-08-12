# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-08-11 15:10:45
# Name:tu1.py
# Version:V1.0

import numpy
from pylab import *
import copy

# 定义一个含有障碍物的20×20的栅格地图
# 10表示可通行点
# 0表示障碍物
# 7表示起点
# 5表示终点
map_grid = numpy.full((20, 20), int(10), dtype=numpy.int8)
# print(map_grid)
map_grid[3, 3:8] = 0
map_grid[3:10, 7] = 0
map_grid[10, 3:8] = 0
map_grid[17, 13:17] = 0
map_grid[10:17, 13] = 0
map_grid[10, 13:17] = 0
map_grid[5, 2] = 7
map_grid[15, 15] = 5
# 画出定义的栅格地图

# plt.imshow(map_grid, cmap=plt.cm.hot, interpolation='nearest', vmin=0, vmax=10)
# plt.colorbar()
# xlim(-1, 20)  # 设置x轴范围
# ylim(-1, 20)  # 设置y轴范围
# my_x_ticks = numpy.arange(0, 20, 1)
# my_y_ticks = numpy.arange(0, 20, 1)
# plt.xticks(my_x_ticks)
# plt.yticks(my_y_ticks)
# plt.grid(True)
# plt.show()


class AStar(object):
    """
    创建一个A*算法类
    """

    def __init__(self):
        """
        初始化
        """
        self.f = 0
        self.g = 0
        self.last_point = numpy.array([])  # 上一个目标点不断取得更新
        self.current_point = numpy.array([])  # 当前目标点不断取得更新
        self.open = numpy.array([[], []])  # 先创建一个空的open表
        self.closed = numpy.array([[], []])  # 先创建一个空的closed表
        self.taboo = numpy.array([[], []])  # 创建一个禁忌表，用于放置不再搜索的点
        self.start = numpy.array([5, 2])  # 起点坐标
        self.goal = numpy.array([15, 15])  # 终点坐标

    def h_value_tem(self, cur_p):
        """
        计算拓展节点和终点的h值
        :param cur_p:子搜索节点坐标
        :return:
        """
        h = (cur_p[0] - 15)**2 + (cur_p[1] - 15)**2
        h = numpy.sqrt(h)  # 计算h
        return h

    def g_value_tem(self, chl_p, cu_p):
        """
        计算拓展节点和父节点的g值
        其实也可以直接用1或者1.414代替
        :param chl_p:子节点坐标
        :param cu_p:父节点坐标，也就是self.current_point
        :return:返回子节点到父节点的g值，但不是全局g值
        """
        g1 = cu_p[0] - chl_p[0]
        g2 = cu_p[1] - chl_p[1]
        g = g1**2 + g2**2
        g = numpy.sqrt(g)
        return g

    def f_value_tem(self, chl_p, cu_p):
        """
        求出的是临时g值和h值的和，还需加上累计g值得到全局f值
        :param chl_p: 父节点坐标
        :param cu_p: 子节点坐标
        :return:
        """
        f = self.g_value_tem(chl_p, cu_p) + self.h_value_tem(cu_p)
        return f

    def min_f(self):
        """
        找出open中f值最小的节点坐标，记录为current_point
        :return:返回open表中最小值的位置索引和在map_grid中的坐标
        对撞墙后的处理方式是，随机选择一个方向进行搜索
        并且将open列表清零，不然一直是死循环
        这种处理方式以后待改进！！！
        """
        tem_f = []  # 创建一个记录f值的临时列表
        for i in range(self.open.shape[1]):
            # 计算拓展节点的全局f值
            f_value = self.f_value_tem(self.current_point,
                                       self.open[:, i]) + self.g
            tem_f.append(f_value)
        index = tem_f.index(min(tem_f))  # 返回最小值索引
        location = self.open[:, index]  # 返回最小值坐标
        print('打印位置索引和地图坐标：')
        print(index, location)
        return index, location

    def child_point(self, x):
        """
        拓展的子节点坐标
        :param x: 父节点坐标
        :return: 子节点存入open表，返回值是每一次拓展出的子节点数目，用于撞墙判断
        当搜索的节点撞墙后，如果不加处理，会陷入死循环
        """
        tem_open = numpy.array([[], []])  # 统计拓展节点的临时数组
        tem_open_shape = 0  # 统计临时数组的长度
        # 开始遍历周围8个节点
        for j in range(-1, 2, 1):
            for q in range(-1, 2, 1):
                if j == 0 and q == 0:  # 搜索到父节点去掉
                    continue

                # print(map_grid[int(x[0] + j), int(x[1] + q)])
                if map_grid[int(x[0] + j), int(x[1] + q)] == 0:  # 搜索到障碍物去掉
                    continue
                if x[0] + j < 0 or x[0] + j > 19 or x[1] + q < 0 or x[
                        1] + q > 19:  # 搜索点出了边界去掉
                    continue
                # 在open表中，则去掉搜索点
                a = self.judge_location(x, j, q, self.open)
                if a == 1:
                    continue
                # 在closed表中,则去掉搜索点
                b = self.judge_location(x, j, q, self.closed)
                if b == 1:
                    continue

                # 在taboo表中，则去掉搜索点
                c = self.judge_location(x, j, q, self.taboo)
                if c == 1:
                    continue

                m = numpy.array([x[0] + j, x[1] + q])

                tem_open = numpy.c_[tem_open, m]  #

                tem_open_shape = tem_open.shape[1]  # 求出tem_open的长度

                self.open = numpy.c_[self.open, m]  # 搜索出的子节点加入open
                # print('打印第一次循环后的open：')
                # print(self.open)
        return tem_open_shape

    def judge_location(self, x, j, q, list_co):
        """
        判断拓展点是否在open表或者closed表中
        :return:
        """
        jud = 0
        for i in range(list_co.shape[1]):

            if x[0] + j == list_co[0, i] and x[1] + q == list_co[1, i]:

                jud = jud + 1
            else:
                jud = jud
        # if a != 0:
        #     continue
        return jud

    def draw_path_closed(self):
        """
        画出closed表中的坐标点图
        :return:
        """
        map_closed = copy.deepcopy(map_grid)
        for i in range(self.closed.shape[1]):
            x = self.closed[:, i]

            map_closed[int(x[0]), int(x[1])] = 5

        plt.imshow(map_closed,
                   cmap=plt.cm.hot,
                   interpolation='nearest',
                   vmin=0,
                   vmax=10)
        # plt.colorbar()
        xlim(-1, 20)  # 设置x轴范围
        ylim(-1, 20)  # 设置y轴范围
        my_x_ticks = numpy.arange(0, 20, 1)
        my_y_ticks = numpy.arange(0, 20, 1)
        plt.xticks(my_x_ticks)
        plt.yticks(my_y_ticks)
        plt.grid(True)
        # plt.show()

    def draw_init_map(self):
        """
        画出起点终点图
        :return:
        """

        plt.imshow(map_grid,
                   cmap=plt.cm.hot,
                   interpolation='nearest',
                   vmin=0,
                   vmax=10)
        # plt.colorbar()
        xlim(-1, 20)  # 设置x轴范围
        ylim(-1, 20)  # 设置y轴范围
        my_x_ticks = numpy.arange(0, 20, 1)
        my_y_ticks = numpy.arange(0, 20, 1)
        plt.xticks(my_x_ticks)
        plt.yticks(my_y_ticks)
        plt.grid(True)
        # plt.show()

    def draw_path_open(self):
        """
        画出open表中的坐标点图
        :return:
        """
        map_open = copy.deepcopy(map_grid)
        for i in range(self.closed.shape[1]):
            x = self.closed[:, i]

            map_open[int(x[0]), int(x[1])] = 1

        plt.imshow(map_open,
                   cmap=plt.cm.hot,
                   interpolation='nearest',
                   vmin=0,
                   vmax=10)
        # plt.colorbar()
        xlim(-1, 20)  # 设置x轴范围
        ylim(-1, 20)  # 设置y轴范围
        my_x_ticks = numpy.arange(0, 20, 1)
        my_y_ticks = numpy.arange(0, 20, 1)
        plt.xticks(my_x_ticks)
        plt.yticks(my_y_ticks)
        plt.grid(True)
        # plt.show()

    def draw_three_axes(self):
        """
        将三张图画在一个figure中
        :return:
        """
        plt.figure()
        ax1 = plt.subplot(221)

        ax2 = plt.subplot(222)
        ax3 = plt.subplot(223)
        plt.sca(ax1)
        self.draw_init_map()
        plt.sca(ax2)
        self.draw_path_open()
        plt.sca(ax3)
        self.draw_path_closed()
        plt.show()

    def main(self):
        """
        main函数
        :return:
        """
        self.open = numpy.column_stack((self.open, self.start))  # 起点放入open
        self.current_point = self.start  # 起点放入当前点，作为父节点
        # self.closed
        ite = 1
        while ite <= 2000:

            # open列表为空，退出
            if self.open.shape[1] == 0:
                print('没有搜索到路径！')
                return

            last_point = self.current_point  # 上一个目标点不断取得更新

            while True:

                index, self.current_point = self.min_f()  # 判断open表中f值
                print('检验第%s次当前点坐标' % ite)
                print(self.current_point)

                # 选取open表中最小f值的节点作为best，放入closed表
                self.closed = numpy.c_[self.closed, self.current_point]

                if self.current_point[0] == 15 and self.current_point[
                        1] == 15:  # 如果best是目标点，退出
                    print('搜索成功！')
                    return

                tem_open_shape = self.child_point(
                    self.current_point)  # 生成子节点并判断数目
                self.open = delete(self.open, index, axis=1)  # 删除open中最优点

                if tem_open_shape == 0:
                    self.closed = delete(self.closed, -1,
                                         axis=1)  # 删除closed中不合理的点
                    self.taboo = numpy.c_[self.taboo,
                                          self.current_point]  # 将撞墙点加入禁忌表
                    self.current_point = last_point
                    self.g = self.g + self.g_value_tem(self.current_point,
                                                       last_point)
                    continue
                else:
                    break

                # print(self.open)

            ite = ite + 1


if __name__ == '__main__':

    a1 = AStar()
    a1.main()
    a1.draw_three_axes()
