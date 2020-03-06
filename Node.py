import socket
import queue

class Node():

    def __init__(self, port):
        self.port_num = port
        self.messages = queue #not sure if this is right, feel free to replace. we just want a message queue
        #make list of other nodes on the network
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            self.socket = s
            s.bind(('', port))
            s.listen()

    def heartbeat(self):
        #send out messages to the other nodes, make sure they are still alive
        return

    def solve(self, number):
        #number is the number to be prime factorized
        #replace with the different types of algorithms
        result = 0
        return result

    def acceptClient(self):
        self.socket.accept()

    def getWork(self):
        data = self.socket.recv()
        #parse data here
        return data

    def disconnect(self):
        self.socket.close()


def main(self):
    server_list = {'0.0.0.0', '127.0.0.1'}

    port = 9999

    node_list = []

    node = Node(port)

    while True:
        node.acceptClient()

        data = node.getWork()

        node.solve(data)

        node.disconnect()



if __name__ == '__main__':
    main()




