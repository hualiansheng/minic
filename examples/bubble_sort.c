int rand_int(int mod);
void print_int(int num);
void print_char(char c);
void print_ln();

int main()
{
	int i, j, a[128], temp;
	for (i = 0; i < 128; i++)
		a[i] = rand_int(128);
	for(i=0; i<128; i++){
		print_int(a[i]);
		print_char(10);
	}
	print_char(10);
	print_char(10);
	for (i = 0; i < 128; i++)
		for (j = i+1; j < 128; j++)
			if(a[j] < a[i])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
	for(i=0; i<128; i++){
		print_int(a[i]);
		print_char(10);
	}
	print_ln();
	print_ln();
	return 0;
}

