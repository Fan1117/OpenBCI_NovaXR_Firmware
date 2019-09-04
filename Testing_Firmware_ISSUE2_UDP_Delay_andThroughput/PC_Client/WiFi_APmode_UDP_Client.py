'''
Author: Ioannis Smanis
Project: NovaXR PC UDP_Client  
Goal: Test the recieving throughput to the Host Side
The Host Computer that run this script acts as a UDP Client
Server Mode:  Acccess Point Mode
QA Test:  ISSUE2
'''


import socket
import datetime as dt
import time
import sys


serverName = '192.168.4.1' # type your device/server IP address
serverPort = 8090

# Create a UDP socket
UDP_client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send a Test packet
message = bytes("ST", "utf-8")
sent = UDP_client.sendto(message,(serverName,serverPort))


# Keep recieving all packets are sent 
while(1):

	try:
		startT=time.time()
		endT=startT +1.0
		totalSize=0
		i=0
		addr=0

		# ---------------------------------------------
		poinT1=time.time()
		# -- measure recieved data during 1 second ----
		while time.time() <= endT:
			data, server = UDP_client.recvfrom(2048)
			i=i+1
			totalSize = totalSize + (len(data)+1)
		# ---------------------------------------------
		poinT2=time.time()
		# ---------------------------------------------

		duration=poinT2-poinT1

		if duration <= 1.09:
			print("Recieved: ",totalSize, " Bytes/Second " , i, " RX-Buffers")

		totalSize =0
		i=0

	except KeyboardInterrupt:
	    UDP_client.close
	    break