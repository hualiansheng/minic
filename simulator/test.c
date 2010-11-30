
int a;

int Factorial(int n){
  if(n==1) return 1;
  return n*Factorial(n-1);
}

int main(){
  a=1;
  Factorial(6);
  return 0;
}
