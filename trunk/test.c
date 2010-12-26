int print_int(int x);

int main()
{
	int a[10],x;
	a[0] = 255;
	a[1] = 100;
	a[1] = a[1]*512;
	x = a[1];
	print_int(x);
	return 0;
}
