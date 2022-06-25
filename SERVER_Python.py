import socket               # Import socket module
from _thread import *
import serial
import threading
import json
import numpy as np
from logging import exception
from tkinter import E

#list = []
running = True

host = '172.16.0.52'
port = 25280
threadCounter = 0

disconnect = False  #Might not use

#CLIENT HANDLING
def client_handler(conn):
    list = []
    conn.send(str.encode("Connected to the server."))
    while True:
        try:
            data= conn.recv(1024)
            msg = data.decode(1024)
            if msg == 'Y' or msg == 'y':
                break
            #place here append to data list
                
            list.append(msg)
                
            # reply = f'Server: {msg}'
            # conn.sendall(str.encode(reply))
        except:
            #create csv file to send to database
            print("Save Data")
            # filename = "Vibration_Data.csv"
            # print("Saving data on " + filename + "\n")
            # np.savetxt(filename, list, delimiter=", ", fmt='% s')
        conn.close()

#CLIENT CONNECTION ACCEPT
def connection_accept(s):
    c, address = s.accept()
    print(f'Connected to: {address[0]}:{str(address[1])}')
    start_new_thread(client_handler, (c, ))

#SERVER START
def start_server(host, port):
    s = socket.socket()
    try:
        s.bind((host, port))
    except socket.error as e:
        print(str(e))
    print(f'Server is listening on the port {port}...')
    s.listen()

    while True:
        connection_accept(s)
        
start_server(host, port)


#PREVIOUS CODE
s.listen(5)                 # Now wait for client connection.
c, addr = s.accept()        # Establish connection with client.
print ('\nSERVER: Got connection from', addr, "\n")
h='Thank you for connecting'
c.send(h.encode())

while 1:
    try:
        data = c.recv(1024)
        if data:
            print(data)
            datastring = data.decode()
            list.append(datastring)
    except:
        print("\nClient has Disconnected.\n")
        disconnect = True
        #Place CSV creation here...
        filename = "Vibration_Data.csv"
        print("Saving data on " + filename + "\n")
        np.savetxt(filename, list, delimiter=", ", fmt='% s')
        c.close()
