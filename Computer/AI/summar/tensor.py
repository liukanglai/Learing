# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-07-28 07:56:26
# Name:tensor.py
# Version:V1.0

import tensorflow as tf
import numpy as np

# 动/静态图

print("Tensorflow Version:", tf.__version__)

# 张量
a = tf.constant([[1, 2], [3, 4]])
b = tf.cast(a, dtype=tf.float32)
b = tf.constant(0.1, dtype=tf.float64)
a.numpy()
print(a)

# tf.ones
# 正态分布 normal
# 均匀分布 uniform
'''
tf.reshape(a, [2, 3, 4])
tf.expand_dims(a, -1)
tf.squeeze()
tf.transpose()
tf.concat()
tf.split()
tf.stack()
tf.gather_nd()
'''
