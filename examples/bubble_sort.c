int rand_int(int mod);
void print_int(int num);
void print_char(char c);
void print_ln();

int main()
{
	int i, j, a[128], temp, b[128], c[128];
	for (i = 0; i < 128; i++){
		a[i] = 128-i;
		b[i] = 128 - i;
		c[i] = 128-i;
	}
	for(i=0; i<128; i++){
		print_int(a[i]);
		print_int(b[i]);
		print_int(c[i]);
		print_char('\n');
	}
	print_char('\n');
	print_char('\n');
	for (i = 0; i < 128; i++)
		for (j = i+1; j < 128; j++){
			if(a[j] < a[i])
			{
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
			if(b[j] < b[i])
			{
				temp = b[i];
				b[i] = b[j];
				b[j] = temp;
			}
			if(c[j] < c[i])
			{
				temp = c[i];
				c[i] = c[j];
				c[j] = temp;
			}
		}
	for(i=0; i<128; i++){
		print_int(a[i]);
		print_int(b[i]);
		print_int(c[i]);
		print_char('\n');
	}
	print_ln();
	print_ln();
	return 0;
}

