#include <linalg.h>

void	rotate_x(float *mat, float rad);
void	rotate_y(float *mat, float rad);
void	rotate_z(float *mat, float rad);
void	translate(float *mat, float x, float y, float z);
void	scale(float *mat, float x, float y, float z);

void	projection(float *matrix, float aspect_ratio, float near, float far)
{
	matrix[0] = 1.0f / aspect_ratio;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;
	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;
	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = (far + near) / (far - near);
	matrix[11] = 1.0f;
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = -2 * (far * near) / (far - near);
	matrix[15] = 0.0f;
}

void	view(float *matrix, t_vec3 t, t_vec3 r)
{
	t_vec3 const	v = (t_vec3)
	{sin(r.y) * cos(r.x), sin(r.x), cos(r.y) * cos(r.x)};
	t_vec3 const	u = (t_vec3)
	{-sin(r.y) * sin(r.x), cos(r.x), -cos(r.y) * sin(r.x)};
	t_vec3 const	w = (t_vec3)
	{u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};

	matrix[0] = w.x;
	matrix[1] = u.x;
	matrix[2] = v.x;
	matrix[3] = 0.0f;
	matrix[4] = w.y;
	matrix[5] = u.y;
	matrix[6] = v.y;
	matrix[7] = 0.0f;
	matrix[8] = w.z;
	matrix[9] = u.z;
	matrix[10] = v.z;
	matrix[11] = 0.0f;
	matrix[12] = -(w.x * t.x + w.y * t.y + w.z * t.z);
	matrix[13] = -(u.x * t.x + u.y * t.y + u.z * t.z);
	matrix[14] = -(v.x * t.x + v.y * t.y + v.z * t.z);
	matrix[15] = 1.0f;
}

float const	*model(float *matrix, t_vec3 t, t_vec3 r, t_vec3 s)
{
	mat4_identity(matrix);
	scale(matrix, s.x, s.y, s.z);
	rotate_z(matrix, r.z);
	rotate_x(matrix, r.x);
	rotate_y(matrix, r.y);
	translate(matrix, t.x, t.y, t.z);
	return (matrix);
}
