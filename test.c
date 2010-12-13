int f(int x)
{
	return x * 2;
}
int main()
{
	int a, b;
	if (!(a || b))
		f(a);
	else
		f(b);
	return 0;
}
