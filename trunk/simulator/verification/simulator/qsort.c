void qsort(int *data,int begin, int end){
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
  qsort(data,begin, j-1);
  qsort(data,j, end);
}

int main(){
  int i;
  int data[20];
  for(i=0; i<20; i++)
    data[i] = 20-i;
  qsort(data,0, 20);
  return 0;
}
