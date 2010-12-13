int f(int a)
{
	return a;
}

int main()
{
	int a, b, c;
	if (((a == b) || (a + 1)) && (b && ((a < b) && ((b >= a) || (a != b)))))
		if (!((a <= b) || (b > a)))
			f(c);
		else
			f(c+1);
	else
		f(c+2);
	return c;
}

