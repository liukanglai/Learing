# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-07-20 16:38:11
# Name:numpy.py
# Version:V1.0

import numpy as np

a = np.array([1, 2, 3, 4], dtype=np.int64)
print(a)
print(type(a))
print(a[0:3])
print(a.ndim)  # 维度
print(a.shape)
print(a.size)  # 总数
print(a.dtype)  # 元素类型
print(a.itemsize)  # 每个元素字节个数

# np.arrage(start, end, step, dtype)
# np.ones(shape, dtype)
# np.zeros(shape, dtype)
# np.eye(shape, dtype)
# np.linspace(start, stop, num = 50)
# np.logspace(start, stop, num = 50, base = 10)
# np.asarray(start, stop, num = 50, base = 10) vs array ?

b = np.array([[0, 1, 2, 3], [4, 5, 6, 7], [8, 9, 10, 11]])
print(b[1:3])
print(b[0:2, 0:2])
print(b[:, 0])

# np.reshape(shape)
# np.resize(shape)

# 数组运算
t = np.arange(24).reshape(2, 3, 4)
print(t)
t1 = np.arange(24).reshape(2, 3, -1)  # -1表示自动计算
print(t + t1)
print(t**b)
print(t * t1)
print(np.matmul(t, t1))  # 矩阵相乘
print(np.dot(t, t1))  # 矩阵相乘
# 矩阵转置 np.transpose()
# 求逆 np.linalg.inv()

# np.sum(t, axis=0) 对哪一维进行运算 np.prod() np.diff() np.sqrt() np.exp() np.abs()

x = np.array([1, 2, 3])
y = np.array([4, 5, 6])
print(np.stack((x, y), axis=0))
print(np.stack((x, y), axis=1))

# 矩阵
m1 = np.matrix('1 2 3 ; 4 5 6')
m2 = np.mat([[1, 2, 3], [4, 5, 6]])
print(m1)
print(m1.T)
print(m1.I)
print(m1 * m2)

# 随机数
np.random.seed(612)
print(np.random.rand(2, 3))
print(np.random.uniform(1, 5, (2, 3)))
print(np.random.randint(1, 5, (2, 3)))
print(np.random.randn(2, 3))
print(np.random.normal(0, 1, (2, 3)))
np.random.shuffle()
