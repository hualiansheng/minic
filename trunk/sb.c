//1 2 3 4 6
int a,b;
int print_int(int x);
int print_char(char x);
int main()
{
	int a[10],b,i,j,k;
	i = 0;
	j = 1;
	b = i + j;
	for(k = 0 ; k < 10; k++)
	{
		a[i+j] = i + j;
		if(k<5)
			i++;
		else
			b = i + j;
		b = i + j;
	}
	for(i = 1 ; i < 5; i++)
	{	
		print_int(a[i]);
		print_char('\n');
	}
	print_int(b);
}
