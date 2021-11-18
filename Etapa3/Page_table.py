#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

MAX_NODES = 20 # Nodos de 72bytes dentro de los 1440 de espacio del disco

class Node:
    def __init__(self,value):
        self.page = value
        self.frecuency = 0
        self.ram_space = value
        self.next = None

    def __repr__(self):
        return self.frecuency
#Node ends

class List:
    def __init__(self):
        self.first = None
        self.max_nodes = MAX_NODES

    def append(self,value):
        New = Node(value)
        if self.first is None:
            self.first = New
        else:
            tmp = self.first
            while(tmp.next):
                tmp = tmp.next
            tmp.next = New

    def show_page_table(self):
        value = self.first
        while(value):
            print(f"[Page: {value.page} : {value.frecuency}]\n")
            value = value.next
    
    def init_list(self):
        for index in range(1, MAX_NODES + 1):
            self.append(index)

    def search_table(self,target): #Buscar
        is_found = False
        tmp = self.first
        frecuency = -1
        while(is_found == False):
            if(tmp):
                if tmp.page == target:
                    is_found = True            
                    frecuency = tmp.frecuency
                tmp = tmp.next
            else:
                is_found = False
        return frecuency

    def modify_table(self,new_page,space):
        temp = self.first
        while temp is not None:
            if temp.ram_space == space:
                temp.frecuency = 0
                temp.ram_space = temp.page
            if temp.page == new_page:
                temp.frecuency = 1
                temp.ram_space = space
            temp = temp.next


    def get_ram_space(self,page_num):
        tmp = self.first
        while tmp is not None:
            if tmp.frecuency !=0:
                if tmp.page == page_num:
                    return tmp.ram_space
            tmp = tmp.next
    
    def get_space(self,space):
        tmp = self.first
        while tmp is not None:
            if tmp.frecuency !=0:
                if tmp.ram_space == space:
                    return tmp.page
                elif tmp.next == self.first:
                    return -1
                else:
                    tmp = tmp.next
            else:
                tmp = tmp.next
        return -1
#List ends