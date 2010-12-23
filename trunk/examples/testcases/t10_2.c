
extern int printline_int(int x);
extern int print_string(char * x);
extern void scan_int(int* num);

int a[10];

int main()
{
  int i,j,tmp;
  for(i=0; i<10; i++)
	  scan_int(&a[i]);

  if (tmp!=0)
    return -1;
  
  for (i=0;i<9;i++)
    for (j=i+1;j<10;j++)
      if (a[i]>a[j])
      {
        tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
      }

  print_string("Result:\n");
  for (i=0;i<10;i++)
    printline_int(a[i]);

  
  return 0;
}
