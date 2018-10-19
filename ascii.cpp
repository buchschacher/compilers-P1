#include <cstdio>
#include <cstdlib>
#include <cctype>

int main()
{
	for (int i = 0; i < 128; i++)
	{
		if (ispunct(i))
		{
			printf("%c: %d\n", i, i);
		}
	}
	return 0;
}
