#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

#Definir maximo
MAX_DISK_SPACE = 1000

#Esto puede cambiar segun las decisiones grupales 
MAX_PACKAGE_LENGTH = 30

STRING_MAX_LENGTH = 33 # Puede cambiar segun estandares del grupo

class Disk:

    def __init__(self):
        self.stringZero = ""
        self.disk_mem = []
        self.IPs_order = []
        self.initial_index = 0

    def add_province(self,ip):
        tmp_ip = ""
        for index in range(0,MAX_PACKAGE_LENGTH - 1):
            if index < 2:
                self.stringZero+= ip[index]
                tmp_ip+= ip[index]
            else:
                self.stringZero+='0'
        tmp_ip = "000"
        self.IPs_order.append(tmp_ip)
        tmp_ip = ""
        #Esto puede cambiar por los estandaraes de nuestro grupo
        my_id = ip[2]
        my_id += my_id[3]
        #####################################################
        my_partner = self.get_relative(my_id)
        tmp = my_id[0]
        tmp += my_id[1]
        if my_partner < 10:
            tmp += '0'
            tmp += str(my_partner)
        else:
            tmp += str(my_partner)
        tmp_ip = tmp
        tmp_ip += '0'
        self.IPs_order.append(tmp_ip)
        for index in range(0,MAX_PACKAGE_LENGTH - 1):
            if index < 4:
                self.stringZero += tmp[index]
            else:
                self.stringZero += '0'
    def add_ip(self,ip):
        self.IPs_order.append(ip)
        for index in range (0,MAX_PACKAGE_LENGTH - 1):
            if index < 5:
                self.stringZero += ip[index]
            else:
                self.stringZero += '0'
    
    def init_disk(self,my_ip):
        tmp_ip = ""
        tmp_ip = my_ip
        self.add_province(tmp_ip)
        tmp_ip = tmp_ip.rstrip(tmp_ip[-1])
        for index in range(1,9):
            new_ip = tmp_ip+str(index)
            self.add_ip(new_ip)
            tmp_ip = my_ip
            self.pair_cantons(tmp_ip)
            self.copy_string_disk()

    def get_relative(self,my_id):
        id_int = int(my_id)
        if id_int % 2 == 0 :
            id_int = id_int - 1
            return id_int
        else:
            id_int = id_int + 1
            return id_int

    def pair_cantons(self,my_ip):
        my_id = my_ip[2]
        my_id += my_ip[3]
        my_partner = self.get_relative(my_id)
        tmp = my_ip[0]
        tmp += my_ip[1]
        for index in range(1,STRING_MAX_LENGTH):
            temp = tmp
            if(index != int(my_id)):
                if index < 10:
                    if index != my_partner:
                        temp += '0'
                        temp += str(index)
                        temp += '0'
                        self.add_ip(temp)
                    else:
                        if index != my_partner:
                            temp += str(index)
                            temp += '0'
                            self.add_ip(temp)
    def modify_page(self,page_num, page):
        space = page_num * 58
        char = ''
        counter = 0
        for index in range(space, space+58):
            char = page[counter]
            self.disk_mem[index] = char
            counter += 1
        
    def get_page(self,page_num):
        page = page_num - 1
        space = page * 58
        info = []
        for index in range(space, space + 58):
            info.append(self.disk_mem[index])
        return info

    def copy_string_disk(self):
        for index in range(0,MAX_DISK_SPACE):
            self.disk_mem.append(self.stringZero[index].encode())

    def know_page_ip(self,ip):
        index = self.IPs_order(ip)
        print(self.IPs_order)
        print(len(self.IPs_order))
        if index % 2 == 0 :
            index = int((index+2) / 2)
        else:
            index = int((index+1) / 2)
        print(index)

#disk ends
