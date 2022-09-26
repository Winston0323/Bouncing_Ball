#include "Cube.h"
////////////////////////////////////////////////////////////////////////////////
Cube::Cube(GLfloat size, glm::vec3 origin) 
{
	this->size = size;
	this->origin = origin;
	//top
	glm::vec3 topOrigin = glm::vec3(origin.x, origin.y + size/ 2, origin.z);
	top = new Plain(size, topOrigin, glm::vec3(0.1f, 0.1f, 0.5f));
	top->spin(180, glm::vec3(1, 0, 0));
	top->setNorm(glm::vec3(0, -1, 0));
	//bottom
	glm::vec3 botOrigin = glm::vec3(origin.x, origin.y - size / 2, origin.z);
	bot = new Plain(size, botOrigin, glm::vec3(0.9f, 0.9f, 0.5f));
	bot->setNorm(glm::vec3(0, 1, 0));
	//left 
	glm::vec3 leftOrigin = glm::vec3(origin.x - size / 2, origin.y, origin.z);
	left = new Plain(size, leftOrigin, glm::vec3(0.6f, 0.1f, 0.3f));
	left->spin(-90, glm::vec3(0,0,1));
	left->setNorm(glm::vec3(1, 0, 0));
	//right 
	glm::vec3 rightOrigin = glm::vec3(origin.x + size / 2, origin.y, origin.z);
	right = new Plain(size, rightOrigin, glm::vec3(0.4f, 0.9f, 0.7f));
	right->spin(90, glm::vec3(0, 0, 1));
	right->setNorm(glm::vec3(-1, 0, 0));
	//front 
	glm::vec3 frontOrigin = glm::vec3(origin.x, origin.y, origin.z + size / 2);
	front = new Plain(size, frontOrigin, glm::vec3(0.5f, 0.7f, 0.4f));
	front->spin(-90, glm::vec3(1, 0, 0));
	front->setNorm(glm::vec3(0, 0, -1));
	//back 
	glm::vec3 backOrigin = glm::vec3(origin.x, origin.y, origin.z - size / 2);
	back = new Plain(size, backOrigin, glm::vec3(0.5f, 0.3f, 0.6f));
	back->spin(90, glm::vec3(1, 0, 0));
	back->setNorm(glm::vec3(0, 0, 1));

	//add all plain in scene
	this->plains.push_back(top);
	this->plains.push_back(bot);
	this->plains.push_back(left);
	this->plains.push_back(right);
	this->plains.push_back(front);
	this->plains.push_back(back);

}

////////////////////////////////////////////////////////////////////////////////

Cube::~Cube()
{

}

////////////////////////////////////////////////////////////////////////////////
void Cube::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	for (Plain* p : plains) {
		p->draw(viewProjMtx, shader);
	}
}
////////////////////////////////////////////////////////////////////////////////
std::vector<Plain*> Cube::checkHit(glm::vec3 position, GLfloat radius) {
	std::vector<Plain*> result;
	if (position.x - radius<= -this->size / 2 + EPSILON) {
		//left
		result.push_back(left);
	}
	else if (position.x + radius>= this->size / 2 - EPSILON) {
		//right
		result.push_back(right);
	}
	else if (position.y + radius>= this->size / 2 - EPSILON) {
		//top
		result.push_back(top);
	}
	else if (position.y - radius<= -this->size / 2 + EPSILON) {
		//bottom
		result.push_back(bot);
	}
	else if (position.z + radius>= this->size / 2 - EPSILON) {
		//front
		result.push_back(front);
	}
	else if (position.z - radius <= -this->size / 2 + EPSILON) {
		//back
		result.push_back(back);
	}
	else {
		//printf("Inside box");
		
	}
	return result;
}
