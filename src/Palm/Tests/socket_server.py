import socket
import select
from test_support import verbose

try:
    import palmnet
    palmnet.open()
    palmnet.settimeout(20000)
except ImportError:
    pass

hostname = ''
#hostname = '204.181.152.60'
#hostname = '10.20.30.40'
PORT = 5007


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print "socket: ", s
s.bind((hostname, PORT))
print "socket bound"
print "beginning to listen"
s.listen(1)
if verbose:
    print 'Server accepting'
conn, addr = s.accept()
if verbose:
    print 'connected by', addr
# couple of interesting tests while we've got a live socket
f = conn.fileno()
if verbose:
    print 'fileno:', f
p = conn.getpeername()
if verbose:
    print 'peer:', p
n = conn.getsockname()
if verbose:
    print 'sockname:', n
try:
    f = conn.makefile()
except AttributeError:
    print "we don't have makefile"
if verbose:
    print 'file obj:', f
while 1:
    data = conn.recv(1024)
    if not data:
        break
    if verbose:
        print 'received:', data
    conn.send(data)
conn.close()

# close the server -
s.shutdown(2)
s.close()

