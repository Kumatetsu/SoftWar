import random

class Action(object):

        def identity(self, socket):
                base_name = "#0x0"
                rand_name = random.randint(1, 9)
                identity  = 'identify|' + base_name + str(rand_name)
                socket.send(identity)
        
        def forward(self, socket):
                socket.send('forward|null')

        def backward(self, socket):
                socket.send('backward|null')

        def leftfwd(self, socket):
                socket.send('leftfwd|null')

        def rightfwd(self, socket):
                socket.send('rightfwd|null')

        def right(self, socket):
                socket.send('right|null')

        def left(self, socket):
                socket.send('left|null')

        def looking(self, socket):
                socket.send('looking|null')

        def gather(self, socket):
                socket.send('gather|null')

        def watch(self, socket):
                socket.send('watch|null')

        def attack(self, socket):
                socket.send('attack|null')

        def selfid(self, socket):
                socket.send('selfid|null')

        def selfstats(self, socket):
                socket.send('selfstats|null')

        def inspect(self, socket, process):
                socket.send('inspect|' + process)

        def next(self, socket):
                socket.send('next|null')

        def jump(self, socket):
                socket.send('jump|null')
