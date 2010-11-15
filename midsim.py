import sys
import string

def read_file(file_path):
    global data
    f = file(file_path, "r")
    data = f.read()

def func_call(argv):
    for i in range(0, len(argv)):
        exec(argv[i][0]+" = "+argv[i][1])
    p=3
    print p

read_file(sys.argv[1])
data = string.split(data,"\n")
for i in range(0, len(data)):
    data[i]=string.split(data[i])

a=[]
p=1
a.append(("p","2"))
func_call(a)
print p
