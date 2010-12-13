int main()
{
	int a, b, c;
	if (a && (b && (a && (b || a))))
		if (a && b)
			c = 1;
		else
			c = 2;
	else
		c = 3;
	return c;
}

