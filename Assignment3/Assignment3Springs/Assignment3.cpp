/*
 Assignment 3: Springs
 Yannick Willemsen
 Sieuwerd Broekema

 A plane suspended using 4 spring-like lines
 Use 'wasdqe' to move the plane
 Use 'r' to reset the location/physics,
 Use '1234' to increase the weight
 Use '5678' to decrease the weight
*/

#include <gl/glut.h>
#include <cyclone/cyclone.h>
#include <cyclone/pfgen.h>
#include "src\app.h"
#include "src\timing.h"

#include <stdio.h>
#include <cassert>

#define CABLE_COUNT 1
#define ROD_COUNT 6
#define SPRING_COUNT 4
#define BUNGEE_COUNT 1
#define BASE_MASS 3

class Assignment_3_Springs : public MassAggregateApplication
{
    cyclone::ParticleCableConstraint *supports;
	cyclone::ParticleAnchoredBungee *bungees;
    cyclone::ParticleRod *rods;
	cyclone::ParticleSpring *springs;
	cyclone::ParticleSpring *springA;
	cyclone::ParticleSpring *springB;
	cyclone::ParticleSpring *springC;
	cyclone::ParticleSpring *springD;

public:
    Assignment_3_Springs();
    virtual ~Assignment_3_Springs();
	void InitializeParticleArray();
	void CreateRods();
	void CreateSprings();
    virtual const char* getTitle();
    virtual void display();
    virtual void update();
    virtual void key(unsigned char key);
};

Assignment_3_Springs::Assignment_3_Springs()
:
MassAggregateApplication(8), supports(0), rods(0)
{	
	InitializeParticleArray();
	CreateSprings();
	CreateRods();	
}

void Assignment_3_Springs::InitializeParticleArray()
{
	// Here we create 4 points in the air that are the anchors of the springs.
	particleArray[0].setPosition(-2.5f,6,1);
	particleArray[1].setPosition(2.5f,6,1);
	particleArray[3].setPosition(-2.5f,6,-1);
	particleArray[2].setPosition(2.5f,6,-1);

	// Next we declare 4 points that will represent the corners of the plane, connected by the wires
	particleArray[4].setPosition(-2.5f,5,1);
	particleArray[5].setPosition(2.5f,5,1);
	particleArray[7].setPosition(-2.5f,5,-1);
	particleArray[6].setPosition(2.5f,5,-1);

	// Add physics to the 4 particles in the plane
	for(int i = 4; i < 8; i++)
	{
		particleArray[i].setMass(BASE_MASS); // Add the base amount of mass, without they will always float
		particleArray[i].setDamping(0.5f);  //Add the damping value
		particleArray[i].setAcceleration(cyclone::Vector3::GRAVITY); //Add Gravity
		particleArray[i].clearAccumulator();
	}	
}

void Assignment_3_Springs::CreateSprings()
{
	//Some variables that hold the Constant of the springs
	float springConstantAB = 15.0f;
	float springConstantCD = 25.0f; 

	// Create 4 springs that will suspend the plane from the 4 anchors
	// First we create a spring force with an anchorpoint,
	//  the elastic properties of the wire and its normal unstretched length
	// This we add to the world's Force Registry together with the attached point of the plane
	//  so we can update the forces
	springA = new cyclone::ParticleSpring(&particleArray[0], springConstantAB, 2);
	world.getForceRegistry().add(&particleArray[4], springA);

	springB = new cyclone::ParticleSpring(&particleArray[1], springConstantAB, 2);
	world.getForceRegistry().add(&particleArray[5], springB);

	springC = new cyclone::ParticleSpring(&particleArray[2], springConstantCD, 2);
	world.getForceRegistry().add(&particleArray[6], springC);

	springD = new cyclone::ParticleSpring(&particleArray[3], springConstantCD, 2);
	world.getForceRegistry().add(&particleArray[7], springD);

}

void Assignment_3_Springs::CreateRods()
{
	// Create the sides and inner diagonal lines of the plane to make sure 
	//  it remains in the same form and doesn't deform due to the forces
	rods = new cyclone::ParticleRod[ROD_COUNT];
	float width = 4.0f;
	float depth = 2.0f;
	int r = 0;
	// The four outer borders of the plane
	rods[r].particle[0] = &particleArray[4];
	rods[r].particle[1] = &particleArray[5];
	rods[r].length = width;
	r++;
	rods[r].particle[0] = &particleArray[5];
	rods[r].particle[1] = &particleArray[6];
	rods[r].length = depth;
	r++;
	rods[r].particle[0] = &particleArray[6];
	rods[r].particle[1] = &particleArray[7];
	rods[r].length = width;
	r++;
	rods[r].particle[0] = &particleArray[7];
	rods[r].particle[1] = &particleArray[4];
	rods[r].length = depth;
	r++;

	// The diagonals that prevent the plane from becoming skewed
	rods[r].particle[0] = &particleArray[4];
	rods[r].particle[1] = &particleArray[6];
	rods[r].length = sqrt(width*width + depth*depth);
	r++;
	rods[r].particle[0] = &particleArray[5];
	rods[r].particle[1] = &particleArray[7];
	rods[r].length = sqrt(width*width + depth*depth);
	r++;

	// Add the rods the the ContactGenerator of the world so they are found by the update
	for (unsigned i = 0; i < ROD_COUNT; i++)
        world.getContactGenerators().push_back(&rods[i]);
}


