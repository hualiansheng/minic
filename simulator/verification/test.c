
int Factorial(int n){
  if(n==1) return 1;
  return n*Factorial(n-1);
}

int main(){
  Factorial(6);
  return 0;
}
