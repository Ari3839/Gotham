#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <Windows.h> //sleep
#include <bass.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Sound.h"
const float toRadians = 3.14159265f / 180.0f;

//Movimiento ave (compleja automática)
float xbird = 0.0f, ybird = 0.0f, zbird = 0.0f, rotBird = 0.0f, rotWings = 0.0f, movbird =0.0f;
float countBird = 0.0f;
bool wings = true, sube = true;

//Movimiento batmobile (simple por teclado)
float movBatmobile = 0.0f, movBatmobile2 = 0.0f, rotllanta = 0.0f, rotBatmobile = 180.0f;
int batmobile = 1;

//Movimiento dirigible (compleja *Keyframe)
float movDirigible = 0.0f, a = 0.0f, b = 0.0f, countDirigible = 0.0f;

//Movimiento show de luces (compleja por teclado)
float xluz1 = 0.0f, zluz1 = 0.0f, xluz2 = 0.0f, zluz2 = 0.0f, xluz3 = 0.0f, zluz3 = 0.0f, movLuz=0.0f;
int luces = 1;

//Movimiento batiseñal (simple automática)
float rotBatsignal =0.0f, escalaBatsignal=1.0f;

//Movimiento RedHood (compleja) automática
float xRh= 125.0f, yRh= 0.0f, zRh= -250.0f;
float brazo1 = 0.0f, brazo2 = 0.0f, pierna1 = 0.0f, pierna2 = 0.0f, cuerpo1 = 0.0f;
bool movBrazo = true, movBrazo2 = false, movPierna1=false, movPierna2=true, fin1=false;
int  countDisp = 0;

//Movimiento Nightwing (compleja) automática
float xNw = -230.0f, yNw = 0.3f, zNw = 17.0f;
float brazo3 = 0.0f, brazo4 = 0.0f, pierna3 = 0.0f, pierna4 = 0.0f, cuerpo2 = 0.0f, movY=0.0f;
bool movBrazo3 = true, movBrazo4 = false, movPierna3 = false, movPierna4 = true, movCuerpo2 = true, salto = false, fin2 = false;
int  countSalto = 0;

//Movimiento Tim (simple) por teclado
float brazo5 = 0.0f, brazo6 = 0.0f, pierna5 = 0.0f, pierna6 = 0.0f, cuerpo3 = 0.0f;
float staffScale = 1.0f, staffRot = 0.0f, movimientoY = 0.0f, ganchoTras = 0.0f, ganchoRot = 0.0f, ganchoMovZ=0.0f;
bool movBrazo5 = true, movBrazo6 = false, movPierna5 = false, movPierna6 = true, movCuerpo3 = true, fin3 = false;
int cambio = 1;

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

Sound music = Sound("Music/The Batman - Michael Giacchino.mp3");
Sound Tim = Sound("Music/twenty one pilots Stressed Out.mp3");
Sound Jason = Sound("Music/Bohnes - Middle Finger.mp3");
Sound Dick = Sound("Music/Fall Out Boy - Where Did The Party Go.mp3");
Sound Disparo = Sound("Music/Disparo.mp3");

Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
unsigned int count = 0, countSem=0, cicloDia=2000, cicloDiaNoche=4000;

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


//Animacion helicoptero
void inputKeyframes_Helicopter(bool* keys);
bool animacion_Helicopter = false;
float reproduciranimacion_Helicopter, habilitaranimacion_Helicopter, guardoFrame_Helicopter, reinicioFrame_Helicopter, ciclo_Helicopter, ciclo2_Helicopter;
float leoFrame_Helicopter=0.0f, reinicioLeoFrame = 0.0f, contador_Helicopter = 0;
bool xokay=false, yokay = false, zokay = false, heliceokay=false;

glm::vec3 posh = glm::vec3(0.0f, 0.0f, 0.0f);
float posXh = 190.0f, posYh = 18.0f, posZh = 160.0f;

//NEW// Keyframes
float movh_x = 0.0f, movh_y = 0.0f;
float giroh = 0, rot_helice=0.0f;

#define MAX_FRAMES_Helicopter 10 //Cuantos cuadros se guardan
int i_max_steps_Helicopter = 90; //Valores intermedios, entre mayor, mas suave se ve, pero ocupa mas recursos
int i_curr_steps_Helicopter = 3; //Numero de frames declarados
typedef struct _frame_Helicopter
{
	//Variables para GUARDAR Key Frames
	float movh_x;
	float movh_y;
	float movh_xInc;
	float movh_yInc;
	float giroh;
	float girohInc;
	float rot_helice;
	float rot_heliceInc;
	//Por cada variable, se necesita otra incremental
}FRAME;

FRAME KeyFrame_Helicopter[MAX_FRAMES_Helicopter];
int FrameIndex_Helicopter = 3;			//introducir datos
bool play_Helicopter = false;
int playIndex_Helicopter = 0;

void saveFrame_Helicopter(void)
{
	using namespace std;
	printf("Se guardo el frame (%d)\n", FrameIndex_Helicopter);

	KeyFrame_Helicopter[FrameIndex_Helicopter].movh_x = movh_x;
	KeyFrame_Helicopter[FrameIndex_Helicopter].movh_y = movh_y;
	KeyFrame_Helicopter[FrameIndex_Helicopter].giroh = giroh;
	KeyFrame_Helicopter[FrameIndex_Helicopter].rot_helice = rot_helice;

	ofstream archivo("Frames_Helicoptero.txt", ios::app);
	if (archivo.is_open()) {
		archivo << "****************************** " << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].movh_x=" + std::to_string(movh_x) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].movh_y=" + std::to_string(movh_y) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].giroh=" + std::to_string(giroh) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].rot_helice=" + std::to_string(rot_helice) << endl;
		/*archivo << "\ni_curr_steps= " + std::to_string(i_curr_steps) << endl;*/
		archivo.close();
	}
	else cerr << "Error de apertura del archivo." << endl;

	FrameIndex_Helicopter++;
}

