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
	int tmp, tmp2;
	for(i=0; i<400; i++){
		matrix1[i] = 400 - i;
		matrix2[i] = 400 - i;
	}
	// caculate
	for(i=0; i<20; i++){
		tmp2=i*20;
		for(j=0; j<20; j++){
			tmp = 0;
			for(k=0; k<20; k++)
				tmp = tmp + matrix1[tmp2+k]*matrix2[k*20+j];
			result[tmp2+j] = tmp;
		}
	}
	// output
	print_string("Matrix1:\n");
	for(i=0; i<20; i++){
		tmp2=i*20;
		for(j=0; j<20; j++){
			print_int(matrix1[tmp2+j]);
			print_char(' ');
		}
		print_char('\n');
	}
	print_char('\n');
	print_string("Matrix2:\n");
	for(i=0; i<20; i++){
		tmp2=i*20;
		for(j=0; j<20; j++){
			print_int(matrix2[tmp2+j]);
			print_char(' ');
		}
		print_char('\n');
	}
	print_char('\n');
	print_string("Result:\n");
	for(i=0; i<20; i++){
		tmp2=i*20;
		for(j=0; j<20; j++){
			print_int(result[tmp2+j]);
			print_char(' ');
		}
		print_char('\n');
	}
	return 0;
}
