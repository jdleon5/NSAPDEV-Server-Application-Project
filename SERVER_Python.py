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

host = '192.168.0.107'#'172.16.0.52'
port = 8080#25280
threadCounter = 0

disconnect = False  #Might not use

#CLIENT HANDLING
def client_handler(conn):
    list = []
    conn.send(str.encode("Connected to the server."))
    while True:
        try:
            data = conn.recv(1024)
            if data:
                print(data)
                msg = data.decode()
                if(msg == 'Y' or msg == 'y'):
                    break
                else:
                    list.append(msg)
        except Exception as e:
            print(e)
    #create csv file to send to database
    print("Client is disconnecting...")
    print("Saving Data...")
    filename = "Vibration_Data.csv"
    np.savetxt(filename, list, delimiter=", ", fmt='% s')
    print("Data saved on " + filename + "\n")
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
    s.listen(5)

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
