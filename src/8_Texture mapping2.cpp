#include <stdlib.h>
#include "GL/glut.h"
//设置长宽  
int width = 512;
int height = 256;
//设置图像指针
typedef unsigned char byte;
using namespace std;
#include <cvd/colourmap.h>
#include <cvd/image_io.h>
template<typename TYPE>
static inline void LoadImage(CVD::Image<TYPE>& img1, CVD::Image<TYPE>& img2,
	const std::string& fileName) {
	CVD::img_load(img1, fileName);
	if (img1.size().x % 4 == 0)
		img2 = img1;
	else {
		const int width1 = (img1.size().x + 3) & (~3), height1 = img1.size().y;
		img2.resize(CVD::ImageRef(width1, height1));
		img2.zero();
		for (int y = 0; y < height1; ++y)
			memcpy(img2[y], img1[y], sizeof(TYPE) * img1.row_stride());
	}
}

GLuint load_texture()
{
	//OpenGL纹理用整型数表示
	GLuint texture_ID;
	//获取图像指针
	string filename = "C://Users//Yao//Desktop//3.png";
	CVD::Image<CVD::Rgb<byte> >    m_imgRGB, m_imgRGBTmp;
	::LoadImage(m_imgRGBTmp, m_imgRGB, filename);
	cout << (m_imgRGB.size().x) << "," <<( m_imgRGB.size().y) << endl;
	unsigned char* data = (byte*)m_imgRGB.data();
	//将texture_ID设置为2D纹理信息
	glGenTextures(2, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	//纹理放大缩小使用线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//纹理水平竖直方向外扩使用重复贴图
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//纹理水平竖直方向外扩使用边缘像素贴图(与重复贴图二选一)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//将图像内存用作纹理信息
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	//free(data);
	return texture_ID;
}

void display()
{
	// 清除屏幕
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//获取纹理对象
	GLuint image = load_texture();
	cout << image << endl;
	//重新设置OpenGL窗口：原点位置为左上角，x轴从左到右，y轴从上到下，坐标值与像素坐标值相同
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);

	//显示纹理
	glEnable(GL_TEXTURE_2D);	//允许使用纹理
	glBindTexture(GL_TEXTURE_2D, image);	//选择纹理对象

	//原始完全填充四边形
	//glBegin(GL_POLYGON);	//设置为多边形纹理贴图方式并开始贴图
	//glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);	//纹理左上角对应窗口左上角
	//glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);	//纹理左下角对应窗口左下角
	//glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);	//纹理右下角对应窗口右下角
	//glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);	//纹理右上角对应窗口右上角
	//glEnd();	//结束贴图

	//三角形
	/*glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
	glEnd();*/

	//多边形
	glBegin(GL_POLYGON);
	glTexCoord2f(0.33f, 0.0f); glVertex2f(width/3, 0);
	glTexCoord2f(0.67f, 0.0f); glVertex2f(width*2/3, 0);
	glTexCoord2f(1.0f, 0.5f); glVertex2f(width, height/2);
	glTexCoord2f(0.5f, 1.0f); glVertex2f(width/2, height);
	glTexCoord2f(0.0f, 0.5f); glVertex2f(0, height/2);
	glEnd();

	//任意变换
	/*glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(width/4, height/4);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height*2/3);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(width*4/5, 50);
	glEnd();*/

	//边缘贴图效果
	/*glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
	glTexCoord2f(0.0f, 2.0f); glVertex2f(0, height);
	glTexCoord2f(2.0f, 2.0f); glVertex2f(width, height);
	glTexCoord2f(2.0f, 0.0f); glVertex2f(width, 0);
	glEnd();*/

	glDisable(GL_TEXTURE_2D);	//禁止使用纹理

	//双缓存交换缓存以显示图像
	glutSwapBuffers();
}

void main(int argc, char** argv)
{
	//初始化GL
	glutInit(&argc, argv);
	//设置显示参数(双缓存，RGB格式)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//设置窗口尺寸：width*height
	glutInitWindowSize(width, height);
	//设置窗口位置：在屏幕左上角像素值(100,100)处
	glutInitWindowPosition(100, 100);
	//设置窗口名称
	glutCreateWindow("OpenGL");
	//显示函数，display事件需要自行编写
	glutDisplayFunc(display);

	//重复循环GLUT事件
	glutMainLoop();
}
