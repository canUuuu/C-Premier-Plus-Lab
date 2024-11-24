#pragma once
#define MAX_OBJECTS 10000
#define DEFAULT_POINT_SIZE 10
#define DEFAULT_LINE_WIDTH 5
#define DEFAULT_BEZIER_CONTROL_POINTS_NUM 4
#define MAX_BEZIER_CONTROL_POINTS_NUM 13
#define DEFAULT_PICK_BUFFER_SIZE 32
#define DEFAULT_PICK_WINDOW_SIZE 10
#define MAX_BEZIER_LINE_NUM 5
#define PI 3.14159265358979323846 // pi


#include<math.h>
#include<glut.h>
#include<random>

const GLdouble derta = 10;
enum State {
	MoveObject,
	MakePoint,
	AutoMove
};

struct GLColor;
struct GLTransform;

class Object;
class World;

// GLObject 派生类
class Executor; // 点
class GLBezier; // 贝塞尔曲线

struct GLColor
{
	GLfloat r = 0, g = 0, b = 0;
};

/*
N:1,0,0
S:0,1,0
E:0,0,1
W:1,1,0
*/

struct GLTransform
{
	// 世界坐标，轴旋转，轴缩放
	GLdouble x = 0, y = 0, z = 0;
	GLdouble rx = 0, ry = 0, rz = 0;
	GLdouble sx = 1, sy = 1, sz = 1;
};

class Object
{
public:
	char dir = 'N';
	GLint id = 0;
	GLColor color;
	GLTransform transform;
	bool movable = false;
	bool visible = true;

	World* parentWorld;

	virtual void Draw(GLenum RenderMode = GL_RENDER) {}

	virtual void MakePoint(GLdouble x, GLdouble y, GLdouble z) {}

	virtual void KeyboardMove(GLdouble right, GLdouble up) {}
	
	virtual void AutoMove() {};

	void SetPosition(GLdouble x, GLdouble y, GLdouble z) {
		transform.x = x;
		transform.y = y;
		transform.z = z;
	}

};

class World
{
public:
	Object* objects[MAX_OBJECTS];
	GLint count = 0;

	World() = default;
	~World() {
		for (int i = count - 1; i >= 0; --i)
		{
			delete objects[i];
		}

	}

	template < typename T>
	T* NewObject() {
		if (count < MAX_OBJECTS)
		{
			T* ret = new T;
			objects[count] = (Object*)ret;
			objects[count]->parentWorld = this;
			objects[count]->id = count;
			++count;
			return ret;
		}
		else return nullptr;

	}

	void DeleteObject(GLint id) {
		delete objects[id];
		for (int i = id; i < count - 1; i++)
		{
			objects[i] = objects[i + 1];
		}
		count -= 1;

	}
	void Empty() {
		for (; count  > 0; count--)
		{
			delete objects[count];
		}

	}

	void DrawObjects(GLenum RenderMode = GL_RENDER) {
		for (int i = 0; i < count; ++i)
		{
			objects[i]->Draw(RenderMode);
		}
	}
	GLint pickedObjectId = -1;
	void PickObject(GLint xMouse, GLint yMouse) {
		pickRects(GLUT_LEFT_BUTTON, GLUT_DOWN, xMouse, yMouse);

	}
private:
	void pickRects(GLint button, GLint action, GLint xMouse, GLint yMouse);
	void processPicks(GLint nPicks, GLuint pickBuffer[]);
};

// 使用外部定义的世界坐标系裁剪空间，供设置选取时的裁剪窗口
extern GLfloat xwcMin, xwcMax, ywcMin, ywcMax;

