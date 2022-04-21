//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <Windows.h> //sleep

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

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera cameraPiso;
Camera cameraAerea;

Texture plainTexture;
Texture pisoTexture;
Texture TimTexture;

Skybox skybox;
Skybox skybox2;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

GLint count = 0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

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

	Mesh* cara = new Mesh();
	cara->CreateMesh(cara_vertices, cara_indices, 192, 36);
	meshList.push_back(cara);

	Mesh* cuerpo = new Mesh();
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
	Shader* shader1 = new Shader();
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

	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	cameraPiso = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -50.0f, 0.0f, 0.5f, 0.5f);
	cameraAerea = Camera(glm::vec3(0.0f, 350.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 0.5f, 0.5f);


	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	TimTexture = Texture("Textures/TimTexture.tga");
	TimTexture.LoadTextureA();

	Model Camino_M;
	Model Batmobile_M; 
	Model llanta_B_M;
	Model WE_M;
	Model ClockTower_M;
	Model Coffe_M;
	Model Building1_M;
	Model Building2_M;
	Model Building3_M;
	Model Building4_M;
	Model Building5_M;
	Model Building6_M;
	Model Tree_M;
	Model Tree2_M;
	Model Tree3_M;
	Model tren_M;
	Model BaseTren_M;
	Model Auto1_M;
	Model Auto2_M;
	Model Dirigible_M;
	Model Patrulla_M;
	Model Moto1_M;
	Model Moto2_M;
	Model Moto3_M;
	Model helicopter_M;
	Model helice_M;
	Model Nw_M;
	Model Rh_M;

	Model Pole_M;
	Model Banca_M;
	Model Batiseñal_M;
	Model BlueBird_M;
	Model BlueBirdWings_M;
	Model Bote_M;
	Model Semaforo_M;
	Model PhoneBox_M;

	//Personajes
	Nw_M = Model();
	Nw_M.LoadModel("Modelos_obj/Personas/Nightwing.obj");
	Rh_M = Model();
	Rh_M.LoadModel("Modelos_obj/Personas/RedHood2.obj");

	//Calle
	Camino_M = Model();
	Camino_M.LoadModel("Modelos_obj/Others-street/camino.obj");
	Tree_M = Model();
	Tree_M.LoadModel("Modelos_obj/Arbol/Tree_obj.obj");
	Tree2_M = Model();
	Tree2_M.LoadModel("Modelos_obj/Arbol/arbol2.obj");
	Tree3_M = Model();
	Tree3_M.LoadModel("Modelos_obj/Arbol/Jacaranda.obj");
	BlueBird_M = Model();
	BlueBird_M.LoadModel("Modelos_obj/Others-street/Bluebird.obj");
	BlueBirdWings_M = Model();
	BlueBirdWings_M.LoadModel("Modelos_obj/Others-street/Bluebird_wings.obj");
	Pole_M = Model();
	Pole_M.LoadModel("Modelos_obj/Poles/pole2.obj");
	Banca_M = Model();
	Banca_M.LoadModel("Modelos_obj/Others-street/banca.obj");
	Batiseñal_M = Model();
	Batiseñal_M.LoadModel("Modelos_obj/Others-street/batisenal2.obj");
	Semaforo_M = Model();
	Semaforo_M.LoadModel("Modelos_obj/Others-street/semaforo.obj");
	PhoneBox_M = Model();
	PhoneBox_M.LoadModel("Modelos_obj/Others-street/phonebox.obj");
	Bote_M = Model();
	Bote_M.LoadModel("Modelos_obj/Others-street/boteBasura.obj");
	
	//Transportes
	Batmobile_M = Model();
	Batmobile_M.LoadModel("Modelos_obj/Batmobile.obj");
	llanta_B_M = Model();
	llanta_B_M.LoadModel("Modelos_obj/LlantaBatmobile.obj");
	tren_M = Model();
	tren_M.LoadModel("Modelos_obj/Transportes/tren.obj");
	BaseTren_M = Model();
	BaseTren_M.LoadModel("Modelos_obj/Transportes/baseTren.obj");
	Auto1_M = Model();
	Auto1_M.LoadModel("Modelos_obj/Transportes/Auto1.obj");
	Auto2_M = Model();
	Auto2_M.LoadModel("Modelos_obj/Transportes/Auto2.obj");
	Dirigible_M = Model();
	Dirigible_M.LoadModel("Modelos_obj/Transportes/Dirigible.obj");
	Patrulla_M = Model();
	Patrulla_M.LoadModel("Modelos_obj/Transportes/Patrulla.obj");
	Moto1_M = Model();
	Moto1_M.LoadModel("Modelos_obj/Transportes/Moto.obj");
	Moto2_M = Model();
	Moto2_M.LoadModel("Modelos_obj/Transportes/moto2.obj");
	Moto3_M = Model();
	Moto3_M.LoadModel("Modelos_obj/Transportes/moto3.obj");
	helicopter_M = Model();
	helicopter_M.LoadModel("Modelos_obj/Transportes/helicopter.obj");
	helice_M = Model();
	helice_M.LoadModel("Modelos_obj/Transportes/helicopter_helice.obj");

	//Edificios
	WE_M = Model();
	WE_M.LoadModel("Modelos_obj/Edificios/WE.obj");
	ClockTower_M = Model();
	ClockTower_M.LoadModel("Modelos_obj/Edificios/clockTower.obj");
	Coffe_M = Model();
	Coffe_M.LoadModel("Modelos_obj/Edificios/coffeeShop.obj");
	Building1_M = Model();
	Building1_M.LoadModel("Modelos_obj/Edificios/building1.obj");
	Building2_M = Model();
	Building2_M.LoadModel("Modelos_obj/Edificios/building2.obj");
	Building3_M = Model();
	Building3_M.LoadModel("Modelos_obj/Edificios/building3.obj");
	Building4_M = Model();
	Building4_M.LoadModel("Modelos_obj/Edificios/building4.obj");
	Building5_M = Model();
	Building5_M.LoadModel("Modelos_obj/Edificios/building5.obj");
	Building6_M = Model();
	Building6_M.LoadModel("Modelos_obj/Edificios/building6.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/cupertin-lake_rt_noche.tga");
	skyboxFaces2.push_back("Textures/Skybox/cupertin-lake_lf_noche.tga");
	skyboxFaces2.push_back("Textures/Skybox/cupertin-lake_dn_noche.tga");
	skyboxFaces2.push_back("Textures/Skybox/cupertin-lake_up_noche.tga");
	skyboxFaces2.push_back("Textures/Skybox/cupertin-lake_bk_noche.tga");
	skyboxFaces2.push_back("Textures/Skybox/cupertin-lake_ft_noche.tga");
	
	skybox2 = Skybox(skyboxFaces2);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	/*
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.7f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.7f, 1.0f,//amb (igual que en la direccional),difusa (intensidad,no alcance)
		2.0f, 1.5f, 1.5f,//Pos
		0.3f, 0.2f, 0.1f); //Ec A=0 o C=0 y B>>>A,C
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.7f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.7f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz de helicóptero
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		0.7f, 2.0f,
		0.0f, 5.0f, -1.0,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		60.0f);
	spotLightCount++;

	//luz de faro del auto
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.7f, 5.0f,
		0.0f, 0.3f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++; */


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

		if (mainWindow.getCamAerea()) {
			cameraAerea.keyControl(mainWindow.getsKeys(), deltaTime, mainWindow.getCamAerea());
			cameraAerea.mouseControl(mainWindow.getXChange());
		}
		else {
			cameraPiso.keyControl(mainWindow.getsKeys(), deltaTime, mainWindow.getCamAerea());
			cameraPiso.mouseControl(mainWindow.getXChange());
		}

		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if ((count < 1000) && (mainWindow.getCamAerea())) {
			skybox.DrawSkybox(cameraAerea.calculateViewMatrix(), projection);
		}else if ((count < 1000) && (mainWindow.getCamAerea()==false)) {
			skybox.DrawSkybox(cameraPiso.calculateViewMatrix(), projection);
		}else if ((count >= 1000) && (mainWindow.getCamAerea())) {
			skybox2.DrawSkybox(cameraAerea.calculateViewMatrix(), projection);
		}else {
			skybox2.DrawSkybox(cameraPiso.calculateViewMatrix(), projection);
		}
		count++;
		if (count >= 2000) {
			count = 0;
		}


		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		if (mainWindow.getCamAerea()) {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraAerea.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, cameraAerea.getCameraPosition().x, cameraAerea.getCameraPosition().y, cameraAerea.getCameraPosition().z);
		}
		else {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraPiso.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, cameraPiso.getCameraPosition().x, cameraPiso.getCameraPosition().y, cameraPiso.getCameraPosition().z);
		}

		

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

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

		//////////////////////////////////////////////////////////////////////////////////////////

		//Personaje

		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -18.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.2f, 1.6f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[4]->RenderMesh();

		//cuello
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.4f, 0.1f, 0.35f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[11]->RenderMesh();

		//cara
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[3]->RenderMesh();

		//brazo izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.775f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 1.2f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[5]->RenderMesh();

		//brazo der
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.775f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 1.2f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[6]->RenderMesh();

		//pierna izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.375f, -1.25f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.45f, 0.95f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[7]->RenderMesh();

		//pie izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.125f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[9]->RenderMesh();

		//pierna der
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.375f, -1.25f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.45f, 0.95f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[8]->RenderMesh();

		//pie der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.125f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		meshList[10]->RenderMesh();

		//////////////////////////////////////////////////////////////////////////////////////////
		//WE 0,0
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -3.3f, -60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 7.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WE_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 1.4f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 1.4f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 1.4f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 1.4f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Phonebox
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.4f, 20.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PhoneBox_M.RenderModel();

		//Phonebox
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.4f, -20.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PhoneBox_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 1.4f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 1.4f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 1.4f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 1.4f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 1.3f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 1.3f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 1.3f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 1.3f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////

		//ClockTower 1,0
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -3.3f, -60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ClockTower_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 1.4f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 1.4f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 1.4f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 1.4f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-72.0f, 1.4f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 1.4f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 1.4f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();


		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 1.4f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 1.4f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();


		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.3f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 1.3f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 1.3f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 1.3f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//Building 1,-1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -2.0f, -190.0f));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building1_M.RenderModel();

		//RedHood
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -2.0f, -145.0f));
		model = glm::scale(model, glm::vec3(0.48f, 0.5f, 0.48f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rh_M.RenderModel();

		//Moto RedHood
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.9f, -135.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto1_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-72.0f, 0.1f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, 45.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 45.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 45.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Building3 0,-1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, -190.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building3_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(72.0f, 2.1f, 45.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 2.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 2.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();


		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();


		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 2.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 2.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();


		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Building5 -1,-1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -4.0f, -190.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building5_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(72.0f, 2.1f, 45.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 2.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 2.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 2.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Building2 -1,0
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -2.0f, -60.0f));
		modelaux = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building2_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(72.0f, 0.1F, -45.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Building6 -1,1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -4.7f, 70.0f));
		modelaux = model;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building6_M.RenderModel();

		//Nigthwing
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.48f, 0.49f, 0.48f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nw_M.RenderModel();

		//Batiseñal
		model = modelaux;
		model = glm::translate(model, glm::vec3(-12.0f, 85.5f, -24.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Batiseñal_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(35.0f, 2.7f, 35.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-35.0f, 2.7f, 35.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(35.0f, 2.7f, -35.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-35.0f, 2.7f, -35.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 2.7f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 2.7f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(72.0f, 2.8f, -45.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 2.7f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 2.7f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 2.7f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 2.7f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 2.7f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 2.7f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 2.7f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 2.7f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();


		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Building4 0,1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.6f, 70.0f));
		modelaux = model;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building4_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 0.6f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.6f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 0.6f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.6f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-72.0f, 0.7f, -45.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();


		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.6f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.6f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 0.6f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.6f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.6f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.6f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 0.6f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 0.6f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 0.6f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(70.0f, 0.6f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//CoffeeShop 1,1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -2.0, 70.0f));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coffe_M.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree_M.RenderModel();

		//Jacaranda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tree3_M.RenderModel();

		//Semaforo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-72.0f, 0.1f, -45.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();

		//Bote
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_M.RenderModel();


		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		//pole
		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pole_M.RenderModel();

		
		////////////////////////////////////////////////////////////////////////////////////////////
		//Calle

		//camino der
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, -1.5f, 5.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 23.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, 1.2f, 40.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();
		
		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, 1.2f, -100.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, 1.2f, 120.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, 1.2f, 240.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, 1.2f, -240.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//Patrulla
		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -0.4f, -22.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 70.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 170.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		////////////////////////////////////////////////////////////////////////////////////////////
		//camino izq
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.5f, 5.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 23.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 30.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -110.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 130.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 240.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -240.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//Patrulla
		model = modelaux;
		model = glm::translate(model, glm::vec3(-9.0f, -0.4f, -22.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 70.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 170.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//Moto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		////////////////////////////////////////////////////////////////////////////////////////////
		//camino atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.9f, -125.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(23.0f, 2.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, -0.3f, -9.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(170.0f, 1.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-130.0f, 1.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		////////////////////////////////////////////////////////////////////////////////////////////
		//camino frente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.9f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(23.0f, 2.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();

		//Auto
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, -0.3f, 9.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		////////////////////////////////////////////////////////////////////////////////////////////
		//Transportes

		//Plataforma tren
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(24.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseTren_M.RenderModel();

		//Tren
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tren_M.RenderModel();

		//Dirigible
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 150.0f, 100.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dirigible_M.RenderModel();

		//Helicopter
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 150.0f, -130.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helicopter_M.RenderModel();

		//helice
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 3.8f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helice_M.RenderModel();
		
		
		//batmobile
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.55f, -5.0f));
		//model = glm::translate(model, glm::vec3(mainWindow.getAutomovil(), 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Batmobile_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.44f, -1.4f, -1.6f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		//model = glm::rotate(model, mainWindow.getLlanta(), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();


		//Llanta posterior derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.1f, -1.4f, -1.6f));
		//model = glm::rotate(model, mainWindow.getLlanta(), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();

		//Llanta delantera izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.44f, -1.4f, 1.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getLlanta(), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();

		//Llanta posterior izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.1f, -1.4f, 1.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getLlanta(), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();
		

		//Bird
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 100.0f, 50.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlueBird_M.RenderModel();

		//alas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlueBirdWings_M.RenderModel();
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
