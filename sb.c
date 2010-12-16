int main()
{
	int i, j, a[128], temp;
	for (i = 0; i < 128; i++)
		a[i] = 128 - i;
	for (i = 0; i < 128; i++)
		for (j = i+1; j < 128; j++)
			if(a[j] < a[i])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
	return 0;
}

