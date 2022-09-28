#include "ParticleSystem.h"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         #include "ParticleSystem.h"


////////////////////////////////////////////////////////////////////////////////
ParticleSystem::ParticleSystem(std::vector<Plain*> plains, GLfloat mass, 
	GLfloat gravMult, GLfloat lifeSpan)
{
	this->restoreDefault();
	this->mass = mass;
	this->gravMult = gravMult;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	this->lifeSpan = lifeSpan;
	this->model = glm::mat4(1);
	this->velMag = 0;

	this->positions.reserve(3000);
	this->accelerations.reserve(3000);
	this->velocitys.reserve(3000);

	this->color = glm::vec3(1.0f, 0.95f, 0.1f);
	this->plains = plains;

	this->defaultForces();
	this->RoundOffError = 0;
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glDisable(GL_CULL_FACE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(std::vector<Plain*> plains, GLfloat mass,
	GLfloat gravMult, GLfloat lifeSpan, GLfloat velMag)
{
	this->restoreDefault();
	this->mass = mass;
	this->gravMult = gravMult;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	this->lifeSpan = lifeSpan;
	this->model = glm::mat4(1);
	this->velMag = velMag;

	this->positions.reserve(3000);
	this->accelerations.reserve(3000);
	this->velocitys.reserve(3000);

	this->color = glm::vec3(1.0f, 0.95f, 0.1f);
	this->plains = plains;

	this->defaultForces();
	this->RoundOffError = 0;
	this->radius = 2;
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glDisable(GL_CULL_FACE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

////////////////////////////////////////////////////////////////////////////////
ParticleSystem::~ParticleSystem()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::genParticles(GLfloat deltaTime) {
	int num = floor(deltaTime * crtRate);
	RoundOffError = RoundOffError + deltaTime * crtRate - num;
	if (RoundOffError > 1) {
		num++;
		RoundOffError = RoundOffError - 1;
	}
	for (int i = 0; i < num; i++) {
		//caculating initial space
		GLfloat initX = glm::gaussRand(initPos.x, (float)sqrt(posVarX));
		GLfloat initY = glm::gaussRand(initPos.y, (float)sqrt(posVarY));
		GLfloat initZ = glm::gaussRand(initPos.z, (float)sqrt(posVarZ));
		glm::vec3 iPos = glm::vec3(initX, initY, initZ);

		GLfloat velX = glm::gaussRand(velocity.x, (float)sqrt(velVarX));
		GLfloat velY = glm::gaussRand(velocity.y, (float)sqrt(velVarY));
		GLfloat velZ = glm::gaussRand(velocity.z, (float)sqrt(velVarZ));
		glm::vec3 iVel = glm::vec3(velX, velY, velZ);
		//Particle* particle = new Particle(initPos, initVel, lifeSpan,
		//									grav,airResist, 
		//									radius, elastic, friction, plain);
		//particles.push_back(particle);
		if (avaInd.empty() == false) {
			int ind = avaInd.back();
			avaInd.pop_back();
			positions[ind] = iPos;
			velocitys[ind] = iVel;
			lifeTimes[ind] = lifeSpan;
			isActive[ind] = true;
		}
		else {
			positions.push_back(iPos);
			velocitys.push_back(iVel);
			lifeTimes.push_back(lifeSpan);
			if (positions.size() != velocitys.size() || lifeTimes.size() != positions.size()) {
				std::cout << "Postion velocity lifetime size is not matching !!!\n";
			}
			isActive.push_back(true);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::genParticlesCircle(GLfloat deltaTime, GLfloat radius) {
	int num = floor(deltaTime * crtRate);
	RoundOffError = RoundOffError + deltaTime * crtRate - num;
	if (RoundOffError > 1) {
		num++;
		RoundOffError = RoundOffError - 1;
	}
	for (int i = 0; i < num; i++) {
		//caculating initial space
		//GLfloat initX = glm::gaussRand(initPos.x, (float)sqrt(posVarX));
		//GLfloat initY = glm::gaussRand(initPos.y, (float)sqrt(posVarY));
		//GLfloat initZ = glm::gaussRand(initPos.z, (float)sqrt(posVarZ));

		glm::vec3 init = glm::ballRand(radius);
		init = glm::vec3(initPos.x + init.x, initPos.y + init.y, initPos.z + init.z);
		glm::vec3 iPos = init;


		if (avaInd.empty() == false) {
			int ind = avaInd.back();
			avaInd.pop_back();
			positions[ind] = iPos;
			velocitys[ind] = this->velocity;
			lifeTimes[ind] = lifeSpan;
			isActive[ind] = true;
		}
		else {
			positions.push_back(iPos);
			velocitys.push_back(this->velocity);
			lifeTimes.push_back(lifeSpan);
			if (positions.size() != velocitys.size() || lifeTimes.size() != positions.size()) {
				std::cout << "Postion velocity lifetime size is not matching !!!\n";
			}
			isActive.push_back(true);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::cleanParticles()
{
	//clean particles
	for (int i = 0; i < lifeTimes.size(); i++) {
		GLfloat currLifeTime = lifeTimes[i];
		bool currActive = isActive[i];
		if (currLifeTime <= 0 && currActive == true) {
			avaInd.push_back(i);
			positions[i] = glm::vec3(100,100,100);
			isActive[i] = false;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::update(GLfloat deltaTime, glm::vec3 pos, glm::vec3 vel, GLfloat radius)
{
	//caclculate gravForce
	genParticlesCircle(deltaTime, radius);
	cleanParticles();
	this->initPos = pos;
	//this->velocity = -vel;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);

	//update position data 
	
	for (int i = 0; i < positions.size(); i++) {
		if (isActive[i] == true) {
			integration(i, deltaTime);
			//positions[i] = glm::vec3(positions[i].x, positions[i].y - 0.01, positions[i].z);

		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::update(GLfloat deltaTime, glm::vec3 pos, glm::vec3 vel)
{
	//caclculate gravForce
	genParticles(deltaTime);
	cleanParticles();
	this->initPos = pos;
	if (velMag != 0) {
		this->velocity = -glm::normalize(vel) * velMag;
	}
	else {
		this->velocity = -vel;
	}
	
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);

	//update position data 

	for (int i = 0; i < positions.size(); i++) {
		if (isActive[i] == true) {
			integration(i, deltaTime);
			//positions[i] = glm::vec3(positions[i].x, positions[i].y - 0.01, positions[i].z);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	renderUpdate();
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(this->radius);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, positions.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void ParticleSystem::integration(int index, GLfloat timeStep) {

	//get and check next state
	glm::vec3 acc = this->force / this->mass;
	//calculate x(n+1)
	glm::vec3 nextPos = this->positions[index] + velocitys[index] * timeStep;
	glm::vec3 nextVel = velocitys[index] + acc * timeStep;

	//detection

	std::vector<Plain*> hitPlains;
#pragma omp parallel
#pragma omp for
	for (int i = 0; i < plains.size(); i++) {
		Plain* p = plains[i];
		if (p->checkHit(positions[index], nextPos, velocitys[index], this->radius)) {
			hitPlains.push_back(p);
		}
	}

	//determination
	GLfloat f = 1;//give f a biggest value
	GLfloat restTime = 0.0f;
	//when there is at least one plain collide
	if (hitPlains.size() != 0) {
		Plain* hitPlain = hitPlains[0];
		//loop through all possible plains
		for (Plain* plain : hitPlains) {
			glm::vec3 p = plain->getOrigin();
			glm::vec3 nhat = plain->getNorm();
			//check signs
			GLfloat distN = glm::dot((positions[index] - p), nhat);
			GLfloat distNPONE = glm::dot((nextPos - p), nhat);

			glm::vec3 posCol = positions[index] - (1 + this->elastic) * distNPONE * nhat;
			glm::vec3 vn = glm::dot(nextVel, nhat) * nhat;
			glm::vec3 vt = nextVel - vn;
			glm::vec3 velCol = -this->elastic * vn + (1 - this->friction) * vt;
			this->positions[index] = posCol;
			this->velocitys[index] = velCol;
			this->lifeTimes[index] = this->lifeTimes[index] - timeStep;
			defaultForces();
		}
	}
	else {
		this->positions[index] = nextPos;
		this->velocitys[index] = nextVel;
		this->lifeTimes[index] = this->lifeTimes[index] - timeStep;
		defaultForces();
	}
	//renderUpdate();
}
////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::defaultForces()
{
	this->force = gravForce - this->airResist * this->velocity + this->airResist * this->windVelocity;
}
void ParticleSystem::renderUpdate()
{
	if (!positions.empty()) {

		// Bind VBO to the bound VAO, and store the point data
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
		// Enable Vertex Attribute 0 to pass point data through to the shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Unbind the VBO/VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::restoreDefault()
{
	elastic = DEFAULT_ELASTIC;
	airResist = DEFAULT_AIRRESIST;

	crtRate = DEFAULT_CRTRATE;
	lifeSpan = DEFAULT_LIFE_SPAN;
	lastLifeSpan = lifeSpan;
	grav = GRAV_COE;
	gravMult = DEFAULT_GRAVMULT;
	mass = DEFAULT_MASS;

	radius = DEFAULT_RADIUS;
	friction = DEFAULT_FRICTION;


	initPos = glm::vec3(0.0f, 20.0f, 0.0f);
	velocity = glm::vec3(0.0f, -1.0f, 0.0f);
	windVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	posVarX = 1;
	posVarY = 1;
	posVarZ = 1;
	velVarX = 1;
	velVarY = 0;
	velVarZ = 1;
}