Assignment_3_Springs::~Assignment_3_Springs()
{
    if (rods) delete[] rods;
    if (supports) delete[] supports;
	if (springs) delete[] springs;
}

void Assignment_3_Springs::display()
{
	// Draws the 3D world
	MassAggregateApplication::display();

	// Start drawing OpenGL lines
	glBegin(GL_LINES);

	// Red lines for the suspended plane
	glColor3f(1,0,0);

	// Draw the plane by going through the all the connections
	for(int i = 4; i < 8; i++)
	{
		for(int j = 4; j < 8; j++)
		{
			if(i == j) // No connections with itself
				continue;

			// Get the particles
			const cyclone::Vector3 &p0 = particleArray[i].getPosition();
			const cyclone::Vector3 &p1 = particleArray[j].getPosition();
			glVertex3f(p0.x, p0.y, p0.z);
			glVertex3f(p1.x, p1.y, p1.z);
		}
	}
	
	//Black color for the Springs
	glColor3f(0,0,0);
	for(int i = 0; i < 4; i++)
	{
		// Get the particles
		const cyclone::Vector3 &p0 = particleArray[i].getPosition();
		const cyclone::Vector3 &p1 = particleArray[i+4].getPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}

	glEnd(); // Stop OpenGL coloring
}

void Assignment_3_Springs::update()
{
    MassAggregateApplication::update();
}

// Keypress handling for manipulating the plane
void Assignment_3_Springs::key(unsigned char key)
{
	// Create a velocity vector
	cyclone::Vector3 *velocity = new cyclone::Vector3(0,0,0);
	int speed = 3;
    switch(key)
    {
	case 'w':
		velocity = new cyclone::Vector3(0,speed,0);
        break;
	case 'a':
        velocity = new cyclone::Vector3(-speed,0,0);
        break;
    case 's':
        velocity = new cyclone::Vector3(0,-speed,0);
        break;
	case 'd':
        velocity = new cyclone::Vector3(speed,0,0);
        break;
	case 'q':
        velocity = new cyclone::Vector3(0,0,-speed);
        break;
	case 'e':
        velocity = new cyclone::Vector3(0,0,speed);
        break;

	// Resets the plane to its original location, does exactly what we did in the Initialization
	case 'r':
		particleArray[4].setPosition(-2.5f,5,1);
		particleArray[5].setPosition(2.5f,5,1);
		particleArray[7].setPosition(-2.5f,5,-1);
		particleArray[6].setPosition(2.5f,5,-1);
		for(int i = 4; i < 8; i++)
		{
			particleArray[i].setVelocity(0,0,0);
			particleArray[i].setMass(BASE_MASS); // Add the base amount of mass, without they will always float
			particleArray[i].setDamping(0.5f); //Add Damping Value
			particleArray[i].setAcceleration(cyclone::Vector3::GRAVITY); //Add the Gravity
			particleArray[i].clearAccumulator();
		}
		break;
	// These increase/decrease the weight on each particle
	case '1':
		particleArray[4].setMass(particleArray[4].getMass()+1);
        break;
	case '2':
		particleArray[5].setMass(particleArray[5].getMass()+1);
        break;
	case '3':
		particleArray[6].setMass(particleArray[6].getMass()+1);
        break;
	case '4':
		particleArray[7].setMass(particleArray[7].getMass()+1);
        break;
	case '5':
		if(particleArray[4].getMass() > 1) // Make sure the weight never turns to 0 or below
			particleArray[4].setMass(particleArray[4].getMass()-1);
        break;
	case '6':
		if(particleArray[5].getMass() > 1) // Make sure the weight never turns to 0 or below
			particleArray[5].setMass(particleArray[5].getMass()-1);
        break;
	case '7':
		if(particleArray[6].getMass() > 1) // Make sure the weight never turns to 0 or below
			particleArray[6].setMass(particleArray[6].getMass()-1);
        break;
	case '8':
		if(particleArray[7].getMass() > 1) // Make sure the weight never turns to 0 or below
			particleArray[7].setMass(particleArray[7].getMass()-1);
        break;
	case '+': case '=':
		break;
	case '-': case '_':
		break;
    default:
        MassAggregateApplication::key(key);
    }
	// Add the velocity to each particle, moving the plane into a direction
	if(velocity->x == 0 && velocity->y == 0 && velocity->z == 0)
		return;
	for(int i = 0; i < 4; i++)
	{
		particleArray[i+4].setVelocity(velocity->x, velocity->y, velocity->z);
	}
}

const char* Assignment_3_Springs::getTitle()
{
    return "Springs Assignment";
}

Application* getApplication()
{
    return new Assignment_3_Springs();
}