#include <linalg.h>

void	mat4_identity(float *dst)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			if (i == j)
				dst[i + j * 4] = 1;
			else
				dst[i + j * 4] = 0;
			j++;
		}
		i++;
	}
}

void	mat4_mulmm(float *dst, float *a, float *b)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			dst[i + j * 4] = 0;
			while (k < 4)
			{
				dst[i + j * 4] += a[k + j * 4] * b[i + k * 4];
				k++;
			}
			j++;
		}
		i++;
	}
}