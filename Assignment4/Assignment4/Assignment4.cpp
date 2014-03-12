/*
* The main entry point for all demos.
*
* Part of the Cyclone physics system.
*
* Copyright (c) Icosagon 2003. All Rights Reserved.
*
* This software is distributed under licence. Use of this software
* implies agreement with all terms and conditions of the accompanying
* software licence.
*/

#include "stdafx.h"
using namespace cyclone;

class Crate : public CollisionBox
{
	public:
	Crate(){}
	Crate(Vector3 position){
		this->position = position;
		body = new cyclone::RigidBody();
		body->setPosition(position.x, position.y, position.z);
		body->setMass(2.0f);
		body->addVelocity(cyclone::Vector3(3,5,0));
		body->setAcceleration(cyclone::Vector3(0,-1,0));
		body->setDamping(0.99f,0);
	}

	~Crate(){
		delete body;
	}
	/** Draws the round. */
	void render()
	{
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glColor3f(0, 0, 0);
		glutSolidCube(1);
		glColor3f(0.1, 0.1, 0.1);
		glutSolidCube(0.8);
		glPopMatrix();		
	}
private:
	Vector3 position;
};

ParticleWorld* world;
const int nrOfCrates = 16;
Crate* crates[nrOfCrates];
Crate* playerCrate = new Crate();

void init()
{
	for(int i=0;i<nrOfCrates;i++)
	{
		Vector3 pos;
		if(i < 4) {
			pos = Vector3(2+i,-2,0);
		}
		else if(i < 8) {
			pos = Vector3(-2+i,-1,0);
		}
		else if(i < 12) {
			pos = Vector3(-6+i,0,0);
		}
		else if(i < 16) {
			pos = Vector3(-10+i,1,0);
		}
		std::cout << "Postion X: "<< pos.x << std::endl;
		crates[i] = new Crate(pos);
	}
}

/**
* Called each frame to update the 3D scene. Delegates to
* the application.
*/
void update(void)
{
	glutPostRedisplay();
	for(int i=0;i<nrOfCrates;i++)
	{		
		crates[i]->body->integrate(0.01);
	}
}


void renderQuadPrimitive(void){

	glBegin(GL_QUADS);	
	glVertex3f(-1.0f,-1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,0.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glEnd();
}

/**
* Called each frame to display the 3D scene. Delegates to
* the application.
*/
void display(void){
	///////	DONT TOUCH	////////	
	glClear(GL_COLOR_BUFFER_BIT); // <----- Has to be called, dunno why

	/////// DO WHATEVER YOU WANT /////////
	//glColor3f(0.5, 0.0, 0.5); // <---- setting the draw color;	
	//renderQuadPrimitive();  //Rendering a quad in the color set above.
	for(int i=0;i<nrOfCrates;i++)
	{		
		crates[i]->render();
	}

	///// Last call /////
	glutSwapBuffers();
}

/**
* Called when a mouse button is pressed. Delegates to the
* application.
*/
void mouse(int button, int state, int x, int y)
{

}

/**
* Called when the display window changes size.
*/
void reshape(int width, int height)
{

}

/**
* Called when a key is pressed.
*/
void keyboard (unsigned char key, int x, int y)
{

	// we'll switch between red and blue when the user presses a key:
	GLfloat colors[][3] = { { 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f } };
	static int back;

	switch (key) {
	case 27: 
		exit(0);
	default:
		back ^= 1;
		glClearColor(colors[back][0], colors[back][1], colors[back][2], 1.0f);

	}

}

/**
* Called when the mouse is dragged.
*/
void motion(int x, int y)
{

}

/**
* The main entry point. We pass arguments onto GLUT.
*/
int main(int argc, char** argv)
{
	//Setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("Dimension");
	glScalef(0.1f,0.1f,0.1f);
	glClearColor(0.5, 0.5, 0.5, 1); //<---- Color of background
	init();
	// Set up the appropriate handler functions
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Run the application
	glutMainLoop();

	return 0;
}
