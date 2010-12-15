int data[128];
unsigned int a = 0;

void qsort(int begin, int end){
  if(end <= begin + 1)
    return;
  int i, j, tmp;
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
}

int main(){
  int i;
  a = (unsigned int)data;
  for(i=0; i<128; i++)
    data[i] = 128-i;
  qsort(0, 128);
  return 0;
}
