#include <math.h>

void	rotate_x(float *mat, float rad)
{
	float const	s = sin(rad);
	float const	c = cos(rad);
	float		tmpy;
	float		tmpz;
	int			i;

	i = 0;
	while (i < 4)
	{
		tmpy = mat[1 + 4 * i] * c + mat[2 + 4 * i] * s;
		tmpz = mat[1 + 4 * i] * -s + mat[2 + 4 * i] * c;
		mat[1 + 4 * i] = tmpy;
		mat[2 + 4 * i] = tmpz;
		i++;
	}
}

void	rotate_y(float *mat, float rad)
{
	float const	s = sin(rad);
	float const	c = cos(rad);
	float		tmpx;
	float		tmpz;
	int			i;

	i = 0;
	while (i < 4)
	{
		tmpx = mat[4 * i] * c + mat[2 + 4 * i] * -s;
		tmpz = mat[4 * i] * s + mat[2 + 4 * i] * c;
		mat[4 * i] = tmpx;
		mat[2 + 4 * i] = tmpz;
		i++;
	}
}

void	rotate_z(float *mat, float rad)
{
	float const	s = sin(rad);
	float const	c = cos(rad);
	float		tmpx;
	float		tmpy;
	int			i;

	i = 0;
	while (i < 4)
	{
		tmpx = mat[4 * i] * c + mat[1 + 4 * i] * s;
		tmpy = mat[4 * i] * -s + mat[1 + 4 * i] * c;
		mat[4 * i] = tmpx;
		mat[1 + 4 * i] = tmpy;
		i++;
	}
}

void	translate(float *mat, float x, float y, float z)
{
	mat[12] += x;
	mat[13] += y;
	mat[14] += z;
}

void	scale(float *mat, float x, float y, float z)
{
	mat[0] *= x;
	mat[5] *= y;
	mat[10] *= z;
}
