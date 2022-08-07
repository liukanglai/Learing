# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-07-21 21:16:26
# Name:minist.py
# Version:V1.0

import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np
import pandas as pd

mnist = tf.keras.datasets.mnist
(train_x, train_y), (test_x, test_y) = mnist.load_data()

plt.axis('off')
plt.imshow(train_x[0], cmap='gray')
plt.show()

for i in range(4):
    num = np.random.randint(1, 60000)

    plt.subplot(1, 4, i + 1)
    plt.axis("off")
    plt.imshow(train_x[num], cmap='gray')
    plt.title(train_y[num])

plt.show()

# 数据处理

x_train, x_test = tf.cast(train_x, dtype=tf.float32) / 255.0, tf.cast(
    test_x, dtype=tf.float32) / 255.0  # 转化为tensor张量
y_train, y_test = tf.cast(train_y, dtype=tf.int32), tf.cast(test_y,
                                                            dtype=tf.int32)

# 转化为四维数据
x_train = train_x.reshape(60000, 28, 28, 1)
x_test = test_x.reshape(10000, 28, 28, 1)

# CNN
model = tf.keras.Sequential([

    # unit 1
    tf.keras.layers.Conv2D(16,
                           kernel_size=(3, 3),
                           padding="same",
                           activation=tf.nn.relu,
                           input_shape=(28, 28, 1)),  # 卷积层
    tf.keras.layers.MaxPool2D(pool_size=(2, 2)),  # 池化层

    # unit 2
    tf.keras.layers.Conv2D(32,
                           kernel_size=(3, 3),
                           padding="same",
                           activation=tf.nn.relu),
    tf.keras.layers.MaxPool2D(pool_size=(2, 2)),

    # unit 3
    tf.keras.layers.Flatten(),

    # unit 4
    tf.keras.layers.Dense(128, activation="relu"),  # 隐含层
    tf.keras.layers.Dense(10, activation="softmax")  # 输出层
    # tf.layers.dropout(,rate,seed,,name)
])

print(model.summary())

# 配置训练方法
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['sparse_categorical_accuracy'])

# 训练模型
history = model.fit(x_train,
                    y_train,
                    batch_size=64,
                    epochs=5,
                    validation_split=0.2)

# 评估
model.evaluate(x_test, y_test, verbose=2)

# 保存训练日志文件并绘制曲线
pd.DataFrame(history.history).to_csv("training_log.csv", index=False)

graph = pd.read_csv("training_log.csv")
graph.plot(figsize=(8, 5))
plt.grid(1)
plt.xlim(0, 4)
plt.ylim(0, 1)
plt.show()

# 预制数据
plt.figure()
for i in range(10):
    num = np.random.randint(1, 10000)

    plt.subplot(2, 5, i + 1)
    plt.axis('off')
    plt.imshow(test_x[num], cmap='gray')
    demo = tf.reshape(x_test[num], (1, 28, 28, 1))
    y_pred = np.argmax(model.predict(demo))
    plt.title('real:' + str(test_y[num]) + '\npredict:' + str(y_pred))

plt.show()

# 保存加载模型
model.save_weights('mnist.h5')
# model.load_weights('mnist.h5')
