#include <stdlib.h>
#include<iostream>
#include<GL/glew.h>
#include<GL/glut.h>

void userInit();  //自定义初始化
void display(void);
  
GLuint VAO;
GLuint VBO;
unsigned int fragmentShader;
unsigned int vertexShader;
unsigned int shaderProgram;
const GLfloat vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.25f,0.5f,0.0f,
		-0.25f,0.5f,0.0f,
};

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);
	return 0;
}

void userInit()
{
	glClearColor(1.0f, 0.3f, 0.3f, 1.0f);//背景颜色
	//创建VAO对象	
	glGenVertexArrays(1, &VAO);
	//VAO绑定
	glBindVertexArray(VAO);
	//创建VBO对象
	glGenBuffers(1, &VBO);
	//VBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//给VBO传入数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//解绑VAO
	glBindVertexArray(0);
	//解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//绘制回调函数    
void display(void)
{

	//绑定VA0
	glBindVertexArray(VAO);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO); // 这里仅仅绑定了VAO，无需再绑定VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	glutSwapBuffers();
}

