import zmq
import random
import action

class First_IA(object):
	Act = action.Action()

	def ia(self, soket):
		Act.looking(soket)
