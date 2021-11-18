#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

import socket
import time
import _thread
import array
#import table_node
import threading, queue
from threading import Thread
from enrutador import enrutador

def run_router(thread_number, file_mutex, print_mutex):
    router = enrutador("1_vecino.txt")
    file_mutex.acquire()
    router.read_initial_configuration(thread_number)
    file_mutex.release()
    router.initialize_rooting_table()
    router.run_server(print_mutex)
    pass

def create_routers(routers_number):
    file_mutex = threading.Lock()
    print_mutex = threading.Lock()
    for i in range(0, routers_number):
        _thread.start_new_thread(run_router, (i,file_mutex,print_mutex,))
    print("Hello from main thread")
    time.sleep(15)
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
