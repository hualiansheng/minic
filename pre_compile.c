#include <string.h>
#include <stdio.h>

void pre_compile(char* src, char* dst){
	int i, j;
	int len;
	int pos;
	i = 0;
	pos = 0;
	len = strlen(src);
	while(i<len){
		if(src[i] == '/' && src[i+1] == '/')
			while(i<len && src[i] != '\n')
				i++;
		else if(src[i] == '/' && src[i+1] == '*')
			while(i<len)
				if(src[i-1] == '/' && src[i-2] == '*')
					break;
				else
					i++;
		else if(src[i] == '\''){
			dst[pos] = src[i];
			i++;
			pos++;
			while(i<len && src[i] != '\''){
				dst[pos] = src[i];
				i++;
				pos++;
			}
			dst[pos] = src[i];
			i++;
			pos++;
			continue;
		}else if(src[i] == '"'){
			dst[pos] = src[i];
			i++;
			pos++;
			while(i<len && src[i] != '"'){
				dst[pos] = src[i];
				i++;
				pos++;
			}
			dst[pos] = src[i];
			i++;
			pos++;
			continue;
		}else{
			dst[pos] = src[i];
			i++;
			pos++;
		}
	}
	dst[pos] = '\0';
}


//int main(){
//	char input[500]="#include <iostream> //abc\nint main(){\n /*abc\naaa\naaa\n*/ \n return 0;\n}\n";
//	char output[500];
//	printf("%s\n", input);
//	pre_compile(input, output);
//	printf("%s\n", output);
//	return 0;
//}

