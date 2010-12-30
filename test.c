void print_int(int x);
int main()
{
	int i, a;
	i = 1;
	if(i++ && i == 1) a = 1;
	else a = 2;
	print_int(a);
}
