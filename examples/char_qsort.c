void print_int(int i);
void print_char(char c);
void print_string(char* s);
void print_ln();
int rand_int(int num);
char data[128];
void qsort(int begin, int end){
  int i, j;
  char tmp;
  if(end <= begin + 1)
    return;
  j=begin+1;
  for(i=begin+1; i<end; i++){
    if(data[i] < data[begin]){
      tmp = data[j];
      data[j] = data[i];
      data[i] = tmp;
      j++;
    }
  }
  tmp = data[begin];
  data[begin] = data[j-1];
  data[j-1] = tmp;
  qsort(begin, j-1);
  qsort(j, end);
  return;
}

int main(){
  int i;
  char* p;
  p = data;
  for(i= 0 ; i<26; i++)
    data[i] = 90 - i;
  data[26]=0;
  print_string(data);
  for(i=0; i<26; i++){
    print_char(data[i]);
  	print_char(' ');
  }
  print_char('\n');
  qsort(0, 26);
  for(i=0; i<26; i++){
    print_char(data[i]);
	print_char(' ');
  }
  return 0;
}
