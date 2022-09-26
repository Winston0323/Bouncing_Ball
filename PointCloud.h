#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"
#include "Material.h"
#include "Light.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	GLuint VAO, VBOvertex, VBOnormal, EBO;
	GLfloat pointSize;
	Material material;
	glm::vec3 center;
	bool shadeForLight;


public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader, const glm::vec3 viewPos, Light& light);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float angle, glm::vec3 axis);
	void objParser(string objFilename);
	void scale(float level);
	void translation(glm::vec3 destination);
	void litTrans(glm::vec3 destination);
	void setMaterial(Material material);
	void setNormalLighting(bool normal);
	glm::vec3 getCenter();
	void setCenter(glm::vec3 center);
	void setShadeMode();

};

#endif
