#include <stdlib.h>
#include<iostream>
#include<GL/glew.h>
#include<GL/glut.h>
#include <opencv.hpp>
cv::Mat I = cv::imread("C://Users//Yao//Desktop//wall.jpg");
int width = I.cols;
int height = I.rows;
static GLuint FBO;
static GLuint texture_in;
static GLuint texture_out;
static  GLfloat outPutFb[512 * 512];   //输出纹理缓冲
static GLubyte  pData[512 * 512];   //存储最终的图像数据
void initFBO()
{
	//创建FBO，准备屏幕外帧缓存
	glGenFramebuffersEXT(1, &FBO);

	//绑定屏幕外帧缓存，即避开了窗口系统默认的渲染目标
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);


}


void init()
{

	glShadeModel(GL_FLAT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, width, height);
	//设置输入纹理
	glGenTextures(1, &texture_in);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture_in);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//纹理过滤的方式不应该设置为线性插值
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GLubyte* pixels;
	int pixellength = I.rows * I.cols * 3;
	pixels = new GLubyte[pixellength];
	memcpy(pixels, I.data, pixellength * sizeof(char));
	if (pixels)
	{
		cv::imshow("原始图像", I);
		cv::waitKey();
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//设置输出纹理
	glGenTextures(1, &texture_out);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture_out);

	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//纹理过滤的方式不应该设置为线性插值
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//没有给输出的纹理数据，等待程序进行赋值
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture_in);

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//关联输出缓存至FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_RECTANGLE_ARB, texture_out, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBegin(GL_QUADS);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
	glVertex2f(-1.0, -1.0);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0, 512.0);
	glVertex2f(-1.0, 1.0);

	glMultiTexCoord2f(GL_TEXTURE0, 512.0, 512.0);
	glVertex2f(1.0, 1.0);

	glMultiTexCoord2f(GL_TEXTURE0, 512.0, 0.0);
	glVertex2f(1.0, -1.0);

	glEnd();
	glFlush();
}
const char* vShader = {
	//"#version 110  \n "
	"void main()"
	"{"
		"gl_TexCoord[0] = gl_MultiTexCoord0;"
		"gl_Position = ftransform();"
	"}"
};

//片元着色器
const char* fShader = {
	//"#version 110          \n "
	"#extension GL_ARB_texture_rectangle : enable  \n"
	"uniform sampler2DRect  LenaTexture;         \n"
	"void main()         \n"
	"{          \n"
		"vec2     pos1 = vec2(gl_TexCoord[0].s,512.0-gl_TexCoord[0].t);\n"
		"vec4   texColor   =  texture2DRect(LenaTexture,pos1 );         \n"
		"float mid=(texture2DRect(LenaTexture,pos1 ).r,texture2DRect(LenaTexture,pos1 ).g,texture2DRect(LenaTexture,pos1 ).g)/3;\n"
		"gl_FragColor=vec4(mid,mid,mid,1);\n"
	"}        \n"
};

GLhandleARB initShaders(const char* vShaderCode, const char* fShaderCode)
{
	GLhandleARB vertHandle, fragHandle, progHandle;   //对象句柄
	GLint vertCompiled, fragCompiled;				//状态值
	GLint linked;

	//创建顶点着色器对象和片元着色器对象
	vertHandle = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragHandle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	//将源代码字符串加载到着色器中
	glShaderSource(vertHandle, 1, &vShaderCode, NULL);
	glShaderSource(fragHandle, 1, &fShaderCode, NULL);

	//编译码块顶点着色器并打印编译器日志文件
	glCompileShaderARB(vertHandle);
	glGetObjectParameterivARB(vertHandle, GL_OBJECT_COMPILE_STATUS_ARB, &vertCompiled);

	//编译码块片元着色器并打印编译器日志文件
	glCompileShaderARB(fragHandle);
	glGetObjectParameterivARB(fragHandle, GL_OBJECT_COMPILE_STATUS_ARB, &fragCompiled);

	if (!vertCompiled || !fragCompiled)
		return -1;

	//创建一个程序对象并附加两个编译好的着色器
	progHandle = glCreateProgramObjectARB();
	glAttachObjectARB(progHandle, vertHandle);
	glAttachObjectARB(progHandle, fragHandle);

	//链接程序对象并打印信息日志
	glLinkProgramARB(progHandle);
	glGetObjectParameterivARB(progHandle, GL_OBJECT_LINK_STATUS_ARB, &linked);

	if (!linked)
		return -2;

	//将程序对象安装为当前状态的一部分
	glUseProgramObjectARB(progHandle);     //改为运行的函数，用于测试该算法的时间

	return progHandle;
}
void ReadTexture(GLfloat* Pixels)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, width, height,
		GL_LUMINANCE, GL_FLOAT, Pixels);
}
int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_LUMINANCE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("FBO ");
	glewInit();
	initFBO();
	init();
	GLhandleARB progHandle = 0;
	progHandle = initShaders(vShader, fShader);
	std::cout << progHandle << std::endl;
	glUniform1i(glGetUniformLocation(progHandle, "LenaTexture"), 0);   //0 是纹理的句柄
	display();
	//glutDisplayFunc(display);

	ReadTexture(outPutFb);
	//因为outputFb中保存的图像灰度都是0~1之间的数值表示的
	//这里我们要将0~1之间的数值乘以255变为0~255之间的灰度
	for (int i = 0; i < width *height; ++i)
	{
		pData[i] = (char)(outPutFb[i] * 255);
	}
	cv::Mat gray1_mat(512, 512, CV_32F, outPutFb);//为了显示图片 先改为mat类型
	imshow("输出图像", gray1_mat);
	cv::waitKey();
	gray1_mat = gray1_mat * 255;//0-1变为0-255
	cv::imwrite("test1.jpg  ", gray1_mat);
	return 0;
}