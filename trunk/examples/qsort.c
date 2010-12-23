void print_int(int i);
void print_char(char c);
void print_string(char* s);
int rand_int(int num);

void qsort(int *data,int begin, int end){
  int i, j, tmp;
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
  qsort(data,begin, j-1);
  qsort(data,j, end);
  return;
}

int main(){
  int i;
  int data[128];
  for(i=0; i<128; i++)
      data[i] = 128-i;
  for(i=0; i<128; i++){
    print_int(data[i]);
    print_char(' ');
  }
  print_char('\n');
  print_char('\n');
  qsort(data,0, 128);
  for(i=0; i<128; i++){
    print_int(data[i]);
    print_char(' ');
  }
  print_char('\n');
  print_char('\n');
  return 0;
}
