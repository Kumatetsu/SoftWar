import argparse

parser = argparse.ArgumentParser(description='Connect your client to Softwar server')

parser.add_argument('--host', default="localhost")
parser.add_argument('--port', default="4242")
parser.add_argument('--rand', default="false")

args = parser.parse_args()

port = args.port
host = args.host
rand_ia = args.rand
