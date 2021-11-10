#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

MAX_NODES = 20 #depende del grupo

class Node:
    def __init__(self,value):
        self.page = value
        self.presencia = 0 #Cambiar nombre a esta variable
        self.ram_space = value
        self.next = None

    def __repr__(self):
        return self.presencia
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

    def print_list(self):
        value = self.first
        while(value):
            print(f"[Page: {value.page} : {value.presencia}]\n")
            value = value.next
    
    def init_list(self):
        for index in range(1, MAX_NODES + 1):
            self.append(index)

    def search_table(self,target):
        is_found = False
        tmp = self.first
        presencia = -1
        while(is_found == False):
            if(tmp):
                if tmp.page == target:
                    is_found = True            
                    presencia = tmp.presencia
                tmp = tmp.next
            else:
                is_found = False
        return presencia

    def modify_table(self,new_page,space):
        temp = self.first
        while temp is not None:
            if temp.ram_space == space:
                temp.presencia = 0
                temp.ram_space = temp.page
            if temp.page == new_page:
                temp.presencia = 1
                temp.ram_space = space
            temp = temp.next
    
    def get_virtual_mem(self,page_num):
        tmp = self.first
        while tmp is not None:
            if tmp.page == page_num:
                return tmp.ram_space
            tmp = tmp.next

    def get_ram_space(self,page_num):
        tmp = self.first
        while tmp is not None:
            if tmp.presencia !=0:
                if tmp.page == page_num:
                    return tmp.ram_space
            tmp = tmp.next
    
    def get_space(self,space):
        tmp = self.first
        while tmp is not None:
            if tmp.presencia !=0:
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