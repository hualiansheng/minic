import os.path
import os
import string
import sys

files = os.listdir(sys.argv[1]);

count=0

for i in files:
    if(os.path.isfile(i)):
        if(i[-2:len(i)]==".c" or i[-2:len(i)]==".h"):
            f=open(sys.argv[1]+"/"+str(i), "r")
            data=f.read()
            data = string.split(data, "\n")
            count=count+len(data)

print count
    
