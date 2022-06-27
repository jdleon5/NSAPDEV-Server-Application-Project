import socket               # Import socket module
from _thread import *
import numpy as np
import datetime

running = True

host = '172.16.0.52'
port = 8080 
threadCounter = 0
PSHK = "32CAFE015"
Authenticated = False 

#CLIENT HANDLING
def client_handler(conn, addr):
    list = []
    data = conn.recv(1024)
    PSHKEncoded = PSHK.encode() #Need to encode PSHK to compare with Hash Key of Client Device 
    data.decode()
    if PSHKEncoded == data:
        print("Authentication Successful")
        while True:
            try:
                data = conn.recv(1024)
                if data:
                    msg = data.decode()
                    if len(msg) >= 10:
                        print(data)
                    if(msg == 'Y' or msg == 'y'):
                        Authenticated = True
                        break
                    else:
                        list.append(msg)
            except Exception as e:
                print(e)
    else:
        print("Authentication Failed")
        Authenticated = False
        conn.close()
    #create csv file to send to save
    if Authenticated == True:
        print("Client is disconnecting...")
        print("Saving Data...")
        dateNtime = str(datetime.datetime.now()).replace(":", "-").split(".")
        filename = "./CSV_DATA/" + "ADXLData" + " " +  addr + " " + dateNtime[0] + ".csv"
        np.savetxt(filename, list, delimiter=", ", fmt='% s')
        print("Data saved on " + filename + "\n")
        conn.close()

#CLIENT CONNECTION ACCEPT
def connection_accept(s):
    c, address = s.accept()
    print(f'Connected to: {address[0]}:{str(address[1])}')
    start_new_thread(client_handler, (c, address[0], ))

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