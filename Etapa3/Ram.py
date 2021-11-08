#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

import random
SIZE_OF_RAM = 16

class Node:
    def __init__(self,index):
        self.page = []
        self.bitR = 0
        self.bitM = 0
        self.space = 0
        self.next = None
#Node ends

class Clock_List:
    def __init__(self):
        self.first = None
        self.hand = None

    def is_empty(self):
        return self.first is None
    
    def append(self,index):
        new_node = Node[index]
        if self.is_empty():
            self.first = new_node
            self.hand = new_node
            new_node.next = self.first
        else:
            tmp = self.first
            while tmp.next is not self.first:
                tmp = tmp.next
            tmp.next = new_node
            new_node.next = self.first

    def show_RAM(self):
        tmp = self.first
        printf(f"Page: {tmp.page} : {tmp.bitR}]\n")
        while tmp.next is not self.first:
            tmp = tmp.next
            printf(f"Page: {tmp.page} : {tmp.bitR}]\n")

    def get_page(self, space):
        tmp = self.first
        while tmp is not None:
            if tmp.space == space:
                tmp.bitR = 1
                return tmp.page
            elif tmp.next == self.first:
                return -1
            else:
                tmp = tmp.next
        return -1

    def create_RAM(self):
        for index in range(0,SIZE_OF_RAM):
            self.append(index)
    
    def clock_algorithm(self):
        tmp = None
        value = []
        while(True):
            if self.hand.bitR == 1 :
                self.hand.bitR = 0
                tmp = self.hand.next
                self.hand = tmp
            else:
                value.append(self.hand.space)
                value.append(self.hand.bitM)
                tmp = self.hand.next
                self.hand = tmp
                break
        return value

    def replace_page(self,space,page):
        tmp = self.first
        while tmp is not None:
            if tmp.space == space:
                tmp.bitR = 0
                tmp.bitM = 0
                tmp.page = page
                return 1
            elif tmp.next == self.first:
                return -1
            else:
                tmp = tmp.next
        return -1

    def modify_page(self,space,page):
        tmp = self.first
        while tmp is not None:
            if tmp.space == space:
                tmp.bitR = 1
                tmp.bitM = 1
                tmp.page = page
                return 1
            elif tmp.next == self.first:
                return -1
            else:
                tmp = tmp.next
        return -1
#Clock list ends