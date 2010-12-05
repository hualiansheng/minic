int main()
{
	int i, n, min, temp;
	min = 0; 
	n = 10;
	for(i = 0 ;i < n ; i++) {
		temp = i - 1;
		if(temp < min)
			temp = i;
		min = i + 1;
	}
}

