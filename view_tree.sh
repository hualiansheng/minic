#!/bin/sh

./minic -xo $1 > out.dot ;
if [ $? -eq 0 ];then
	dot -Tjpeg out.dot > out.jpeg;
	eog out.jpeg &
fi 

