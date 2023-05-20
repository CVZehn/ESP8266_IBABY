import os 
import binascii
 
 
# 读取二进制文本并显示为16进制
def readBinfile(binFile_path:str):
    with open(binFile_path, 'rb') as f:
        num = 0
        while 1:
            a = f.read(1)
            if not a:
                break
 
            if num % 16 == 0:
                print()
            hexstr = binascii.b2a_hex(a)
            print('0x'+ hexstr.decode().upper() + ',', end=' ')
            num += 1
 
 
if __name__ == '__main__':
	readBinfile('.\\1.bin')