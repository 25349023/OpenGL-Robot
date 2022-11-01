#include "../Include/Common.h"

using namespace glm;

//GLubyte timer_cnt = 0;
//bool timer_enabled = true;
//unsigned int timer_speed = 16;

//int program_idx = 0;

mat4 model(1.0f), view(1.0f), projection(1.0f);

const GLuint um4mvp_loc = 0;
//GLuint m_time_circular;
GLuint program;

//GLuint programs[2];
bool playAnimation = false;

using Shape = std::vector<MeshData>;

struct Model {
	Shape shape;
	GLuint vao;
	GLuint buffer;
	int parent;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	mat4 localModelMat;

	Model(Shape s, int p = -1) : shape(s), parent(p), 
		position(vec3(0)), rotation(vec3(0)), scale(vec3(1)) {}
};

std::vector<Model> robots;

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

void bindArrayAndBuffers(Model& m)
{
	glGenVertexArrays(1, &m.vao);
	glBindVertexArray(m.vao);

	glGenBuffers(1, &m.buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m.buffer);
	glBufferData(GL_ARRAY_BUFFER, m.shape[0].positions.size() * sizeof(float),
		m.shape[0].positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void buildRobot()
{
	// torso
	robots.emplace_back(loadObj("../Objects/Cube.obj"));
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0, 1, -5);
	robots.back().scale = vec3(2, 2, 1);
	
	// head
	robots.emplace_back(loadObj("../Objects/Sphere.obj"), 0);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0, 1.7, 0);

	// L1 arm
	robots.emplace_back(loadObj("../Objects/Cylinder.obj"), 0);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(-1.5, 0.5, 0);
	robots.back().rotation = vec3(0, 0, radians(-30.0f));
	robots.back().scale = vec3(0.5, 0.5, 0.5);

	// L2 arm
	robots.emplace_back(loadObj("../Objects/Cylinder.obj"), robots.size() - 1);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0, -1.2, 0);
	robots.back().scale = vec3(0.5, 0.5, 0.5);

	// R1 arm
	robots.emplace_back(loadObj("../Objects/Cylinder.obj"), 0);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(1.5, 0.5, 0);
	robots.back().rotation = vec3(0, 0, radians(30.0f));
	robots.back().scale = vec3(0.5, 0.5, 0.5);

	// R2 arm
	robots.emplace_back(loadObj("../Objects/Cylinder.obj"), robots.size() - 1);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0, -1.2, 0);
	robots.back().scale = vec3(0.5, 0.5, 0.5);

	// L1 leg
	robots.emplace_back(loadObj("../Objects/Cube.obj"), 0);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(-0.5, -1.7, 0);
	robots.back().scale = vec3(0.8, 1, 0.8);

	// L2 leg
	robots.emplace_back(loadObj("../Objects/Cube.obj"), robots.size() - 1);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0, -1.3, 0);
	robots.back().scale = vec3(0.8, 1.2, 0.8);

	// L foot
	robots.emplace_back(loadObj("../Objects/Cube.obj"), robots.size() - 1);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(-0.2, -1, 0);
	robots.back().scale = vec3(1.2, 0.2, 0.8);

	// R1 leg
	robots.emplace_back(loadObj("../Objects/Cube.obj"), 0);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0.5, -1.7, 0);
	robots.back().scale = vec3(0.8, 1, 0.8);

	// R2 leg
	robots.emplace_back(loadObj("../Objects/Cube.obj"), robots.size() - 1);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0, -1.3, 0);
	robots.back().scale = vec3(0.8, 1.2, 0.8);

	// R foot
	robots.emplace_back(loadObj("../Objects/Cube.obj"), robots.size() - 1);
	bindArrayAndBuffers(robots.back());
	robots.back().position = vec3(0.2, -1, 0);
	robots.back().scale = vec3(1.2, 0.2, 0.8);


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
	glUseProgram(program);

	buildRobot();
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Model &m : robots) {
		glBindVertexArray(m.vao);

		mat4 T(1.0), R(1.0), S(1.0);

		T = translate(T, m.position);
		R = mat4_cast(quat(m.rotation));
		S = scale(S, m.scale);

		m.localModelMat = T * R;

		if (m.parent != -1) {
			m.localModelMat = robots.at(m.parent).localModelMat * m.localModelMat;
		}
		mat4 mvp = projection * view * m.localModelMat * S * model;
		glUniformMatrix4fv(um4mvp_loc, 1, GL_FALSE, value_ptr(mvp));

		glBindVertexArray(m.vao);
		glDrawArrays(GL_TRIANGLES, 0, m.shape[0].positions.size() / 3);
	}

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
	switch (key)
	{
	case 'a': case 'A':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			robots.at(0).rotation.y += radians(5.0f);
		}
		else {
			robots.at(0).position += vec3(-0.5, 0, 0);
		}
		break;
	case 'd':case 'D':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			robots.at(0).rotation.y -= radians(5.0f);
		}
		else {
			robots.at(0).position += vec3(0.5, 0, 0);
		}
		break;
	case 'w':case 'W':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			robots.at(0).rotation.x -= radians(5.0f);
		}
		else {
			robots.at(0).position += vec3(0, 0.5, 0);
		}
		break;
	case 's': case 'S':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			robots.at(0).rotation.x += radians(5.0f);
		}
		else {
			robots.at(0).position += vec3(0, -0.5, 0);
		}
		break;
	case 'q':case 'Q':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			robots.at(0).rotation.z += radians(5.0f);
		}
		else {
			robots.at(0).position += vec3(0, 0, -0.5);
		}
		break;
	case 'e': case 'E':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			robots.at(0).rotation.z -= radians(5.0f);
		}
		else {
			robots.at(0).position += vec3(0, 0, 0.5);
		}
		break;
	default:
		printf("Other key is pressed at (%d, %d)\n", x, y);
		break;
	}
	glutPostRedisplay();
}

void My_SpecialKeys(int key, int x, int y)
{

}

void My_Menu(int action) {
	switch (action)
	{
	case 1:
		playAnimation = true;
		break;
	case 2:
		playAnimation = false;
		break;
	default:
		break;
	}
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


	int menu_id = glutCreateMenu(My_Menu);
	glutAddMenuEntry("Play Animation", 1);
	glutAddMenuEntry("Stop Animation", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
//	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
//	glutTimerFunc(timer_speed, My_Timer, 0);

	glutMainLoop();

	return 0;
}
