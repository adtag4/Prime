import socket

class User():
    def __init__(self, ip, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((ip, port))

    def parseInput(self, work):
        parsedWork = 0
        return parsedWork

    def heartbeat(self):
        msg = "alive"
        wrappedmsg = self.wrapMessage(msg)
        self.sock.send(wrappedmsg.encode())
        return

    def getAnswer(self):
        return self.sock.recv(4096).decode()

    def requestWork(self, work):
        workmsg = self.wrapMessage(str(work))
        self.sock.send(workmsg.encode())

    def disconnect(self):
        self.sock.close()

    def wrapMessage(self, msg):
        wrappedmsg = ""
        return wrappedmsg

def sendMenu():
    """TODO: Fill this out so that algorithm for prime calculation can be selected"""
    return

def main():
    node_list = {'0.0.0.0', '127.0.0.1'}

    server_list = []

    port = 9999

    for ip in node_list:
        server_list.append(User(ip, port))

    for s in server_list:
        #Parse arguments from command line here, depends on alg
        s.requestWork("prime factorization number")

    notSolved = True

    while notSolved:
        ## Receive up to 4096 bytes from a peer
        #put select to deal with the different servers
        data = 0
        for s in server_list:
            s.heartbeat()
            data = s.getAnswer()
            if data == 0:
                print("connection broken")
            if len(data) > 0:
                notSolved = False
                break
        print(data)

    for s in server_list:
        #after recieve answer, close on all sockets
        ## Close the socket connection, no more data transmission
        s.disconnect()

if __name__ == '__main__':
    main()


