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
		-0.5f, 0.5f,  0.0f, 1.0f,//正方形左上
		-0.5f, -0.5f,  0.0f, 1.0f,//正方形左下
		0.5f,  -0.5f,  0.0f, 1.0f,//正方形右下
		0.5f, 0.5f, 0.0f, 1.0f,//正方形右上
	};

void
init(void)
{

	//这里没设置VAO但是程序也能运行的原因貌似是opengl有默认VAO,因此如果不对VAO设置改变的话display()函数
	//中的glBindVertexArray(VAOs[Triangles])可以取消掉,因为init()里已经绑定过一次了.
	glCreateVertexArrays(NumVAOs,VAOs);
	glBindVertexArray(VAOs[0]);
	glCreateBuffers(NumBuffers, Buffers);//生成NumBuffers个顶点缓存对象,返回NumBuffers个未使用对象名到数组Buffers中
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[0]);
	glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);//也可以用这句代替
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer 指定了
//渲染时索引值为 index（vPosition） 的顶点属性数组的数据格式和位置，也就是指着色器中location=0的数据
	glEnableVertexAttribArray(vPosition);//启用顶点属性数组


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glNamedBufferStorage(Buffers[1], sizeof(vertices2), vertices2, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer 指定了
//渲染时索引值为 index（vPosition） 的顶点属性数组的数据格式和位置，也就是指着色器中location=0的数据
	glEnableVertexAttribArray(vPosition);//启用顶点属性数组

//ebo测试
	static const GLushort vertex_indices[] =
	{
		0, 1, 2, 0,2,3
	};
	glBindVertexArray(VAOs[2]);
	glCreateBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glNamedBufferStorage(ebo[0], sizeof(vertex_indices), vertex_indices, GL_DYNAMIC_STORAGE_BIT);

	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glNamedBufferStorage(Buffers[2], sizeof(vertex_positions), vertex_positions, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT,
		GL_FALSE, 4 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer 指定了
//渲染时索引值为 index（vPosition） 的顶点属性数组的数据格式和位置，也就是指着色器中location=0的数据
	glEnableVertexAttribArray(vPosition);//启用顶点属性数组





	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);//读取文件和创建着色器程序
	glUseProgram(program);//使用链接过的着色器程序

	uniformindex =glGetUniformLocation(program,"time");
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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
	glClearBufferfv(GL_COLOR, 0, black);//清楚帧缓存
    static bool cur = 0;
	  if (cur == 0)
	  {
		glBindVertexArray(VAOs[0]);//选择作为顶点数据使用的顶点数组
		vertices[0][0] = { -0.90f };
		cur = !cur;
	  }
	  else if(cur==1)
	  {
		glBindVertexArray(VAOs[1]);//选择作为顶点数据使用的顶点数组
		vertices[0][0] = { -0.10f };
		cur = !cur;
	  }

	
	//glDrawArrays(GL_TRIANGLES, 0, NumVertices);//使用当前绑定的顶点数组元素建立一系列的几何图元

	glBindVertexArray(VAOs[2]);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,NULL);
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Te", NULL, NULL);

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
