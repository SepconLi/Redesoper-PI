#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"

import Disk as Disk
import Ram as Ram
import TLB as Tlb
import Page_table as Ptable

class Memory:

    def __init__(self):
        self.disk = Disk.Disk()
        self.disk.init_disk("0000")
        self.ram = Ram.Clock_List()
        self.ram.create_RAM()
        self.page = Ptable.List()
        self.page.init_list()
        self.tlb = Tlb.Clock_List()
        self.tlb.init_TLB()

    def get_page(self,page_num):
        space = self.tlb.get_page_space(page_num) #Search in TLB
        if space == -1:
            weight = self.page.search_table(page_num) #If itsn't there, search on page table
            if weight == 0:
                space = self.ram.clock_algorithm() #If either isn't there, searh in RAM
                if space[1] == 1:
                    #if isn't even there, them doesn't exists and we need to use disk
                    virtual_mem = self.page.get_space(space[0]) #So we look for it
                    new_page = self.ram.get_page(space[0])
                    self.disk.modify_page(virtual_mem,new_page)
                virtual_mem = self.page.get_ram_space(page_num) #Simulates disk from RAM (MMU)
                page = self.disk.get_page(virtual_mem)
                self.ram.replace_page(space[0],page)
                self.page.modify_table(page_num,space[0])
                tlb_space = self.tlb.clock_algorithm()
                self.tlb.replace_page(tlb_space,page_num,space[0])
            else: #if its on RAM
                ram_space = self.page.get_ram_space(page_num)
                page = self.ram.get_page(ram_space)
                tlb_space = self.tlb.clock_algorithm()
                self.tlb.replace_page(tlb_space,page_num,ram_space)
        else: #if its on Page table
            page = self.ram.get_page(space)
        return page

