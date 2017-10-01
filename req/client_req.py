import sys
import time
import random
from datetime import datetime
import zmq

#nos import
import option_req as opt
import action
import first_ia

process_player = 0
Act = action.Action()
IA = first_ia.First_IA()
RUNNING = True
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
        IA.ia(socket)
        #while RUNNING:
        #    print 'in normal'



main()
