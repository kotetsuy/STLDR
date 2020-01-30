import sys

if __name__ == "__main__":
    args = sys.argv
    with open(args[1], 'rb') as f:
        c = f.read()
    print("uint8_t array[] = {")
    r = 0
    s = ""
    for i in range(len(c)):
        r = 1
        s += "0x" + format(c[i], "x") + ","
        if i % 16 == 15:
            print(s)
            s = ""
            r = 0
    if r == 1:
        print(s)
    print("};")
