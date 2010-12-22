int main()
{
	int a[10], b[10], i, temp;
	for(i = 0 ; i < 10 ; i ++){
		temp = a[i];
		a[i] = b[i];
		b[i] = temp;
	}
}
