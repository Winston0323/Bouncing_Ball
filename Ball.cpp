#include "Ball.h"


////////////////////////////////////////////////////////////////////////////////

Ball::Ball(glm::vec3 position, glm::vec3 velocity,
	glm::vec3 windVelocity,GLfloat grav, GLfloat airResist, 
	GLfloat radius, GLfloat elastic, GLfloat friction, Cube* cube)
{

	//update parameter
	this->position = position;
	this->velocity = velocity;

	this->mass = DEFAULT_MASS;
	this->radius = radius;
	//inital parameters
	

	//gravity
	this->gravMult = DEFAULT_GRAVMULT;
	this->grav = grav;
	this->gravForce = glm::vec3(0, -mass * grav, 0);
	this->gravForceMult = glm::vec3(0, -mass * grav * gravMult, 0);
	
	//constant
	this->airResist = airResist;
	this->windVelocity = windVelocity;
	this->elastic = elastic;
	this->friction = friction;
	this->defaultForces();//initialize forces
	//representation
	this->sphere = new Sphere(1.0, glm::vec3(0.3, 0.3, 0.4));
	this->cube = cube;
	std::vector<Plain*>p;
	this->lastHit = false;
	//psb = new ParticleSystem(p, 1.0f, 0.0f, 0.5f, 0.5f);
	
}

