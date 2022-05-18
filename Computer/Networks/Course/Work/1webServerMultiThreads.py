# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-05-17 15:48:33
# Name:1webServer.py
# Version:V1.0

import datetime
import os
import threading
import time
# import socket module
from socket import *

serverSocket = socket(AF_INET, SOCK_STREAM)
# Prepare a sever socket
serverSocket.bind(('', 5000))


def tcplink(connectionsocket, addr0):
    print('Accept new connection from %s:%s...' % addr0)
    # loop can't be used in here!
    try:
        message = connectionsocket.recv(1024)
        # print(message)
        filename = message.split()[1]
        f = open(filename[1:], encoding='utf-8')
        outputdata = f.read()

        # Send one HTTP header line into socket
        header = ' HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nDate: %s\r\nServer: liukanglai (Archlinux)\r\nLast-Modified: %s\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n' % (
            os.path.getmtime('.' + filename.decode()), datetime.datetime.now(),
            len(outputdata))
        connectionsocket.send(header.encode())

        # Send the content of the requested file to the client
        for i in range(0, len(outputdata)):
            connectionsocket.send(outputdata[i].encode())
        connectionsocket.close()
    except IOError:
        # Send response message for file not found
        # header = ' HTTP/1.1 404 Not Found\r\nConnection: Keep-Alive\r\nDate: %s\r\nServer: liukanglai (Archlinux)\r\nContent-Type: text/html\r\n\r\n\r\n' % datetime.datetime.now()
        header = 'HTTP/1.1 404 Not Found'
        connectionsocket.send(header.encode())

        #Close client socket
    connectionsocket.close()


serverSocket.listen(10)
while True:
    #Establish the connection
    print('Ready to serve...')
    connectionSocket, addr = serverSocket.accept()
    t = threading.Thread(target=tcplink, args=(connectionSocket, addr))
    t.start()

serverSocket.close()
