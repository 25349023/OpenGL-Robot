#include "../Include/Common.h"

using namespace glm;

//GLubyte timer_cnt = 0;
//bool timer_enabled = true;
//unsigned int timer_speed = 16;

//int program_idx = 0;

mat4 mvp(1.0f);
mat4 model(1.0f), view(1.0f), projection(1.0f);

const GLuint um4mvp_loc = 0;
//GLuint m_time_circular;
GLuint program;

//GLuint programs[2];

vec3 translateBody(0, 0, 0);
float rotateBodyAng = 0.0f;


std::vector<MeshData> bodyCube;

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
	fclose(fp);

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

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char** vertexShaderSource = loadShaderSource("vertex.vs.glsl");
	char** fragmentShaderSource = loadShaderSource("fragment.fs.glsl");
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	freeShaderSource(vertexShaderSource);
	freeShaderSource(fragmentShaderSource);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	printGLShaderLog(vertexShader);
	printGLShaderLog(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
//	um4mvp_circular = glGetUniformLocation(program_circular, "um4mvp");
//	m_time_circular = glGetUniformLocation(program_circular, "time");
//	screen_center_circular = glGetUniformLocation(program_circular, "screenCenter");
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	bodyCube = loadObj("../Objects/Cube.obj");

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, bodyCube[0].positions.size() * sizeof(float),
		bodyCube[0].positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 T(1.0), R(1.0);

	T = translate(T, translateBody);
	R = rotate(R, rotateBodyAng, vec3(0, 1, 0));

	mvp = projection * view * R * T * model;
	glUniformMatrix4fv(um4mvp_loc, 1, GL_FALSE, value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, bodyCube[0].positions.size() / 3);

	glutSwapBuffers();
}

void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	float viewportAspect = (float)width / (float)height;

	// perspective(fov, aspect_ratio, near_plane_distance, far_plane_distance)
	// ps. fov = field of view, it represent how much range(degree) is this camera could see 
	projection = perspective(radians(60.0f), viewportAspect, 0.1f, 1000.0f);

	// lookAt(camera_position, camera_viewing_vector, up_vector)
	// up_vector represent the vector which define the direction of 'up'
	view = lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
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

void My_Keyboard(unsigned char key, int x, int y)
{
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
}

void My_SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			printf("rotating counter-clockwise\n");
			rotateBodyAng += 10;
		}
		else {
			printf("moving left\n");
			translateBody += vec3(-1, 0, 0);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			printf("rotating clockwise\n");
			rotateBodyAng -= 10;
		}
		else {
			printf("moving right\n");
			translateBody += vec3(1, 0, 0);
		}
		break;

		/////////////////////////////
	default:
		printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
	glutPostRedisplay();
}

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
	printGLContextInfo();

	My_Init();

//	// Register GLUT callback functions.
//	///////////////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
//	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
//	glutTimerFunc(timer_speed, My_Timer, 0);
//	///////////////////////////////


	glutMainLoop();

	return 0;
}
