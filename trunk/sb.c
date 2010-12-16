int main()
{
	int a, b[10];
	int *p;
	p = &a;
	a = p - b;
	return a;
}
