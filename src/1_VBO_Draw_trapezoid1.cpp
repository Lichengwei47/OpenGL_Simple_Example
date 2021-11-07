#include <stdlib.h>
#include<iostream>
#include<GL/glew.h>
#include<GL/glut.h>
void userInit();  //自定义初始化
void display(void);

GLuint vboId;//vertex buffer object句柄    
GLuint vaoId;//vertext array object句柄    
GLuint programId;//shader program 句柄 
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Rectangle demo");

	//使用glew，需要执行glewInit，不然运行过程会报错
	//glewInit要放在glut完成了基本的初始化之后执行
	glewInit();

	//自定义初始化，生成VAO，VBO对象
	userInit();


	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

void userInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//创建顶点数据    
	const GLfloat vertices[] = {
		-0.5f,-0.5f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,1.0f,
		0.25f,0.5f,0.0f,1.0f,
		-0.25f,0.5f,0.0f,1.0f,
	};


	//创建VBO对象	
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//传入VBO数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//解除VBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//绘制回调函数    
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);

	//解释顶点数据方式
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//绘制模型
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}
