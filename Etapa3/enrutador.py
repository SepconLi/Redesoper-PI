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
import threading
from threading import Thread

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)


class enrutador:
    def __init__(self):
        self.my_ip = "Vacio"
        self.neighbors = []
        pass
    
    def connect():
        pass
    
    def routing(self, info):
        message = var.decode(info, 'utf8')
        
        pass

    def read_initial_configuration(self, thread_number):
        f = open("provincial.txt", "r")
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
            self.neighbors.append(f.readline())
        f.close()
        pass

    def print_router_info(self):
        print("My_ip: " + self.my_ip + "\n")
        for i in range(0, len(self.neighbors)):
            print("\tNeighbor: " + self.neighbors[i])
        pass

def run_router(thread_number, mutex):
    router = enrutador()
    print("Hello from thread ", end = "")
    print(thread_number)
    mutex.acquire()
    router.read_initial_configuration(thread_number)
    router.print_router_info()
    mutex.release()
    pass

def create_routers(routers_number):
    mutex = threading.Lock()
    for i in range(0, routers_number):
        _thread.start_new_thread(run_router, (i,mutex,))
    print("Hello from main thread")
    time.sleep(3)
    pass

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    create_routers(5)

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
