import socket
from test_support import verbose

#hostname = '204.181.152.60'
hostname = '10.20.30.40'
PORT = 5007

#  Need to refine the following after os module has been ported
try:
    import palmnet
    palmnet.open()
except ImportError:
    pass


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
if verbose:
    print 'Client connecting'
s.connect((hostname, PORT))
msg = 'this is a socket test'
s.send(msg)
data = s.recv(1024)
if msg <> data:
    print 'Client/Server mismatch'
s.close()
