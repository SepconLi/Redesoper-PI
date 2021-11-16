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

class enrutador:
    #T,idProvincia,Puerto,Hops
    def __init__(self):
        self.my_ip = "Vacio"
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
        self.my_ip = f.readline()
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
        pass

    def print_router_info(self):
        print("My_ip: " + self.my_ip)
        print("Rooting table: ")
        for i in range(0, len(self.rooting_table)):
            print("\t\t", end = "")
            self.rooting_table[i].print_route()
        print()
        pass

def run_router(thread_number, file_mutex, print_mutex):
    router = enrutador()
    file_mutex.acquire()
    router.read_initial_configuration(thread_number)
    print_mutex.acquire()
    router.print_router_info()
    print_mutex.release()
    file_mutex.release()
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
