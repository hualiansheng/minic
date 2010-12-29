#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void pre_compile(char* filename){
	char *src, *dst;
	FILE *fr, *fw;
	char input[500], output[500];
	int i;
	int len;
	int pos;
	sprintf(input, "%s.c", filename);
	sprintf(output, "%s.p", filename);
	fr = fopen(input, "r");
	if(fr == NULL){
		fprintf(stderr, "File %s is not existed.\n", input);
		exit(1);
	}
	fw = fopen(output, "w+");
	if(fw == NULL){
		fprintf(stderr, "Error : file %s open error.\n", output);
		exit(1);
	}
	fseek(fr, 0, SEEK_END);
	len = ftell(fr);
	//printf("length : %d\n", len);
	src = malloc(len * sizeof(char) + 1);
	dst = malloc(len * sizeof(char) + 1);
	fseek(fr, 0, SEEK_SET);
	fread(src, len*sizeof(char), 1, fr);
	i = 0;
	pos = 0;
	len = strlen(src);
	while(i<len){
		if(src[i] == '/' && src[i+1] == '/')
			while(i<len && src[i] != '\n')
				i++;
		else if(src[i] == '/' && src[i+1] == '*'){
			i = i + 2;
			while(i<len){
				if(src[i-1] == '\n'){
					dst[pos] = '\n';
					pos++;
					i++;
				}
				else if(src[i-1] == '/' && src[i-2] == '*')
					break;
				else
					i++;
			}
		}
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
	//printf("Initial : \n%s\n", src);
	//printf("\n\nTarget : %s\n", dst);
	fputs(dst, fw);
	fclose(fr);
	fclose(fw);
	free(src);
	free(dst);
}

