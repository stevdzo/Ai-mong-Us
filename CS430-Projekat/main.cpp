#include "iostream"
#include "GameWorld.h"

GameWorld* world;

double delta_time = 0.0f, previous_time = 0.0f;

void display(void) {

	int current_time = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (double)(current_time - previous_time) / 1000;
	previous_time = current_time;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	// sve se desava ovde
	world->game_loop(delta_time);

	glutSwapBuffers();
	glutPostRedisplay();
}

void init(void) {

	glClearColor(1.0, 0.0, 0.0, 0.5);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);

	world = new GameWorld(SCREEN_WIDTH, SCREEN_WIDTH);
}

void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void game_over() { delete world; }

void timer(int val) {

	glutPostRedisplay();
	glutTimerFunc(15, timer, 0);
}

void mouse(int button, int state, int x, int y) { std::cout << x << " : " << y << std::endl; world->get_input()->mouse(button, state, x, y); }
void keyboard_press(unsigned char key, int x, int y) { world->get_input()->keyboard_press(key, x, y); }
void keyboard_up(unsigned char key, int x, int y) { world->get_input()->keyboard_up(key, x, y); }

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Projekat");		
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard_press);
	glutKeyboardUpFunc(keyboard_up);

	init();

	glutMainLoop();

	game_over();

	return 0;
}