/*
 * Toroide.h
 * Basado en el código de esfera creado por
 * Created on: 06/09/2016
 * Author: reynaldo Martell
 */

#ifndef TOROIDE_H_
#define TOROIDE_H_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "definition.h"

class Toroide {
public:
	Toroide(float r, float R, int slices, int stacks);
	void init();
	void load();
	void render();
	virtual ~Toroide();
private:
	std::vector<VertexColor> vertexC;
	std::vector<GLuint> index;
	float r, R;
	int slices;
	int stacks;

	int _numIndices = 0;
	int _primitiveRestartIndex = 0;


	GLuint VAO, VBO, EBO;
};

#endif
