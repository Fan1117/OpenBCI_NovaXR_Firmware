
'''
Author: Ioannis Smanis
Project: NovaXR PC server side 
Goal: Test the recieving throughput to the PC Server Side
'''
import socket
import datetime as dt
import time
import sys


s = socket.socket()
#s.bind(('192.168.3.100', 8090 ))
s.bind(('', 8000))
s.listen(0)            

client, addr = s.accept()


while True:
 
	
	startT=time.time()
	endT=startT +1.0
	totalSize=0
	i=0
	addr=0

	
	while time.time() <= endT:
		data = client.recv(1024)
		#data,addr = s.recvfrom(2048)
		print(data.decode('utf-8'))
		print(len(data))
		i=i+1
		totalSize = totalSize + len(data)
 


	print(totalSize, "Recieved Bytes/Second" , i, "RX-Buffers")
	totalSize =0
	i=0
