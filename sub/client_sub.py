import sys
import zmq
import json
from pprint import pprint
from graphics import *

win = GraphWin('Softwar', 500, 600)

def clear(win):
    for item in win.items[:]:
        item.undraw()
    win.update()

def clear_label(win, label):
    label.undraw()
    win.update()

def write_notif(win, notif_type):
    if notif_type == '0':
        text = "Cycle info"
    elif notif_type == '1':
        text = "Game started"
    elif notif_type == '2':
        text = "Game finished"
    elif notif_type == '3':
        text = "Client dead"
    elif notif_type == '4':
        text = "Client Win"
    else:
        text = "Notif type does not exist"
    label = Text(Point(100, 550), text)
    label.draw(win)
    return label

def draw_client(win, x, y, identity, gap):
    client_x = x * gap - (gap / 2)
    client_y = y * gap - (gap / 2)
    cir = Circle(Point(client_x, client_y), (gap / 2))
    cir.setFill('yellow')
    cir.draw(win)
    label = Text(Point(client_x, client_y), identity)
    label.draw(win)

def draw_cell(win, x, y, value, gap):
    corner1 = Point((gap * (x - 1)), (gap * (y - 1)))
    corner2 = Point((gap * x), (gap * y))
    cell = Rectangle(corner1, corner2)
    cell.setFill('blue')
    cell.draw(win)
    label = Text(Point((gap * x - gap / 2), (gap * y - gap / 2)), value)
    label.draw(win)

def my_json(win, messagedata):
    data = json.loads(messagedata)
    map_size = data["map_size"]
    clear(win)
    gap = aff_map(win, map_size)
    game_status = data["game_status"]
    energy_cells = data["energy_cells"]
    if energy_cells:
        for j in range(0, len(energy_cells)):
            cell_x = energy_cells[j]["x"] + 1
            cell_y = energy_cells[j]["y"] + 1
            cell_value = energy_cells[j]["value"]
            draw_cell(win, cell_x, cell_y, cell_value, gap)
    players = data["players"]
    if (players != None):
        for i in range(0, len(players)):
            pl_identity = players[i]["identity"]
            pl_x = players[i]["x"] + 1
            pl_y = players[i]["y"] + 1 
            draw_client(win, pl_x, pl_y, pl_identity, gap)
        
def socket(win):
    port = "4243"
    if len(sys.argv) > 1:
        port = sys.argv[1]
        int(port)

    server = "localhost"
    if len(sys.argv) > 2:
        server = sys.argv[2]

    context = zmq.Context()
    socket = context.socket(zmq.SUB)

    print "Collecting updates from softwar server..."
    socket.connect ("tcp://%s:%s" %(server, port))
    
    topicfilter = "Softwar"
    socket.setsockopt(zmq.SUBSCRIBE, topicfilter)

    label = None

    while True:
        string = socket.recv()
        topic, notif_type, messagedata = string.split(" ", 2)
        print notif_type
        if notif_type == '0':
            my_json(win, messagedata)
        if label != None:
            clear_label(win, label)
        label = write_notif(win, notif_type)
        
def close(win):
    win.getMouse()
    win.close()

def aff_map(win, map_size):
    gap = 500 / map_size
    x = gap
    while (x <= 500):
        pt = Point(x, 0)
   	horizontal = Line(pt, Point(x,500))
   	horizontal.draw(win)
        pt2 = Point(0, x)
        vertical = Line(pt2, Point(500, x))
        vertical.draw(win)
        x = x + gap
    return gap

def main():
    socket(win)
    close(win)    

main()
