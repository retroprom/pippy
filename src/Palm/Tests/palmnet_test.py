
import palmnet

# 10 second timeout

print dir(palmnet)

palmnet.open()

print "setting timeout"
palmnet.settimeout(3)
print "refnum:", palmnet.refnum()
print "getting timeout"
print palmnet.gettimeout()

print "closing net"
palmnet.close()


