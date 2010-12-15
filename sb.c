int main()
{
	int i,j, a[10],tmp;
	for(i = 0 ; i < 10 ; i++)
		a[i] = 10-i;

	for(i = 0; i < 10 ; i++)
		for(j = i; j < 10; j++)
			if( a[j] < a[i])
			{
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
				
	return a;
}

