int a = 6;

int main(){
  int sum;
  sum = 1;
  while(1){
    if(a == 1)
      break;
    sum = sum * a;
    a --;
  }
  return sum;
}
