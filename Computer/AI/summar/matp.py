# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-07-20 18:44:56
# Name:matp.py
# Version:V1.0

import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(3, 2), facecolor="green")
#plt.rcParams["font.sans-serif"] = "Noto Sans CJK SC"
plt.plot()
plt.suptitle("hhh", fontsize=20, color="red", backgroundcolor="yellow")
plt.subplot(2, 2, 1)
plt.title(1)
plt.subplot(222)
plt.title(2)
plt.subplot(223)
plt.title(3)
plt.subplot(224)
plt.title(4)
plt.tight_layout(rect=[0, 0, 1, 0.9])  # 调整布局
plt.rcdefaults()
plt.show()

# 散点图Scatter
n = 1024
x1 = np.random.normal(0, 1, n)
y1 = np.random.normal(0, 1, n)

x2 = np.random.uniform(-4, 4, (1, n))
y2 = np.random.uniform(-4, 4, (1, n))
plt.rcParams["axes.unicode_minus"] = False  # 正常显示负号

plt.scatter(x1, y1, color='blue', marker='*', label="正态分布")
plt.scatter(x2, y2, color='yellow', marker='o', label="均匀分布")

plt.legend()
plt.title("标准正态分布", fontsize=20)
plt.text(2.5, 2.5, "均值：0\n标准差:1")

plt.xlim(-4, 4)
plt.ylim(-4, 4)
plt.xlabel('横坐标x', fontsize=14)
plt.ylabel('纵坐标y', fontsize=14)
plt.show()

# 折线图Line Chart
n = 24
y1 = np.random.randint(27, 37, n)
y2 = np.random.randint(40, 60, n)

plt.plot(y1, label='w')
plt.plot(y2, label='s')

plt.xlim(0, 23)
plt.ylim(20, 70)
plt.xlabel('hour', fontsize=12)
plt.ylabel('result', fontsize=12)

plt.title('temperature', fontsize=16)

plt.legend()
plt.show()

# 柱状图Bar Chart
n = 24
y1 = np.random.randint(27, 37, n)
y2 = np.random.randint(-60, -20, n)

plt.bar(range(len(y1)),
        y1,
        width=0.8,
        facecolor='green',
        edgecolor='white',
        label='w')
plt.bar(range(len(y2)),
        y2,
        width=0.8,
        facecolor='red',
        edgecolor='white',
        label='y')

plt.title('temperature', fontsize=16)

plt.legend()
plt.show()
