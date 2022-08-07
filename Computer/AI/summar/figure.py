# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-07-21 20:47:11
# Name:figure.py
# Version:V1.0

# bitmap(2) Gray Image(8) RGB(24) RGBA(RGB+8) CMYK YCbCr HSI
# BMP JPEG PNG GIF TIFF
import matplotlib.pyplot as plt
from PIL import Image
import numpy as np

img1 = Image.open('lufei.jpg')
img1.save('cope.bmp')
img1_r, img1_g, img1_b = img1.split()
img_gray = img1.convert("L")

print(img1.format)
print(img1.size)
print(img1.mode)

plt.figure(figsize=(5, 5))
plt.subplot(221)
plt.imshow(img1_r, cmap='gray')
plt.subplot(222)
plt.imshow(img1_g, cmap='gray')
plt.subplot(223)
plt.imshow(img1_b, cmap='gray')
plt.subplot(224)
img_rgb = Image.merge("RGB", [img1_r, img1_g, img1_b])
plt.imshow(img_rgb, cmap='gray')
plt.show()

arr_img_gray = np.array(img1)
print('Shape:', arr_img_gray.shape, '\n')
print(arr_img_gray)

arr_img_new = 255 - arr_img_gray
plt.subplot(121)
plt.imshow(arr_img_gray)
plt.subplot(122)
plt.imshow(arr_img_new)
plt.show()

img_small = img1.resize((64, 64))
plt.imshow(img_small)
plt.show()
