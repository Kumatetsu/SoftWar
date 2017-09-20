class Player(object):
    looking = ""
    energy = 50

    def __init__(self, process_id, signature, position, orientation):
        self.process_id = process_id
        self.signature = signature
        self.position = position
        self.orientation = orientation

    def create_processus(process_id, signature, position, orientation):
        processus = Processus(process_id, signature, position, orientation)
        return processus
