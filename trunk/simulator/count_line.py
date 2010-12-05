import os.path
import os
import string
import sys

files = os.listdir(sys.argv[1]);

count=0

for i in files:
    if(os.path.isfile(i)):
        if(i[-2:len(i)]==".c" or i[-2:len(i)]==".h" or i[-2:len(i)]==".y" or i[-2:len(i)]==".l"):
            if(i!="minic.yy.c" and i!="minic.tab.c" and i!="minic.tab.h" and i!="minic.yy.h"):
                f=open(sys.argv[1]+"/"+str(i), "r")
                data=f.read()
                data = string.split(data, "\n")
                print i+" : "+str(len(data))+" lines."
                for j in data:
                    if(len(j)>0):
                        count=count+1

print "----Total : " + str(count) + " lines.----"
    
