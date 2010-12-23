#include <string.h>

void pre_compile(char* src, char* dst){
	int i;
	int len;
	len = strlen(src);
	for(i=0; i<len; i++){
		if(src[i]== '\\' && src[i-1] == '\\'){
			int pos = i;
			int in_string = 0;
			while(1){
				if(pos == 0 || src[pos] == '\n')
					break;
			}
		}
	}
}
