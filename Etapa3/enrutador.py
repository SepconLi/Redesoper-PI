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
import threading, queue
from threading import Thread
import re
import networkx as nx
from networkx.drawing.layout import planar_layout

# Tabla de enrutamiento
class table_node:
    # IP_destino Port_destino IP_enviar Port_enviar HOPS
    def __init__(self, ip, port, ip_dest, port_dest, hops):
        self.ip = ip
        self.port = port
        self.ip_dest = ip_dest
        self.port_dest = port_dest
        self.hops = hops
        pass

    def print_route(self):
        print("\t\t\t[" + str(self.ip) + ", " + str(self.port) + ", " + str(self.ip_dest) + ", " + str(self.port_dest) + ", " + str(self.hops) + "]")
        pass

    def to_string(self):
        return "[t," + str(self.ip) + "," + str(self.port) + "," + str(self.ip_dest) + "," + str(self.port_dest) + "," + str(self.hops) + "]"
    
    def compare_to(self, origin, dest, hops):
        if(self.port == origin and self.port_dest == dest and self.hops == hops):
            return True
        return False
            
class enrutador:
    #T,idProvincia,Puerto,Hops
    def __init__(self, file, file_mutex, print_mutex, mutex_red, queue):
        #Paralelismo
        self.mutex_red = mutex_red
        self.file_mutex = file_mutex
        self.print_mutex = print_mutex
        self.queue = queue
        #Inicializacion
        self.file_route = file
        #Datos de este router
        self.my_ip = ""
        self.my_port = 0
        self.neighbors_number = 0
        self.neighbors_ip = []
        self.neighbors_port = []
        #Array de routers en la red
        self.vector_routers = []# 1024, 1124, 1324
        #Edges para Dijkstra, aqui estan todas las rutas de distancia 1 de cada router
        self.edges = []
        #Tabla de enrutamiento
        self.routing_table = []
        pass

    # Sacar informacion de el router actual
    def read_initial_configuration(self, thread_number):
        self.file_mutex.acquire()
        f = open(self.file_route, "r")
        # Saltar info de routers ajenos
        for i in range(0, thread_number):
            f.readline()# Saltar linea del nombre del router
            f.readline()# Saltar ip
            f.readline()# Saltar puerto
            neigh_num = f.readline()
            for i in range(0, int(neigh_num)):
                f.readline()
                f.readline()
        # Leer mi router
        f.readline()# Saltar linea del nombre del router
        aux = f.readline()
        aux = aux[0:(len(aux)-1)]
        self.my_ip = aux# Guardar la ip/host
        aux = f.readline()
        aux = aux[0:(len(aux)-1)]
        self.my_port = aux# Guardar el puerto
        aux = f.readline()# Leer numero de vecinos
        self.neighbors_number = int(aux)
        for i in range(0, self.neighbors_number):
            tmp = f.readline()#Leyendo ip de vecino
            tmp = tmp[0:(len(tmp)-1)]
            self.neighbors_ip.append(tmp)
            tmp = f.readline()#Leyendo puerto de vecino
            tmp = tmp[0:(len(tmp)-1)]
            self.neighbors_port.append(tmp)
        f.close()
        self.file_mutex.release()
        pass


    def update_routing_table(self):
        self.routing_table.clear()#Limpio la tabla
        for i in range(0, self.neighbors_number):#Meto mis vecinos
            node = table_node("127.0.0.1", self.neighbors_port[i], "127.0.0.1", self.neighbors_port[i], 1)
            self.routing_table.append(node)
        G = nx.Graph()
        for i in self.vector_routers:
            G.add_node(i)
        G.add_edges_from(self.edges)
        for i in self.vector_routers:
            print(str(i) + ":")
            for k in self.vector_routers:
                if i == k:
                    continue
                else:
                    length = nx.shortest_path_length(G, source=i, target=k, weight='hops')
                    print(f"\tfrom {str(i)} to {str(k)} : " + str(length) + " hops")
                    if(k == self.neighbors_port[0]):#Como solo hay un vecino en este test local
                        self.routing_table.append(table_node("127.0.0.1", k, "127.0.0.1", i, length+1))

    # TODO: Hacer esta vara conectado con lo de Oper
    def initialize_routing_table(self):
        for i in range(0, self.neighbors_number):
            node = table_node(self.neighbors_ip[i], self.neighbors_port[i], self.neighbors_ip[i], self.neighbors_port[i], 1)
            self.routing_table.append(node)
            self.edges.append(( node.port, node.port_dest, {'hops':node.hops} ))
        pass

    def print_router_info(self):
        print("My_ip: " + self.my_port)
        print("Routing table: ")
        for i in range(0, len(self.routing_table)):
            print("\t\t", end = "")
            self.routing_table[i].print_route()
        print()
        pass

    def create_routing_table_message(self):
        separation_char = ","
        message = "\nMy routing table: "
        message += "[" + self.my_ip + separation_char + self.my_port + "]\n"
        for i in range(0, len(self.routing_table)):
            message += "\t" + self.routing_table[i].to_string() + "\n"
        return message

    '''
        def resend_routing_table(self, msg):
            
            pass
    '''

    # El primer char era 't'
    def analize_routing_table_message(self, msg):
        split_array = msg.split(",")
        flag = True
        # 127.0.0.1 1224 127.0.0.1 1324 1
        table_length = int(len(split_array)/5)
        for i in range(0, table_length, 5):
            node = table_node(split_array[i], split_array[i+1], split_array[i+2], split_array[i+3], split_array[i+4])
            #Llena vector_routers
            for i in range(0, len(self.vector_routers)):
                if(self.vector_routers[i] == node.port):
                    flag = False
            if(flag == True):
                self.vector_routers.append(node.port) 
            #for i in range(0, len(self.vector_routers)): //Imprimir ports en la lista
                #print(self.vector_routers[i])
            #for i in range(0, len(self.edges)):
            flag = True
            for i in range(0,len(self.edges)):
                origin = 0
                dest = 0
                hops = 0
                edg = str(self.edges[i])
                m = re.search("'(.*?)', ",edg)
                if m:
                    origin = int(m.group(1))
                c = re.search(", '(.*?)',",edg)
                if c:
                    dest = int(c.group(1))
                z = re.search("{'hops': '(.*?)'}",edg)
                if z:
                    hops = int(z.group(1))

                if(node.compare_to(origin,dest,hops)):
                    flag = False
            if(flag == True):
                self.edges.append((node.port, node.port_dest, {'hops':int(node.hops)}))
        #self.update_routing_table()
        pass

    # El primer char era 'm'
    def process_data_message(self, msg):
        pass

    def process_entry_message(self, msg):
        # "t,127.0.0.1,1324,127.0.0.1,1424,1"
        message_type = msg[0]
        if(message_type == 't'):# El mensaje recibido es la routing table de algun router
            self.analize_routing_table_message(msg[2:len(msg)])
            #self.resend_routing_table(msg)
        elif(message_type == 'm'):# El mensaje son los datos que tiene que recibir/redirigir
            self.process_data_message(msg[2:len(msg)])
        else:
            print("Error en tipo de paquete recibido: ", end = "")
            print(msg[0])
        pass

    # Procesa las entradas de un cliente enviandolas al metodo de procesar entrada
    def on_new_client(self, conn, adrr):
        msg = "a"
        while True:
            msg = conn.recv(1024)
            self.print_mutex.acquire()
            print("\tStoring in: ", end = "")
            print(self.my_port, end = " ")
            print(msg)
            self.print_mutex.release()
            self.queue.put(msg.decode())
        conn.close()

    # Manda todas las entradas a un thread propio 
    def listener_thread(self):
        s = socket.socket()
        s.bind((self.my_ip, int(self.my_port)))
        s.listen(1)
        while True:
            conn, addr = s.accept()
            msg = conn.recv(1024)
            self.print_mutex.acquire()
            print("\t\t\t\tRecibi: ",end="")
            print(msg)
            self.print_mutex.release()
            conn.close()
        self.print_mutex.acquire()
        print("Thread (listener/server): "+str(self.my_port)+" ded")
        self.print_mutex.release()
        pass

    # TODO: Conectar esto con Oper y que la cola venga de la memoria
    def sender_thread(self):
        msg = self.queue.get(block=True, timeout=None)
        c = socket.socket()
        while True:
            # TODO: Hacer ram ram, vrum vrum, oper oper con la cola
            # Saca de la cola el siguiente mensaje a mandar
            self.print_mutex.acquire()
            print("Router: " + str(self.my_port) + ":\tWaiting in Queue")
            self.print_mutex.release()
            msg = self.queue.get(block=True, timeout=None)
            self.mutex_red.acquire()
            try:
                c.connect((self.neighbors_ip[0], int(self.neighbors_port[0])))
                self.print_mutex.acquire()
                print("\t\t\tConnecting: <" + str(self.my_port) + "> <" + str(self.neighbors_port[0]) + ">")
                self.print_mutex.release()
                c.send(msg.encode('utf-8'))
                c.close()
                self.mutex_red.release()
            except:
                self.print_mutex.acquire()
                print("\t\t\tFailed connecting: <" + str(self.my_port) + "> <" + str(self.neighbors_port[0]) + ">")
                self.print_mutex.release()
        self.print_mutex.acquire()
        print("Thread (sender/client): "+str(self.my_port)+" ded")
        self.print_mutex.release()
        pass

    def run_server(self):
        self.queue.put("t,127.0.0.1,1124,127.0.0.1,1224,1", block=True, timeout=None)
        _thread.start_new_thread(self.listener_thread, ())
        time.sleep(1)
        self.sender_thread()
