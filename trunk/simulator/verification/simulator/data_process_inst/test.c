unsigned int r0 = 0x7fffffff;
unsigned int r1 = 0x00070001;
unsigned int arg3 = 0xffffffff;
int main(){
	printf("0x%x\n",r0&r1);
	printf("0x%x\n",r0^r1);
	printf("0x%x\n",r0-r1);
	printf("0x%x\n",r1-r0);
	printf("0x%x\n",r0+r1);
	printf("0x%x\n",r0|r1);
	printf("0x%x\n",r0&~r1);
	printf("0x%x\n",~r0);
	
	printf("0x%x\n",arg3+1);
	printf("0x%x\n",arg3+1+1);

	printf("0x%x\n",0-1);
	printf("0x%x\n",-1-(-1)+1);
	
}
