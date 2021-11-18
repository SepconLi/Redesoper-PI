#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

#Definir maximo
MAX_DISK_SPACE = 1440 # 20 Nodos de 72 de espacio cada uno

#Esto puede cambiar segun las decisiones grupales 
MAX_PACKAGE_LENGTH = 30

class Disk:

    def __init__(self):
        self.stringZero = ""
        self.disk_mem = []
        self.IPs_order = []
        self.initial_index = 0

    def add_province(self,ip): #Recibe ip = (#ID)(#ID)(IP)
        tmp_ip = ""
        for index in range(0,MAX_PACKAGE_LENGTH - 1):
            if index < 2:
                self.stringZero+= ip[index]
                tmp_ip+= ip[index]
            else:
                self.stringZero+='0'
        tmp_ip = "|||" # Separador
        self.IPs_order.append(tmp_ip)
        tmp_ip = ""
        my_id = ip[2]
        for index in range (3,MAX_PACKAGE_LENGTH-1):
            my_id += ip[index]
        self.IPs_order.append(my_id)


    def init_disk(self,my_ip):
        tmp_ip = ""
        tmp_ip = my_ip
        self.add_province(tmp_ip)
        tmp_ip = tmp_ip.rstrip(tmp_ip[-1])
        for index in range(1,9):
            new_ip = tmp_ip+str(index)
            self.add_ip(new_ip)
            tmp_ip = my_ip

    def modify_page(self,page_num, page):
        space = page_num * 72
        char = ''
        counter = 0
        for index in range(space, space+72):
            char = page[counter]
            self.disk_mem[index] = char
            counter += 1
        
    def get_page(self,page_num):
        page = page_num - 1
        space = page * 72
        info = []
        for index in range(space, space + 72):
            info.append(self.disk_mem[index])
        return info

#disk ends
