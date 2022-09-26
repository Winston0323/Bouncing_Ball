#pragma once
#ifndef _BALL_H_
#define _BALL_H_

#include "core.h"
#include "plain.h"
#include "cube.h"
#include "Sphere.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

class Ball
{
private:
	//parameters
	glm::vec3 position, velocity, force, 
		gravForce, gravForceMult, norm, windVelocity;
	GLfloat mass, radius, elastic, 
		friction, airResist, gravMult, grav;
	
	Sphere* sphere;
	Cube* cube;
	bool lastHit;
	Plain* lastHitPlain;
	Plain* dummyCube;


public:
	Ball(glm::vec3 position, glm::vec3 velocity, glm::vec3 windVelocity, 
		GLfloat grav, GLfloat airResist, 
		GLfloat radius, GLfloat elastic,
		GLfloat friction, Cube * cube);
	~Ball();

	void update(GLfloat timeStep, GLfloat& restTime);
	void draw(const glm::mat4& projection, const glm::mat4& view, GLuint shader);
	void ApplyForce(glm::vec3 f);
	
	glm::vec3 GetPos();
	glm::vec3 GetVelocity();

	void addNorm(glm::vec3 normal);
	void clearNorm();

	void normalize();
	void translate(float dist);

	glm::vec3 getNorm();
	glm::vec3 getPos();
	void boxCollision(GLfloat timeStep, GLfloat& restTime);
	void renderUpdate();

	void defaultForces();
	GLfloat* getMass();
	GLfloat* getGravMult();
	GLfloat* getAirResist();
	GLfloat getRadius();
	GLfloat* getSphereRadius() { this->radius = * sphere->getRadius(); return sphere->getRadius(); }
	GLfloat* getFriction();
	GLfloat* getElastic();
	GLfloat* getPosX();
	GLfloat* getPosY();
	GLfloat* getPosZ();
	GLfloat* getWindX() { return &this->windVelocity.x; }
	GLfloat* getWindY() { return &this->windVelocity.y; }
	GLfloat* getWindZ() { return &this->windVelocity.z; }
	GLfloat* getVelX();
	GLfloat* getVelY();
	GLfloat* getVelZ();

	void restoreDefault();
};

////////////////////////////////////////////////////////////////////////////////

#endif
