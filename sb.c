int f(int a)
{
	return a;
}

int main()
{
	int a, b, c;
	if (a && (b && (a && (b || a))))
		if (a && b)
			f(c);
		else
			f(c+1);
	else
		f(c+2);
	return c;
}

