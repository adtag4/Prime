import socket

class User():
    def __init__(self, ip, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.sock.connect((ip, port))

    def parseInput(self, work):
        parsedWork = 0
        return parsedWork

    def heartbeat(self):
        return

    def getAnswer(self):
        return self.socket.recv(4096)

    def requestWork(self, work):
        self.sock.send(self.parseInput(work))

    def disconnect(self):
        self.socket.close()



def sendMenu():
    return


def main():
    node_list = {'0.0.0.0', '127.0.0.1'}

    server_list = []

    port = 9999

    for ip in node_list:

        server_list.add(User(ip, port))

    for s in server_list:
        #Parse arguments from command line here, depends on alg
        s.requestWork("prime factorization number")

    notSolved = True

    while notSolved:
        ## Receive up to 4096 bytes from a peer
        #put select to deal with the different servers
        for s in server_list:
            s.hearbeat()
            data = s.getAnswer()
            if data > 0:
                notSolved = False
                break

    print(data)

    for s in server_list:
        #after recieve answer, close on all sockets
        ## Close the socket connection, no more data transmission
        s.disconnect()

if __name__ == '__main__':
    main()


