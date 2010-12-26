int f()
{
	int i;
	if(i > 1) return 1;
	else return 0;
}
int main()
{
	int a[10], b[10], i;
	int temp;
	for(i = 0 ; i < 10 ; i ++){
		temp = a[i];
		a[i] = b[i];
		b[i] = temp;
	}
	return f();
}
