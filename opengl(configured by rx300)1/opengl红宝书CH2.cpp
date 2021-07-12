//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#ifndef _STADX_H
#define _STADX_H
#include "stadx.h"
#endif // !stadx.h

size_t 
TypeSize(GLenum type)
{
	size_t size;
    #define CASE(Enum,Count,Type) \
	case Enum:size=Count*sizeof(Type);break
	switch (type)
	{
		CASE(GL_FLOAT, 1, GLfloat);
		CASE(GL_FLOAT_VEC2, 2, GLfloat);
		CASE(GL_FLOAT_VEC3, 3, GLfloat);
		CASE(GL_FLOAT_VEC4, 4, GLfloat);
		CASE(GL_INT, 1, GLint);
		CASE(GL_INT_VEC2, 2, GLint);
		CASE(GL_INT_VEC3, 3, GLint);
		CASE(GL_INT_VEC4, 4, GLint);
		CASE(GL_UNSIGNED_INT, 1, GLint);
		CASE(GL_UNSIGNED_INT_VEC2, 2, GLint);
		CASE(GL_UNSIGNED_INT_VEC3, 3, GLint);
		CASE(GL_UNSIGNED_INT_VEC4, 4, GLint);
		CASE(GL_BOOL, 1, GLboolean);
		CASE(GL_BOOL_VEC2, 2, GLboolean);
		CASE(GL_BOOL_VEC3, 3, GLboolean);
		CASE(GL_BOOL_VEC4, 4, GLboolean);
		CASE(GL_FLOAT_MAT2, 4, GLfloat);
		CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		CASE(GL_FLOAT_MAT3, 9, GLfloat);
		CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		CASE(GL_FLOAT_MAT4, 16, GLfloat);
		CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
        #undef CASE
	default:
		fprintf(stderr,"Unknown type:0x%x\n",type);
		exit(EXIT_FAILURE);
		break;
	}
	return size;
}

//----------------------------------------------------------------------------
//
// init
//

void
init(void)
{


	glClearColor(1,0,0,1);
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "CHapetr2_vShader.vert" },
		{ GL_FRAGMENT_SHADER, "CHapetr2_vShader.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);//读取文件和创建着色器程序
	glUseProgram(program);//使用链接过的着色器程序
	//初始化uniform块中“Uniform”中的变量
	GLuint uboIndex;
	GLint uboSize;
	GLuint ubo;
	GLvoid *buffer;


	//查找“Uniform”块的uniform缓存索引，并判断整个块的大小。
	uboIndex = glGetUniformBlockIndex(program,"Uniform");
	glGetActiveUniformBlockiv(program,uboIndex,GL_UNIFORM_BLOCK_DATA_SIZE,&uboSize);
	buffer = malloc(uboSize);

	if (buffer == NULL)
	{
		fprintf(stderr, "Unable to allocate buffer\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		enum { Translation, Scale, Rotation, Enabled, NumUniforms };
		/*准备存储在缓存对象中的值*/
		GLfloat scale = 0.5;
		GLfloat translation[] = { 0.1,0.1,0.0 };
		GLfloat rotation[] = { 90,  0.0,  0.0,  1.0 };
		GLboolean enabled = GL_TRUE;

		/*我们可以建立一个变量名称数组，对应块中已知的uniform变量*/
		const char*names[NumUniforms] = {
		"translation",
		"scale",
		"rotation",
		"enabled"
		};
		GLuint indices[NumUniforms];
		GLint size[NumUniforms];
		GLint offset[NumUniforms];
		GLint type[NumUniforms];
		/*查询对应的属性，以判断向数据缓存中写入数组的值*/
		glGetUniformIndices(program, NumUniforms, names, indices);
		glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_OFFSET, offset);
		glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_SIZE, size);
		glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_TYPE, type);

		/*将uniform变量值拷贝到缓存中*/
		memcpy(static_cast<int *>(buffer) + offset[Scale], &scale, size[Scale] * TypeSize(type[Scale]));
		memcpy(static_cast<int *>(buffer) + offset[Translation], &translation, size[Translation] * TypeSize(type[Translation]));
		memcpy(static_cast<int *>(buffer) + offset[Rotation], &rotation, size[Rotation] * TypeSize(type[Rotation]));
		memcpy(static_cast<int *>(buffer) + offset[Enabled], &enabled, size[Enabled] * TypeSize(type[Enabled]));

		/*建立uniform缓存对象，初始化存储内容，并且与着色器程序建立关联*/
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, uboSize, buffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uboIndex, ubo);
	}

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
	glUniform1f(uniformindex, timeLoc);
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);//清楚帧缓存
	static bool cur = 0;
	if (cur == 0)
	{
		glBindVertexArray(VAOs[0]);//选择作为顶点数据使用的顶点数组
		cur = !cur;
	}
	else if (cur == 1)
	{
		glBindVertexArray(VAOs[1]);//选择作为顶点数据使用的顶点数组
		cur = !cur;
	}


	glDrawArrays(GL_TRIANGLES, 0, NumVertices);//使用当前绑定的顶点数组元素建立一系列的几何图元
	_sleep(15);

}

//----------------------------------------------------------------------------
//
// main
//

/*
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
*/