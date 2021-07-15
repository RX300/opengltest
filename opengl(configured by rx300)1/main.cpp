//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#ifndef _STADX_H
#define _STADX_H
#include "stadx.h"
#endif // !stadx.h



enum VAO_IDs { Triangles, NumVAOs=3 };
enum Buffer_IDs { ArrayBuffer, NumBuffers=3 };
enum Attrib_IDs { vPosition = 0 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint ebo[1];
GLint  uniformindex;
GLfloat timeLoc=0;


const GLuint  NumVertices = 6;

//----------------------------------------------------------------------------
//
// init
//
GLfloat  vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f,  0.90f },{  0.90f, -0.85f },  { -0.85f,  0.90f }   // Triangle 2
	};
	GLfloat  vertices2[NumVertices][2] = {
{ -0.50f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f,  0.90f },{  0.90f, -0.85f },  { -0.85f,  0.90f }   // Triangle 2
	};
	// A single triangle
static const GLfloat vertex_positions[] =
	{
		-0.5f, 0.5f,  0.0f, 1.0f,//����������
		-0.5f, -0.5f,  0.0f, 1.0f,//����������
		0.5f,  -0.5f,  0.0f, 1.0f,//����������
		0.5f, 0.5f, 0.0f, 1.0f,//����������
	};
static const GLfloat vertex_colors[] =
{
	0.5f, 0.8f, 1.0f, 1.0f,
};
void
init(void)
{

	//����û����VAO���ǳ���Ҳ�����е�ԭ��ò����opengl��Ĭ��VAO,����������VAO���øı�Ļ�display()����
	//�е�glBindVertexArray(VAOs[Triangles])����ȡ����,��Ϊinit()���Ѿ��󶨹�һ����.
	glCreateVertexArrays(NumVAOs,VAOs);
	glBindVertexArray(VAOs[0]);
	glCreateBuffers(NumBuffers, Buffers);//����NumBuffers�����㻺�����,����NumBuffers��δʹ�ö�����������Buffers��
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[0]);
	glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);//Ҳ������������
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer ָ����
//��Ⱦʱ����ֵΪ index��vPosition�� �Ķ���������������ݸ�ʽ��λ�ã�Ҳ����ָ��ɫ����location=0������
	glEnableVertexAttribArray(vPosition);//���ö�����������


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glNamedBufferStorage(Buffers[1], sizeof(vertices2), vertices2, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer ָ����
//��Ⱦʱ����ֵΪ index��vPosition�� �Ķ���������������ݸ�ʽ��λ�ã�Ҳ����ָ��ɫ����location=0������
	glEnableVertexAttribArray(vPosition);//���ö�����������

//ebo����
	static const GLushort vertex_indices[] =
	{
		0, 1, 2, 0,2,3
	};
	glBindVertexArray(VAOs[2]);
	glCreateBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glNamedBufferStorage(ebo[0], sizeof(vertex_indices), vertex_indices, GL_DYNAMIC_STORAGE_BIT);

	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);





	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);//��ȡ�ļ��ʹ�����ɫ������
	glUseProgram(program);//ʹ�����ӹ�����ɫ������

	uniformindex =glGetUniformLocation(program,"time");

}

//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	timeLoc += 0.01;
	if (timeLoc >= 2)
		timeLoc = 0;
	glUniform1f(uniformindex,timeLoc);
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);//���֡����
    static bool cur = 0;
	  if (cur == 0)
	  {
		glBindVertexArray(VAOs[0]);//ѡ����Ϊ��������ʹ�õĶ�������
		vertices[0][0] = { -0.90f };
		cur = !cur;
	  }
	  else if(cur==1)
	  {
		glBindVertexArray(VAOs[1]);//ѡ����Ϊ��������ʹ�õĶ�������
		vertices[0][0] = { -0.10f };
		cur = !cur;
	  }

	
	//glDrawArrays(GL_TRIANGLES, 0, NumVertices);//ʹ�õ�ǰ�󶨵Ķ�������Ԫ�ؽ���һϵ�еļ���ͼԪ

	glBindVertexArray(VAOs[2]);
	glDrawElements(GL_TRIANGLE_STRIP,6,GL_UNSIGNED_SHORT,NULL);
	_sleep(15);
	
}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024, 720, "Te", NULL, NULL);

	glfwMakeContextCurrent(window);
	gl3wInit();

	init();

	while (!glfwWindowShouldClose(window))
	{

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
