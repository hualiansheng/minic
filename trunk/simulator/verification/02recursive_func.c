int a = 6;

int Factorial(int n){
  if(n==1) return 1;
  return n*Factorial(n-1);
}

int main(){
  a = Factorial(a);
  return 0;
}
