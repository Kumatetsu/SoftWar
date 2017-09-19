import random

def identity():
        base_name = "#0x"
        rand_name = random.randint(01, 99)
        identity  = 'identify|' + base_name + str(rand_name)
        return identity
