a=0
b=0

def addToGlobal(x,y):
    global a
    global b
    count=3
    while(count>0):
        a = a + x
        b = b + y
        count = count -1

def setGlobal():
    global a
    global b
    if(1<2):
        a = 1
        if(1<2):
            b = 1
    else:
        a = 0
        b = 0

def oprOnGlobal():
    global a
    global b
    b = 2*b+15-14
    a = a*b-a
    return a+b
}

def main():
    setGlobal()
    count = 5
    while(count > 2):
        addToGlobal(1,1)
        if(count == 4 or count==5):
            addToGlobal(1,1)
        else:
            if(count+3 == 6):
                addToGlobal(3,3)
            else:
                addToGlobal(0,0)

    res = oprOnGlobal()
    if(res >0 and res< 256):
        return res
    else:
        return 0


# Boilerplate
if __name__ == "__main__":
    import sys
    ret=main()
    sys.exit(ret)

