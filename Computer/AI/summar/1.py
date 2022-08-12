# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-08-11 11:55:12
# Name:1.py
# Version:V1.0

import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np
import pandas as pd

mnist = tf.keras.datasets.mnist
(train_x, train_y), (test_x, test_y) = mnist.load_data()
graph = pd.read_csv("training_log.csv")
graph.plot(figsize=(8, 5))
plt.grid(1)
plt.xlim(0, 4)
plt.ylim(0, 1)
plt.show()
