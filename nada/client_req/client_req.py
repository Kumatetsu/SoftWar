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

def main():
    context = zmq.Context()
    print "Connecting to server..."
    socket = context.socket(zmq.REQ)
    socket.connect ('tcp://%s:%s' % (opt.host, opt.port))
    Act.identity(socket)
    message = socket.recv()
    if message == 'ko|game full':
            print message
    else:
        while true:
            socket.send("hello")

if __name__ == "__main__":
    main()
