import zmq
import time
import sys

context = zmq.Context()
socket = context.socket(zmq.REP)
port = 4242

socket.bind("tcp://*:%s" % port)

while True:
        #  Wait for next request from client
            message = socket.recv()
            print "Received request: ", message
            time.sleep (1)
            socket.send("World from %s" % port)
