import os.path
import os
import string
import sys

files = os.listdir(sys.argv[1]);

count=0

for i in files:
    if(os.path.isfile(i)):
        if(i[-2:len(i)]==".c" or i[-2:len(i)]==".h" or i[-2:len(i)]==".l" or i[-2:len(i)]==".y"):
            f=open(sys.argv[1]+"/"+str(i), "r")
            data=f.read()
            data = string.split(data, "\n")
            length = 0
            for j in data:
                if(len(j) != 0):
                    length = length + 1
            print str(i)+" : "+str(length)+" lines."
            count=count+length

print count
    
