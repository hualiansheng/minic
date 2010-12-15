int* main()
{
	int *p, a[10];
	a[5] = 1;
	p = &a[5];
	printf("%d\n", *p);
	return p;
}

