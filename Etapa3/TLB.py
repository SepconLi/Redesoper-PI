#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

TLB_SIZE = 8

class Node:
    def __init__(self,index):
        self.page_number = 0
        self.bitR = 0
        self.ram_space = 0
        self.TLB_space = index
        self.next = None

#Node ends

class Clock_List:
    def __init__(self):
        self.first = None
        self.hand = None

    def isEmpty(self):
        return self.first is None

    def append(self,index):
        new_node = Node(index)
        if self.isEmpty():
            self.first = new_node
            new_node.next = self.first
        else:
            temp = self.first
            while temp.next is not self.first:
                temp = temp.next
                new_node.next = self.first

    def show_TLB(self):
        temp = self.first
        print(f"[Page: {temp.page_number} : {temp.bitR}]\n")
        while temp.next is not self.first:
            temp = self.first
            print(f"[Pagina: {temp.page_number} : {temp.bitR}]\n")

    def get_page_space(self, target):
        temp = self.first
        while temp is not None:
            if temp.page_number == target :
                temp.bitR =  1
                return temp.ram_space
            elif temp.next == self.first:
                return -1 #error
            else:
                temp = temp.next
        return -1

    def clock_algorithm(self):
        while(True):
            if self.hand.bitR == 1:
                self.hand.bitR = 0
                tmp = self.hand.next
                self.hand = tmp

            else:
                tmp = self.hand
                tmp2 = self.hand.bitR
                self.hand = tmp2
                return tmp.TLB_space
    
    def replace_page(self, space, new_page, ram_space):
        tmp = self.first
        while tmp is not None:
            if tmp.TLB_space == space:
                tmp.bitR = 0
                tmp.ram_space = ram_space
                tmp.page_number = new_page
                return 1
            elif tmp.next == self.first :
                return -1
            else:
                tmp = tmp.next
        return -1

    def init_TLB(self):
        for index in range (0,TLB_SIZE):
            self.append(index)
        self.hand = self.first

#Clocklist ends