int print_int(int num);
int print_char(char c);

int mod(int x, int y)
{
	while (x >= y)
		x = x - y;
	return x;
}

int div(int x, int y)
{
	int k;
	for (k = 0; x >= y; k++)
		x = x - y;
	return k;
}

int main()
{
	int a[20], b[20], ans[40], len1, len2, i, j, jw, temp;
	a[0] = 1;
	b[0] = 1;
	ans[0] = 0;
	len1 = 10;
	for (i = 0; i < 40 ; i++)
		ans[i] = 0;
	for (i = 0; i < len1; i++)
		a[i] = 9;
	len2 = 8;
	for (i = 0; i < len2; i++)
		b[i] = 5;
	for (i = 0; i < len1; i++)
		for (j = 0; j < len2; j++)
			ans[i+j] = ans[i+j] + a[i] * b[j];
	jw = 0;
	for (i = 0; i < len1+len2; i++)
	{
		temp = ans[i] + jw;
		ans[i] = mod(temp, 10);
		jw = div(temp, 10);
	}
	for(i=0; i<40; i++){
		print_int(ans[i]);
	}
	print_char('\n');
	return 0;
}
