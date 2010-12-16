#include <stdio.h>
#include <stdlib.h>

void print_int(int i){
  printf("%d", i);
}

void print_blank(){
  printf(" ");
}

void print_ln(){
  printf("\n");
}

void print_char(char c){
  printf("%c", c);
}

void print_string(char* s){
  printf("%s", s);
}

void scan_int(int* num){
	scanf("%d", num);
}

void scan_char(char* c){
	scanf("%c", c);
}

void scan_string(char* s){
	scanf("%s", s);
}

int rand_int(int mod){
	return rand() % mod;
}

