#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include "objloader.h"

using namespace glm;

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glutSwapBuffers();

}


int main(int argc, char **argv)
{
	

	int window;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(300, 100);



	window = glutCreateWindow("Object Loader");

	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glFrustum(-1, 1, -1, 1, 2, 12);
	//	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);


	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	bool res = loadOBJ("bunny.obj", vertices, normals);


	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


	/*
	while (1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		

		glutSwapBuffers();
	}

	*/
	
	

	return 0;
}