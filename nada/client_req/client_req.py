import sys
import time
import random
from datetime import datetime
import zmq

IP_SERV = '192.168.1.22'
IP_LOCAL = '127.0.0.1'
IP_USED = IP_LOCAL
PORT = 5555

process_player = 0

#c'est juste un debut d'initialisation qui ne marche pas encore
class Processus(object):
    process_id = ""
    signature = ""
    position = ""
    orientation = ""
    energy = 50
    action = 2
    can_play = 1

    def __init__(self, process_id, signature, position, orientation):
        self.process_id = process_id
        self.signature = signature
        self.position = position
        self.orientation = orientation

def create_processus(process_id, signature, position, orientation):
    processus = Processus(process_id, signature, position, orientation)
    return processus

def main():
    context = zmq.Context()
    print "Connecting to server..."
    socket = context.socket(zmq.REQ)
    socket.connect ("tcp://localhost:%s" % PORT)
    for request in range (1,10):
        print "Sending request ", request,"..."
        socket.send ("Hello")
        #  Get the reply.
        message = socket.recv()
        print "Received reply ", request, "[", message, "]"

if __name__ == "__main__":
    main()
