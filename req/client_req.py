import sys
import time
import random
from datetime import datetime
import zmq

#nos import
import option_req as opt
import action

process_player = 0
Act = action.Action()
RUNNING = True

def first_ia(socket):
    message = Act.watch(socket)
    #message = ['#0x01', 'ok|["#0x01", "empty", "energy", "empty"]']
    identity, cells = message[1].split("[")
    values = cells.split("\"")
    for i in range(1, len(values)):
        if (values[i][0] == '#'):
            Act.attack(socket)
        elif (values[i] == 'energy'):
            Act.gather(socket)
        else:
            num = random.randint(9, 15)
            Act.act_func(num, socket)

def main():
    context = zmq.Context()
    print "Connecting to server..."
    socket = context.socket(zmq.REQ)
    print opt.host, opt.port;
    socket.connect ('tcp://%s:%s' % (opt.host, opt.port))
    Act.identity(socket)
    if (opt.rand_ia == 'true'):
        while RUNNING:
            time.sleep(1)
            print 'in rand'
            num = random.randint(0, 15)
            Act.act_func(num, socket)
    else :
        print 'OK'
        while RUNNING:
           print 'in normal'
           first_ia(socket)



main()
#first_ia()
