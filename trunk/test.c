int main()
{
	int a,b,c,*p,*q;
	if(a == 1)
		p = &a;
	else 
		p = &b;
	*p = 1;
	while(a)
	{
		*p = 2;
		q = p;
		*q = 1;
		q = &c;
		*q = 0;
	}
}
