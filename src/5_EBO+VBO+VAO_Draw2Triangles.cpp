#include <stdlib.h>
#include<iostream>
#include<GL/glew.h>
#include<GL/glut.h>
void userInit();  //自定义初始化
void display(void);

GLuint VAO; 
GLuint VBO;
GLuint EBO; 
GLuint programId;
float vertices[] = {
	-1.0f, -0.5f, 0.0f,   
	 0.f, -0.5f, 0.0f,  
	-0.5f, 0.5f, 0.0f, 

	 1.0f,-0.5f,0.0f,   
	 0.5f,0.5f,0.0f
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 2, // 第一个三角形
	1, 3, 4  // 第二个三角形
};
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
	
	glGenBuffers(1, &EBO);//创建EBO
	glGenVertexArrays(1, &VAO);//创建VAO
	glGenBuffers(1, &VBO);//创建VBO
	glBindVertexArray(VAO);//绑定VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//这里缓冲定义和之前不一样
	//解释顶点数据方式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//解除VAO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//绘制回调函数    
void display(void)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//绑定VAO
	glBindVertexArray(VAO);


	//绘制模型
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}
