# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-05-17 15:48:33
# Name:1webServer.py
# Version:V1.0

import threading, time
import datetime
import os
#import socket module
from socket import *

serverSocket = socket(AF_INET, SOCK_STREAM)
#Prepare a sever socket
serverSocket.bind(('', 5000))

serverSocket.listen(1)
while True:
    #Establish the connection
    print('Ready to serve...')
    connectionSocket, addr = serverSocket.accept()
    try:
        message = connectionSocket.recv(1024)
        print(message)
        filename = message.split()[1]
        f = open(filename[1:], encoding='utf-8')
        outputdata = f.read()

        #Send one HTTP header line into socket
        header = ' HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nDate: %s\r\nServer: liukanglai (Archlinux)\r\nLast-Modified: %s\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n' % (
            os.path.getmtime('.' + filename.decode()), datetime.datetime.now(),
            len(outputdata))
        connectionSocket.send(header.encode())

        #Send the content of the requested file to the client
        for i in range(0, len(outputdata)):
            connectionSocket.send(outputdata[i].encode())
        connectionSocket.close()
    except IOError:
        #Send response message for file not found
        # header = ' HTTP/1.1 404 Not Found\r\nConnection: Keep-Alive\r\nDate: %s\r\nServer: liukanglai (Archlinux)\r\nContent-Type: text/html\r\n\r\n\r\n' % datetime.datetime.now()
        header = 'HTTP/1.1 404 Not Found'
        connectionSocket.send(header.encode())

        #Close client socket
        connectionSocket.close()

serverSocket.close()
