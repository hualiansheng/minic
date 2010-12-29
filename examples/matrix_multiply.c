// Matrix multiply

int rand_int(int num);
void print_int(int num);
void print_char(char c);
void print_string(char* s);

int matrix1[400];
int matrix2[400];
int result[400];

int main(){
	// initial
	int i, j, k;
	int tmp;
	for(i=0; i<400; i++){
		matrix1[i] = rand_int(10);
		matrix2[i] = rand_int(10);
	}
	// caculate
	for(i=0; i<5; i++)
		for(j=0; j<5; j++){
			tmp = 0;
			for(k=0; k<5; k++)
				tmp = tmp + matrix1[i*5+k]*matrix2[k*5+j];
			result[i*5+j] = tmp;
		}
	// output
	print_string("Matrix1:\n");
	for(i=0; i<5; i++){
		for(j=0; j<5; j++){
			print_int(matrix1[i*5+j]);
			print_char(' ');
		}
		print_char('\n');
	}
	print_char('\n');
	print_string("Matrix2:\n");
	for(i=0; i<5; i++){
		for(j=0; j<5; j++){
			print_int(matrix2[i*5+j]);
			print_char(' ');
		}
		print_char('\n');
	}
	print_char('\n');
	print_string("Result:\n");
	for(i=0; i<5; i++){
		for(j=0; j<5; j++){
			print_int(result[i*5+j]);
			print_char(' ');
		}
		print_char('\n');
	}
	return 0;
}
