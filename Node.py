import socket
import threading
import time

class Node():

    def __init__(self, port):
        self.port_num = port
        # make list of other nodes on the network, not sure if are all needed but we shall see
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket = s
        s.settimeout(.01)
        s.bind(('', port))
        s.listen()

    def heartbeat(self, conn):
        # send out messages to the other nodes, make sure they are still alive
        conn.setTimeout(60)
        conn.send("heartbeat".encode())
        data = conn.recv(1024).decode()
        if data == 0:
            return False
        else:
            return True

    def returnIP(self):
        host = socket.gethostname()
        ip = socket.gethostbyname(host)
        return ip

    def connectNode(self, ip):
        try:
            conn = self.socket.connect((ip, self.port_num))
        except socket.timeout:
            print("timeout")
            time.sleep(1)
        return

    def solve(self, conn):
        # number is the number to be prime factorized
        # replace with the different types of algorithms
        number = self.getWork(conn) #get work calls reciev on the socket
        result = 0

        #send the result and close the socket
        conn.send(str(result).encode())
        conn.close()
        return

    def acceptClient(self):
        #accepts the new clients on the socket, then sends the work in a new thread
        connection, client_address = self.socket.accept()

        #thread solves it with the new connection on the socket
        t = threading.Thread(target=self.solve, args=(connection,))
        t.start()

    def getWork(self, conn):
        data = conn.recv(1024)
        if data == 0:
            print("Connection lost")  # also handle lost connection. make new method
        dataToParse = data.decode()
        # parse data here
        return dataToParse

    def disconnect(self):
        self.socket.close()

    def wrapMessage(self, msg):
        #TODO: Figure out this wrapping
        wrappedmsg = ""
        return wrappedmsg

    def parseMessage(self, msg):
        #TODO: after figuring out wrapping, figure out unwrapping
        return msg


def main():
    # this code is run on each server
    node_list = {'45.76.232.110', '144.202.74.41'}
    peerNodes = []
    port = 10001
    node = Node(port)
    #explicity connect the nodes on the same ip with different ports for simulation purposes

    for currNode in node_list:  # connect every node to every other node
        if currNode != node.returnIP():  # check to make sure ip isnt the node's ip
            peerNodes.append(node.connectNode(currNode))
            time.sleep(3)

    # listen for new connections from clients, send messages to other nodes as needed
    heartbeatThreads = []
    for c in peerNodes:
        heartbeatThreads.append(threading.Thread(target=node.heartbeat, args=(c,)))


    while True:
        node.acceptClient()
        for t in heartbeatThreads:
            if not t.is_alive():
                t.start()

if __name__ == '__main__':
    main()
