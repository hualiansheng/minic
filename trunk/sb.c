int main()
{
	int i, j, n;
	int a[10], min, temp;
	min = 0; 
	n = 10;
	for(i = 0 ;i < n ; i++){
		min = a[i];
		temp = i;
		for(j = i ; j < n ; j ++){
			if(a[j] < min){
				min = a[j];
				temp = j;		
			}					
		}
		a[temp] = a[i];
		a[i] = min;
	}
}
