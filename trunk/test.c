int f(int a, int b, int c)
{
	return a + b + c;
}
int main()
{
	int a, b, temp;
	b = 64;
	a = 100;
	while (b)
	{
		temp = b;
		b = a - b;
		a = temp;
	}
	b = f(a*2, f(2+b,b,b-2), a+b);
	return 0;
}