# FIN DE CLASE ENRUTADOR ---------------------------------------------------------
queue = queue.Queue()#Test Queue

def run_router(thread_number, file_mutex, print_mutex, mutex_red):
    router = enrutador("1_vecino.txt", file_mutex, print_mutex, mutex_red, queue)
    router.read_initial_configuration(thread_number)
    router.initialize_routing_table()
    print(router.create_routing_table_message())
    router.run_server()
    pass

def simulate(routers_number):
    file_mutex = threading.Lock()
    print_mutex = threading.Lock()
    mutex_red = threading.Lock()
    for i in range(0, routers_number):
        _thread.start_new_thread(run_router, (i,file_mutex,print_mutex,mutex_red,))
    print("Hello from main thread, I will sleep")
    time.sleep(5)
    pass

def serial_router():
    mutex_red = threading.Lock()
    file_mutex = threading.Lock()
    print_mutex = threading.Lock()
    router = enrutador("1_vecino.txt", file_mutex, print_mutex, mutex_red, queue)
    router.read_initial_configuration(0)
    router.initialize_routing_table()
    print(router.create_routing_table_message())
    router.process_entry_message("t,127.0.0.1,1124,127.0.0.1,1224,1")
    router.process_entry_message("t,127.0.0.1,1224,127.0.0.1,1324,1")
    router.process_entry_message("t,127.0.0.1,1324,127.0.0.1,1424,1")
    router.process_entry_message("t,127.0.0.1,1424,127.0.0.1,1024,1")
    router.update_routing_table()
    print(router.create_routing_table_message())
    pass

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    simulate(5)# Multiple routers simulation
    #serial_router()