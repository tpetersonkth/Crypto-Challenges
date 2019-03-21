import sys

#Obtained from https://old.nixaid.com/advanced-encryption-standard-aes-with-python/
def GF(a, b):
    r = 0
    for times in range(8):
        if (b & 1) == 1: r = r ^ a
        if r > 0x100: r = r ^ 0x100
        # keep r 8 bit
        hi_bit_set = (a & 0x80)
        a = a << 1
        if a > 0x100:
            # keep a 8 bit
            a = a ^ 0x100
        if hi_bit_set == 0x80:
            a = a ^ 0x1b
        if a > 0x100:
            # keep a 8 bit
            a = a ^ 0x100
        b = b >> 1
        if b > 0x100:
            # keep b 8 bit
            b = b ^ 0x100
    return r


if __name__ == "__main__":
    print("-"*20 +"GF2 table" + "-"*20)
    for i in range(0,256):
        sys.stdout.write("0x{:02x}".format(GF(2,i)) + " ")
        #sys.stdout.write(str(hex(GF(2,i))) + " ")
        if ((i+1) % 16 == 0):
            sys.stdout.write("\n")

    print("")

    print(""+"-"*20 +"GF3 table" + "-"*20)
    for i in range(0,256):
        sys.stdout.write("0x{:02x}".format(GF(3,i)) + " ")
        #sys.stdout.write(str(hex(GF(2,i))) + " ")
        if ((i+1) % 16 == 0):
            sys.stdout.write("\n")
