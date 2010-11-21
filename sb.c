int num[100000];
int shai(int x)
{
	int k;
	k = x * 2;
	while (k < 100000)
	{
		num[k] = 1;
		k = k + x;
	}
	i++;
}
int main()
{
	int i;	
	for (i = 2; i < 100000; i++)
	{
		if (num[i] == 0)
			shai(i);
	}
}
