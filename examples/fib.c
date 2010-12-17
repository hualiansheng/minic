void print_int(int num);

int fib(int n)
{
	if (n == 0 || n == 1)
		return 1;
	return fib(n-1) + fib(n-2);
}
int main()
{
	print_int(fib(10));
}

