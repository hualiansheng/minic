import sys
import string

def read_file(file_path):
    global data
    f = file(file_path, "r")
    data = f.read()

def func_call(argv):
    for i in range(0, len(argv)):
        exec(argv[i][0]+" = "+argv[i][1])

read_file(sys.argv[1])
data = string.split(data,"\n")
entry = string.atoi(data[0])
for i in range(1, len(data)):
    data[i-1]=string.split(data[i])

