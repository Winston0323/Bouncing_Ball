#pragma once
#pragma once
#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "core.h"
#include "plain.h"
#include <iostream>
#include "glm/gtc/random.hpp"
#include <glm/gtc/type_ptr.hpp>
#define MASS_VALUE 1
////////////////////////////////////////////////////////////////////////////////

class ParticleSystem
{
private:
	GLuint VAO, VBO;
	GLfloat pointSize;
	glm::mat4 model;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> accelerations;
	std::vector<glm::vec3> velocitys;
	std::vector<GLfloat> lifeTimes;
	std::vector<GLfloat> forces;

	std::vector<Plain*> plains;
	std::vector<int> avaInd;
	std::vector<bool> isActive;


	//std::vector<Particle *> particles;
	int crtRate;
	GLfloat lifeSpan;
	GLfloat mass;
	GLfloat lastLifeSpan;
	GLfloat grav;
	GLfloat gravMult;
	GLfloat airResist;
	GLfloat radius;
	GLfloat friction;
	GLfloat elastic;
	GLfloat velMag;

	glm::vec3 initPos;
	glm::vec3 velocity;
	glm::vec3 color;
	glm::vec3 force;
	glm::vec3 gravForce;
	glm::vec3 windVelocity;

	int posVarX;
	int posVarY;
	int posVarZ;
	int velVarX;
	int velVarY;
	int velVarZ;

public:
	GLfloat RoundOffError;
	ParticleSystem(std::vector<Plain*> plains, GLfloat mass, 
		GLfloat gravMult, GLfloat lifeSpan);
	ParticleSystem(std::vector<Plain*> plains, GLfloat mass,
		GLfloat gravMult, GLfloat lifeSpan, GLfloat velMag);
	~ParticleSystem();
	//void eraseParticle(int index);
	void genParticles(GLfloat deltaTime);
	void genParticlesCircle(GLfloat deltaTime, GLfloat radius);
	void cleanParticles();
	void defaultForces();
	//bool checkHit();
	void integration(int index, GLfloat timeStep);
	void renderUpdate();
	void update(GLfloat deltaTime, glm::vec3 pos, glm::vec3 vel);
	void update(GLfloat deltaTime, glm::vec3 pos, glm::vec3 vel, GLfloat radius);
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);

	int* getCrtRate() { return &this->crtRate; }
	GLfloat* getLifeSpan() { return &this->lifeSpan; }
	GLfloat* getGrav() { return &this->grav; }
	GLfloat* getGravTimes() { return &this->gravMult; }
	GLfloat* getAirResist() { return &this->airResist; }
	GLfloat* getRadius() { return &this->radius; }
	GLfloat* getFriction() { return &this->friction; }
	GLfloat* getElastic() { return &this->elastic; }
	GLfloat* getPosX() { return &this->initPos.x; }
	GLfloat* getPosY() { return &this->initPos.y; }
	GLfloat* getPosZ() { return &this->initPos.z; }

	GLfloat* getVelX() { return &this->velocity.x; }
	GLfloat* getVelY() { return &this->velocity.y; }
	GLfloat* getVelZ() { return &this->velocity.z; }

	int* getPosVarX() { return &this->posVarX; }
	int* getPosVarY() { return &this->posVarY; }
	int* getPosVarZ() { return &this->posVarZ; }

	int* getVelVarX() { return &this->velVarX; }
	int* getVelVarY() { return &this->velVarY; }
	int* getVelVarZ() { return &this->velVarZ; }
	int getNum() { return positions.size() - avaInd.size(); }
	void restoreDefault();
	//void setColor();
};

////////////////////////////////////////////////////////////////////////////////

#endif
