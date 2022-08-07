# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-07-20 20:18:06
# Name:keras.py
# Version:V1.0

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

boston_housing = tf.keras.datasets.boston_housing  # in ~/.keras
(trains_x, trains_y), (test_x, test_y) = boston_housing.load_data(test_split=0)

print("Training set:", len(trains_x))
print("Testing set:", len(test_x))

plt.figure(figsize=(5, 5))
'''
for i in range(13):
    plt.subplot(4, 4, (i + 1))
    plt.scatter(trains_x[:, i], trains_y)
    plt.xlabel("RM")
    plt.ylabel("Price($1000's)")
    plt.title("5. RM-Price")

plt.show()
'''

TRAIN_URL = "http://download.tensorflow.org/data/iris_training.csv"
fname_list = TRAIN_URL.split('/')
train_path = tf.keras.utils.get_file(fname_list[-1], TRAIN_URL)
COLUMN_NAMES = [
    'SepalLength', 'SepalWidth', 'PetalLength', 'PetalWidth', 'Species'
]
df_iris = pd.read_csv(train_path, header=0, names=COLUMN_NAMES)
print(df_iris)
print(df_iris.head())
print(df_iris.head(8))
print(df_iris.tail(8))
print(df_iris[10:16])
print(df_iris.describe)

iris = np.array(df_iris)
# iris = df_iris.values
# iris = df_iris.as_matrix()

plt.scatter(iris[:, 2], iris[:, 3], c=iris[:, 4], cmap='brg')
plt.title('Species')
plt.xlabel = (COLUMN_NAMES[2])
plt.ylabel = (COLUMN_NAMES[3])
plt.show()
