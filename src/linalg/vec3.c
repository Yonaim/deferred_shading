#include <linalg.h>

t_vec3	vec3(float x, float y, float z)
{
	return ((t_vec3){x, y, z});
}

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return (vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return (vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

t_vec3	vec3_mul(t_vec3 v, float a)
{
	return (vec3(v.x * a, v.y * a, v.z * a));
}

float	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vec3	vec3_hadamard(t_vec3 v1, t_vec3 v2)
{
	return (vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z));
}

t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	t_vec3	res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return (res);
}

float	vec3_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	const float	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	if (length == 0)
		return (vec3(0, 0, 0));
	return (vec3(v.x / length, v.y / length, v.z / length));
}