////////////////////////////////////////////////////////////////////////////////
Ball::~Ball()
{
	delete cube;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::update(GLfloat timeStep, GLfloat& restTime)
{
	//this->partSys->update(timeStep);
	this->gravForceMult = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	boxCollision(timeStep, restTime);
	//psb->update(timeStep, this->position, this->velocity,this->radius);
	
}
////////////////////////////////////////////////////////////////////////////////
void Ball::draw(const glm::mat4& projection, const glm::mat4& view, GLuint shader, GLuint pShader)
{
	sphere->draw(view, projection, shader);
	//psb->draw(view, projection, pShader);
}
////////////////////////////////////////////////////////////////////////////////
void Ball::ApplyForce(glm::vec3 f) 
{
	this->force = this->force + f;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::defaultForces() {
	this->force = gravForceMult - this->airResist * this->velocity + this->airResist * this->windVelocity;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::GetPos()
{
	return this->position;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::GetVelocity()
{
	return this->velocity;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::addNorm(glm::vec3 normal) {
	this->norm = this->norm + normal;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::clearNorm() {
	this->norm = glm::vec3(0.0);
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::getNorm() {
	return this->norm;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::normalize() {
	this->norm = glm::normalize(this->norm);
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::getPos() {
	return this->position;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::translate(float dist) {
	this->position = this->position + glm::vec3(0.0f, dist, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
void Ball::boxCollision(GLfloat timeStep, GLfloat &restTime) {
	
	//get and check next state
	glm::vec3 acc = this->force / this->mass;
	//calculate x(n+1)
	glm::vec3 nextPos = position + velocity * timeStep;
	glm::vec3 nextVel = velocity + acc * timeStep;

	//detection
	std::vector<Plain*> hitPlains;
	if (lastHit == false) {
		hitPlains = cube->checkHit(position, this->radius);
		lastHit = false;
	}
	//determination
	GLfloat f = 1;//give f a biggest value
	//GLfloat restTime = 0.0f;
	//when there is at least one plain collide
	if (hitPlains.size() != 0) {
		Plain* hitPlain = hitPlains[0];
		//loop through all possible plains
		for (Plain* p : hitPlains) {
			glm::vec3 plainLevel = p->getOrigin();
			//check which is the dimension we are dealing
			if (plainLevel.x != 0) {
				GLfloat Zc = plainLevel.x;
				GLfloat Zn = 0.0f;
				GLfloat Znone = 0.0f;
				if (plainLevel.x < 0) {
					Zn = position.x - radius;
					Znone = nextPos.x - radius;
				}
				else {

					Zn = position.x + radius;
					Znone = nextPos.x + radius;

				}
				GLfloat ftemp = (Zc - Zn) / (Znone - Zn);
				
				//when this plain f is smaller than f of other plain
				if (ftemp < f) { 
					f = ftemp; 
					hitPlain = p;
				}
			}
			else if (plainLevel.y != 0) {
				GLfloat Zc = plainLevel.y;
				GLfloat Zn = 0.0f;
				GLfloat Znone = 0.0f;
				if (plainLevel.y < 0) {
					Zn =  position.y - radius;
					Znone = nextPos.y - radius;
				}
				else {
				
					Zn = position.y + radius;
					Znone = nextPos.y + radius;

				}
				GLfloat ftemp = (Zc - Zn) / (Znone- Zn);
				//std::cout << "ftemp is" << ftemp;
				//when this plain f is smaller than f of other plain
				if (ftemp < f) {
					f = ftemp;
					hitPlain = p;
				}
			}
			else if (plainLevel.z != 0) {
				GLfloat Zc = plainLevel.z;
				GLfloat Zn = 0.0f;
				GLfloat Znone = 0.0f;
				if (plainLevel.z < 0) {
					Zn = position.z - radius;
					Znone = nextPos.z - radius;
				}
				else {

					Zn = position.z + radius;
					Znone = nextPos.z + radius;

				}
				GLfloat ftemp = (Zc - Zn) / (Znone - Zn);
				//when this plain f is smaller than f of other plain
				if (ftemp < f) {
					f = ftemp;
					hitPlain = p;
				}
			}
			else {
				printf("Error detected, all plain demension is zero");
			}
		}
		//position and velocity on collision
		glm::vec3 posCol = position + f * timeStep * velocity;
		glm::vec3 vecCol = velocity + f * timeStep * acc;
		restTime = (1 - f) * timeStep;
		//Response
		glm::vec3 nhat = hitPlain->getNorm();
		//std::cout << "nhat is" << nhat.x << " ," << nhat.y << " ," << nhat.z << "\n";
		//seperate velocity
		glm::vec3 vn = glm::dot(vecCol, nhat) * nhat;
		glm::vec3 vt = vecCol - vn;
		//calculate next step velocity
		glm::vec3 vtnext;
		glm::vec3 vnnext = -this->elastic * vn;//elasticity
		if (glm::length(vt)!= 0) {
			vtnext = vt - std::min(this->friction * glm::length(vn), glm::length(vt)) * glm::normalize(vt);
		}
		else {
			vtnext = glm::vec3(0);
		}
		
		glm::vec3 vnext = vnnext + vtnext;
		//update state
		//avoid stuck inside plain
		if (hitPlain->getOrigin().x != 0) {
			if (hitPlain->getOrigin().x < 0) {
				this->position = glm::vec3(posCol.x + TOLERANCE, posCol.y, posCol.z);
			}
			else {
				this->position = glm::vec3(posCol.x - TOLERANCE, posCol.y, posCol.z);
			}
		}
		if (hitPlain->getOrigin().y != 0) {
			if (hitPlain->getOrigin().y < 0) {
				this->position = glm::vec3(posCol.x, posCol.y + TOLERANCE, posCol.z);
			}
			else {
				this->position = glm::vec3(posCol.x, posCol.y - TOLERANCE, posCol.z);
			}
		}
		if (hitPlain->getOrigin().z != 0) {
			if (hitPlain->getOrigin().z < 0) {
				this->position = glm::vec3(posCol.x, posCol.y , posCol.z + TOLERANCE);
			}
			else {
				this->position = glm::vec3(posCol.x, posCol.y, posCol.z - TOLERANCE);
			}
		}
		this->velocity = vnext;
		//this->force = gravForceMult;//clear force
		defaultForces();
		this->sphere->changeColor(hitPlain->getColor());
		lastHit = true;
	}
	else {//when no collision detected
		//update state
		this->position = nextPos;
		this->velocity = nextVel;
		//this->force = gravForceMult;//clear force
		defaultForces();
		lastHit = false;
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void Ball::renderUpdate() 
{
	sphere->translation(this->position);
}

////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getGravMult()
{
	return &this->gravMult;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getAirResist()
{
	return &this->airResist;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat Ball::getRadius()
{
	return this->radius;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getFriction()
{
	return &this->friction;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getElastic()
{
	return &this->elastic;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getMass()
{
	return &this->mass;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getPosX()
{
	return &this->position.x;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getPosY()
{
	return &this->position.y;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getPosZ()
{
	return &this->position.z;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getVelX()
{
	return &this->velocity.x;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getVelY()
{
	return &this->velocity.y;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getVelZ()
{
	return &this->velocity.z;
}

////////////////////////////////////////////////////////////////////////////////
void Ball::restoreDefault()
{
	this->position = glm::vec3(0);
	this->velocity = glm::vec3(0);
	this->windVelocity = glm::vec3(0);
	this->mass = DEFAULT_MASS;
	this->elastic = DEFAULT_ELASTIC;
	this->airResist = DEFAULT_AIRRESIST;
	this->gravMult = DEFAULT_GRAVMULT;
	this->radius = DEFAULT_RADIUS;
	*this->getSphereRadius() = DEFAULT_RADIUS;
	this->friction = DEFAULT_FRICTION;
	this->sphere->changeColor(glm::vec3(0.3, 0.3, 0.4));
	//initPos = glm::vec3(0.0f, 10.0f, 0.0f);
	//velocity = glm::vec3(0.0f, -1.0f, 0.0f);
}