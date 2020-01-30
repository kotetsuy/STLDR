import sys
import re

if __name__ == "__main__":
    ostr = ""
    n = 0
    fn = "Split" + str(n) + ".hex"
    fp = open(fn, mode='w')
    n += 1
    while True:
        try:
            istr = input()
            m = re.match("^:[0-9A-F]{2}[0-9A-F]{4}([0-9A-F]{2})([0-9A-F]{4})", istr)
            if m != None and m.group(1) == "04" and (int(m.group(2),16) % 2) == 0:
            #if m != None and m.group(1) == "04":
                ostr += ":00000001FF\n"
                fp.write(ostr)
                fp.close()
                fn = "Spilt" + str(n) + ".hex"
                fp = open(fn, mode='w')
                n += 1
                ostr = ""
                print(m.group(1,2))
            ostr += istr + "\n"
        except EOFError:
            break
    fp.write(ostr)
    fp.close()
