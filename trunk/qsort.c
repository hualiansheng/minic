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
  int i, j;
  int data[128];
  for(i=0; i<8; i++)
    for(j=0; j<16;j++)
		data[i*16+j] = j;
  qsort(data,0, 128);
  return 0;
}
