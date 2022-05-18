# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-05-17 19:53:19
# Name:1webClient.py
# Version:V1.0

import datetime
import os
import sys
from socket import *

clientName = sys.argv[1]
clientPortStr = sys.argv[2]
filename = sys.argv[3]
clientPort = int(clientPortStr)

clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((clientName, clientPort))

get = '''GET /%s HTTP/1.1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.9,en-CN;q=0.8,en;q=0.7
Cache-Control: max-age=0
Connection: keep-alive
Cookie: io=apFMGAYoDjq8KVWwAAAA
Host: %s
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.64 Safari/537.36
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="101", "Google Chrome";v="101"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"''' % (filename,
                                  (clientName + ':' + str(clientPort)))

# get = 'GET %s HTTP/1.1\r\nConnection: Keep-Alive\r\nDate: %s\r\n: liukanglai (Archlinux)\r\nLast-Modified: %s\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n' % (
# os.path.getmtime('.' + filename.decode()), datetime.datetime.now(),
# len(outputdata))

clientSocket.send(get.encode())
# message = clientSocket.recv(1024)
# for i in range(0, len(message)):
# print(message.decode())
date = 1
while date:
    date = clientSocket.recv(1024)
    print(date.decode())

clientSocket.close()
