#ifndef LINALG_H
#define LINALG_H

# include <math.h>

# define DEG_TO_RAD(degrees) ((degrees) * M_PI / 180.0)

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}t_vec3;

// MVP
void	projection(float *matrix, float aspect_ratio, float near, float far);
void	view(float *matrix, t_vec3 t, t_vec3 r);
float const	*model(float *matrix, t_vec3 t, t_vec3 r, t_vec3 s);

// matrix4
void	mat4_identity(float *this);
void	mat4_mulmm(float *dst, float *a, float *b);

// vector3
t_vec3	vec3(float x, float y, float z);
t_vec3	vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_mul(t_vec3 v, float a);
float	vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_hadamard(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2);
float	vec3_length(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);

#endif