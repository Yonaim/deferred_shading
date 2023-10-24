#ifndef VAO_H
#define VAO_H

// vao의 attribute pointer 순서, 쉐이더 프로그램의 layout 번호에서 쓰임
enum attribute_index
{
	POSITION_INDEX,
	NORMAL_INDEX, 
	ALBEDO_INDEX
};

typedef struct s_vao
{
	GLuint	id;
	int		vertices_cnt;
} t_vao;

void vao_init_quad(t_vao *vao_ptr);
void vao_init_cube(t_vao *vao_ptr);

#endif
