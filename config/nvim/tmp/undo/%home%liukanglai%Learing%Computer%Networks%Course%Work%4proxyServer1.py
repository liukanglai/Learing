Vim�UnDo� ʷ=��-�BدVUB�F��p��{�L�#�@�                                      b��0    _�                             ����                                                                                                                                                                                                                                                                                                                                                             b��N     �               �               �               �               �               �               �                  5��                                                  �                                                �                          /                      �                          C                      �                          K                      �                          f                      �                          ~                      5�_�                            ����                                                                                                                                                                                                                                                                                                                                                             b��U     �                  �               5��                          �                      �                          �                      5�_�                    	        ����                                                                                                                                                                                                                                                                                                                                                             b��]    �   	            �   	            5��    	               Q       �               V
      5�_�                    
        ����                                                                                                                                                                                                                                                                                                                            
                      v        b��}     �   	      Z      	import os   
import sys   from socket import *       if len(sys.argv) <= 1:   
    print(   g        'Usage : "python ProxyServer.py server_ip"\n [server_ip : It is the IP Address Of Proxy Server'       )       sys.exit(2)       ?# Create a server socket, bind it to a port and start listening   )tcpSerSock = socket(AF_INET, SOCK_STREAM)   tcpSerPort = int(sys.argv[1])   !tcpSerSock.bind(("", tcpSerPort))   print(tcpSerPort)       tcpSerSock.listen(10)5��    	                      �       �              5�_�                    
        ����                                                                                                                                                                                                                                                                                                                            
                      v        b���     �   	      J      tcpSerSock.listen(10)   while 1:   *    # Strat receiving data from the client       print('Ready to serve...')   *    tcpCliSock, addr = tcpSerSock.accept()   .    print('Received a connection from:', addr)   ,    message = tcpCliSock.recv(1024).decode()       print(message)           if(message == ''):           continue       1    # Extract the filename from the given message       print(message.split()[1])   3    filename = message.split()[1].partition("/")[2]       print(filename)       fileExist = "false"       filetouse = "/" + filename       print(filetouse)           try:5��    	                     �                     5�_�                    
        ����                                                                                                                                                                                                                                                                                                                            
                      v        b���     �   	      6         try:   2        # Check wether the file exist in the cache   +        with open(filetouse[1:], "r") as f:   &            outputdata = f.readlines()               fileExist = "true"       H        # ProxyServer finds a cache hit and generates a response message   .        tcpCliSock.send("HTTP/1.0 200 OK\r\n")   5        tcpCliSock.send("Content-Type:text/html\r\n")   #        tcpCliSock.send(outputdata)            print('Read from cache')       0    # Error handling for file not found in cache5��    	                      �       �              5�_�                            ����                                                                                                                                                                                                                                                                                                                                       '           v        b���     �   
      )          except IOError:            if fileExist == "false":   0            # Create a socket on the proxyserver   ,            c = socket(AF_INET, SOCK_STREAM)   1            hostn = filename.replace("www.","",1)               print(hostn)               try:   2                # Connect to the socket to port 80   !                c.connect("", 80)   m                # Create a temporary file on this socket and ask port 80 for the file requested by the client   ,                fileobj = c.makefile('r', 0)   L                fileobj.write("GET "+"http://" + filename + " HTTP/1.0\n\n")   /                # Read the response into buffer                    # Fill in start.                   # Fill in end.   H                # Create a new file in the cache for the requested file.   o                # Also send the response in the buffer to client socket and the corresponding file in the cache   4                tmpFile = open("./" + filename,"wb")                    # Fill in start.                   # Fill in end.               except:   (                print("Illegal request")           else:   6            # HTTP response message for file not found               # Fill in start.               # Fill in end.   -    # Close the client and the server sockets       tcpCliSock.close()       # Fill in start.5��    
                      �       �              5�_�      
              
        ����                                                                                                                                                                                                                                                                                                                                                  v        b���     �   	   
          0    # Error handling for file not found in cache5��    	                      �       1               5�_�         	       
   	        ����                                                                                                                                                                                                                                                                                                                            
           
           v        b���     �      (          �   	   
       5��                          �               �      5�_�   
                         ����                                                                                                                                                                                                                                                                                                                            (           (           v        b���     �      
   )    5��                          �                      5�_�                            ����                                                                                                                                                                                                                                                                                                                            )           )           v        b���     �         *       �      	   *    5��                         �               �      5�_�                            ����                                                                                                                                                                                                                                                                                                                            6           6           v        b���     �      	   7    5��                          �                      5�_�                    
        ����                                                                                                                                                                                                                                                                                                                            
                      v        b��     �   	      8      2        # Check wether the file exist in the cache   +        with open(filetouse[1:], "r") as f:   &            outputdata = f.readlines()               fileExist = "true"       H        # ProxyServer finds a cache hit and generates a response message   .        tcpCliSock.send("HTTP/1.0 200 OK\r\n")   5        tcpCliSock.send("Content-Type:text/html\r\n")   #        tcpCliSock.send(outputdata)            print('Read from cache')           5��    	                      �       �              5�_�                    	        ����                                                                                                                                                                                                                                                                                                                            
           
           v        b��     �      	          try:5��                          �                      5�_�                    	        ����                                                                                                                                                                                                                                                                                                                            	           	           v        b��     �      	              5��                          �                      5�_�                    	        ����                                                                                                                                                                                                                                                                                                                            	           	           v        b��     �      	           5��                          �                      5�_�                     	        ����                                                                                                                                                                                                                                                                                                                            	           )           v        b��/    �               �                # Name:4proxyServer1.py�                # Time:2022-05-22 11:21:50�                # Blog:�                # Author:liukanglai�                # -*- coding: utf-8 -*-�                 # !usr/bin/env python3�                # Version:V1.0    �      	       !   ,# Error handling for file not found in cache       except IOError:            if fileExist == "false":   0            # Create a socket on the proxyserver   ,            c = socket(AF_INET, SOCK_STREAM)   1            hostn = filename.replace("www.","",1)               print(hostn)               try:   2                # Connect to the socket to port 80   !                c.connect("", 80)   m                # Create a temporary file on this socket and ask port 80 for the file requested by the client   ,                fileobj = c.makefile('r', 0)   L                fileobj.write("GET "+"http://" + filename + " HTTP/1.0\n\n")   /                # Read the response into buffer                    # Fill in start.                   # Fill in end.   H                # Create a new file in the cache for the requested file.   o                # Also send the response in the buffer to client socket and the corresponding file in the cache   4                tmpFile = open("./" + filename,"wb")                    # Fill in start.                   # Fill in end.               except:   (                print("Illegal request")           else:   6            # HTTP response message for file not found               # Fill in start.               # Fill in end.   -    # Close the client and the server sockets       tcpCliSock.close()           # Fill in start.   # Fill in end.5��           !               �                     �                          ~                      �                                                �                                              �                         /                     �                         C                     �                         K                     �                         f                     �                          ~                      5�_�              
   	           ����                                                                                                                                                                                                                                                                                                                            
           
           v        b���     �              �   
             -## Error handling for file not found in cache       except IOError:            if fileExist == "false":   0            # Create a socket on the proxyserver   ,            c = socket(AF_INET, SOCK_STREAM)   1            hostn = filename.replace("www.","",1)               print(hostn)               try:   2                # Connect to the socket to port 80   !                c.connect("", 80)   m                # Create a temporary file on this socket and ask port 80 for the file requested by the client   ,                fileobj = c.makefile('r', 0)   L                fileobj.write("GET "+"http://" + filename + " HTTP/1.0\n\n")   /                # Read the response into buffer                    # Fill in start.                   # Fill in end.   H                # Create a new file in the cache for the requested file.   o                # Also send the response in the buffer to client socket and the corresponding file in the cache   4                tmpFile = open("./" + filename,"wb")                    # Fill in start.                   # Fill in end.               except:   (                print("Illegal request")           else:   6            # HTTP response message for file not found               # Fill in start.               # Fill in end.   -    # Close the client and the server sockets       tcpCliSock.close()        Fill in end.5��    
                     �               �      5��