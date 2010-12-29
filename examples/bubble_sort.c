int rand_int(int mod);
void print_int(int num);
void print_char(char c);
void print_ln();

int main()
{
	int i, j, a[1000], temp;
	for (i = 0; i < 1000; i++){
		a[i] = rand_int(1000);
	}
	for(i=0; i<1000; i++){
		print_int(a[i]);
		print_char('\n');
	}
	print_char('\n');
	print_char('\n');
	for (i = 0; i < 1000; i++)
		for (j = i+1; j < 1000; j++){
			if(a[j] < a[i])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	for(i=0; i<1000; i++){
		print_int(a[i]);
		print_char('\n');
	}
	print_ln();
	print_ln();
	return 0;
}

