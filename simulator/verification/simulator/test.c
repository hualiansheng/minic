extern void print_int(int i);
extern void print_char(char c);
extern void print_string(char* s);

int a = 100;
char c = 'b';
char s[100]="ni jiu shi ge sha bi\n";

int main(){
  print_int(100);
  print_char(c);
  print_string(s);
  return 0;
}
