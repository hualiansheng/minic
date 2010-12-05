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
	return 0;
}
