import time

if 1:
    print "timezone:", time.timezone
    time.settimezone(7*3600)
#    print "timezone:", time.timezone
    t = time.time()
    print "time.time():", t
    print "gmtime: ", time.gmtime(t)
    print "back to gmtime:", time.gmtime(time.mktime(time.gmtime(t)))
    print "mktime from gmtime:", time.mktime(time.gmtime(t))
    print "localtime: ", time.localtime(t)

    print "mktime: ", time.mktime( time.localtime(t))
    print time.daylight
    if long(time.mktime(time.localtime(t))) <> long(t):
        print 'time.mktime(time.localtime(t)) <> t'

#     print "sleeping 5 seconds"
#     tt = time.time()
#     time.sleep((5,0))
#     print "calculated sleep:", time.time() - tt
#     time.timezone

    print "time to convert:", time.localtime(t)
    print "converted time:", time.asctime(time.localtime(t))
    print "len:", len(time.asctime(time.localtime(t)))
    print "timezone:", time.timezone
#    print time.mktime((2**15,)*9)
    print "time.gmtime(25*3600)", time.gmtime(25*3600)
if 0:

    # expected errors
    try:
        time.asctime(0)
    except TypeError:
        pass

    try:
        time.mktime((999999, 999999, 999999, 999999,
                     999999, 999999, 999999, 999999,
                     999999))
    except OverflowError:
        pass
