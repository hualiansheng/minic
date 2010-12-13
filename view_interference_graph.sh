#!/bin/sh

./minic -g $1 > out.dot ;
if [ $? -eq 0 ];then
	dot -Teps out.dot > out.eps;
	eog out.jpeg &
fi 

