int f(int a, int b, int c)
{
	return a + b + c;
}
char g()
{
	char c;
	return c;
}

int main()
{
	int a, b, temp;
	int c[10];
	int *p;
	b = *c;
	a = c[0];
	while (b)
	{
		temp = b;
		b = a - b;
		a = temp;
	}
	g();
	b = f(a*2, f(2+b,b,b-2), a+b);
	p = &a;
	b = *p;
	return 0;
}
