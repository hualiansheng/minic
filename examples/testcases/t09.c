extern int print_char(char x);
extern int printline_int(int x);

int func1(int x, int y, char z)
{
  if (x != y)
    print_char(z);
  print_char('\n'); 
  return x+y;
}

int main()
{
  printline_int(func1(1,2,'Y'));
}
