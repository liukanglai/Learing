# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-05-18 20:31:35
# Name:2UDPHeartbeatClient.py
# Version:V1.0

from socket import *
import time

serverName = '127.0.0.1'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
clientSocket.settimeout(1)

for i in range(0, 10):
    sendTime = time.time()
    message = 'number: %d' % (i + 1) + ' time: %fs' % sendTime
    try:
        clientSocket.sendto(message.encode(), (serverName, serverPort))
        modifiedMessage, serverAddress = clientSocket.recvfrom(
            1024)  # 从服务器接收信息，同时也能得到服务器地址
        rtt = time.time() - sendTime  # 计算往返时间
        print('Sequence %d: Reply from %s    RTT = %.6fs' %
              (i + 1, serverName, rtt))  # 显示信息

    except Exception as e:
        print('Sequence %d: Request timed out' % (i + 1))

clientSocket.close()
