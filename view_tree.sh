#!/bin/sh

./minic -o $1 > out.dot ;
dot -Tjpeg out.dot > out.jpeg;
eog out.jpeg & 
