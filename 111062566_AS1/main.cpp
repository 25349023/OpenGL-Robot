#include "../Include/Common.h"

GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;

using namespace glm;

int program_idx = 0;

mat4 mvp(1.0f);
GLuint um4mvp_circular, um4mvp_checked;
GLuint m_time_circular, m_time_checked;

GLuint screen_center_circular;

GLuint program_circular;
GLuint program_checked;

GLuint programs[2];

char** loadShaderSource(const char* file)
{
	FILE* fp = fopen(file, "rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* src = new char[sz + 1];
	fread(src, sizeof(char), sz, fp);
	src[sz] = '\0';
	char** srcp = new char* [1];
	srcp[0] = src;
	return srcp;
}

void freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

void My_Init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	GLuint fragmentCircularShader = glCreateShader(GL_FRAGMENT_SHADER);
//	GLuint fragmentCheckedShader = glCreateShader(GL_FRAGMENT_SHADER);
//	char** vertexShaderSource = loadShaderSource("vertex.vs.glsl");
//	char** fragmentCircularShaderSource = loadShaderSource("fragment_circular.fs.glsl");
//	char** fragmentCheckedShaderSource = loadShaderSource("fragment_checked.fs.glsl");
//	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
//	glShaderSource(fragmentCircularShader, 1, fragmentCircularShaderSource, NULL);
//	glShaderSource(fragmentCheckedShader, 1, fragmentCheckedShaderSource, NULL);
//	freeShaderSource(vertexShaderSource);
//	freeShaderSource(fragmentCircularShaderSource);
//	freeShaderSource(fragmentCheckedShaderSource);
//
//	glCompileShader(vertexShader);
//	glCompileShader(fragmentCircularShader);
//	glCompileShader(fragmentCheckedShader);
//	shaderLog(vertexShader);
//	shaderLog(fragmentCircularShader);
//	shaderLog(fragmentCheckedShader);
//
//	program_circular = glCreateProgram();
//	glAttachShader(program_circular, vertexShader);
//	glAttachShader(program_circular, fragmentCircularShader);
//	glLinkProgram(program_circular);
//	um4mvp_circular = glGetUniformLocation(program_circular, "um4mvp");
//	m_time_circular = glGetUniformLocation(program_circular, "time");
//	screen_center_circular = glGetUniformLocation(program_circular, "screenCenter");
//	glUseProgram(program_circular);
//	programs[0] = program_circular;
//
//	program_checked = glCreateProgram();
//	glAttachShader(program_checked, vertexShader);
//	glAttachShader(program_checked, fragmentCheckedShader);
//	glLinkProgram(program_checked);
//	um4mvp_checked = glGetUniformLocation(program_checked, "um4mvp");
//	m_time_checked = glGetUniformLocation(program_checked, "time");
//	programs[1] = program_checked;
//
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	float data[9] = {
//		-0.5f, -0.4f, 0.0f,
//		0.5f, -0.4f, 0.0f,
//		0.0f,  0.6f, 0.0f
//	};
//	GLuint buffer;
//	glGenBuffers(1, &buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
}
//
//// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	float viewportAspect = (float)width / (float)height;
	mvp = ortho(-1 * viewportAspect, 1 * viewportAspect, -1.0f, 1.0f);
	mvp = mvp * lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	glUniform2f(screen_center_circular, (float)width / 2, (float)height / 2);
}

//void My_Timer(int val)
//{
//	timer_cnt++;
//	glutPostRedisplay();
//	if (timer_enabled)
//	{
//		glutTimerFunc(timer_speed, My_Timer, val);
//	}
//}
//
//void My_Mouse(int button, int state, int x, int y)
//{
//	if (state == GLUT_DOWN)
//	{
//		printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
//	}
//	else if (state == GLUT_UP)
//	{
//		printf("Mouse %d is released at (%d, %d)\n", button, x, y);
//	}
//}
//
//void My_Keyboard(unsigned char key, int x, int y)
//{
//	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
//}
//
//void My_SpecialKeys(int key, int x, int y)
//{
//	switch (key)
//	{
//	case GLUT_KEY_F1:
//		printf("F1 is pressed at (%d, %d)\n", x, y);
//		break;
//	case GLUT_KEY_PAGE_UP:
//		printf("Page up is pressed at (%d, %d)\n", x, y);
//		break;
//		//TODO:
//		//New 2 cases for switch the different fragment shader using LEFT/RIGHT arrow key
//		/////////////////////////////
//
//	case GLUT_KEY_LEFT:
//	case GLUT_KEY_RIGHT:
//		program_idx = (program_idx + 1) % 2;
//		glUseProgram(programs[program_idx]);
//		break;
//
//		/////////////////////////////
//	default:
//		printf("Other special key is pressed at (%d, %d)\n", x, y);
//		break;
//	}
//}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	
	// You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
	glutCreateWindow("111062566_AS1"); 

#ifdef _MSC_VER
	glewInit();
#endif
	dumpInfo();

	My_Init();

//	// Register GLUT callback functions.
//	///////////////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
//	glutMouseFunc(My_Mouse);
//	glutKeyboardFunc(My_Keyboard);
//	glutSpecialFunc(My_SpecialKeys);
//	glutTimerFunc(timer_speed, My_Timer, 0);
//	///////////////////////////////
//

	glutMainLoop();

	return 0;
}
