extern void print_int(int i);
extern void print_char(char c);
extern void print_string(char* s);

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
    data[i] =128-i;
  qsort(data,0, 128);
  print_string("The result is:\n");
  for(i=0; i<128; i++){
    print_int(data[i]);
    print_char(' ');
  }
  print_string("\n");
  return 0;
}