void readFrame_Helicopter(void)
{
	using namespace std;
	string aux = "";
	fstream fichero;
	char linea[200];
	fichero.open("Frames_Helicoptero.txt", ios::in);
	if (fichero.fail()) {
		cerr << "Error al abrir el archivo Pruebas.txt" << endl;
	}
	else {
		fichero >> linea;           // Primera linea
		while (!fichero.eof())      
		{
			//cout << texto << endl;    // Muestrar el contenido en terminal 
			if (linea[0] == 'K') {
				if (linea[17] == 'x') {
					aux = aux + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
					movh_x = stof(aux);
					aux = "";
					xokay = true;
					cout << "movh_x =" + to_string(movh_x) << endl;
				}
				else if (linea[17] == 'y') {
					aux = aux + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
					movh_y = stof(aux);
					aux = "";
					yokay = true;
					cout << "movh_y =" + to_string(movh_y) << endl;
				}
				else if (linea[12] == 'g') {
					aux = aux + linea[18] + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24];
					giroh = stof(aux);
					aux = "";
					zokay = true;
					cout << "giroh =" + to_string(giroh) << endl;
				}
				else if (linea[12] == 'r') {
					aux = aux + linea[23] + linea[24] + linea[25] + linea[26] + linea[27] + linea[28];
					rot_helice = stof(aux);
					aux = "";
					heliceokay = true;
					cout << "rot_helice =" + to_string(rot_helice) << endl;
				}
			}

			if (xokay && yokay && zokay && heliceokay) {
				xokay = false;
				yokay = false;
				zokay = false;
				heliceokay = false;
				KeyFrame_Helicopter[FrameIndex_Helicopter].movh_x = movh_x;
				KeyFrame_Helicopter[FrameIndex_Helicopter].movh_y = movh_y;
				KeyFrame_Helicopter[FrameIndex_Helicopter].giroh = giroh;
				KeyFrame_Helicopter[FrameIndex_Helicopter].rot_helice = rot_helice;
				cout << "\nKeyFrame [" + to_string(FrameIndex_Helicopter) + "] leido correctamente" << endl;
				FrameIndex_Helicopter++;
			}

			fichero >> linea;         // Seguimos leyendo 
		}
		fichero.close();
	}
}


void resetElements_Helicopter(void)
{
	movh_x = KeyFrame_Helicopter[0].movh_x;
	movh_y = KeyFrame_Helicopter[0].movh_y;
	giroh = KeyFrame_Helicopter[0].giroh;
	rot_helice = KeyFrame_Helicopter[0].rot_helice;
}

void interpolation_Helicopter(void)
{
	KeyFrame_Helicopter[playIndex_Helicopter].movh_xInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].movh_x - KeyFrame_Helicopter[playIndex_Helicopter].movh_x) / i_max_steps_Helicopter;
	KeyFrame_Helicopter[playIndex_Helicopter].movh_yInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].movh_y - KeyFrame_Helicopter[playIndex_Helicopter].movh_y) / i_max_steps_Helicopter;
	KeyFrame_Helicopter[playIndex_Helicopter].girohInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].giroh - KeyFrame_Helicopter[playIndex_Helicopter].giroh) / i_max_steps_Helicopter;
	KeyFrame_Helicopter[playIndex_Helicopter].rot_heliceInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].rot_helice - KeyFrame_Helicopter[playIndex_Helicopter].rot_helice) / i_max_steps_Helicopter;
}

