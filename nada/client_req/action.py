import random

class Action(object):

        def identity(self, socket):
                base_name = "#0x0"
                rand_name = random.randint(1, 9)
                identity  = 'identify|' + base_name + str(rand_name)
                socket.send(identity)
                message = socket.recv()
                if message == 'ko|identity already exists':
                        print message
                        print 'trying with another name'
                        identity()
                elif message == 'ko|game full':
                        print message
                        exit()
                else:
                        return
                        
        def forward(self, socket):
                socket.send('forward|null')
                message = socket.recv()
                print message

        def backward(self, socket):
                socket.send('backward|null')
                message = socket.recv()
                print message
                
        def leftfwd(self, socket):
                socket.send('leftfwd|null')
                message = socket.recv()
                print message
                
        def rightfwd(self, socket):
                socket.send('rightfwd|null')
                message = socket.recv()
                print message
                
        def right(self, socket):
                socket.send('right|null')
                message = socket.recv()
                print message
                
        def left(self, socket):
                socket.send('left|null')
                message = socket.recv()
                print message
                
        def looking(self, socket):
                socket.send('looking|null')
                message = socket.recv()
                print message
                
        def gather(self, socket):
                socket.send('gather|null')
                message = socket.recv()
                print message
                
        def watch(self, socket):
                socket.send('watch|null')
                message = socket.recv()
                print message
                
        def attack(self, socket):
                socket.send('attack|null')
                message = socket.recv()
                print message
                
        def selfid(self, socket):
                socket.send('selfid|null')
                message = socket.recv()
                print message
                
        def selfstats(self, socket):
                socket.send('selfstats|null')
                message = socket.recv()
                print message
                
        def inspect(self, socket, process):
                socket.send('inspect|' + process)
                message = socket.recv()
                print message
        
        def next(self, socket):
                socket.send('next|null')
                message = socket.recv()
                print message
                
        def jump(self, socket):
                socket.send('jump|null')
                message = socket.recv()
                print message
