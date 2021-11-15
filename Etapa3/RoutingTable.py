#!/usr/bin/env python3

import socket

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 65432        # The port used by the server

class clientTCP:
    def __init__(self):
        pass

    # Meter 0 a la izquierda
    def fill_integer(self, number):
        four_bytes = str(number)
        fill = 4 - len(four_bytes)
        for i in range(0, fill):
            four_bytes = '0' + four_bytes
        return four_bytes

    # Tipo: Mensaje de Datos de Vacunacion
    def encode_data_vac(self, type, prov_destino, canton_destino, area_salud_destino, data):
        message = type + self.fill_integer(prov_destino) + self.fill_integer(canton_destino) \
            + self.fill_integer(area_salud_destino) + data
        print(message)
        return message

    # Recibe un unicode como b'M000200710321Tamborlin, el grande'
    # Lo procesa con su tabla de enrutamiento para reenviarlo o dejarselo
    def process_message(self, msg):
        print('\n\nProcessing message:\n')
        type = msg[0:1]
        prov_destino = msg[1:5]
        canton_destino = msg[5:9]
        area_salud_destino = msg[9:13]
        data = msg[13:len(msg)]
        print(type)
        print(prov_destino)
        print(canton_destino)
        print(area_salud_destino)
        print(data)
        pass
    #
    #   Prov RoutingTable
    #   Cant RoutingTable
    #   Oficina RoutingTable
    #   
    #