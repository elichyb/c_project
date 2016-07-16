# include "head.h"

int main()
{
	int n = 0;
	for (n;n<60;n++)
		getname();
	return 0;
}

void getname()
{

	if (pointer > MAX_NAMES-1)
		pointer = 0;
	printf("%s\n", Name[pointer]);
	pointer++;
}