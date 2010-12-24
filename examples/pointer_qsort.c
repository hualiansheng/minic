int print_int(int a);
int print_ln();
int print_char(char c);
int src[128];
int rand_int(int i);
int main()
{
	int i;
	for(i = 0; i <128; i++)
		src[i] = 128-i;
	for(i = 0; i <128; i++)
	{
		print_int(src[i]);
		print_char(' ');
	}
	print_ln();
	qsort(src, src + 127);
	for(i = 0; i <128; i++)
	{
		print_int(src[i]);
		print_char(' ');
	}
	return 0;
}

int qsort(int* begin, int* end)
{
	int *p, *q, *r, pivot;
	int crap;
	if (begin >= end)
		return 0;
	pivot = *begin;
	r = begin;
	q = end;
	for (p = begin+1; p <= q; crap)
	{
		for (crap ; q > r && *q > pivot; q--)
			;
		if (q > r)
		{
			*r = *q;
			r = q--;
		}
		for (crap ; p < r && *p <= pivot; p++)
			;
		if (p < r)
		{
			*r = *p;
			r = p++;
		}
	}
	*r = pivot;
	qsort(begin, r - 1);
	qsort(r + 1, end);
	return 0;
}
