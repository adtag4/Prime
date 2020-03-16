import socket
import queue
import threading


class Node():

    def __init__(self, port):
        self.port_num = port
        self.messages = queue  # not sure if this is right, feel free to replace. we just want a message queue
        # make list of other nodes on the network
        self.activeNodeList = []
        self.activeClientList = []
        self.clientConnections = []
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            self.socket = s
            s.bind(('', port))
            s.listen()

    def heartbeat(self):
        # send out messages to the other nodes, make sure they are still alive
        for node in self.activeNodeList:
            self.socket.send("heartbeat".encode())
        return

    def returnIP(self):
        return self.socket.gethostname()

    def connectNode(self, ip):
        conn = self.socket.connect((ip, self.port_num))
        self.activeNodeList.append(ip)
        return conn

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
        self.activeClientList.append(client_address)
        self.clientConnections.append(connection)
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


def main(self):
    # this code is run on each server
    node_list = {'45.76.232.110', '144.202.74.41'}
    peerNodes = []
    port = 10001
    port2 = 10002
    node = Node(port)

    #explicity connect the nodes on the same ip with different ports for simulation purposes
    #peerNodes.append(node.connectNode(node2))

    for currNode in node_list:  # connect every node to every other node
        if currNode != node.returnIP():  # check to make sure ip isnt the node's ip
            peerNodes.append(node.connectNode(currNode))

    # listen for new connections from clients, send messages to other nodes as needed
    while True:
        node.acceptClient()


if __name__ == '__main__':
    main()
