int main()
{
	int i, j, a[10], temp;
	for (i = 0; i < 10; i++)
		a[i] = 10 - i;
	for (i = 1; i < 10; i++)
	{
		for (j = 0; j < 10-i; j++)
		{
			if (a[j] > a[j+1])
			{
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}			
	}
	return 0;
}

