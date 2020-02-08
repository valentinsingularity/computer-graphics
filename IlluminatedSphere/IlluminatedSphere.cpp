
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>


double x_pos = 0;
double y_pos = 0;
double z_pos = 0;
double R = 10;
double phi = 0;
double theta = 0;
int shading_type = 2;
int light0_switch = 1;
int light1_switch = 1;

void moveCamera()
{
	glLoadIdentity();
	if (cos(theta) >= 0) gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, 1, 0);
	else gluLookAt(x_pos, y_pos, z_pos, 0, 0, 0, 0, -1, 0);
}

void processKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': theta += 0.1; break;
	case 's': theta -= 0.1; break;
	case 'a': phi += 0.1; break;
	case 'd': phi -= 0.1; break;
	case 't': if (R >= 8) R -= 0.5; break;
	case 'g': if (R < 100) R += 0.5; break;
	case 'y': if (shading_type == 1) shading_type = 2;
			  else shading_type = 1; break;
	case 'u': if ( light0_switch == 0) light0_switch = 1;
			  else light0_switch = 0; break;
	case 'i': if (light1_switch == 0) light1_switch = 1;
			  else light1_switch = 0; break;
	case 27 : exit(0); break;
	}

	x_pos = R * sin(theta) * cos(phi);
	y_pos = R * sin(theta) * sin(phi);
	z_pos = R * cos(theta);

	glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (shading_type == 1) glShadeModel(GL_FLAT);
	else glShadeModel(GL_SMOOTH);
	if (light0_switch == 1) glEnable(GL_LIGHT0);
	else glDisable(GL_LIGHT0);
	if (light1_switch == 1) glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);
	glPushMatrix();
	glTranslatef(3, -1, 0);
	glutSolidSphere(3.5, 50, 50);
	glPopMatrix();

	GLfloat light_ambient1[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient1);

	GLfloat light_position1[] = { -5, 5, 10, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);


	
	GLfloat light_ambient2[] = { 1, 1, 0.9, 1 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);

	GLfloat light_position2[] = { 10, -3, 5, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

	GLfloat white_light[] = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

	GLfloat blue_light[] = { 0, 0, 1, 1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, blue_light);

	

	GLfloat mat_Ka[] = { 0.11, 0.11, 0.11, 1 };
	GLfloat mat_Kd[] = { 0.43, 0.47, 0.57, 1 };
	GLfloat mat_Ks[] = { 1, 1, 1, 1 };
	GLfloat mat_Ke[] = { 0, 0, 0, 0 };
	GLfloat mat_shininess[] = { 20 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_Ke);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	

	moveCamera();
	glFlush();
}

void init(void)
{
	x_pos = R * sin(theta) * cos(phi);
	y_pos = R * sin(theta) * sin(phi);
	z_pos = R * cos(theta);
	moveCamera();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(90, 1, 1, 1000);

}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sphere");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processKeys);


	glutMainLoop();
	return 0;
}