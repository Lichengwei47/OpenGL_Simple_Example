#include <stdlib.h>
#include<iostream>
#include<GL/glew.h>
#include<GL/glut.h>
void userInit();  //自定义初始化
void display(void);

GLuint VAO; 
GLuint VBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
float vertices[] = {
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f   // top 
};
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
/*here we have the input Color. We just write it out to FragColor. FragColor is our own output variable which GL automatically bind to output 0, therefore we don't need to set it up from our C++ side of the code.*/
"}\n\0";


void keyboardFunc(unsigned char c, int x, int y)//响应敲击键盘函数
{

	float timeValue = glutGet(GLUT_ELAPSED_TIME) / 1000;
	float time_current;
	std::cout << "开始无限循环" << std::endl;
	while (true)
	{
		time_current = glutGet(GLUT_ELAPSED_TIME) / 1000;
		if (time_current > timeValue)
		{
			display();
			timeValue = time_current;
		}
	}
}
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
	glutKeyboardFunc(keyboardFunc);
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

void userInit()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
   // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
   // glBindVertexArray(0);


   // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
   // just bind it beforehand before rendering the respective triangle; this is another approach.
	glBindVertexArray(VAO);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

//绘制回调函数    
void display(void)
{
	glBindVertexArray(VAO);
	// render
// ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// be sure to activate the shader before any calls to glUniform
	glUseProgram(shaderProgram);

	// update shader uniform
	float timeValue = glutGet(GLUT_ELAPSED_TIME)/1000;
	std::cout <<timeValue <<"秒"<< std::endl;
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	// render the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDisable(0);
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glBindVertexArray(0);
	glUseProgram(0);
	glutSwapBuffers();
}

