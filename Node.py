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
        self.socket.connect((ip, self.port_num))
        self.activeNodeList.append(ip)

    def solve(self, conn):
        # number is the number to be prime factorized
        # replace with the different types of algorithms
        number = self.getWork(conn)
        result = 0
        conn.send(str(result).encode())
        conn.close()
        return

    def acceptClient(self):
        connection, client_address = self.socket.accept()
        t = threading.Thread(target=self.solve, args=(connection,))
        self.activeClientList.append(client_address)  # when you get internet, see if accept() returns something to append to the list
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
        wrappedmsg = ""
        return wrappedmsg


def main(self):
    # this code is run on each server
    node_list = {'0.0.0.0', '127.0.0.1', '0.0.0.0'}
    port = 9999
    node = Node(port)

    for currNode in node_list:  # connect every node to every other node
        if currNode != node.returnIP():  # check to make sure ip isnt the node's ip
            node.connectNode(currNode)

    # listen for new connections from clients, send messages to other nodes as needed
    while True:
        node.acceptClient()


if __name__ == '__main__':
    main()
