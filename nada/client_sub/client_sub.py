import sys
import zmq
import json
from pprint import pprint

port = "5556"
if len(sys.argv) > 1:
    port =  sys.argv[1]
    int(port)
    
# Socket to talk to server
context = zmq.Context()
socket = context.socket(zmq.SUB)

print "Collecting updates from softwar server..."
socket.connect ("tcp://localhost:%s" % port)

topicfilter = "SoftWar"
socket.setsockopt(zmq.SUBSCRIBE, topicfilter)

# Process 5 updates
total_value = 0
for update_nbr in range (5):
    string = socket.recv()
    topic, messagedata = string.split()
    print topic, messagedata


#test de lecture de json
with open('data.json') as data_file:
    data = json.load(data_file)
pprint(data)
