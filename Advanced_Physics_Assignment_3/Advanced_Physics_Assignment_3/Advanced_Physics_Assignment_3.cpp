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

struct AmmoRound
{
	cyclone::Particle particle;

	/** Draws the round. */
	void render()
	{
		cyclone::Vector3 position;
		particle.getPosition(&position);

		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.5f, 36, 2);
		glPopMatrix();		
	}
};

AmmoRound* shot = new AmmoRound();
bool shotFired;


/**
* Called each frame to update the 3D scene. Delegates to
* the application.
*/
void update()
{
	glutPostRedisplay();
	if(shotFired)
		shot->particle.integrate(0.001);
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
	glColor3f(0.5, 0.0, 0.5); // <---- setting the draw color;	
	//renderQuadPrimitive();  //Rendering a quad in the color set above.
	if(shotFired)
		shot->render();
	///// Last call /////
	glutSwapBuffers();
}

/**
* Called when a mouse button is pressed. Delegates to the
* application.
*/
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN){
		if(!shotFired){
			shotFired = true;
			shot->particle.setMass(2.0f); // 2.0kg
			shot->particle.setVelocity(50.0f, 0.0f, 0.0f); // 35m/s
			shot->particle.setAcceleration(0.0f, -1.0f, 0.0f);
			shot->particle.setDamping(0.99f);

			// Set the data common to all particle types
			shot->particle.setPosition(0.0f, 1.5f, 0.0f);

			// Clear the force accumulators
			//shot->particle.clearAccumulator();
		}

	}
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
