int f(int* a, int b, int c)
{
	a[b] = c;
	return 1;
}
int main()
{
	int a[100];
	int i, j;
	int max;
	a[i] = j;
	i=0;
	j=1;
	a[i]=100;
	a[j]=200;
	for(i = 0 ; i < 100 ; i++)
		a[i] = 1;
	f(a, i, j);
}


