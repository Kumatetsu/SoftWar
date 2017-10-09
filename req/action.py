import random
import zmq

class Action(object):

        name = ""

        def identity(self, socket):
                base_name = "#0x0"
                rand_name = random.randint(1, 9)
                full_name = base_name + str(rand_name)
                identity  = 'identify|' + full_name
                self.name = full_name
                print 'action: ' + identity
                socket.send_multipart([full_name, identity])
                message = socket.recv_multipart()
                print message[1]
                if message[1] == 'ko|identity already exists':
                        print message
                        print 'trying with another name'
                        self.identity(socket)
                elif message[1] == 'game full':
                        print message
                        exit()
                else:
                        return()
                        
        def forward(self, socket):
                print 'action: ' + 'forward|null'
                socket.send_multipart([self.name, 'forward|null'])
                message = socket.recv_multipart()
                print message
                if message == 'ko|null':
                        watch(socket)

        def backward(self, socket):
                print 'action: ' + 'backward|null'
                socket.send_multipart([self.name, 'backward|null'])
                message = socket.recv_multipart()
                print message
                
        def leftfwd(self, socket):
                print 'action: ' + 'leftfwd|null'
                socket.send_multipart([self.name, 'leftfwd|null'])
                message = socket.recv_multipart()
                print message
                
        def rightfwd(self, socket):
                print 'action: ' + 'rightfwd|null'
                socket.send_multipart([self.name, 'rightfwd|null'])
                message = socket.recv_multipart()
                print message
                
        def right(self, socket):
                print 'action: ' + 'right|null'
                socket.send_multipart([self.name, 'right|null'])
                message = socket.recv_multipart()
                print message
                
        def left(self, socket):
                print 'action: ' + 'left|null'
                socket.send_multipart([self.name, 'left|null'])
                message = socket.recv_multipart()
                print message
                
        def looking(self, socket):
                print 'action: ' + 'looking|null'
                socket.send_multipart([self.name, 'looking|null'])
                message = socket.recv_multipart()
                print message
                
        def gather(self, socket):
                print 'action: ' + 'gather|null'
                socket.send_multipart([self.name, 'gather|null'])
                message = socket.recv_multipart()
                print message
                
        def watch(self, socket):
                print 'action: ' + 'watch|null'
                socket.send_multipart([self.name, 'watch|null'])
                message = socket.recv_multipart()
                print message
                
        def attack(self, socket):
                print 'action: ' + 'attack|null'
                socket.send_multipart([self.name, 'attack|null'])
                message = socket.recv_multipart()
                print message
                
        def selfid(self, socket):
                print 'action: ' + 'selfid|null'
                socket.send_multipart([self.name, 'selfid|null'])
                message = socket.recv_multipart()
                print message
                
        def selfstats(self, socket):
                print 'action: ' + 'selfstats|null'
                socket.send_multipart([self.name, 'selfstats|null'])
                message = socket.recv_multipart()
                print message
                
        def inspect(self, socket):
                process = '#0x0' + str(random.randint(0, 9))
                print 'action: ' + 'inspect|' + process
                socket.send_multipart([self.name, 'inspect|' + process])
                message = socket.recv_multipart()
                print message
        
        def next(self, socket):
                print 'action: ' + 'next|null'
                socket.send_multipart([self.name, 'next|null'])
                message = socket.recv_multipart()
                print message
                
        def jump(self, socket):
                print 'action: ' + 'jump|null'
                socket.send_multipart([self.name, 'jump|null'])
                message = socket.recv_multipart()
                print message
                
        def act_func(self, num, socket) :
                func = [
                        self.jump,
                        self.next,
                        self.inspect,
                        self.selfstats,
                        self.selfid,
                        self.attack,
                        self.gather,
                        self.watch,
                        self.looking,
                        self.right,
                        self.left,
                        self.forward,
                        self.backward,
                        self.forward,
                        self.rightfwd,
                        self.leftfwd
                ]
                func[num](socket)
