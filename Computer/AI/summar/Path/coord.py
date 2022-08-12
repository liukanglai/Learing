# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-08-11 18:06:13
# Name:zuo.py
# Version:V1.0

import re
import env


def turn():
    print("请输入观测点，以空格隔开（火灾检验1，2为35，26；地面塌陷1，2为37,38）")
    a = input()
    b = re.findall(r"\d+", a)
    statue = []
    for i in b:
        a = int(i)
        if a == 1:
            statue.append(env.coordinate_change(2235, 1949))
            continue
        if a == 2:
            statue.append(env.coordinate_change(2235, 1913))
            continue
        if a == 3:
            statue.append(env.coordinate_change(2274, 1969))
            continue
        if a == 4:
            statue.append(env.coordinate_change(2277, 1930))
            continue
        if a == 5:
            statue.append(env.coordinate_change(2277, 1888))
            continue
        if a == 6:
            statue.append(env.coordinate_change(2102, 1819))
            continue
        if a == 7:
            statue.append(env.coordinate_change(2122, 1764))
            continue
        if a == 8:
            statue.append(env.coordinate_change(2162, 1670))
            continue
        if a == 9:
            statue.append(env.coordinate_change(2218, 1841))
            continue
        if a == 10:
            statue.append(env.coordinate_change(2198, 1793))
            continue
        if a == 11:
            statue.append(env.coordinate_change(2198, 1741))
            continue
        if a == 12:
            statue.append(env.coordinate_change(2198, 1720))
            continue
        if a == 13:
            statue.append(env.coordinate_change(2166, 1650))
            continue
        if a == 14:
            statue.append(env.coordinate_change(2173, 1640))
            continue
        if a == 15:
            statue.append(env.coordinate_change(2245, 1538))
            continue
        if a == 16:
            statue.append(env.coordinate_change(2244, 1548))
            continue
        if a == 17:
            statue.append(env.coordinate_change(2282, 1491))
            continue
        if a == 18:
            statue.append(env.coordinate_change(2262, 1669))
            continue
        if a == 19:
            statue.append(env.coordinate_change(2315, 1705))
            continue
        if a == 20:
            statue.append(env.coordinate_change(2315, 1727))
            continue
        if a == 21:
            statue.append(env.coordinate_change(2315, 1747))
            continue
        if a == 22:
            statue.append(env.coordinate_change(2315, 1787))
            continue
        if a == 23:
            statue.append(env.coordinate_change(2315, 1808))
            continue
        if a == 24:
            statue.append(env.coordinate_change(2343, 1751))
            continue
        if a == 25:
            statue.append(env.coordinate_change(2394, 1754))
            continue
        if a == 26:
            statue.append(env.coordinate_change(2374, 1846))
            continue
        if a == 35:
            statue.append(env.coordinate_change(2339, 1731))
            continue
        if a == 36:
            statue.append(env.coordinate_change(2194, 1930))
            continue
        if a == 37:
            statue.append(env.coordinate_change(2269, 1940))
            continue
        if a == 38:
            statue.append(env.coordinate_change(2273, 1835))
            continue

    return statue


def back(statue):
    original = []
    for i in statue:
        x, y = env.coordinate_back(i[0], i[1])
        print(x, y)
        if x == 2235 and y == 1949:
            original.append(1)
            continue
        if x == 2235 and y == 1913:
            original.append(2)
            continue
        if x == 2274 and y == 1969:
            original.append(3)
            continue
        if x == 2277 and y == 1930:
            original.append(4)
            continue
        if x == 2277 and y == 1888:
            original.append(5)
            continue
        if x == 2102 and y == 1819:
            original.append(6)
            continue
        if x == 2122 and y == 1764:
            original.append(7)
            continue
        if x == 2162 and y == 1670:
            original.append(8)
            continue
        if x == 2218 and y == 1841:
            original.append(9)
            continue
        if x == 2198 and y == 1793:
            original.append(10)
            continue
        if x == 2198 and y == 1741:
            original.append(11)
            continue
        if x == 2198 and y == 1720:
            original.append(12)
            continue
        if x == 2166 and y == 1650:
            original.append(13)
            continue
        if x == 2173 and y == 1640:
            original.append(14)
            continue
        if x == 2245 and y == 1538:
            original.append(15)
            continue
        if x == 2244 and y == 1548:
            original.append(16)
            continue
        if x == 2282 and y == 1491:
            original.append(17)
            continue
        if x == 2262 and y == 1669:
            original.append(18)
            continue
        if x == 2315 and y == 1705:
            original.append(19)
            continue
        if x == 2315 and y == 1727:
            original.append(20)
            continue
        if x == 2315 and y == 1747:
            original.append(21)
            continue
        if x == 2315 and y == 1787:
            original.append(22)
            continue
        if x == 2315 and y == 1808:
            original.append(23)
            continue
        if x == 2343 and y == 1751:
            original.append(24)
            continue
        if x == 2394 and y == 1754:
            original.append(25)
            continue
        if x == 2374 and y == 1846:
            original.append(26)
            continue
        if x == 2339 and y == 1731:
            original.append(35)
            continue
        if x == 2194 and y == 1930:
            original.append(36)
            continue
        if x == 2269 and y == 1940:
            original.append(37)
            continue
        if x == 2273 and y == 1835:
            original.append(38)
            continue
    return original
