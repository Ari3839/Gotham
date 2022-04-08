#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture TimTexture;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	//En create objects: 
	unsigned int vegetacionIndices[] = {
			0, 1, 2,
			0, 2, 3,
			4,5,6,
			4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
	// fin create objects
}

void CrearPersonaje()
{
	unsigned int cara_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	
	GLfloat cara_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.09375f,	0.87697f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.26660f,	0.87697f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.26660f,	0.99902f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.09375f,	0.99902f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.46584f,	0.36994f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.61035f,	0.36994f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.61035f,	0.49611f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.46584f,	0.49611f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.46584f,	0.36994f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.61035f,	0.36994f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.61035f,	0.49611f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.46584f,	0.49611f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.46584f,	0.36994f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.61035f,	0.36994f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.61035f,	0.49611f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.46584f,	0.49611f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.89551f,	0.87009f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.99802f,	0.87009f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.99802f,	0.77541f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.89551f,	0.77541f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int cuerpo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cuerpo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.07812f,	0.65529,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.28613f,	0.65429f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.28613f,	0.85156f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.07812f,	0.85156f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.10742f,	0.15234f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.25390f,	0.15234f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.25390f,	0.35056f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.10742f,	0.35056f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.45019f,  0.65234f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.65625f,	0.65234f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.65625f,	0.84765f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.45019f,	0.84765f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.10742f,	0.15234f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.25390f,	0.15234f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.25390f,	0.35056f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.10742f,	0.35056f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.752929f,	0.99802f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.87792f,	0.99902f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.87792f,	0.89648f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.752929f,	0.89648f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.75097f,		0.39648f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.88083f,	0.39648f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.88083f,	0.50781f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.75097f,		0.50781f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int brazoIzq_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat brazoIzq_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.00392f,	0.69726f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.06152f,	0.69726f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.06152f,	0.84472f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.00392f,	0.84472f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,		0.29396f,	0.20312f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.36621f,	0.20312f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.36621f,	0.35156f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.29396f,	0.35156f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.67089f,	0.69628f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.72949f,	0.69628f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.72949f,	0.84277f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.67089f,	0.84277f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.29396f,	0.20312f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.36621f,	0.20312f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.36621f,	0.35156f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.29396f,	0.35156f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.89553f,	0.77539f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.99804f,	0.77539f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.99804f,	0.87011f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.89553f,	0.87011f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,	0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int brazoDer_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat brazoDer_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.06152f,	0.69726f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.00392f,	0.69726f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.00392f,	0.84472f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.06152f,	0.84472f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.36621f,	0.20312f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.29396f,	0.20312f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.29396f,	0.35156f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.36621f,	0.35156f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.72949f,	0.69628f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.67089f,	0.69628f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.67089f,	0.84277f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.72949f,	0.84277f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.36621f,	0.20312f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.29396f,	0.20312f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.29396f,	0.35156f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.36621f,	0.35156f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.89553f,	0.77539f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.99804f,	0.77539f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.99804f,	0.87011f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.89553f,	0.87011f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int piernaIzq_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat piernaIzq_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.08107f,	0.5f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.14941f,	0.5f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.14941f,	0.64550f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.08107f,	0.64550f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.88867f,  0.53320f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.53320f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	0.63085f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.88867f,	0.63085f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.45214f,	0.498f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.52148f,	0.498f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.52148f,	0.64157f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.45214f,	0.64157f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.06640f,	0.00000f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.14160f,	0.00000f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.14160f,	0.14843f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.06640f,	0.14843f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int piernaDer_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat piernaDer_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.14941f,	0.5f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.08107f,	0.5f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.08107f,	0.64550f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.14941f,	0.64550f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.14160f,	0.00000f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.06640f,	0.00000f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.06640f,	0.14843f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.14160f,	0.14843f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.52148f,	0.498f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.45214f,	0.498f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.45214f,	0.64157f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.52148f,	0.64157f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.88867f,  0.53320f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.53320f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	0.63085f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.88867f,	0.63085f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int pieIzq_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat pieIzq_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		1.0f,	0.53320f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		1.0f,	0.63085f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.88867f,	0.63085f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.88867f,  0.53320f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.53320f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	0.63085f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.88867f,	0.63085f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.88867f,  0.53320f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.53320f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	0.63085f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.88867f,	0.63085f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.88867f,  0.53320f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.53320f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	0.63085f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.88867f,	0.63085f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int pieDer_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat pieDer_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		1.0f,	0.53320f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		1.0f,	0.63085f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.88867f,	0.63085f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.88867f,  0.53320f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.53320f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	0.63085f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.88867f,	0.63085f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.88867f,  0.53320f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.53320f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	0.63085f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.88867f,	0.63085f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.88867f,  0.53320f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.53320f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	0.63085f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.88867f,	0.63085f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.88867f,  0.53320f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.53320f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	0.63085f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.88867f,	0.63085f,		0.0f,	-1.0f,	0.0f,
	};

	unsigned int cuello_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat cuello_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.89553f,	0.77539f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.99804f,	0.77539f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.99804f,	0.87011f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.89553f,	0.87011f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.89553f,	0.77539f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.99804f,	0.77539f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.99804f,	0.87011f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.89553f,	0.87011f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.89553f,	0.77539f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.99804f,	0.77539f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.99804f,	0.87011f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.89553f,	0.87011f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.89553f,	0.77539f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.99804f,	0.77539f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.99804f,	0.87011f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.89553f,	0.87011f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.89553f,	0.77539f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.99804f,	0.77539f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.99804f,	0.87011f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.89553f,	0.87011f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.89553f,	0.77539f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.99804f,	0.77539f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.99804f,	0.87011f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.89553f,	0.87011f,		0.0f,	-1.0f,	0.0f,
	};

	Mesh *cara = new Mesh();
	cara->CreateMesh(cara_vertices, cara_indices, 192, 36);
	meshList.push_back(cara);

	Mesh *cuerpo = new Mesh();
	cuerpo->CreateMesh(cuerpo_vertices, cuerpo_indices, 192, 36);
	meshList.push_back(cuerpo);

	Mesh* b1 = new Mesh();
	b1->CreateMesh(brazoIzq_vertices, brazoIzq_indices, 192, 36);
	meshList.push_back(b1);

	Mesh* b2 = new Mesh();
	b2->CreateMesh(brazoDer_vertices, brazoDer_indices, 192, 36);
	meshList.push_back(b2);

	Mesh* p1 = new Mesh();
	p1->CreateMesh(piernaIzq_vertices, piernaIzq_indices, 192, 36);
	meshList.push_back(p1);

	Mesh* p2 = new Mesh();
	p2->CreateMesh(piernaDer_vertices, piernaDer_indices, 192, 36);
	meshList.push_back(p2);

	Mesh* pie1 = new Mesh();
	pie1->CreateMesh(pieIzq_vertices, pieIzq_indices, 192, 36);
	meshList.push_back(pie1);

	Mesh* pie2 = new Mesh();
	pie2->CreateMesh(pieDer_vertices, pieDer_indices, 192, 36);
	meshList.push_back(pie2);

	Mesh* cuello = new Mesh();
	cuello->CreateMesh(cuello_vertices, cuello_indices, 192, 36);
	meshList.push_back(cuello);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearPersonaje();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	TimTexture = Texture("Textures/TimTexture.tga");
	TimTexture.LoadTextureA();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		//piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Personaje
		
		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.4f, 3.2f, 1.6f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[5]->RenderMesh();
		
		//cuello
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.6f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.2f, 0.7f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[12]->RenderMesh();
		
		//cara
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 1.1f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.6f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[4]->RenderMesh();
		
		//brazo izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.55f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 2.4f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[6]->RenderMesh();
		
		//brazo der
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.55f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 2.4f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[7]->RenderMesh();
		
		//pierna izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, -2.5f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.9f, 1.9f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[8]->RenderMesh();
		
		//pie izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.25f));
		model = glm::scale(model, glm::vec3(0.9f, 0.5f, 1.3f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[10]->RenderMesh();

		//pierna der
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.75f, -2.5f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.9f, 1.9f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[9]->RenderMesh();
		
		//pie der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.25f));
		model = glm::scale(model, glm::vec3(0.9f, 0.5f, 1.3f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[11]->RenderMesh();
		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
