int f(int x, int y)
{
	if (x == 0)
		return 0;
	return f(x-1, y-1) + f(x-2, y-2);
}

int main()
{
	int a, b, c;
	int *p;
	a = 1;
	c = 2;
	p = &b;
	p = p + 1;
	*p = 5;
	c = f(a, c);
	return c;
}