void animate_Helicopter(void)
{
	//Movimiento del objeto
	if (play_Helicopter)
	{
		if (i_curr_steps_Helicopter >= i_max_steps_Helicopter) //end of animation between frames
		{
			playIndex_Helicopter++;
			printf("Frame (%d) reproducido\n", playIndex_Helicopter);
			if (playIndex_Helicopter > FrameIndex_Helicopter - 2)	//end of total animation
			{
				printf("El ultimo frame es [%d]\n", FrameIndex_Helicopter);
				printf("Termina animacion\n");
				playIndex_Helicopter = 0;
				play_Helicopter = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps_Helicopter = 0; //Reset counter
				//Interpolation
				interpolation_Helicopter();
			}
		}
		else
		{
			//Draw animation
			movh_x += KeyFrame_Helicopter[playIndex_Helicopter].movh_xInc;
			movh_y += KeyFrame_Helicopter[playIndex_Helicopter].movh_yInc;
			giroh += KeyFrame_Helicopter[playIndex_Helicopter].girohInc;
			rot_helice += KeyFrame_Helicopter[playIndex_Helicopter].rot_heliceInc;
			i_curr_steps_Helicopter++;
		}
	}
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearPersonaje();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 10.0f, 0.5f);
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
	Model Estacion_M;
	Model Auto1_M;
	Model Auto2_M;
	Model Dirigible_M;
	Model Patrulla_M;
	Model Moto1_M;
	Model Moto2_M;
	Model Moto3_M;
	Model helicopter_M;
	Model helice_M;
	Model Pole_M;
	Model Banca_M;
	Model Batiseñal1_M;
	Model Batiseñal2_M;
	Model BlueBird_M;
	Model BlueBirdWings_M;
	Model Bote_M;
	Model PhoneBox_M; 
	Model Semaforo_M;
	Model Semaforo_Pole_M;
	Model Semaforo_Top_M;
	Model Semaforo_V_M;
	Model Semaforo_A_M;
	Model Semaforo_R_M;
	Model PlatTren_M;
	Model PlatTren_luz_M;
	Model SignalB1_M;
	Model SignalB1_luz_M;
	Model WE2_M;
	Model WE2_luz_M;
	Model WESignal_M;
	Model WESignal_luz_M;
	Model GotSignal_M;
	Model GotSignal_luz_M;
	Model CloudSignal_M;
	Model CloudSignal_luz_M;
	Model b4Signal_M;
	Model b4Signal_luz_M;
	Model Escenario_M;
	Model Nw_Cuerpo_M;
	Model Nw_Brazo_M;
	Model Nw_Pierna_M;
	Model Rh_Cuerpo_M;
	Model Rh_Brazo_M;
	Model Rh_Pierna_M;
	Model Helipuerto_M;
	Model pista_M;
	Model boStaff_M;
	Model boStaff2_M;
	Model gancho_M;


	//Personajes
	Nw_Cuerpo_M = Model();
	Nw_Cuerpo_M.LoadModel("Modelos_obj/Personas/Nightwing_Cuerpo.obj");
	Nw_Pierna_M = Model();
	Nw_Pierna_M.LoadModel("Modelos_obj/Personas/Nightwing_Pierna.obj");
	Nw_Brazo_M = Model();
	Nw_Brazo_M.LoadModel("Modelos_obj/Personas/Nightwing_Brazo.obj");
	Rh_Cuerpo_M = Model();
	Rh_Cuerpo_M.LoadModel("Modelos_obj/Personas/Jason_Cuerpo.obj");
	Rh_Pierna_M = Model();
	Rh_Pierna_M.LoadModel("Modelos_obj/Personas/Jason_Pierna.obj");
	Rh_Brazo_M = Model();
	Rh_Brazo_M.LoadModel("Modelos_obj/Personas/Jason_Brazo.obj");
	boStaff_M = Model();
	boStaff_M.LoadModel("Modelos_obj/Poles/boStaff.obj");
	boStaff2_M = Model();
	boStaff2_M.LoadModel("Modelos_obj/Poles/boStaff_2.obj");
	gancho_M = Model();
	gancho_M.LoadModel("Modelos_obj/Personas/Gancho.obj");


	//Señales
	SignalB1_M = Model();
	SignalB1_M.LoadModel("Modelos_obj/Edificios/RRSignal.obj");
	SignalB1_luz_M = Model();
	SignalB1_luz_M.LoadModel("Modelos_obj/Edificios/RRSignalLight.obj");
	WE2_M = Model();
	WE2_M.LoadModel("Modelos_obj/Edificios/WE2.obj");
	WE2_luz_M = Model();
	WE2_luz_M.LoadModel("Modelos_obj/Edificios/WE2_luz.obj");
	WESignal_M = Model();
	WESignal_M.LoadModel("Modelos_obj/Edificios/WESignal.obj");
	WESignal_luz_M = Model();
	WESignal_luz_M.LoadModel("Modelos_obj/Edificios/WESignal_luz.obj");
	GotSignal_M = Model();
	GotSignal_M.LoadModel("Modelos_obj/Edificios/GothamSignal.obj");
	GotSignal_luz_M = Model();
	GotSignal_luz_M.LoadModel("Modelos_obj/Edificios/GothamSignal_luz.obj");
	CloudSignal_M = Model();
	CloudSignal_M.LoadModel("Modelos_obj/Edificios/cloudSignal.obj");
	CloudSignal_luz_M = Model();
	CloudSignal_luz_M.LoadModel("Modelos_obj/Edificios/cloudSignal_luz.obj");
	b4Signal_M = Model();
	b4Signal_M.LoadModel("Modelos_obj/Edificios/b4Signal_nl.obj");
	b4Signal_luz_M = Model();
	b4Signal_luz_M.LoadModel("Modelos_obj/Edificios/b4Signal_luz.obj");
	PlatTren_M = Model();
	PlatTren_M.LoadModel("Modelos_obj/Transportes/baseTren_nl.obj");
	PlatTren_luz_M = Model();
	PlatTren_luz_M.LoadModel("Modelos_obj/Transportes/baseTren_luz.obj");

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
	Batiseñal1_M = Model();
	Batiseñal1_M.LoadModel("Modelos_obj/Others-street/batisenal.obj");
	Batiseñal2_M = Model();
	Batiseñal2_M.LoadModel("Modelos_obj/Others-street/batisenal2.obj");
	PhoneBox_M = Model();
	PhoneBox_M.LoadModel("Modelos_obj/Others-street/phonebox.obj");
	Bote_M = Model();
	Bote_M.LoadModel("Modelos_obj/Others-street/boteBasura.obj");
	Semaforo_M = Model();
	Semaforo_M.LoadModel("Modelos_obj/Others-street/semaforo.obj");
	Semaforo_Pole_M = Model();
	Semaforo_Pole_M.LoadModel("Modelos_obj/Others-street/semaforo_pole.obj");
	Semaforo_Top_M = Model();
	Semaforo_Top_M.LoadModel("Modelos_obj/Others-street/Semaforo_top_apagado.obj");
	Semaforo_V_M = Model();
	Semaforo_V_M.LoadModel("Modelos_obj/Others-street/Semaforo_top_v.obj");
	Semaforo_A_M = Model();
	Semaforo_A_M.LoadModel("Modelos_obj/Others-street/Semaforo_top_a.obj");
	Semaforo_R_M = Model();
	Semaforo_R_M.LoadModel("Modelos_obj/Others-street/Semaforo_top_r.obj");
	Escenario_M = Model();
	Escenario_M.LoadModel("Modelos_obj/Others-street/Escenario.obj");
	pista_M = Model();
	pista_M.LoadModel("Modelos_obj/Transportes/pista.obj");

	//Transportes
	Batmobile_M = Model();
	Batmobile_M.LoadModel("Modelos_obj/Transportes/Batmobile.obj");
	llanta_B_M = Model();
	llanta_B_M.LoadModel("Modelos_obj/Transportes/LlantaBatmobile.obj");
	tren_M = Model();
	tren_M.LoadModel("Modelos_obj/Transportes/tren.obj");
	BaseTren_M = Model();
	BaseTren_M.LoadModel("Modelos_obj/Transportes/baseTren.obj");
	Estacion_M = Model();
	Estacion_M.LoadModel("Modelos_obj/Transportes/Estacion.obj");
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
	Helipuerto_M = Model();
	Helipuerto_M.LoadModel("Modelos_obj/Transportes/Helipuerto.obj");

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

	//skyboxes
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

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;


	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame_Helicopter[0].movh_x = -5.0f;
	KeyFrame_Helicopter[0].movh_y = 5.0f;
	KeyFrame_Helicopter[0].giroh = 0;

	KeyFrame_Helicopter[1].movh_x = -10.0f;
	KeyFrame_Helicopter[1].movh_y = 10.0f;
	KeyFrame_Helicopter[1].giroh = 0;

	KeyFrame_Helicopter[2].movh_x = -15.0f;
	KeyFrame_Helicopter[2].movh_y = 15.0f;
	KeyFrame_Helicopter[2].giroh = 0;


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

		//para keyframes
		inputKeyframes_Helicopter(mainWindow.getsKeys());
		animate_Helicopter();
		

		/*if (mainWindow.getCamAerea()) {
			cameraAerea.keyControl(mainWindow.getsKeys(), deltaTime, mainWindow.getCamAerea());
			cameraAerea.mouseControl(mainWindow.getXChange());
		}
		else {
			cameraPiso.keyControl(mainWindow.getsKeys(), deltaTime, mainWindow.getCamAerea());
			cameraPiso.mouseControl(mainWindow.getXChange());
		}*/

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if ((count < cicloDia) && (mainWindow.getCamAerea())) {
			skybox.DrawSkybox(cameraAerea.calculateViewMatrix(), projection);
		}else if ((count < cicloDia) && (mainWindow.getCamAerea()==false)) {
			//skybox.DrawSkybox(cameraPiso.calculateViewMatrix(), projection);
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}else if ((count >= cicloDia) && (mainWindow.getCamAerea())) {
			skybox2.DrawSkybox(cameraAerea.calculateViewMatrix(), projection);
		}else {
			//skybox2.DrawSkybox(cameraPiso.calculateViewMatrix(), projection);
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		count++;
		if (count >= cicloDiaNoche) {
			count = 0;
		}

		if (count >= cicloDia) {

			//luz direccional
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f, 0.1f,
				-1.0f, 0.0f, 0.0f);

			//Luces puntuales

			//Helicoptero
			pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
				0.8f, 10.0f,
				-130.0f, 0.0f, -130.0f,
				0.3f, 0.2f, 0.1f);
			
			//Dirigible
			pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
				0.8f, 10.0f,
				70.0f, 0.0f, 60.0f,
				0.3f, 0.2f, 0.1f);

			pointLights[1].SetPos(glm::vec3(70.0f + a, 0.0f, 60.0f + b));

			//Batiseñal
			pointLights[2] = PointLight(1.0f, 1.0f, 0.0f,
				0.8f, 20.0f,
				-192.0f, 82.8f, 46.0f,
				//-170.0f, 104.0f, 66.0f,
				0.3f, 0.2f, 0.1f);

			pointLightCount = 3;
		}
		else {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f, 0.1f,
				-1.0f, 0.0f, 0.0f);

			pointLightCount = 0;
		}
		
		//Show de luces
		if (mainWindow.getSpotlights()) {
			//luz 1
			spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
				1.0f, 2.0f,
				-35.0f, 20.0f, 160.0f,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				20.0f);

			//luz 2
			spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
				1.0f, 2.0f,
				0.0f, 15.0f, 160.0f,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				20.0f);

			//luz 3
			spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
				1.0f, 2.0f,
				35.0f, 25.0f, 160.0f,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				20.0f);

			spotLightCount = 3;


			xluz2 = (3 + 2 * movLuz * toRadians) * cos(movLuz * toRadians);
			zluz2 = (3 + 2 * movLuz * toRadians) * sin(movLuz * toRadians);
			if (xluz1<=75.0f && luces==1) {
				xluz1 += 0.2f;
				zluz1 += 1.0f;
			}
			else if (xluz1 > 75.0f && luces == 1) {
				luces = 2;
				xluz1 = 0.0f;
			}
			else if (xluz3 <= 75.0f && luces == 2) {
				xluz3 += 0.2f;
				zluz3 += 1.0f;
			}
			else if (xluz3 > 75.0f && luces == 2) {
				luces = 3;
				xluz3 = 0.0f;
			}
			else if (xluz1 <= 75.0f && luces == 3) {
				xluz1 += 0.2f;
				zluz1 += 1.0f;
			}
			else if (xluz1 > 75.0f && luces == 3) {
				luces = 4;
			}
			else if (xluz3 <= 75.0f && luces == 4) {
				xluz3 += 0.2f;
				zluz3 += 1.0f;
				xluz1 -= 0.2f;
				zluz1 += 1.0f;
			}
			else if (xluz3 > 75.0f && luces == 4) {
				luces = 5;
			}
			else if (xluz3 >= 0.0f && luces == 5) {
				xluz3 -= 0.2f;
				zluz3 += 1.0f;
				xluz1 += 0.2f;
				zluz1 += 1.0f;
			}
			else if (xluz3 < 0.0f && luces == 5) {
				luces = 6;
				xluz1 = 0.0f;
				xluz3 = 0.0f;
			}
			else if (movLuz <= 360.0f && luces == 6) {
				movLuz += 1.0f;
			}
			else if (movLuz > 360.0f && luces == 6) {
				luces = 7;
			}
			else if (movLuz >= 0.0f && luces == 7) {
				movLuz -= 1.0f;
			}
			else if (movLuz < 0.0f && luces == 7) {
				luces = 1;
				xluz1 = 0.0f;
				xluz2 = 0.0f;
				xluz3 = 0.0f;
				zluz1 = 0.0f;
				zluz2= 0.0f;
				zluz3 = 0.0f;
				movLuz = 0.0f;
			}

			if (zluz1 > 360.0f) {
				zluz1 = 0.0f;
			}
			if (zluz1 < -360.0f) {
				zluz1 = 0.0f;
			}
			if (zluz3 > 360.0f) {
				zluz3 = 0.0f;
			}
			if (zluz3 < -360.0f) {
				zluz3 = 0.0f;
			}


			spotLights[0].SetPos(glm::vec3(-35.0f + xluz1, 11.0f, 160.0f + 6.0f * cos(zluz1 * toRadians)));
			spotLights[1].SetPos(glm::vec3(0.0f + xluz2, 11.0f, 160.0f + zluz2));
			spotLights[2].SetPos(glm::vec3(35.0f - xluz3, 11.0f, 160.0f + 6.0f * cos(zluz3 * toRadians)));

		}
		else {
			spotLightCount = 0;
		}

		music.play();

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

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, cameraPiso.getCameraPosition().y, cameraPiso.getCameraPosition().z);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

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

		////////////////////////////////////////////////////////////////////////////////////////////
		// Animacion avatar
		if ((mainWindow.getTim()) && (fin3 == false)) {
			//Musica
			music.pause();
			Tim.play();
			Jason.pause();
			Dick.pause();
			Disparo.pause();

			if (cambio == 1 && brazo5 >= -90.0f) {
				brazo5 -= 2.0f;
			}
			else if (cambio == 1 && brazo5 < -90.0f) {
				cambio = 2;
			}
			else if (cambio == 2 && staffScale <= 4.0f) {
				staffScale += 0.2f;
			}
			else if (cambio == 2 && staffScale > 4.0f) {
				cambio = 3;
				staffScale = 1.0f;
			}
			else if (cambio == 3 && staffRot <= 360) {
				staffRot += 2.5f;
			}
			else if (cambio == 3 && staffRot > 360) {
				cambio = 4;
				staffRot = 0.0f;
				staffScale = 4.0f;
			}
			else if (cambio == 4 && staffScale > 1.0f) {
				staffScale -= 0.2f;
			}
			else if (cambio == 4 && staffScale <= 1.0f) {
				cambio = 5;
			}
			else if (cambio == 5 && brazo5 < 0.0f) {
				brazo5 += 2.0f;
			}
			else if (cambio == 5 && brazo5 >= 0.0f) {
				cambio = 6;
			}
			else if (cambio == 6 && cuerpo3 <= 70.0f) {
				//Mov miembros
				if (brazo5 <= 30.0f && movBrazo5 == true) {
					brazo5 += 1.5f;
				}
				else if (brazo5 > 30.0f && movBrazo5 == true) {
					movBrazo5 = false;
				}
				else if (brazo5 >= -30.0f && movBrazo5 == false) {
					brazo5 -= 1.5f;
				}
				else if (brazo5 < -30.0f && movBrazo5 == false) {
					movBrazo5 = true;
				}


				if (brazo6 <= 30.0f && movBrazo6 == true) {
					brazo6 += 1.5f;
				}
				else if (brazo6 > 30.0f && movBrazo6 == true) {
					movBrazo6 = false;
				}
				else if (brazo6 >= -30.0f && movBrazo6 == false) {
					brazo6 -= 1.5f;
				}
				else if (brazo6 < -30.0f && movBrazo6 == false) {
					movBrazo6 = true;
				}


				if (pierna5 <= 30.0f && movPierna5 == true) {
					pierna5 += 1.5f;
				}
				else if (pierna5 > 30.0f && movPierna5 == true) {
					movPierna5 = false;
				}
				else if (pierna5 >= -30.0f && movPierna5 == false) {
					pierna5 -= 1.5f;
				}
				else if (pierna5 < -30.0f && movPierna5 == false) {
					movPierna5 = true;
				}


				if (pierna6 <= 30.0f && movPierna6 == true) {
					pierna6 += 1.5f;
				}
				else if (pierna6 > 30.0f && movPierna6 == true) {
					movPierna6 = false;
				}
				else if (pierna6 >= -30.0f && movPierna6 == false) {
					pierna6 -= 1.5f;
				}
				else if (pierna6 < -30.0f && movPierna6 == false) {
					movPierna6 = true;
				}

				//Traslacion
				cuerpo3 += 0.25f;
				movimientoY += 0.005f;
			}
			else if (cambio == 6 && cuerpo3 > 70.0f) {
				pierna5 = 0.0f;
				pierna6 = 0.0f;
				brazo5 = 0.0f;
				brazo6 = 0.0f;
				cambio = 7;
			}
			else if (cambio == 7 && brazo6 >= -210.0f) {
				brazo6 -= 2.0f;
			}
			else if (cambio == 7 && brazo6 < -210.0f) {
				cambio = 8;
			}
			else if (cambio == 8 && brazo6 <= -180.0f) {
				brazo6 += 5.0f;
			}
			else if (cambio == 8 && brazo6 > -180.0f) {
				cambio = 9;
			}
			else if (cambio == 9 && ganchoTras <= 42.0f) {
				ganchoTras += 0.5f;
				if (ganchoRot <= 110.0f) {
					ganchoRot += 5.0f;
				}
			}
			else if (cambio == 9 && ganchoTras > 42.0f) {
				cambio=10;
			}
			else if (cambio == 10 && movimientoY <= 43.8f) {
				movimientoY+=0.2f;
				ganchoTras -= 0.2f;
				if (cuerpo3 <= 72.0f) {
					cuerpo3 += 0.1f;
				}
			}
			else if (cambio == 10 && movimientoY > 43.8f) {
				cambio=11;
			}
			else if (cambio == 11 && movimientoY <= 48.0f) {
				movimientoY += 0.5f;
				if (cuerpo3 <= 76.0f) {
					cuerpo3 += 0.2f;
				}
			}
			else if (cambio == 11 && movimientoY > 48.0f) {
				brazo6 = 0.0f;
				ganchoRot = 0.0f;
				fin3 = true;
			}
		}
		else{
			//Musica
			music.play();
			Tim.pause();
			Jason.pause();
			Dick.pause();
			Disparo.pause();
		}


		//Personaje

		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, movimientoY, -17.0f+ cuerpo3));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.2f, 1.6f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[11]->RenderMesh();

		//cara
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Hombro izquierdo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.775f, 0.5f, 0.0f));
		model = glm::rotate(model, brazo5* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));


		//brazo izq
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.35f, 1.2f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//bostaff
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.67f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, staffScale));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, staffRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (cambio == 3) {
			boStaff_M.RenderModel();
		}
		else {
			boStaff2_M.RenderModel();
		}

		//hombro derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.775f, 0.5f, 0.0f));
		model = glm::rotate(model, brazo6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//brazo der
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.35f, 1.2f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//Gancho
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.67f - ganchoTras, 0.0f + ganchoMovZ));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, ganchoRot * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gancho_M.RenderModel();

		//union pierna izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.375f, -0.625f, 0.0f));
		model = glm::rotate(model, pierna6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//pierna izq
		model = glm::translate(model, glm::vec3(0.0f, -0.625f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.45f, 0.95f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//pie izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.125f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//union pierna der
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.375f, -0.625f, 0.0f));
		model = glm::rotate(model, pierna5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//pierna der
		model = glm::translate(model, glm::vec3(0.0f, -0.625f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.45f, 0.95f, 0.4f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();

		//pie der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.125f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		
		//cuerpo edif
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 69.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f, 3.52f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			WE2_M.RenderModel();
		}
		else {
			WE2_luz_M.RenderModel();
		}
		
		//Señal1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 141.5f, 9.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			WESignal_M.RenderModel();
		}
		else {
			WESignal_luz_M.RenderModel();
		}

		//Señal2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 141.5f, -9.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			WESignal_M.RenderModel();
		}
		else {
			WESignal_luz_M.RenderModel();
		}

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
		model = glm::translate(model, glm::vec3(-72.0f, 2.0f, 45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem<201) {
			Semaforo_V_M.RenderModel();
		}else if ((countSem < 301) && (countSem > 200)) {
			Semaforo_A_M.RenderModel();
		}
		else {
			Semaforo_R_M.RenderModel();
		}
		countSem++;
		if (countSem >= 600) {
			countSem = 0;
		}
		

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

		//Señal
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 60.0f, 32.0f));
		model = glm::scale(model, glm::vec3(3.4f, 4.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			SignalB1_M.RenderModel();
		}
		else {
			SignalB1_luz_M.RenderModel();
		}
		


		if ( ( (cameraPiso.getCameraPosition().x- xRh <= 15.0f) || (cameraPiso.getCameraPosition().z - zRh <= 15.0f)) && (fin1==false)) {
			//Musica
			music.pause();
			Tim.pause();
			Jason.play();
			Dick.pause();
			Disparo.pause();

			if (cuerpo1 <= 10.0f) {
				//Mov miembros
				if (brazo1 <= 30.0f && movBrazo == true) {
					brazo1 += 1.5f;
				}
				else if (brazo1 > 30.0f && movBrazo == true) {
					movBrazo = false;
				}
				else if (brazo1 >= -30.0f && movBrazo == false) {
					brazo1 -= 1.5f;
				}
				else if (brazo1 < -30.0f && movBrazo == false) {
					movBrazo = true;
				}


				if (brazo2 <= 30.0f && movBrazo2 == true) {
					brazo2 += 1.5f;
				}
				else if (brazo2 > 30.0f && movBrazo2 == true) {
					movBrazo2 = false;
				}
				else if (brazo2 >= -30.0f && movBrazo2 == false) {
					brazo2 -= 1.5f;
				}
				else if (brazo2 < -30.0f && movBrazo2 == false) {
					movBrazo2 = true;
				}


				if (pierna1 <= 30.0f && movPierna1 == true) {
					pierna1 += 1.5f;
				}
				else if (pierna1 > 30.0f && movPierna1 == true) {
					movPierna1 = false;
				}
				else if (pierna1 >= -30.0f && movPierna1 == false) {
					pierna1 -= 1.5f;
				}
				else if (pierna1 < -30.0f && movPierna1 == false) {
					movPierna1 = true;
				}


				if (pierna2 <= 30.0f && movPierna2 == true) {
					pierna2 += 1.5f;
				}
				else if (pierna2 > 30.0f && movPierna2 == true) {
					movPierna2 = false;
				}
				else if (pierna2 >= -30.0f && movPierna2 == false) {
					pierna2 -= 1.5f;
				}
				else if (pierna2 < -30.0f && movPierna2 == false) {
					movPierna2 = true;
				}

				//Traslacion
				cuerpo1 += 0.01f;
			}
			else if (cuerpo1 > 10.0f && brazo1 >= -90.0f) {
				brazo1 -= 1.5f;
			}
			else if (cuerpo1 > 10.0f && brazo1 < -90.0f && countDisp<150) {
				pierna1 = 0.0f;
				pierna2 = 0.0f;
				brazo2 = 0.0f;
				Disparo.play();
				countDisp += 1;
			}
			else if(cuerpo1 > 10.0f && countDisp >= 150) {
				fin1 = true;
				//Musica
				music.play();
				Tim.pause();
				Jason.pause();
				Dick.pause();
				Disparo.pause();
			}
		}
		

		//RedHood
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xRh, yRh, zRh));
		model = glm::translate(model, glm::vec3(cuerpo1, 0.0f, cuerpo1 * cuerpo1));
		model = glm::scale(model, glm::vec3(0.48f, 0.5f, 0.48f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Cuerpo_M.RenderModel();

		//Brazo izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.55f, 1.2f, 0.0f));
		model = glm::rotate(model, brazo1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Brazo_M.RenderModel();

		//Brazo der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.55f, 1.2f, 0.0f));
		model = glm::rotate(model, brazo2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Brazo_M.RenderModel();

		//Pierna izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.75f, -1.63f, 0.0f));
		model = glm::rotate(model, pierna2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Pierna_M.RenderModel();

		//Pierna der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.75f, -1.63f, 0.0f));
		model = glm::rotate(model, pierna1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Pierna_M.RenderModel();


		//Moto RedHood
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.9f, -135.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		model = glm::translate(model, glm::vec3(-72.0f, 0.6f, 45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 201) {
			Semaforo_V_M.RenderModel();
		}
		else if ((countSem < 301) && (countSem > 200)) {
			Semaforo_A_M.RenderModel();
		}
		else {
			Semaforo_R_M.RenderModel();
		}


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

		//Señal
		model = modelaux;
		model = glm::translate(model, glm::vec3(10.5f, 58.8f, 21.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			CloudSignal_M.RenderModel();
		}
		else {
			CloudSignal_luz_M.RenderModel();
		}
		

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
		model = glm::translate(model, glm::vec3(72.0f, 2.6f, 45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 301) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < 501) && (countSem > 300)) {
			Semaforo_V_M.RenderModel();
		}
		else {
			Semaforo_A_M.RenderModel();
		}

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
		model = glm::translate(model, glm::vec3(72.0f, 2.6f, 45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 301) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < 501) && (countSem > 300)) {
			Semaforo_V_M.RenderModel();
		}
		else {
			Semaforo_A_M.RenderModel();
		}

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

		//Señal
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 81.0f, -10.0f ));
		model = glm::scale(model, glm::vec3(1.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			GotSignal_M.RenderModel();
		}
		else {
			GotSignal_luz_M.RenderModel();
		}

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
		model = glm::translate(model, glm::vec3(72.0f, 0.6f, -45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 201) {
			Semaforo_V_M.RenderModel();
		}
		else if ((countSem < 301) && (countSem > 200)) {
			Semaforo_A_M.RenderModel();
		}
		else {
			Semaforo_R_M.RenderModel();
		}


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

		if (((cameraPiso.getCameraPosition().x - xNw <= 15.0f) || (cameraPiso.getCameraPosition().z - zNw <= 15.0f)) && fin2==false) {
			//Musica
			music.pause();
			Tim.pause();
			Jason.pause();
			Dick.play();
			Disparo.pause();

			if (cuerpo2 <= 10.0f && movCuerpo2) {
				//Mov miembros
				if (brazo3 <= 30.0f && movBrazo3 == true) {
					brazo3 += 1.5f;
				}
				else if (brazo3 > 30.0f && movBrazo3 == true) {
					movBrazo3 = false;
				}
				else if (brazo3 >= -30.0f && movBrazo3 == false) {
					brazo3 -= 1.5f;
				}
				else if (brazo3 < -30.0f && movBrazo3 == false) {
					movBrazo3 = true;
				}


				if (brazo4 <= 30.0f && movBrazo4 == true) {
					brazo4 += 1.5f;
				}
				else if (brazo4 > 30.0f && movBrazo4 == true) {
					movBrazo4 = false;
				}
				else if (brazo4 >= -30.0f && movBrazo4 == false) {
					brazo4 -= 1.5f;
				}
				else if (brazo4 < -30.0f && movBrazo4 == false) {
					movBrazo4 = true;
				}


				if (pierna3 <= 30.0f && movPierna3 == true) {
					pierna3 += 1.5f;
				}
				else if (pierna3 > 30.0f && movPierna3 == true) {
					movPierna3 = false;
				}
				else if (pierna3 >= -30.0f && movPierna3 == false) {
					pierna3 -= 1.5f;
				}
				else if (pierna3 < -30.0f && movPierna3 == false) {
					movPierna3 = true;
				}


				if (pierna4 <= 30.0f && movPierna4 == true) {
					pierna4 += 1.5f;
				}
				else if (pierna4 > 30.0f && movPierna4 == true) {
					movPierna4 = false;
				}
				else if (pierna4 >= -30.0f && movPierna4 == false) {
					pierna4 -= 1.5f;
				}
				else if (pierna4 < -30.0f && movPierna4 == false) {
					movPierna4 = true;
				}

				//Traslacion
				cuerpo2 += 0.01f;
			}
			else if (cuerpo2 > 10.0f && movCuerpo2) {
				pierna3 = 0.0f;
				pierna4 = 0.0f;
				movCuerpo2 = false;
			}
			else if (cuerpo2 > 10.0f && pierna3 >= -90.0f && pierna4 <= 90.0f && movCuerpo2 == false && salto == false) {
				pierna3 -= 1.0f;
				pierna4 += 1.0f;
				movY += 0.05f;
			}
			else if (cuerpo2 > 10.0f && pierna3 < -90.0f && pierna4 > 90.0f && movCuerpo2 == false && salto == false) {
				salto = true;
			}
			else if (cuerpo2 > 10.0f && movCuerpo2 == false && movY > 0.0f  && salto == true) {
				pierna3 += 1.0f;
				pierna4 -= 1.0f;
				movY -= 0.05f;
			}
			else if (cuerpo2 > 10.0f && movCuerpo2 == false && movY <= 0.0f && salto == true) {
				fin2 = true;
				pierna3 = 0.0f;
				pierna4 = 0.0f;
				brazo3 = 0.0f;
				brazo4 = 0.0f;
				movY = 0.0f;
				//Musica
				music.play();
				Tim.pause();
				Jason.pause();
				Dick.pause();
				Disparo.pause();
			}
		}

		//Nigthwing
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xNw,yNw,zNw));
		model = glm::translate(model, glm::vec3(cuerpo2*cuerpo2, 0.3f+ movY, cuerpo2));
		model = glm::scale(model, glm::vec3(0.48f, 0.49f, 0.48f));
		modelaux2 = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Cuerpo_M.RenderModel();

		//Brazo izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -1.55f));
		model = glm::rotate(model, brazo3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Brazo_M.RenderModel();

		//Brazo der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 1.55f));
		model = glm::rotate(model, brazo4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Brazo_M.RenderModel();

		//Pierna izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, -0.75f));
		model = glm::rotate(model, pierna4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Pierna_M.RenderModel();

		//Pierna der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.75f));
		model = glm::rotate(model, pierna3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Pierna_M.RenderModel();

		//Batiseñal
		model = modelaux;
		model = glm::translate(model, glm::vec3(-12.0f, 85.5f, -24.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotBatsignal * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBatsignal, escalaBatsignal, escalaBatsignal));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (count < cicloDia) {
			Batiseñal1_M.RenderModel();
			if (escalaBatsignal > 1.0f) {
				escalaBatsignal -= 0.001f;
			}
		}
		else {
			Batiseñal2_M.RenderModel();
			rotBatsignal += 1.0f;
			if (escalaBatsignal < 2.0f) {
				escalaBatsignal += 0.001f;
			}
		}

		if (rotBatsignal > 359.0f) {
			rotBatsignal = 0.0f;
		}
		if (rotBatsignal < -359.0f) {
			rotBatsignal = 0.0f;
		}
		


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
		model = glm::translate(model, glm::vec3(72.0f, 3.3f, -45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 201) {
			Semaforo_V_M.RenderModel();
		}
		else if ((countSem < 301) && (countSem > 200)) {
			Semaforo_A_M.RenderModel();
		}
		else {
			Semaforo_R_M.RenderModel();
		}


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
		model = glm::scale(model, glm::vec3(8.5f, 10.0f, 8.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Building4_M.RenderModel();

		//Señal
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 42.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (count < cicloDia) {
			b4Signal_M.RenderModel();
		}
		else {
			b4Signal_luz_M.RenderModel();
		}

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
		model = glm::translate(model, glm::vec3(-72.0f, 1.2f, -45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 301) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < 501) && (countSem > 300)) {
			Semaforo_V_M.RenderModel();
		}
		else {
			Semaforo_A_M.RenderModel();
		}


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
		model = glm::translate(model, glm::vec3(-72.0f, 0.6f, -45.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Semaforo_Pole_M.RenderModel();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.3f, 3.3f, -1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (countSem < 301) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < 501) && (countSem > 300)) {
			Semaforo_V_M.RenderModel();
		}
		else {
			Semaforo_A_M.RenderModel();
		}

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

		//Base plataforma tren
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 230.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(24.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BaseTren_M.RenderModel();

		//Plataforma tren
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 26.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (count < cicloDia) {
			PlatTren_M.RenderModel();
		}
		else {
			PlatTren_luz_M.RenderModel();
		}

		//Tren
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 12.8f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 18.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tren_M.RenderModel();

		//Estacion
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 262.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.6f, 5.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Estacion_M.RenderModel();

		//Elipse
		a = 100 * cos(movDirigible * toRadians);
		b = 150 * sin(movDirigible * toRadians);
		if (countDirigible > 850.0f) {
			movDirigible += 0.5f;
		}
		else {
			countDirigible += 1.0f;
		}

		//Dirigible
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f+a, 175.0f, 60.0f+b));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, -a * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dirigible_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////
		//Escenario spotlight
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 160.0f));
		model = glm::scale(model, glm::vec3(17.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escenario_M.RenderModel();
		//////////////////////////////////////////////////////////////////////////////////////////
		
		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-190.0f, -2.0f, 160.0f));
		model = glm::scale(model, glm::vec3(9.0f, 2.0f, 9.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pista_M.RenderModel();

		//Helipuerto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -2.0f, 160.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(8.0f, 5.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helipuerto_M.RenderModel();


		//punto de giro
		model = glm::mat4(1.0);
		posh = glm::vec3(posXh + movh_x-10.0f, posYh + movh_y, posZh);
		model = glm::translate(model, posh);
		model = glm::rotate(model, giroh * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));


		//Helicopter
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		helicopter_M.RenderModel();

		//helice
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 3.8f, 0.0f));
		model = glm::rotate(model, rot_helice * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helice_M.RenderModel();
		
		if(mainWindow.getAnimacionBatmobile()){
			if (movBatmobile <= 160.0f && batmobile == 1) {
				movBatmobile += 0.25f;
				rotllanta += 0.1f;
				rotBatmobile = 180.0f;
			}
			else if (movBatmobile > 160.0f && batmobile == 1) {
				batmobile = 2;
				rotBatmobile = 180.0f;
			}
			else if (movBatmobile2 <= 108.0f && batmobile == 2) {
				movBatmobile2 += 0.25f;
				rotllanta += 0.1f;
				rotBatmobile = 90.0f;
			}
			else if (movBatmobile2 > 108.0f && batmobile == 2) {
				batmobile = 3;
				rotBatmobile = 90.0f;
			}
			else if (movBatmobile >= 0.0f && batmobile == 3) {
				movBatmobile -= 0.25f;
				rotllanta += 0.1f;
				rotBatmobile = 0.0f;
			}
			else if (movBatmobile < 0.0f && batmobile == 3) {
				batmobile = 4;
				rotBatmobile = 0.0f;
			}
			else if (movBatmobile2 >=0.0f && batmobile == 4) {
				movBatmobile2 -= 0.25f;
				rotllanta += 0.1f;
				rotBatmobile = -90.0f;
			}
			else if (movBatmobile2 < 0.0f && batmobile == 4) {
				batmobile = 1;
				rotBatmobile = -90.0f;
			}
		}

		if (rotllanta > 359.0f) {
			rotllanta = 0.0f;
		}
		if (rotllanta < -359.0f) {
			rotllanta = 0.0f;
		}

		//batmobile sin movimiento
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(78.0f - movBatmobile, 0.8f, -5.5f - movBatmobile2));
		model = glm::rotate(model, rotBatmobile * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Batmobile_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.44f, -1.4f, -1.6f));
		model = glm::rotate(model, -rotllanta, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();


		//Llanta posterior derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.1f, -1.4f, -1.6f));
		model = glm::rotate(model, -rotllanta, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();

		//Llanta delantera izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.44f, -1.4f, 1.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();

		//Llanta posterior izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.1f, -1.4f, 1.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta_B_M.RenderModel();



		xbird = (10 + 15 * movbird * toRadians) * cos(movbird * toRadians);
		zbird = (10 + 15 * movbird * toRadians) * sin(movbird * toRadians);
		if (countBird >= 500.0f) {
			if (sube == true && movbird <= 359.0f) {
				movbird += 0.2f;
				rotBird += 0.3f;
				ybird += 0.02f;
				if (rotWings <= 45.0f && wings == true) {
					rotWings += 0.75f;
					//printf("\nRotWings: %f\n", rotWings);
				}
				else if (rotWings > 45.0f && wings == true) {
					wings = false;
					//printf("\nCambio a False\n");
				}
				else if (rotWings >= -45.0f && wings == false) {
					rotWings -= 0.75f;
					//printf("\nRotWings: %f\n", rotWings);
				}
				else if (rotWings < -45.0f && wings == false) {
					wings = true;
					//printf("\nCambio a True\n");
				}
			}
			else if (sube == true && movbird > 359.0f) {
				sube = false;
			}
			else if (sube == false && movbird >= 0.0f) {
				movbird -= 0.2f;
				rotBird += 0.3f;
				ybird -= 0.02f;
				if (rotWings <= 45.0f && wings == true) {
					rotWings += 0.75f;
					//printf("\nRotWings: %f\n", rotWings);
				}
				else if (rotWings > 45.0f && wings == true) {
					wings = false;
					//printf("\nCambio a False\n");
				}
				else if (rotWings >= -45.0f && wings == false) {
					rotWings -= 0.75f;
					//printf("\nRotWings: %f\n", rotWings);
				}
				else if (rotWings < -45.0f && wings == false) {
					wings = true;
					//printf("\nCambio a True\n");
				}
			}
			else if (sube == false && movbird < 0.0f) {
				sube = true;
			}
			
		}
		else {
			countBird += 1.0f;
		}

		if (movbird > 359.0f) {
			movbird = 0.0f;
		}
		if (movbird < -359.0f) {
			movbird = 0.0f;
		}
		if (rotBird > 359.0f) {
			rotBird = 0.0f;
		}
		if (rotBird < -359.0f) {
			rotBird = 0.0f;
		}

		//Bird
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f - xbird, 56.0f + ybird, 70.0f - zbird));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotBird * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlueBird_M.RenderModel();

		//alas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -0.2f));
		model = glm::rotate(model, rotWings * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlueBirdWings_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes_Helicopter(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion_Helicopter < 1)
		{
			if (play_Helicopter == false && (FrameIndex_Helicopter > 1))
			{
				resetElements_Helicopter();
				//First Interpolation				
				interpolation_Helicopter();
				play_Helicopter = true;
				playIndex_Helicopter = 0;
				i_curr_steps_Helicopter = 0;
				reproduciranimacion_Helicopter++;
				printf("\nPresiona Enter para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion_Helicopter = 0;

			}
			else
			{
				play_Helicopter = false;
			}
		}
	}
	if (keys[GLFW_KEY_ENTER])
	{
		if (habilitaranimacion_Helicopter < 1)
		{
			reproduciranimacion_Helicopter = 0;
		}
	}

	if (keys[GLFW_KEY_9])
	{
		if (guardoFrame_Helicopter < 1)
		{
			saveFrame_Helicopter();
			printf("\nPresiona 0 para habilitar guardar otro frame\n");
			guardoFrame_Helicopter++;
			reinicioFrame_Helicopter = 0;
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (reinicioFrame_Helicopter < 1)
		{
			guardoFrame_Helicopter = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_x -= 5.0f;
			printf("movh_x es: %f\n", movh_x);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con P\n");
		}
	}
	
	if (keys[GLFW_KEY_J])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_x += 5.0f;
			printf("movh_x es: %f\n", movh_x);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con P\n");
		}
	}

	if (keys[GLFW_KEY_I])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_y += 5.0f;
			printf("movh_y es: %f\n", movh_x);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con P\n");
		}
	}
	if (keys[GLFW_KEY_K])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_y -= 5.0f;
			printf("movh_y es: %f\n", movh_x);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con P\n");
		}
	}

	if (keys[GLFW_KEY_O])
	{
		if (ciclo_Helicopter < 1)
		{
			giroh += 90.0f;
			printf("giroh es: %f\n", giroh);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con P\n");
		}
	}
	

	if (keys[GLFW_KEY_U])
	{
		if (ciclo_Helicopter < 1)
		{
			rot_helice += 90.0f;
			printf("rot_helice es: %f\n", rot_helice);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con P\n");
		}
	}

	if (keys[GLFW_KEY_P])
	{
		if (ciclo2_Helicopter < 1)
		{
			ciclo_Helicopter = 0;
		}
	}


	if (keys[GLFW_KEY_M])
	{
		if (leoFrame_Helicopter < 1)
		{
			printf("\nLeyendo Frames\n");
			readFrame_Helicopter();
			printf("\nPresiona N para habilitar leer nuevamente el archivo\n");
			leoFrame_Helicopter++;
			reinicioLeoFrame = 0;
		}
	}
	if (keys[GLFW_KEY_N])
	{
		if (reinicioLeoFrame < 1)
		{
			leoFrame_Helicopter = 0;
		}
	}

}
