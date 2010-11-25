#include <stdio.h>
int main()
{
	char *a, *b;
	a = "peng lao ban shi sb";
	b = "peng lao ban shi sb";
	a[0] = 'b';
	printf("%p\n%p\n", a, b);
	return 0;
}
