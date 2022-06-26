#Enter Hash Key for Authentication
PSHK = "32CAFE015"

#Import modules
import socket            
from time import sleep
import random

#Create a socket object
s = socket.socket()        

#Initialize Variables
ctr = 1
delay = 1 #second/s

#Connect to the server
s.connect(('172.16.0.52', 8080))
    
#Send/Receive data
print (s.recv(1024).decode())
s.send(PSHK.encode())
string = s.recv(1024).decode()
print(string)
if string == "Authentication Successful":
    try:
        while True:
            x = round(random.uniform(-2.00, -1.50),2)
            y = round(random.uniform(7.50, 8.00),2)
            z = round(random.uniform(3.50, 4.00),2)
            string = "{\"time\":" + str(ctr) + ", " + "\"x\":" + str(x) + ", " + "\"y\":" + str(y) + ", " + "\"z\":" + str(z) + "}"
            s.send(string.encode())
            ctr += 1
            sleep(delay)
    except KeyboardInterrupt:
        s.send("Y".encode())
else:
    print("Authentication Failed. (Wrong Pre-Shared Hash Key)")
    s.close()
    # close the connection
s.close()
print("Connection Closed.")