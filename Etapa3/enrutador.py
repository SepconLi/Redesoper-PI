#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"


# VARIABLE GLOBAL
var = "hola"
import socket
import time
import _thread
import array
#import table_node
import threading
from threading import Thread

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

class table_node:
    #T,idProvincia,Puerto,Hops
    def __init__(self, id_prov, id_port, hops):
        self.id_provincia = id_prov
        self.port = id_port
        self.hops = hops
        pass

    def print_route(self):
        print(str(self.id_provincia) + ", " + str(self.port) + ", " + str(self.hops))
        pass

    def to_string(self):
        return str(self.id_provincia) + "," + str(self.port) + "," + str(self.hops)

class enrutador:
    #T,idProvincia,Puerto,Hops
    def __init__(self):
        self.my_port = 69
        self.neighbors = []
        self.rooting_table = []
        pass
    
    def connect():
        pass
    
    def routing(self, info):
        message = var.decode(info, 'utf8')
        
        pass

    def read_initial_configuration(self, thread_number):
        f = open("cantonal.txt", "r")
        for i in range(0, thread_number):
            f.readline()# Saltar linea del nombre del router
            f.readline()# Saltar ip
            neigh_num = f.readline()
            for i in range(0, int(neigh_num)):
                f.readline()
        f.readline()# Saltar linea del nombre del router
        aux = f.readline()
        aux = aux[0:(len(aux)-1)]
        self.my_port = aux
        my_neighbors_number = f.readline()
        for i in range(0, int(my_neighbors_number)):
            tmp = f.readline()
            tmp = tmp[0:(len(tmp)-1)]
            self.neighbors.append(tmp)
        f.close()
        self.initialize_rooting_table()
        pass

    def initialize_rooting_table(self):
        for i in range(0, len(self.neighbors)):
            self.rooting_table.append(table_node(self.neighbors[i], self.neighbors[i], 1))
        # Revisar cuales vectores/routers ya estan en la tabla
        # Agregar los que faltan con vecino NULL y hops 999999
        pass

    def print_router_info(self):
        print("My_ip: " + self.my_port)
        print("Rooting table: ")
        for i in range(0, len(self.rooting_table)):
            print("\t\t", end = "")
            self.rooting_table[i].print_route()
        print()
        pass

    def create_rooting_table_message(self):
        message = "t," + self.my_port
        for i in range(0, len(self.neighbors)):
            message += "," + self.neighbors[i]
            if(i == len(self.neighbors)+1):
                message += ","
        return message

    def run_server(self, print_mutex):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            print(self.my_port)
            s.bind((HOST, int(self.my_port)))
            s.listen()
            conn, addr = s.accept()
            with conn:
                print('Connected by', addr)
                msg = self.create_rooting_table_message()
                for i in range(0, len(self.rooting_table)):
                    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as c:
                        c.connect((HOST, self.neighbors[i]))
                        c.send(msg.encode('utf-8'))
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    else:
                        print_mutex
                        print("Im the port " + self.my_port + " ", end = "")
                        print(data)
                        print_mutex

def run_router(thread_number, file_mutex, print_mutex):
    router = enrutador()
    file_mutex.acquire()
    router.read_initial_configuration(thread_number)
    #print_mutex.acquire()
    #router.print_router_info()
    #print_mutex.release()
    file_mutex.release()
    #a
    #a
    #a
    #print_mutex.acquire()
    #print("<" + router.create_rooting_table_message() + ">")
    #print_mutex.release()
    router.run_server(print_mutex)
    pass

def create_routers(routers_number):
    file_mutex = threading.Lock()
    print_mutex = threading.Lock()
    for i in range(0, routers_number):
        _thread.start_new_thread(run_router, (i,file_mutex,print_mutex,))
    print("Hello from main thread")
    time.sleep(1)
    pass

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    create_routers(10)

#        s.bind((HOST, PORT))
#        s.listen()
#        conn, addr = s.accept()
#        with conn:
#            print('Connected by', addr)
#            while True:
#                data = conn.recv(1024)
#                if not data:
#                    break
#                else:
#                    print(data)
#                conn.send(data)
