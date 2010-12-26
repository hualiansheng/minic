#!/bin/sh

./minic -b $1 > out.dot ;
if [ $? -eq 0 ];then
	dot -Tpng out.dot > out.png;
	eog out.png &
fi 

