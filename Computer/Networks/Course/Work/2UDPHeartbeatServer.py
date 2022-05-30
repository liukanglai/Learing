# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-05-18 19:22:56
# Name:2UDPHeartbeat.py
# Version:V1.0

import random
import time
from socket import *

serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', 12000))

serverSocket.settimeout(2)

while (True):
    rand = random.randint(0, 10)
    try:
        message, addr = serverSocket.recvfrom(1024)
        if rand < 4:
            continue
        message = message.decode().split()[3]
        message = message[:-1]
        timeDiff = time.time() - float(message)
        print("receive RTT:", timeDiff)
        serverSocket.sendto(message.encode(), addr)
    except Exception as e:
        print('Wait 2s, the client has stopped.')
