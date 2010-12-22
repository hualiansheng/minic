void print_string(char* s);
void scan_string(char* s);

char s[100];
int i;
int len;
char tmp;

int main(){
	scan_string(s);
	len = 0;
	while(s[len] != 0)
		len = len + 1;
	len --;
	for(i=0; i<len; i++){
		tmp = s[i];
		s[i] = s[len];
		s[len] = tmp;
		len = len -1;
	}
	print_string(s);
	return 0;
}
