from graphics import *

def main():

    win = GraphWin('Damier', 400, 400)
    x = 50
    while (x <= 400):
        pt = Point(x, 0)
   	horizontal = Line(pt, Point(x, 400))
   	horizontal.draw(win)
        pt2 = Point(0, x)
        vertical = Line(pt2, Point(400, x))
        vertical.draw(win)
        x = x + 50
    cir = Circle(Point(25, 25), 25)
    cir.setFill('blue')
    cir.draw(win)
    cir = Circle(Point(375, 25), 25)
    cir.setFill('red')
    cir.draw(win)
    cir = Circle(Point(25, 375), 25)
    cir.setFill('yellow')
    cir.draw(win)
    cir = Circle(Point(375, 375), 25)
    cir.setFill('green')
    cir.draw(win)
    win.getMouse()
    win.close()
main()
