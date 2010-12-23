
extern void print_int(int x);
extern void print_ln();

int main()
{
  int a, b, c;
  a = 1;
  b = 2;
  c = 3;
  print_int( ((((((a)))))) + b * a);
  print_ln();
  c = + a + (+b);
  print_int(c);
  print_ln();
  c = -a - (-b);
  print_int(c);
  print_ln();
  return 0;
}
