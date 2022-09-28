#pragma once
#ifndef _PLAIN_H_
#define _PLAIN_H_

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Plain
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;
	glm::vec3 origin;
	glm::vec3 norm;

	// Plain Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

public:
	Plain(GLfloat size, glm::vec3 origin, glm::vec3 color);
	//Plain(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	~Plain();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();
	void spin(GLfloat deg, glm::vec3 axis);
	void setNorm(glm::vec3 norm) { this->norm = norm; }
	glm::vec3 getNorm() { return norm; }
	glm::vec3 getOrigin() { return origin; }
	glm::vec3 getColor() { return color; }
	bool checkHit(glm::vec3 pos, glm::vec3 nextPos, glm::vec3 vel, GLfloat radius);
};

////////////////////////////////////////////////////////////////////////////////

#endif
