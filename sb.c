int f(int a){
	if(a<=1) return 1;
	return f(a-1)+f(a-2);
}

int main(){
	int a;
	a = 5;
	a = f(a);
	return 1;
}
