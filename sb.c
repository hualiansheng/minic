int f(int a,int b,int c,int d,int e)
{
	return a+b+c+d+e;
}
int main()
{
	int a,b,c,d;
	a = f(a,b,c,a+b,c+d);
	return a;
}