void World::pickRects(GLint button, GLint action, GLint xMouse,
	GLint yMouse)
{
	GLuint pickBuffer[DEFAULT_PICK_BUFFER_SIZE];
	GLint nPicks, vpArray[4];

	if (button != GLUT_LEFT_BUTTON || action != GLUT_DOWN)
		return;

	glSelectBuffer(DEFAULT_PICK_BUFFER_SIZE, pickBuffer); // Designate pick buffer.

	glRenderMode(GL_SELECT); // Activate picking operations.

	glInitNames(); // Initialize the object-ID stack.


	// 只使用一个栈顶元素，后续为图形命名使用 glLoadName(id) 替换栈顶

	glPushName(MAX_OBJECTS);

	/* Save current viewing matrix. */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	/* Obtain the parameters for the current viewport. Set up
	25.
	* a 5 x 5 pick window, and invert the input yMouse value
	26.
	* using the height of the viewport, which is the fourth
	27.
	* element of vpArray.
	28.
	*/
	glGetIntegerv(GL_VIEWPORT, vpArray);
	gluPickMatrix(GLdouble(xMouse), GLdouble(vpArray[3] - yMouse), DEFAULT_PICK_WINDOW_SIZE, DEFAULT_PICK_WINDOW_SIZE, vpArray);

	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);
	//rects (GL_SELECT); // Process the rectangles in selection mode.
	this->DrawObjects(GL_SELECT);
	/* Restore original viewing matrix. */
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
	/* Determine the number of picked objects and return to the
	* normal rendering mode.
	*/
	nPicks = glRenderMode(GL_RENDER);
	processPicks(nPicks, pickBuffer); // Process picked objects.


	glutPostRedisplay();
}

void World::processPicks(GLint nPicks, GLuint pickBuffer[])
{

	// 储存获取到的最后一个 ID，即认为后绘制的图像在先绘制的图形之上

	if (nPicks == 0) pickedObjectId = -1;
	else
	{

		// 因为只使用一个栈顶元素，所以可以由公式计算得出最后一个图形ID
		int chunkId = 4 * (nPicks - 1);
		pickedObjectId = pickBuffer[chunkId + 3];
	}
	return;
}

class Executor : public Object
{
public:
	Executor() {
		color.r = 1;
		dir = 'N';
		transform.x = 0.0;
		transform.y = 0.0;
	}
	
	//GLPoint* ctrlPoints[MAX_BEZIER_CONTROL_POINTS_NUM];
	int count = 0;
	virtual void KeyboardMove (GLdouble right,GLdouble up) override
	{
		if (transform.x + right > xwcMax || transform.x + right < xwcMin) {
			if (transform.x + right > xwcMax)
				transform.x = right - xwcMax;
			else
				transform.x = xwcMax + right;
			return;
		}
		transform.x += right;
		if (transform.y + up > ywcMax || transform.y + up < ywcMin) {
			if (transform.y + up > ywcMax)
				transform.y = up - ywcMax;
			else
				transform.y = up + ywcMax;
			return;
		}
		transform.y += up;
	}

	virtual void AutoMove() override {
		// 创建随机数生成器
		std::random_device rd;                     // 随机设备，提供种子
		std::mt19937 gen(rd());                    // Mersenne Twister 生成器
		std::uniform_real_distribution<double> dis(-20.0, 20.0); // [0, 10) 均匀分布

		// 生成随机数
		GLdouble a[2];
		for (int i = 0; i < 2; i++)
		{
			a[i] = GLdouble(dis(gen));
		}
		KeyboardMove(a[0], a[1]);

	}
	virtual void MakePoint(GLdouble x, GLdouble y, GLdouble z) {
		if (count >= MAX_BEZIER_CONTROL_POINTS_NUM) return;
		//parentWorld->NewObject<GLPoint>();
		SetPosition(x, y, z); // 初始位置
		movable = true; // 可以移动
		count++;
	}

	

	virtual void Draw(GLenum RenderMode = GL_RENDER) {
		// 需要在绘制前初始化控制点
		//if (count == 0) return;
		/*
			count = 0意味着这是空的点，是Create Car时候new的，不需要画，
			事实上如果画出来是在窗口原点（中心）
		*/
		if (RenderMode == GL_SELECT) glLoadName((GLuint)id);
		switch (dir)
		{
		case 'N':
			color.r = 1, color.g = 0, color.b = 0;
			break;
		case 'S':
			color.r = 0, color.g = 1, color.b = 0;
			break;
		case 'E':
			color.r = 0, color.g = 0, color.b = 1;
			break;
		case 'W':
			color.r = 1, color.g = 1, color.b = 0;
			break;
		default:
			break;
		}
		glColor3f(color.r, color.g, color.b);
		glPointSize(DEFAULT_POINT_SIZE);

		glBegin(GL_POINTS);
		glVertex3d(transform.x, transform.y, transform.z);
		glEnd();
		
	}
};

