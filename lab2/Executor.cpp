#include "func.h"
#include<iostream>
#include <windows.h>

World MyWorld;

State MyState = MoveObject;
/* Set initial size of the display window. */
GLsizei winWidth = 900, winHeight = 600;

/* Set size of world-coordinate clipping window. */
GLfloat xwcMin = -450.0, xwcMax = 450.0;
GLfloat ywcMin = -300.0, ywcMax = 300.0;

struct menuEntryStruct {
	const char* label;
	char key;
};

static menuEntryStruct mainMenu[] = {
"Reset", '0',
"Creat Car by Mouse", '1',
"Creat Car by Keyboard", '2',

"Auto Move",'3',
"Get Coordinate",'4',
"Quit", 27, //ESC 键（ASCII: 27）
//"MakePoint",'2',//MakePoint
//"MovePoint",'3'
};
int mainMenuEntries = sizeof(mainMenu) / sizeof(menuEntryStruct);





bool globalControlPointsVisible = true;



void Auto(int value) {
	if (MyState != AutoMove)
		return;
	MyWorld.objects[MyWorld.pickedObjectId]->AutoMove();
	glutPostRedisplay();
	glutTimerFunc(600, Auto, 0); // ~1000ms = 1s
}

void userEventAction(char key) {
	switch (key) {
	case '0':
		
		//reset
		MyWorld.Empty();	
		MyWorld.pickedObjectId = -1;
		MyState = MoveObject;
		break;
	case '1': // Creat Car by Mouse
		if (MyState == MoveObject)
		{
			Executor *tem= MyWorld.NewObject<Executor>();
			MyState = MakePoint;
			MyWorld.pickedObjectId = tem->id;
			

		}
		else if (MyState == MakePoint)
		{
			MyState = MoveObject;
			MyWorld.pickedObjectId = -1;
		}
		break;
	case '2': // Creat Car by Keyboard
		if (MyState == MoveObject)
		{
			Executor* tem = MyWorld.NewObject<Executor>();

			MyWorld.pickedObjectId = tem->id;
			std::printf("Please initialize the coordinate by 'int x,int y,char dir':\n ");
			char dot;
			//清除键盘缓存区
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
			FlushConsoleInputBuffer(hStdin);
			std::cin >> tem->transform.x >> dot >> tem->transform.y >> dot >> tem->dir;
			std::cout << "Making Car No." << MyWorld.pickedObjectId << " Successfully!" << std::endl;
			//std::cout<< tem->transform.x << dot << tem->transform.y << dot << tem->dir<<std::endl;
		}
		else if (MyState == MakePoint)
		{
			MyState = MoveObject;
			MyWorld.pickedObjectId = -1;
		}
		break;
	case '3'://Auto Move
		if (MyState == MakePoint)
		{
			MyState = MoveObject;
			MyWorld.pickedObjectId = -1;
			break;
		}
		if (MyState == AutoMove)
		{
			MyState = MoveObject;
			break;
		}
		if (MyWorld.pickedObjectId == -1) {
			std::printf("Error:U've not choosen a car!\n");
			break;
		}
		std::printf("Auto Moving No.%d\n", MyWorld.pickedObjectId);
		MyState = AutoMove;
		glutTimerFunc(0, Auto, 0); // 启动定时器
		break;
	case 32://space
		if (MyState == AutoMove)
			MyState = MoveObject;
		break;
	case 'm':
		if (MyWorld.pickedObjectId != -1) {
			switch (MyWorld.objects[MyWorld.pickedObjectId]->dir)
			{
			case 'N':
				MyWorld.objects[MyWorld.pickedObjectId]->KeyboardMove(0, derta);
				break;
			case 'S':
				MyWorld.objects[MyWorld.pickedObjectId]->KeyboardMove(0, -derta);
				break;
			case 'E':
				MyWorld.objects[MyWorld.pickedObjectId]->KeyboardMove(derta, 0);
				break;
			case 'W':
				MyWorld.objects[MyWorld.pickedObjectId]->KeyboardMove(-derta, 0);
			default:
				break;
			}
			
		}
		break;
	case 'l':
		if (MyWorld.pickedObjectId != -1) {
			switch (MyWorld.objects[MyWorld.pickedObjectId]->dir)
			{
			case 'N':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'W';
				break;
			case 'S':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'E';
				break;
			case 'E':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'N';
				break;
			case 'W':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'S';
				break;
			default:
				break;
			}
			
		}
		break;
	case 'r':
		if (MyWorld.pickedObjectId != -1) {
			switch (MyWorld.objects[MyWorld.pickedObjectId]->dir)
			{
			case 'N':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'E';
				break;
			case 'S':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'W';
				break;
			case 'E':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'S';
				break;
			case 'W':
				MyWorld.objects[MyWorld.pickedObjectId]->dir = 'N';
				break;
			default:
				break;
			}
		}
		break;
	case '4':
		if (MyWorld.pickedObjectId == -1)
			std::printf("None...\n");
		else
		{
			std::cout << "Car No." << MyWorld.pickedObjectId<<": ";
			std::cout <<"(x:"<< MyWorld.objects[MyWorld.pickedObjectId]->transform.x << ",y:" <<
				MyWorld.objects[MyWorld.pickedObjectId]->transform.y << ",dir:" <<
				MyWorld.objects[MyWorld.pickedObjectId]->dir << ")" << std::endl;
		}
		break;
	case 27: // ESC 键（ASCII: 27）退出
		MyWorld.Empty();
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay(); // 重绘
}
void keyboard(unsigned char key, int x, int y) {
	
	userEventAction(key);
}

void selectMain(int choice) // 序号->key 映射
{
	userEventAction(mainMenu[choice].key); // 调用通用动作解释函数
}

void displayFcn(void)
{
	//std::printf("now %d\n", MyWorld.pickedObjectId);
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window.
	MyWorld.DrawObjects(GL_RENDER);
	glFlush();

}

void myReshape(GLint Width, GLint Height) {
	glViewport(0, 0, Width, Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);

	glClear(GL_COLOR_BUFFER_BIT);
}

void unProject(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble* objx,
	GLdouble* objy, GLdouble* objz)
{
	// 从窗口到世界坐标系
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject(winx, winy, winz, modelview, projection, viewport, objx, objy, objz);
}

void mouseButton(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		break;
	default:
		return;
	}
	switch (state) {
	case GLUT_DOWN:
		if (MyState == MoveObject)
		{
			MyWorld.PickObject(x, y);
			if(MyWorld.pickedObjectId == -1)	
				std::printf("NONE...\n");
			else
				std::printf("Choosing Car No.%d \n", MyWorld.pickedObjectId);
		}
		else if (MyState == MakePoint)
		{
			if (Object* ptr = MyWorld.objects[MyWorld.pickedObjectId])
			{
				// 从窗口到世界坐标系
				GLdouble winx = (GLdouble)x;
				GLdouble winy = (GLdouble)winHeight - (GLdouble)y;
				GLdouble winz = 0;
				GLdouble objx = 0;
				GLdouble objy = 0;
				GLdouble objz = 0;
				unProject(winx, winy, winz, &objx, &objy, &objz);
				ptr->MakePoint(objx, objy, objz);
				std::printf("Make Car No.%d Successfully!\n", ptr->id);
			}
			MyState = MoveObject;
		}
		
		break;
	default:
		return;
	glutPostRedisplay();
	}
	
}

void mouseMotion(int x, int y)
{
	if (MyWorld.pickedObjectId == -1) return;
	if (!MyWorld.objects[MyWorld.pickedObjectId]->movable) return;

	Object* ptr = MyWorld.objects[MyWorld.pickedObjectId];

	// 从窗口到世界坐标系
	GLdouble winx = (GLdouble)x;
	GLdouble winy = (GLdouble)winHeight - (GLdouble)y;
	GLdouble winz = 0;
	GLdouble objx = 0;
	GLdouble objy = 0;
	GLdouble objz = 0;
	unProject(winx, winy, winz, &objx, &objy, &objz);

	ptr->SetPosition(objx, objy, objz);
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);//白色背景

	// 绑定菜单
	glutCreateMenu(selectMain); // 使用 selectMain 作为菜单调用函数
	for (int i = 0; i < mainMenuEntries; i++)
	{
		glutAddMenuEntry(mainMenu[i].label, i);
	}
	glutAttachMenu(GLUT_MIDDLE_BUTTON); // 菜单绑定在鼠标中键
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Executor_example");

	init();

	glutDisplayFunc(displayFcn);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);

	
	
	glutReshapeFunc(myReshape);

	glutMainLoop();

	return 0;
}