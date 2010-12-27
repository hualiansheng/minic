// test sb
void print_bin(int x)
{
	int i;// loop variant
	for (i = 0; i < 32; i++)
		printf("abcdefg%bhikilmn'aa'a", (x>>(31-i)) & 1);
	printf("\n");
}

/*
 * afdsadfa
 * asfsadf
 * asdfsaf
 */
int main()
{
	int a;
	a = -1431655765;
	print_bin(a);
	a = ~1430257664;
	print_bin(a);
	a = 1396736;
	print_bin(a);
	a = 1364;
	print_bin(a);
	a = -4016;
	print_bin(a);
	return 0;
}
//the last line
