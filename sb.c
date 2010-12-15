int main()
{
	int i, j, a[10], temp;
	for (i = 0; i < 10; i++)
		a[i] = 10 - i;
	for (i = 0; i < 10; i++)
		for (j = i+1; j < 10; j++)
			if(a[j] < a[i])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
	return 0;
}

