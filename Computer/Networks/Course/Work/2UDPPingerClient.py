# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-05-18 17:17:32
# Name:2UDPPingerClient.py
# Version:V1.0

import time
from socket import *

serverName = '127.0.0.1'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
clientSocket.settimeout(1)  # 设置套接字超时值1秒

miniRTT = 100
maxRTT = 0
allRTT = 0
unsuccess = 0

for i in range(0, 10):
    sendTime = time.time()
    message = ('Ping %d %s' % (i + 1, sendTime)).encode()  # 生成数据报，编码为bytes以便发送
    try:
        clientSocket.sendto(message, (serverName, serverPort))
        modifiedMessage, serverAddress = clientSocket.recvfrom(
            1024)  # 从服务器接收信息，同时也能得到服务器地址
        rtt = time.time() - sendTime  # 计算往返时间
        print('Sequence %d: Reply from %s    RTT = %.6fs' %
              (i + 1, serverName, rtt))  # 显示信息
        if (rtt > maxRTT):
            maxRTT = rtt
        if (rtt < miniRTT):
            miniRTT = rtt  # when write the variable wrong, a mistake will occurs. It's very different with C.
        allRTT += rtt

    except Exception as e:
        print('Sequence %d: Request timed out' % (i + 1))
        unsuccess += 1

print('The minimum RTT is %.6fs' % miniRTT)
print('The maximum RTT is %.6fs' % maxRTT)
print('The average RTT is %.6fs' % (allRTT / (10 - unsuccess)))
print('The packet loss rate is %.2f' % (unsuccess * 10) + '%')

clientSocket.close()
