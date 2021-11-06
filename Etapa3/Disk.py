#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

#Definir maximo
MAX_DISK_SPACE = 1000

MAX_PROVINCES = 30

class Disk:

    def __init__(self):
        self.stringZero = ""
        self.disk_mem = []
        self.IPs_order = []
        self.initial_index = 0

    def add_province(self,ip):
        tmp_ip = ""
        for index in range(0,MAX_PROVINCES - 1):
            if index < 2:
                self.stringZero+= ip[index]
                tmp_ip+= ip[index]
            else:
                self.stringZero+='0'
        tmp_ip = "000"
        self.IPs_order.append(tmp_ip)
        tmp_ip = ""