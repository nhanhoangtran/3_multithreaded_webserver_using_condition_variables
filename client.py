#!/usr/bin/env python3


import sys
import socket

# HOST = '127.0.0.1'  # The server's hostname or IP address
HOST = '10.60.1.80'  # SMP8673B IP address
PORT = 8989        # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    # s.sendall(b'test_files/1.c\n')
    send_data = 'test_files/' + str(sys.argv[1]) + '.c\n'
    print(send_data)
    s.sendall(send_data.encode())
    
    while True:
    	data = s.recv(1024).splitlines()
    	if not data:
    		break
    	for line in data:
	    	print(line)
        	# print (repr(line))

