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
#include "Toroide.h"
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
float brazo1 = 0.0f, brazo2 = 0.0f, pierna1 = 0.0f, pierna2 = 0.0f, cuerpo1 = 0.0f, rotCuerpo = 0.0f;
bool movBrazo = true, movBrazo2 = false, movPierna1=false, movPierna2=true;
int  countDisp = 0;
int cambioJ = 1;

//Movimiento Nightwing (compleja) automática
float xNw = -230.0f, yNw = 0.3f, zNw = 22.0f;
float brazo3 = 0.0f, brazo4 = 0.0f, pierna3 = 0.0f, pierna4 = 0.0f, cuerpo2 = 0.0f, movY=0.0f, rotCuerpo2 = 0.0f, movZ=0.0f;
bool movBrazo3 = true, movBrazo4 = false, movPierna3 = false, movPierna4 = true, movCuerpo2 = true, salto = false;
int  countSalto = 0;
int cambioN = 1;

//Movimiento Tim (simple) por teclado
float brazo5 = 0.0f, brazo6 = 0.0f, pierna5 = 0.0f, pierna6 = 0.0f, cuerpo3 = 0.0f, rotCuerpo3=0.0f;
float staffScale = 1.0f, staffRot = 0.0f, movimientoY = 0.0f, ganchoTras = 0.0f, ganchoRot = 0.0f, ganchoMovZ=0.0f;
bool movBrazo5 = true, movBrazo6 = false, movPierna5 = false, movPierna6 = true, movCuerpo3 = true;
int cambio = 1;

//Variables semáforo
int verde1 = 200, amarillo1 = 300, rojo1 = 600;
int verde2 = 300, amarillo2 = 500;
int cambioAuto = 1;

//Animacion de los autos y motos de la calle.
float movAuto1 = 0.0f, rotLlanta1 = 0.0f;
float movAuto2 = 0.0f, rotLlanta2 = 0.0f;
float movAuto3 = 0.0f, rotLlanta3 = 0.0f;
float movAuto4 = 0.0f, rotLlanta4 = 0.0f;
float movAuto5 = 0.0f, rotLlanta5 = 0.0f;
float movAuto6 = 0.0f, rotLlanta6 = 0.0f;
float movAuto7 = 0.0f, rotLlanta7 = 0.0f;
float movAuto8 = 0.0f, rotLlanta8 = 0.0f;
float movAuto9 = 0.0f, rotLlanta9 = 0.0f;
float movAuto10 = 0.0f, rotLlanta10 = 0.0f;
float movAuto11 = 0.0f, rotLlanta11 = 0.0f;
float movAuto12 = 0.0f, rotLlanta12 = 0.0f;
float movAuto13 = 0.0f, rotLlanta13 = 0.0f;
float movAuto14 = 0.0f, rotLlanta14 = 0.0f;
float movAuto15 = 0.0f, rotLlanta15 = 0.0f;
float movAuto16 = 0.0f, rotLlanta16 = 0.0f;
float movAuto17 = 0.0f, rotLlanta17 = 0.0f;
float movAuto18 = 0.0f, rotLlanta18 = 0.0f;
float moto1 = 0.0f, rotLlantaMoto1 = 0.0f;
float moto2 = 0.0f, rotLlantaMoto2 = 0.0f;
float moto3 = 0.0f, rotLlantaMoto3 = 0.0f;
float moto4 = 0.0f, rotLlantaMoto4 = 0.0f;
float moto5 = 0.0f, rotLlantaMoto5 = 0.0f;
float moto6 = 0.0f, rotLlantaMoto6 = 0.0f;
float moto7 = 0.0f, rotLlantaMoto7 = 0.0f;
float moto8 = 0.0f, rotLlantaMoto8 = 0.0f;

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

Sound music = Sound("Music/Nycteris.mp3");
Sound Tim = Sound("Music/twenty one pilots Stressed Out.mp3");
Sound Jason = Sound("Music/Bohnes - Middle Finger.mp3");
Sound Dick = Sound("Music/Fall Out Boy - Where Did The Party Go.mp3");
Sound Disparo = Sound("Music/Disparo.mp3");

Material MaterialParaLuces;
Material MaterialNormal;

Toroide Tor = Toroide(1.0, 1.5, 10, 10);

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
void inputKeyframes(bool* keys);
bool animacion_Helicopter = false;
float reproduciranimacion_Helicopter, habilitaranimacion_Helicopter, guardoFrame_Helicopter, reinicioFrame_Helicopter, ciclo_Helicopter, ciclo2_Helicopter;
float leoFrame_Helicopter=0.0f, reinicioLeoFrame = 0.0f, contador_Helicopter = 0;
bool xokay=false, yokay = false, zokay = false, gokay=false, heliceokay=false;

glm::vec3 posh = glm::vec3(0.0f, 0.0f, 0.0f);
float posXh = 190.0f, posYh = 18.0f, posZh = 160.0f;


//NEW// Keyframes
float movh_x = 0.0f, movh_y = 0.0f, movh_z = 0.0f;
float giroh = 0, rot_helice=0.0f;

#define MAX_FRAMES_Helicopter 30 //Cuantos cuadros se guardan
int i_max_steps_Helicopter = 100; //Valores intermedios, entre mayor, mas suave se ve, pero ocupa mas recursos
int i_curr_steps_Helicopter = 2; //Numero de frames declarados
typedef struct _frame_Helicopter
{
	//Variables para GUARDAR Key Frames
	float movh_x;
	float movh_y;
	float movh_xInc;
	float movh_yInc;
	float movh_z;
	float movh_zInc;
	float giroh;
	float girohInc;
	float rot_helice;
	float rot_heliceInc;
	//Por cada variable, se necesita otra incremental
}FRAME;

FRAME KeyFrame_Helicopter[MAX_FRAMES_Helicopter];
int FrameIndex_Helicopter = 2;			//introducir datos
bool play_Helicopter = false;
int playIndex_Helicopter = 0;

void saveFrame_Helicopter(void)
{
	using namespace std;
	printf("Se guardo el frame (%d)\n", FrameIndex_Helicopter);

	KeyFrame_Helicopter[FrameIndex_Helicopter].movh_x = movh_x;
	KeyFrame_Helicopter[FrameIndex_Helicopter].movh_y = movh_y;
	KeyFrame_Helicopter[FrameIndex_Helicopter].movh_z = movh_z;
	KeyFrame_Helicopter[FrameIndex_Helicopter].giroh = giroh;
	KeyFrame_Helicopter[FrameIndex_Helicopter].rot_helice = rot_helice;

	ofstream archivo("Frames_Helicoptero.txt", ios::app);
	if (archivo.is_open()) {
		archivo << "****************************** " << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].movh_x=" + std::to_string(movh_x) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].movh_y=" + std::to_string(movh_y) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Helicopter) + "].movh_z=" + std::to_string(movh_z) << endl;
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
	char linea[40];
	fichero.open("Frames_Helicoptero.txt", ios::in);
	if (fichero.fail()) {
		cerr << "Error al abrir el archivo Frames_Helicoptero.txt" << endl;
	}
	else {
		fichero >> linea;           // Primera linea
		while (!fichero.eof())      
		{
			if (FrameIndex_Helicopter < 10) {
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
					else if (linea[17] == 'z') {
						aux = aux + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
						movh_z = stof(aux);
						aux = "";
						zokay = true;
						cout << "movh_z =" + to_string(movh_z) << endl;
					}
					else if (linea[12] == 'g') {
						aux = aux + linea[18] + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24];
						giroh = stof(aux);
						aux = "";
						gokay = true;
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
			}
			else {
				//cout << texto << endl;    // Muestrar el contenido en terminal 
				if (linea[0] == 'K') {
					if (linea[18] == 'x') {
						aux = aux + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25] + linea[26] ;
						movh_x = stof(aux);
						aux = "";
						xokay = true;
						cout << "movh_x =" + to_string(movh_x) << endl;
					}
					else if (linea[18] == 'y') {
						aux = aux + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25] + linea[26];
						movh_y = stof(aux);
						aux = "";
						yokay = true;
						cout << "movh_y =" + to_string(movh_y) << endl;
					}
					else if (linea[18] == 'z') {
						aux = aux + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25] + linea[26];
						movh_z = stof(aux);
						aux = "";
						zokay = true;
						cout << "movh_z =" + to_string(movh_z) << endl;
					}
					else if (linea[13] == 'g') {
						aux = aux + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
						giroh = stof(aux);
						aux = "";
						gokay = true;
						cout << "giroh =" + to_string(giroh) << endl;
					}
					else if (linea[13] == 'r') {
						aux = aux + linea[24] + linea[25] + linea[26] + linea[27] + linea[28] + linea[29];
						rot_helice = stof(aux);
						aux = "";
						heliceokay = true;
						cout << "rot_helice =" + to_string(rot_helice) << endl;
					}
				}
			}

			if (xokay && yokay && zokay && gokay && heliceokay) {
				xokay = false;
				yokay = false;
				zokay = false;
				gokay = false;
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
	movh_z = KeyFrame_Helicopter[0].movh_z;
	giroh = KeyFrame_Helicopter[0].giroh;
	rot_helice = KeyFrame_Helicopter[0].rot_helice;
}

void interpolation_Helicopter(void)
{
	KeyFrame_Helicopter[playIndex_Helicopter].movh_xInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].movh_x - KeyFrame_Helicopter[playIndex_Helicopter].movh_x) / i_max_steps_Helicopter;
	KeyFrame_Helicopter[playIndex_Helicopter].movh_yInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].movh_y - KeyFrame_Helicopter[playIndex_Helicopter].movh_y) / i_max_steps_Helicopter;
	KeyFrame_Helicopter[playIndex_Helicopter].movh_zInc = (KeyFrame_Helicopter[playIndex_Helicopter + 1].movh_z - KeyFrame_Helicopter[playIndex_Helicopter].movh_z) / i_max_steps_Helicopter;
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
			movh_z += KeyFrame_Helicopter[playIndex_Helicopter].movh_zInc;
			giroh += KeyFrame_Helicopter[playIndex_Helicopter].girohInc;
			rot_helice += KeyFrame_Helicopter[playIndex_Helicopter].rot_heliceInc;
			i_curr_steps_Helicopter++;
		}
	}
}



//Animacion Dirigible
bool animacion_Dirigible = false;
float reproduciranimacion_Dirigible, habilitaranimacion_Dirigible, guardoFrame_Dirigible, reinicioFrame_Dirigible, ciclo_Dirigible, ciclo2_Dirigible;
float leoFrame_Dirigible = 0.0f, reinicioLeoFrame_Dirigible = 0.0f, contador_Dirigible = 0;
bool xdokay = false, ydokay = false, zdokay = false;

glm::vec3 posd = glm::vec3(0.0f, 0.0f, 0.0f);
float posXd = 70.0f, posYd = 110.0f, posZd = 60.0f;


//NEW// Keyframes
float movd_x = 0.0f, movd_y = 0.0f;
float girod = 0;

#define MAX_FRAMES_Dirigible 30 //Cuantos cuadros se guardan
int i_max_steps_Dirigible = 100; //Valores intermedios, entre mayor, mas suave se ve, pero ocupa mas recursos
int i_curr_steps_Dirigible = 2; //Numero de frames declarados
typedef struct _frame_Dirigible
{
	//Variables para GUARDAR Key Frames
	float movd_x;
	float movd_y;
	float movd_xInc;
	float movd_yInc;
	float girod;
	float girodInc;
	//Por cada variable, se necesita otra incremental
}FRAMED;

FRAMED KeyFrame_Dirigible[MAX_FRAMES_Dirigible];
int FrameIndex_Dirigible = 2;			//introducir datos
bool play_Dirigible = false;
int playIndex_Dirigible = 0;

void saveFrame_Dirigible(void)
{
	using namespace std;
	printf("Se guardo el frame (%d)\n", FrameIndex_Dirigible);

	KeyFrame_Dirigible[FrameIndex_Dirigible].movd_x = movd_x;
	KeyFrame_Dirigible[FrameIndex_Dirigible].movd_y = movd_y;
	KeyFrame_Dirigible[FrameIndex_Dirigible].girod = girod;

	ofstream archivo("Frames_Dirigible.txt", ios::app);
	if (archivo.is_open()) {
		archivo << "****************************** " << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Dirigible) + "].movd_x=" + std::to_string(movd_x) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Dirigible) + "].movd_y=" + std::to_string(movd_y) << endl;
		archivo << "KeyFrame[" + std::to_string(FrameIndex_Dirigible) + "].girod=" + std::to_string(girod) << endl;
		/*archivo << "\ni_curr_steps= " + std::to_string(i_curr_steps) << endl;*/
		archivo.close();
	}
	else cerr << "Error de apertura del archivo." << endl;

	FrameIndex_Dirigible++;
}

void readFrame_Dirigible(void)
{
	using namespace std;
	string aux = "";
	fstream fichero;
	char linea[40];
	fichero.open("Frames_Dirigible.txt", ios::in);
	if (fichero.fail()) {
		cerr << "Error al abrir el archivo Frames_Dirigible.txt" << endl;
	}
	else {
		fichero >> linea;           // Primera linea
		while (!fichero.eof())
		{
			if (FrameIndex_Dirigible < 10) {
				//cout << texto << endl;    // Muestrar el contenido en terminal 
				if (linea[0] == 'K') {
					if (linea[17] == 'x') {
						aux = aux + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
						movd_x = stof(aux);
						aux = "";
						xdokay = true;
						cout << "movd_x =" + to_string(movd_x) << endl;
					}
					else if (linea[17] == 'y') {
						aux = aux + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
						movd_y = stof(aux);
						aux = "";
						ydokay = true;
						cout << "movd_y =" + to_string(movd_y) << endl;
					}
					else if (linea[12] == 'g') {
						aux = aux + linea[18] + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24];
						girod = stof(aux);
						aux = "";
						zdokay = true;
						cout << "girod =" + to_string(girod) << endl;
					}
				}
			}
			else {
				if (linea[0] == 'K') {
					if (linea[18] == 'x') {
						aux = aux +  linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25] + linea[26] ;
						movd_x = stof(aux);
						aux = "";
						xdokay = true;
						cout << "movd_x =" + to_string(movd_x) << endl;
					}
					else if (linea[18] == 'y') {
						aux = aux  + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25] + linea[26];
						movd_y = stof(aux);
						aux = "";
						ydokay = true;
						cout << "movd_y =" + to_string(movd_y) << endl;
					}
					else if (linea[13] == 'g') {
						aux = aux  + linea[19] + linea[20] + linea[21] + linea[22] + linea[23] + linea[24] + linea[25];
						girod = stof(aux);
						aux = "";
						zdokay = true;
						cout << "girod =" + to_string(girod) << endl;
					}
				}
			}

			if (xdokay && ydokay && zdokay) {
				xdokay = false;
				ydokay = false;
				zdokay = false;
				KeyFrame_Dirigible[FrameIndex_Dirigible].movd_x = movd_x;
				KeyFrame_Dirigible[FrameIndex_Dirigible].movd_y = movd_y;
				KeyFrame_Dirigible[FrameIndex_Dirigible].girod = girod;
				cout << "\nKeyFrame [" + to_string(FrameIndex_Dirigible) + "] leido correctamente" << endl;
				FrameIndex_Dirigible++;
			}

			fichero >> linea;         // Seguimos leyendo 
		}
		fichero.close();
	}
}


void resetElements_Dirigible(void)
{
	movd_x = KeyFrame_Dirigible[0].movd_x;
	movd_y = KeyFrame_Dirigible[0].movd_y;
	girod = KeyFrame_Dirigible[0].girod;
}

void interpolation_Dirigible(void)
{
	KeyFrame_Dirigible[playIndex_Dirigible].movd_xInc = (KeyFrame_Dirigible[playIndex_Dirigible + 1].movd_x - KeyFrame_Dirigible[playIndex_Dirigible].movd_x) / i_max_steps_Dirigible;
	KeyFrame_Dirigible[playIndex_Dirigible].movd_yInc = (KeyFrame_Dirigible[playIndex_Dirigible + 1].movd_y - KeyFrame_Dirigible[playIndex_Dirigible].movd_y) / i_max_steps_Dirigible;
	KeyFrame_Dirigible[playIndex_Dirigible].girodInc = (KeyFrame_Dirigible[playIndex_Dirigible + 1].girod - KeyFrame_Dirigible[playIndex_Dirigible].girod) / i_max_steps_Dirigible;
}

void animate_Dirigible(void)
{
	//Movimiento del objeto
	if (play_Dirigible)
	{
		if (i_curr_steps_Dirigible >= i_max_steps_Dirigible) //end of animation between frames
		{
			playIndex_Dirigible++;
			printf("Frame (%d) reproducido\n", playIndex_Dirigible);
			if (playIndex_Dirigible > FrameIndex_Dirigible - 2)	//end of total animation
			{
				printf("El ultimo frame es [%d]\n", FrameIndex_Dirigible);
				printf("Termina animacion\n");
				playIndex_Dirigible = 0;
				play_Dirigible = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps_Dirigible = 0; //Reset counter
				//Interpolation
				interpolation_Dirigible();
			}
		}
		else
		{
			//Draw animation
			movd_x += KeyFrame_Dirigible[playIndex_Dirigible].movd_xInc;
			movd_y += KeyFrame_Dirigible[playIndex_Dirigible].movd_yInc;
			girod += KeyFrame_Dirigible[playIndex_Dirigible].girodInc;
			i_curr_steps_Dirigible++;
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

	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 10.0f, 0.5f);
	cameraPiso = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -50.0f, 0.0f, 0.5f, 0.5f);
	cameraAerea = Camera(glm::vec3(0.0f, 320.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 2.0f, 0.5f);


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
	Model Auto_llanta;
	Model Dirigible_M;
	Model Patrulla_M;
	Model Moto1_M;
	Model Moto1_LlantaDelantera;
	Model Moto1_LlantaTrasera;
	Model Moto2_M;
	Model Moto3_M;
	Model Moto3_llanta;
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
	Auto_llanta = Model();
	Auto_llanta.LoadModel("Modelos_obj/Transportes/Auto1_llanta.obj");
	Dirigible_M = Model();
	Dirigible_M.LoadModel("Modelos_obj/Transportes/Dirigible.obj");
	Patrulla_M = Model();
	Patrulla_M.LoadModel("Modelos_obj/Transportes/Patrulla.obj");
	Moto1_M = Model();
	Moto1_M.LoadModel("Modelos_obj/Transportes/Moto.obj");
	Moto1_LlantaDelantera = Model();
	Moto1_LlantaDelantera.LoadModel("Modelos_obj/Transportes/Moto_llantaFrente.obj");
	Moto1_LlantaTrasera = Model();
	Moto1_LlantaTrasera.LoadModel("Modelos_obj/Transportes/Moto_llantaAtras.obj");
	Moto2_M = Model();
	Moto2_M.LoadModel("Modelos_obj/Transportes/moto2.obj");
	Moto3_M = Model();
	Moto3_M.LoadModel("Modelos_obj/Transportes/moto3.obj");
	Moto3_llanta = Model();
	Moto3_llanta.LoadModel("Modelos_obj/Transportes/moto2_llanta.obj");
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


	MaterialParaLuces = Material(5.0f, 300);
	MaterialNormal = Material(0.5f, 3);

	Tor.init(); //inicializar toroide
	Tor.load();//enviar la toroide al shader

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;


	//KEYFRAMES DECLARADOS INICIALES HELICOPTERO

	KeyFrame_Helicopter[0].movh_x = 0;
	KeyFrame_Helicopter[0].movh_y = 0;
	KeyFrame_Helicopter[0].movh_z = 0;
	KeyFrame_Helicopter[0].giroh = 0;
	KeyFrame_Helicopter[0].rot_helice = 45.0f;

	KeyFrame_Helicopter[1].movh_x = 0.0f;
	KeyFrame_Helicopter[1].movh_y = 0.0f;
	KeyFrame_Helicopter[1].movh_z = 0.0f;
	KeyFrame_Helicopter[1].giroh = 0;
	KeyFrame_Helicopter[1].rot_helice = 180.0f;

	//KEYFRAMES DECLARADOS INICIALES DIRIGIBLE

	KeyFrame_Dirigible[0].movd_x = 5.0f;
	KeyFrame_Dirigible[0].movd_y = -5.0f;
	KeyFrame_Dirigible[0].girod = 0;

	KeyFrame_Dirigible[1].movd_x = 15.0f;
	KeyFrame_Dirigible[1].movd_y = -15.0f;
	KeyFrame_Dirigible[1].girod = 0;



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
		inputKeyframes(mainWindow.getsKeys());
		animate_Helicopter();
		animate_Dirigible();
		

		if (mainWindow.getCamAerea()) {
			cameraAerea.keyControl(mainWindow.getsKeys(), deltaTime, mainWindow.getCamAerea());
			cameraAerea.mouseControl(mainWindow.getXChange());
		}
		else {
			cameraPiso.keyControl(mainWindow.getsKeys(), deltaTime, mainWindow.getCamAerea());
			cameraPiso.mouseControl(mainWindow.getXChange());
		}

		/*camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());*/

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if ((count < cicloDia) && (mainWindow.getCamAerea())) {
			skybox.DrawSkybox(cameraAerea.calculateViewMatrix(), projection);
		}else if ((count < cicloDia) && (mainWindow.getCamAerea()==false)) {
			skybox.DrawSkybox(cameraPiso.calculateViewMatrix(), projection);
			//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}else if ((count >= cicloDia) && (mainWindow.getCamAerea())) {
			skybox2.DrawSkybox(cameraAerea.calculateViewMatrix(), projection);
		}else {
			skybox2.DrawSkybox(cameraPiso.calculateViewMatrix(), projection);
			//skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		count++;
		if (count >= cicloDiaNoche) {
			count = 0;
		}

		if (count >= cicloDia) {

			//luz direccional
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.9f, 0.1f,
				-1.0f, 0.0f, 0.0f);

			if (mainWindow.getHelicoptero()) {
				//Luces puntuales	

				//Dirigible
				pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
					0.9f, 10.0f,
					posXd, 0.0f, posZd,
					0.3f, 0.2f, 0.1f);

				pointLights[0].SetPos(glm::vec3(posXd + movd_x, 0.0f, posZd + movd_y));

				//Batiseñal
				pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
					0.9f, 20.0f,
					-192.0f, 82.8f, 46.0f,
					//-170.0f, 104.0f, 66.0f,
					0.3f, 0.2f, 0.1f);

				//Helicoptero
				pointLights[2] = PointLight(1.0f, 0.0f, 0.0f,
					0.9f, 10.0f,
					(posXh + movh_x) + 10.0f, 5.0f, posZh,
					0.3f, 0.2f, 0.1f);

				pointLights[2].SetPos(glm::vec3((posXh + movh_x) + 10.0f, 5.0f, posZh));

				pointLightCount = 3;
			}
			else {
				//Luces puntuales	

			//Dirigible
				pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
					0.9f, 10.0f,
					posXd, 0.0f, posZd,
					0.3f, 0.2f, 0.1f);

				pointLights[0].SetPos(glm::vec3(posXd + movd_x, 0.0f, posZd + movd_y));

				//Batiseñal
				pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
					0.9f, 20.0f,
					-192.0f, 82.8f, 46.0f,
					//-170.0f, 104.0f, 66.0f,
					0.3f, 0.2f, 0.1f);

				pointLightCount = 2;
			}

			
		}
		else {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f, 0.1f,
				-1.0f, 0.0f, 0.0f);

			if (mainWindow.getHelicoptero()) {

				//Helicoptero
				pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
					0.9f, 10.0f,
					(posXh + movh_x) + 10.0f, 5.0f, posZh,
					0.3f, 0.2f, 0.1f);

				pointLights[0].SetPos(glm::vec3((posXh + movh_x) + 10.0f, 5.0f, posZh));

				pointLightCount = 1;
			}
			else {
				//Luces puntuales	
				pointLightCount = 0;
			}

			
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

		/*glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, cameraPiso.getCameraPosition().y, cameraPiso.getCameraPosition().z);*/

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
		if (mainWindow.getTim()) {
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
				cambio = 12;
			}
			else if (cambio == 12 && rotCuerpo3 <=180.0f) {
				rotCuerpo3 += 5.0f;
			}
			else if (cambio == 12 && rotCuerpo3 > 180.0f) {
				cambio = 13;
			}

			else if (cambio == 13 && movimientoY >= 2.0f) {
				movimientoY -= 0.5f;
				if (cuerpo3 >= 66.0f) {
					cuerpo3 -= 0.2f;
				}
			}
			else if (cambio == 13 && movimientoY < 2.0f) {
			  cambio = 14;
			}
			else if (cambio == 14 && cuerpo3 >= 0.0f) {
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
				cuerpo3 -= 0.25f;
				movimientoY -= 0.003;
			}
			else if (cambio == 14 && cuerpo3 < 0.0f) {
				cambio=15;
			}
			else if (cambio == 15 && rotCuerpo3 > 0.0f) {
				rotCuerpo3 -= 5.0f;
			}
			else if (cambio == 15 && rotCuerpo3 <= 0.0f) {
				pierna5 = 0.0f;
				pierna6 = 0.0f;
				brazo5 = 0.0f;
				brazo6 = 0.0f;
				cuerpo3 = 0.0f;
				movimientoY = 0.0f;
				cambio = 1;
				mainWindow.setTim(false);
			}
		}
		else{
			//Musica
			Tim.pause();
		}


		//Personaje

		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, movimientoY, -17.0f+ cuerpo3));
		model = glm::rotate(model, rotCuerpo3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.2f, 1.6f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[11]->RenderMesh();

		//cara
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.8f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//pie izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.125f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();

		//pie der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.125f));
		model = glm::scale(model, glm::vec3(0.45f, 0.25f, 0.65f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TimTexture.UseTexture();
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[10]->RenderMesh();

		//////////////////////////////////////////////////////////////////////////////////////////
		//WE 0,0
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -3.3f, -60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 7.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		
		if (countSem<verde1+1) {
			Semaforo_V_M.RenderModel();
		}else if ((countSem < amarillo1) && (countSem > verde1)) {
			Semaforo_A_M.RenderModel();
		}
		else {
			Semaforo_R_M.RenderModel();
		}
		countSem++;
		if (countSem >= rojo1+1) {
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
		


		if ((cameraPiso.getCameraPosition().x- (xRh+cuerpo1) <= 100.0f) && (cameraPiso.getCameraPosition().z -(zRh+(cuerpo1* cuerpo1)) <= 100.0f)) {
			//Musica
			music.pause();
			Tim.pause();
			Jason.play();
			Dick.pause();
			Disparo.pause();

			if (cuerpo1 <= 10.0f && cambioJ==1) {
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
			else if (cuerpo1 > 10.0f && cambioJ == 1) {
				cambioJ = 2;
			}
			else if (brazo1 >= -90.0f && cambioJ == 2) {
				brazo1 -= 1.5f;
			}
			else if (brazo1 < -90.0f && cambioJ == 2 && countDisp < 150) {
				pierna1 = 0.0f;
				pierna2 = 0.0f;
				brazo2 = 0.0f;
				Disparo.play();
				countDisp += 1;
			}
			else if (brazo1 < -90.0f && cambioJ == 2 && countDisp >= 150) {
				Disparo.pause();
				cambioJ = 3;
			}
			else if (rotCuerpo<=180.0f && cambioJ == 3) {
				rotCuerpo += 5.0f;
			}
			else if (rotCuerpo > 180.0f && cambioJ == 3) {
				cambioJ = 4;
			}
			else if (cuerpo1 > 0.0f && cambioJ == 4) {
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
				cuerpo1 -= 0.01f;

			}
			else if (cuerpo1 <= 0.0f && cambioJ == 4) {
				cambioJ = 5;
			}
			else if (rotCuerpo > 0.0f && cambioJ == 5) {
				rotCuerpo -= 5.0f;
			}
			else if (rotCuerpo <= 0.0f && cambioJ == 5) {
				cambioJ = 6;
			}
			else if (rotCuerpo > 0.0f && cambioJ == 6) {
				rotCuerpo -= 5.0f;
			}
			else if (rotCuerpo <= 180.0f && cambioJ == 6) {
				pierna1 = 0.0f;
				pierna2 = 0.0f;
				brazo1 = 0.0f;
				brazo2 = 0.0f;
				cuerpo1 = 0.0f;
				countDisp = 0;
				cambioJ = 1;

				//Musica
				Jason.pause();
			}
		}
		else {
				pierna1 = 0.0f;
				pierna2 = 0.0f;
				brazo1 = 0.0f;
				brazo2 = 0.0f;
				cuerpo1 = 0.0f;
				countDisp = 0;
				cambioJ = 1;

				//Musica
				Jason.pause();
			}

		//RedHood
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xRh, yRh, zRh));
		model = glm::translate(model, glm::vec3(cuerpo1, 0.0f, cuerpo1 * cuerpo1));
		model = glm::scale(model, glm::vec3(0.48f, 0.5f, 0.48f));
		model = glm::rotate(model, rotCuerpo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Cuerpo_M.RenderModel();

		//Brazo izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.55f, 1.2f, 0.0f));
		model = glm::rotate(model, brazo1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Brazo_M.RenderModel();

		//Brazo der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.55f, 1.2f, 0.0f));
		model = glm::rotate(model, brazo2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Brazo_M.RenderModel();

		//Pierna izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.75f, -1.63f, 0.0f));
		model = glm::rotate(model, pierna2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Pierna_M.RenderModel();

		//Pierna der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.75f, -1.63f, 0.0f));
		model = glm::rotate(model, pierna1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rh_Pierna_M.RenderModel();


		//Moto RedHood
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.9f, -135.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Moto1_M.RenderModel();

		//Llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.1f, 1.15f, 0.0f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto1_LlantaDelantera.RenderModel();


		//Llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.0f, 1.15f, 0.0f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto1_LlantaTrasera.RenderModel();

		//arbol
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		if (countSem < verde1 + 1) {
			Semaforo_V_M.RenderModel();
		}
		else if ((countSem < amarillo1) && (countSem > verde1)) {
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
		
		if (countSem < verde2+1) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < amarillo2+1) && (countSem > verde2)) {
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
		if (countSem < verde2 + 1) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < amarillo2 + 1) && (countSem > verde2)) {
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
		if (countSem < verde1 + 1) {
			Semaforo_V_M.RenderModel();
		}
		else if ((countSem < amarillo1) && (countSem > verde1)) {
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

		if ((cameraPiso.getCameraPosition().x - (xNw + cuerpo2) <= 50.0f) && (cameraPiso.getCameraPosition().z - (zNw - movZ) <= 50.0f)) {
			//Musica
			music.pause();
			Tim.pause();
			Jason.pause();
			Dick.play();
			Disparo.pause();

			if (cuerpo2 <= 100.0f && cambioN == 1) {
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
				cuerpo2 += 0.5f;
			}
			else if (cuerpo2 > 10.0f && cambioN == 1) {
				pierna3 = 0.0f;
				pierna4 = 0.0f;
				cambioN = 2;
			}
			else if (rotCuerpo2 <= 90.0f && cambioN == 2) {
				rotCuerpo2 += 5.0f;
			}
			else if (rotCuerpo2 > 90.0f && cambioN == 2) {
				cambioN = 3;
			}
			else if (pierna3 >= -90.0f && pierna4 <= 90.0f && cambioN == 3) {
				pierna3 -= 1.0f;
				pierna4 += 1.0f;
				movY += 0.1f;
				movZ -= 0.01f;
			}
			else if (pierna3 < -90.0f && pierna4 > 90.0f && cambioN == 3) {
				cambioN = 4;
			}
			else if (movY > 0.0f && cambioN == 4) {
				pierna3 += 1.0f;
				pierna4 -= 1.0f;
				movY -= 0.1f;
				movZ -= 0.01f;
			}
			else if (movY <= 0.0f && cambioN == 4) {
				cambioN = 5;
				/*printf("Z final:%f", movZ);*/
			}
			else if (rotCuerpo2 <= 270.0f && cambioN == 5) {
				rotCuerpo2 += 5.0f;
			}
			else if (rotCuerpo2 > 0.0f && cambioN == 5) {
				cambioN = 6;
			}

			else if (movZ <= 0.0f && cambioN == 6) {
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
				movZ += 0.01f;
			}
			else if (movZ > 0.0f && cambioN == 6) {
				cambioN = 7;
			}
			else if (rotCuerpo2 >= 180.0f && cambioN == 7) {
				rotCuerpo2 -= 5.0f;
			}
			else if (rotCuerpo2 < 360.0f && cambioN == 7) {
				cambioN = 8;
			}
			else if (cuerpo2 > 0.0f && cambioN == 8) {
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
				cuerpo2 -= 0.5f;
			}
			else if (cuerpo2 <= 0.0f && cambioN == 8) {
				cambioN=9;
			}
			else if (rotCuerpo2 > 0.0f && cambioN == 9) {
				rotCuerpo2 -= 5.0f;
			}
			else if (rotCuerpo2 <= 0.0f && cambioN == 9) {
				cambioN = 1;
				pierna3 = 0.0f;
				pierna4 = 0.0f;
				brazo3 = 0.0f;
				brazo4 = 0.0f;
				movY = 0.0f;
				cuerpo2 = 0.0f;
				rotCuerpo2 = 0.0f;

				Dick.pause();
			}
		}
		else {
				cambioN = 1;
				pierna3 = 0.0f;
				pierna4 = 0.0f;
				brazo3 = 0.0f;
				brazo4 = 0.0f;
				movY = 0.0f;
				
				Dick.pause();
		}

		//Nigthwing
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xNw,yNw,zNw));
		model = glm::translate(model, glm::vec3(cuerpo2, 0.3f+ movY, -movZ*movZ));
		model = glm::scale(model, glm::vec3(0.48f, 0.49f, 0.48f));
		model = glm::rotate(model, rotCuerpo2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Cuerpo_M.RenderModel();

		//Brazo izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -1.55f));
		model = glm::rotate(model, brazo3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Brazo_M.RenderModel();

		//Brazo der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 1.55f));
		model = glm::rotate(model, brazo4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Brazo_M.RenderModel();

		//Pierna izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, -0.75f));
		model = glm::rotate(model, pierna4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Pierna_M.RenderModel();

		//Pierna der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.75f));
		model = glm::rotate(model, pierna3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nw_Pierna_M.RenderModel();

		//Batiseñal
		model = modelaux;
		model = glm::translate(model, glm::vec3(-12.0f, 85.5f, -24.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotBatsignal * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaBatsignal, escalaBatsignal, escalaBatsignal));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		if (countSem < verde1 + 1) {
			Semaforo_V_M.RenderModel();
		}
		else if ((countSem < amarillo1) && (countSem > verde1)) {
			Semaforo_A_M.RenderModel();
		}
		else {
			Semaforo_R_M.RenderModel();
		}


		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(25.0f, 3.45f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 3.45f, -40.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.2f, 1.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		//Banca
		model = modelaux;
		model = glm::translate(model, glm::vec3(-25.0f, 3.45f, -40.0f));
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
		if (countSem < verde2 + 1) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < amarillo2 + 1) && (countSem > verde2)) {
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
		if (countSem < verde2 + 1) {
			Semaforo_R_M.RenderModel();
		}
		else if ((countSem < amarillo2 + 1) && (countSem > verde2)) {
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

		if ((countSem < amarillo1 + 1) && (cambioAuto == 1)) {
			//Movimiento izq y derecha

			//Carril derecho
			if (625.0f - movAuto1 == 40.0f) {
				movAuto1 = 0.0f;
				//printf("\nSe reinicio mov1");
			}
			movAuto1 += 0.5f;
			rotLlanta1 += 5.0f;

			if (485.0f - movAuto2 == -100.0f) {
				movAuto2 = 0.0f;
				//printf("\nSe reinicio mov2");
			}
			movAuto2 += 0.5f;
			rotLlanta2 += 5.0f;

			
			if (705.0f - movAuto3 == 120.0f) {
				movAuto3 = 0.0f;
				//printf("\nSe reinicio mov3");
			}
			movAuto3 += 0.5f;
			rotLlanta3 += 5.0f;
			
			if (825.0f - movAuto4 == 240.0f) {
				movAuto4= 0.0f;
				//printf("\nSe reinicio mov4");
			}
			movAuto4 += 0.5f;
			rotLlanta4 += 5.0f;

			
			if (345.0f - movAuto5 == -240.0f) {
				movAuto5= 0.0f;
				//printf("\nSe reinicio mov5");
			}
			movAuto5 += 0.5f;
			rotLlanta5 += 5.0f;

			
			if (563.0f - movAuto6 == -22.0f) {
				movAuto6= 0.0f;
				//printf("\nSe reinicio mov6");
			}
			movAuto6 += 0.5f;
			rotLlanta6 += 5.0f;



			if (525.0f - moto1 == -60.0f) {
				moto1 = 0.0f;
				//printf("\nSe reinicio moto1");
			}
			moto1 += 0.5f;
			rotLlantaMoto1 += 5.0f;

			if (655.0f - moto2 == 70.0f) {
				moto2 = 0.0f;
				//printf("\nSe reinicio moto2");
			}
			moto2 += 0.5f;
			rotLlantaMoto2 += 5.0f;


			if (755.0f - moto3 == 170.0f) {
				moto3 = 0.0f;
				//printf("\nSe reinicio moto3");
			}
			moto3 += 0.5f;
			rotLlantaMoto3 += 5.0f;
			
			if (415.0f - moto4 == -170.0f) {
				moto4 = 0.0f;
				//printf("\nSe reinicio moto4");
			}
			moto4 += 0.5f;
			rotLlantaMoto4 += 5.0f;


			//Carril izq
			if (-555.0f + movAuto7 == 30.0f) {
				movAuto7 = 0.0f;
				//printf("\nSe reinicio mov7");
			}
			movAuto7 += 0.5f;
			rotLlanta7 += 5.0f;

			if (-695.0f + movAuto8 == -110.0f) {
				movAuto8 = 0.0f;
				//printf("\nSe reinicio mov8");
			}
			movAuto8 += 0.5f;
			rotLlanta8 += 5.0f;
			
			if (-455.0f + movAuto9 == 130.0f) {
				movAuto9 = 0.0f;
				//printf("\nSe reinicio mov9");
			}
			movAuto9 += 0.5f;
			rotLlanta9 += 5.0f;
			
			if (-345.0f + movAuto10 == 240.0f) {
				movAuto10 = 0.0f;
				//printf("\nSe reinicio mov10");
			}
			movAuto10 += 0.5f;
			rotLlanta10 += 5.0f;
			
			if (-825.0f + movAuto11 == -240.0f) {
				movAuto11 = 0.0f;
				//printf("\nSe reinicio mov11");
			}
			movAuto11 += 0.5f;
			rotLlanta11 += 5.0f;
			
			if (-607.0f + movAuto12 == -22.0f) {
				movAuto12 = 0.0f;
				//printf("\nSe reinicio mov12");
			}
			movAuto12 += 0.5f;
			rotLlanta12 += 5.0f;

			if (-645.0f + moto5 == -60.0f) {
				moto5 = 0.0f;
				//printf("\nSe reinicio moto5");
			}
			moto5 += 0.5f;
			rotLlantaMoto5 += 5.0f;
			
			if (-515.0f + moto6 == 70.0f) {
				moto6 = 0.0f;
				//printf("\nSe reinicio moto6");
			}
			moto6 += 0.5f;
			rotLlantaMoto6 += 5.0f;
			
			if (-415.0f + moto7 == 170.0f) {
				moto7 = 0.0f;
				//printf("\nSe reinicio moto7");
			}
			moto7 += 0.5f;
			rotLlantaMoto7 += 5.0f;
			
			if (-755.0f + moto8 == -170.0f) {
				moto8 = 0.0f;
				//printf("\nSe reinicio moto8");
			}
			moto8 += 0.5f;
			rotLlantaMoto8 += 5.0f;
			
			
		}
		else if ((countSem == amarillo1 + 1) && (cambioAuto == 1)) {
			cambioAuto = 2;
			//printf("Val:%f", 40.0f - movAuto1);
		}
		else if ((countSem > amarillo1 + 1 && countSem < rojo1) && (cambioAuto == 2)) {
			//Mov enfrente y atrás

			//Carril atrás
			if (605.0f - movAuto13 == 20.0f) {
				movAuto13 = 0.0f;
				//printf("\nSe reinicio mov13");
			}
			movAuto13 += 0.5f;
			rotLlanta13 += 5.0f;

			if (755.0f - movAuto14 == 170.0f) {
				movAuto14 = 0.0f;
				//printf("\nSe reinicio mov14");
			}
			movAuto14 += 0.5f;
			rotLlanta14 += 5.0f;
			
			if (455.0f - movAuto15 == -130.0f) {
				movAuto15 = 0.0f;
				//printf("\nSe reinicio mov15");
			}
			movAuto15 += 0.5f;
			rotLlanta15 += 5.0f;

			//Carril enfrente
			if (-565.0f + movAuto16 == 20.0f) {
				movAuto16 = 0.0f;
				//printf("\nSe reinicio mov16");
			}
			movAuto16 += 0.5f;
			rotLlanta16 += 5.0f;

			if (-415.0f + movAuto17 == 170.0f) {
				movAuto17 = 0.0f;
				//printf("\nSe reinicio mov17");
			}
			movAuto17 += 0.5f;
			rotLlanta17 += 5.0f;

			if (-715.0f + movAuto18 == -130.0f) {
				movAuto18 = 0.0f;
				//printf("\nSe reinicio mov18");
			}
			movAuto18 += 0.5f;
			rotLlanta18 += 5.0f;
			
		}
		else if ((countSem > amarillo1 + 1 && countSem == rojo1) && (cambioAuto == 2)) {
			cambioAuto = 1;
		}

		if (rotLlanta1 >= 360.0f) {
			rotLlanta1 = 0.0f;
		}
		if (rotLlanta2 >= 360.0f) {
			rotLlanta2 = 0.0f;
		}
		if (rotLlanta3 >= 360.0f) {
			rotLlanta3 = 0.0f;
		}
		if (rotLlanta4 >= 360.0f) {
			rotLlanta4 = 0.0f;
		}
		if (rotLlanta5 >= 360.0f) {
			rotLlanta5 = 0.0f;
		}
		if (rotLlanta6 >= 360.0f) {
			rotLlanta6 = 0.0f;
		}
		if (rotLlanta7 >= 360.0f) {
			rotLlanta7 = 0.0f;
		}
		if (rotLlanta8 >= 360.0f) {
			rotLlanta8 = 0.0f;
		}
		if (rotLlanta9 >= 360.0f) {
			rotLlanta9 = 0.0f;
		}
		if (rotLlanta10 >= 360.0f) {
			rotLlanta10 = 0.0f;
		}
		if (rotLlanta11 >= 360.0f) {
			rotLlanta11 = 0.0f;
		}
		if (rotLlanta12 >= 360.0f) {
			rotLlanta12 = 0.0f;
		}
		if (rotLlanta13 >= 360.0f) {
			rotLlanta13 = 0.0f;
		}
		if (rotLlanta14 >= 360.0f) {
			rotLlanta14 = 0.0f;
		}
		if (rotLlanta15 >= 360.0f) {
			rotLlanta15 = 0.0f;
		}
		if (rotLlanta16 >= 360.0f) {
			rotLlanta16 = 0.0f;
		}
		if (rotLlanta17 >= 360.0f) {
			rotLlanta17 = 0.0f;
		}
		if (rotLlanta18 >= 360.0f) {
			rotLlanta18 = 0.0f;
		}
		if (rotLlantaMoto1 >= 360.0f) {
			rotLlantaMoto1 = 0.0f;
		}
		if (rotLlantaMoto2 >= 360.0f) {
			rotLlantaMoto2 = 0.0f;
		}
		if (rotLlantaMoto3 >= 360.0f) {
			rotLlantaMoto3 = 0.0f;
		}
		if (rotLlantaMoto4 >= 360.0f) {
			rotLlantaMoto4 = 0.0f;
		}
		if (rotLlantaMoto5 >= 360.0f) {
			rotLlantaMoto5 = 0.0f;
		}
		if (rotLlantaMoto6 >= 360.0f) {
			rotLlantaMoto6 = 0.0f;
		}
		if (rotLlantaMoto7 >= 360.0f) {
			rotLlantaMoto7 = 0.0f;
		}
		if (rotLlantaMoto8 >= 360.0f) {
			rotLlantaMoto8 = 0.0f;
		}
		

		//Auto 285
		model = modelaux;
		if (40.0f - movAuto1 >= -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 40.0f - movAuto1));
		}
		else if (40.0f - movAuto1 < -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 625.0f - movAuto1));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		
		//Auto
		model = modelaux;
		if (-100.0f - movAuto2 >= -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, -100.0f - movAuto2));
		}
		else if (-100.0f - movAuto2 < -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 485.0f - movAuto2));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x - 420 =285
		model = modelaux;
		if (120.0f - movAuto3 >= -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 120.0f - movAuto3));
		}
		else if (120.0f - movAuto3 < -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 705.0f - movAuto3));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x - 540 =285
		model = modelaux;
		if (240.0f - movAuto4 >= -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 240.0f - movAuto4));
		}
		else if (240.0f - movAuto4 < -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 825.0f - movAuto4));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x - 60 =285
		model = modelaux;
		if (-240.0f - movAuto5 >= -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, -240.0f - movAuto5));
		}
		else if (-240.0f - movAuto5 < -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, 1.2f, 345.0f - movAuto5));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Patrulla x - 278 =285
		model = modelaux;
		if (-22.0f - movAuto6 >= -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, -0.4f, -22.0f - movAuto6));
		}
		else if (-22.0f - movAuto6 < -300.0f) {
			model = glm::translate(model, glm::vec3(9.0f, -0.4f, 563.0f - movAuto6));
		}
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, -1.55f));
		model = glm::rotate(model, -rotLlanta6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, -1.57f));
		model = glm::rotate(model, -rotLlanta6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, 1.75f));
		model = glm::rotate(model, -rotLlanta6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, 1.55f));
		model = glm::rotate(model, -rotLlanta6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		
		//Moto x - 240 =285
		model = modelaux;
		if (-60.0f - moto1 >= -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, -60.0f - moto1));
		}
		else if (-60.0f - moto1 < -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, 525.0f - moto1));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//Moto x - 370 =285
		model = modelaux;
		if (70.0f - moto2 >= -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, 70.0f - moto2));
		}
		else if (70.0f - moto2 < -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, 655.0f - moto2));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//Moto x - 470 =285
		model = modelaux;
		if (170.0f - moto3 >= -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, 170.0f - moto3));
		}
		else if (170.0f - moto3 < -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, 755.0f - moto3));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//Moto x - 130 =285
		model = modelaux;
		if (-170.0f - moto4 >= -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, -170.0f - moto4));
		}
		else if (-170.0f - moto4 < -300.0f) {
			model = glm::translate(model, glm::vec3(7.0f, 0.7f, 415.0f - moto4));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		////////////////////////////////////////////////////////////////////////////////////////////
		//camino izq
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.5f, 5.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 23.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();

		//Auto x + 255 = -300
		model = modelaux;
		if (30.0f + movAuto7 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 30.0f + movAuto7));
		}
		else if (30.0f + movAuto7 > 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -555.0f + movAuto7));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta7 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta7 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta7 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta7 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x + 395  = -300
		model = modelaux;
		if (-110.0f + movAuto8 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -110.0f + movAuto8));
		}
		else if (-110.0f + movAuto8 > 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -695.0f + movAuto8));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta8 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta8 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta8 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta8 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x + 155  = -300
		model = modelaux;
		if (130.0f + movAuto9 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 130.0f + movAuto9));
		}
		else if (130.0f + movAuto9 > 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -455.0f + movAuto9));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta9 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta9 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta9 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta9 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x + 45  = -300
		model = modelaux;
		if (240.0f + movAuto10 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 240.0f + movAuto10));
		}
		else if (240.0f + movAuto10 > 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -345.0f + movAuto10));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Auto x + 525  = -300
		model = modelaux;
		if (-240.0f + movAuto11 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -240.0f + movAuto11));
		}
		else if (-240.0f + movAuto11 > 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, 1.2f, -825.0f + movAuto11));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta11 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta11 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta11 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta11 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Patrulla x + 307‬  = -300
		model = modelaux;
		if (-22.0f + movAuto12 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, -0.4f, -22.0f + movAuto12));
		}
		else if (-22.0f + movAuto12 > 285.0f) {
			model = glm::translate(model, glm::vec3(-9.0f, -0.4f, -607.0f + movAuto12));
		}
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, -1.55f));
		model = glm::rotate(model, -rotLlanta12 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, -1.57f));
		model = glm::rotate(model, -rotLlanta12 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, 1.75f));
		model = glm::rotate(model, -rotLlanta12 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, 1.55f));
		model = glm::rotate(model, -rotLlanta12 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//Moto x + 345‬  = -300
		model = modelaux;
		if (-60.0f + moto5 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, -60.0f + moto5));
		}
		else if (-60.0f + moto5 > 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, -645.0f + moto5));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto5 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();


		//Moto x + 215‬  = -300
		model = modelaux;
		if (70.0f + moto6 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, 70.0f + moto6));
		}
		else if (70.0f + moto6 > 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, -515.0f + moto6));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto6 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();


		//Moto x + 115‬  = -300
		model = modelaux;
		if (170.0f + moto7 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, 170.0f + moto7));
		}
		else if (170.0f + moto7 > 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, -415.0f + moto7));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto7 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto7 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();


		//Moto x + 455  = -300
		model = modelaux;
		if (-170.0f + moto8 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, -170.0f + moto8));
		}
		else if (-170.0f + moto8 > 285.0f) {
			model = glm::translate(model, glm::vec3(-6.0f, 0.7f, -755.0f + moto8));
		}
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto2_M.RenderModel();

		//llanta delantera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.72f));
		model = glm::rotate(model, rotLlantaMoto8 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();

		//llanta trasera
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, -0.75f));
		model = glm::rotate(model, rotLlantaMoto8 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Moto3_llanta.RenderModel();


		////////////////////////////////////////////////////////////////////////////////////////////
		//camino atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.9f, -125.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(23.0f, 2.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel(); 

		//Auto x - 320 =285
		model = modelaux;
		if (20.0f - movAuto13 >= -300.0f) {
			model = glm::translate(model, glm::vec3(20.0f - movAuto13, -0.3f, -9.0f));
		}
		else if (20.0f - movAuto13 < -300.0f) {
			model = glm::translate(model, glm::vec3(605.0f - movAuto13, -0.3f, -9.0f));
		}
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, -1.55f));
		model = glm::rotate(model, -rotLlanta13 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, -1.57f));
		model = glm::rotate(model, -rotLlanta13 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, 1.75f));
		model = glm::rotate(model, -rotLlanta13 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, 1.55f));
		model = glm::rotate(model, -rotLlanta13 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//Auto x - 470 =285
		model = modelaux;
		if (170.0f - movAuto14 >= -300.0f) {
			model = glm::translate(model, glm::vec3(170.0f - movAuto14, 1.5f, -9.0f));
		}
		else if (170.0f - movAuto14 < -300.0f) {
			model = glm::translate(model, glm::vec3(755.0f - movAuto14, 1.5f, -9.0f));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta14 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta14 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta14 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta14 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//Auto x - 170 =285
		model = modelaux;
		if (-130.0f - movAuto15 >= -300.0f) {
			model = glm::translate(model, glm::vec3(-130.0f - movAuto15, 1.5f, -9.0f));
		}
		else if (-130.0f - movAuto15 < -300.0f) {
			model = glm::translate(model, glm::vec3(455.0f - movAuto15, 1.5f, -9.0f));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel(); 


		////////////////////////////////////////////////////////////////////////////////////////////
		//camino frente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.9f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(23.0f, 2.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino_M.RenderModel();



		//Auto x + 265 = -300
		model = modelaux;
		if (20.0f + movAuto16 <= 285.0f) {
			model = glm::translate(model, glm::vec3(20.0f + movAuto16, -0.3f, 9.0f));
		}
		else if (20.0f + movAuto16 > 285.0f) {
			model = glm::translate(model, glm::vec3(-565.0f + movAuto16, -0.3f, 9.0f));
		}
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patrulla_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, -1.55f));
		model = glm::rotate(model, -rotLlanta16 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, -1.57f));
		model = glm::rotate(model, -rotLlanta16 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.6f, 1.0f, 1.75f));
		model = glm::rotate(model, -rotLlanta16 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(-1.6f, 1.0f, 1.55f));
		model = glm::rotate(model, -rotLlanta16 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//Auto x + 115‬ = -300
		model = modelaux;
		if (170.0f + movAuto17 <= 285.0f) {
			model = glm::translate(model, glm::vec3(170.0f + movAuto17, 1.5f, 9.0f));
		}
		else if (170.0f + movAuto17 > 285.0f) {
			model = glm::translate(model, glm::vec3(-415.0f + movAuto17, 1.5f, 9.0f));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta17 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta17 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta17 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta17 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//Auto x + 415‬ = -300
		model = modelaux;
		if (-130.0f + movAuto18 <= 285.0f) {
			model = glm::translate(model, glm::vec3(-130.0f + movAuto18, 1.5f, 9.0f));
		}
		else if (-130.0f + movAuto18 > 285.0f) {
			model = glm::translate(model, glm::vec3(-715.0f + movAuto18, 1.5f, 9.0f));
		}
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto1_M.RenderModel();

		//llanta delantera der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta18 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta delantera izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, -1.5f));
		model = glm::rotate(model, -rotLlanta18 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();


		//llanta posterior der
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.4f, -0.4f, 1.7f));
		model = glm::rotate(model, -rotLlanta18 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();

		//llanta posterior izq
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.65f, -0.4f, 1.5f));
		model = glm::rotate(model, -rotLlanta18 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto_llanta.RenderModel();



		////////////////////////////////////////////////////////////////////////////////////////////
		//Transportes

		//Base plataforma tren
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 230.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(24.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BaseTren_M.RenderModel();

		//Plataforma tren
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 26.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tren_M.RenderModel();

		//Estacion
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 262.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.6f, 5.5f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Estacion_M.RenderModel();

		//Dirigible
		model = glm::mat4(1.0); 
		posd = glm::vec3(posXd + movd_x, posYd, posZd + movd_y);
		model = glm::translate(model, posd);
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, girod * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dirigible_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////
		//Escenario spotlight
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 160.0f));
		model = glm::scale(model, glm::vec3(17.0f, 12.0f, 12.0f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		

		//Toroide
		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-190.0f, 1.0f, 160.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tor.render();


		//Helipuerto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -2.0f, 160.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		model = glm::scale(model, glm::vec3(8.0f, 5.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helipuerto_M.RenderModel();


		//punto de giro
		model = glm::mat4(1.0);
		posh = glm::vec3(posXh -10.0f + movh_x, posYh + movh_y, posZh + movh_z);
		model = glm::translate(model, posh);
		model = glm::rotate(model, giroh * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));


		//Helicopter
		model = glm::translate(model, glm::vec3(10.0f , 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
		helicopter_M.RenderModel();

		//helice
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 3.8f, 0.0f));
		model = glm::rotate(model, rot_helice * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialParaLuces.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlueBird_M.RenderModel();

		//alas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -0.2f));
		model = glm::rotate(model, rotWings * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		MaterialNormal.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlueBirdWings_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	//Helicoptero
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
				printf("\nPresiona Enter para habilitar reproducir de nuevo la animacion'\n");
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

	if (keys[GLFW_KEY_F2])
	{
		if (guardoFrame_Helicopter < 1)
		{
			saveFrame_Helicopter();
			printf("\nPresiona F3 para habilitar guardar otro frame\n");
			guardoFrame_Helicopter++;
			reinicioFrame_Helicopter = 0;
		}
	}
	if (keys[GLFW_KEY_F3])
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
			movh_x -= 30.0f;
			printf("movh_x es: %f\n", movh_x);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
		}
	}

	if (keys[GLFW_KEY_J])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_x += 30.0f;
			printf("movh_x es: %f\n", movh_x);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
		}
	}

	if (keys[GLFW_KEY_I])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_y += 25.0f;
			printf("movh_y es: %f\n", movh_y);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
		}
	}
	if (keys[GLFW_KEY_K])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_y -= 25.0f;
			printf("movh_y es: %f\n", movh_y);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
		}
	}

	if (keys[GLFW_KEY_N])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_z += 25.0f;
			printf("movh_z es: %f\n", movh_z);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
		}
	}
	if (keys[GLFW_KEY_M])
	{
		if (ciclo_Helicopter < 1)
		{
			movh_z -= 25.0f;
			printf("movh_z es: %f\n", movh_z);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
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
			printf("reinicia con F5\n");
		}
	}

	if (keys[GLFW_KEY_P])
	{
		if (ciclo_Helicopter < 1)
		{
			giroh -= 90.0f;
			printf("giroh es: %f\n", giroh);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
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
			printf("reinicia con F5\n");
		}
	}


	if (keys[GLFW_KEY_Y])
	{
		if (ciclo_Helicopter < 1)
		{
			rot_helice -= 90.0f;
			printf("rot_helice es: %f\n", rot_helice);
			ciclo_Helicopter++;
			ciclo2_Helicopter = 0;
			printf("reinicia con F5\n");
		}
	}

	if (keys[GLFW_KEY_F5])
	{
		if (ciclo2_Helicopter < 1)
		{
			ciclo_Helicopter = 0;
		}
	}


	if (keys[GLFW_KEY_F6])
	{
		if (leoFrame_Helicopter < 1)
		{
			printf("\nLeyendo Frames\n");
			readFrame_Helicopter();
			printf("\nPresiona F7 para habilitar leer nuevamente el archivo\n");
			leoFrame_Helicopter++;
			reinicioLeoFrame = 0;
		}
	}
	if (keys[GLFW_KEY_F7])
	{
		if (reinicioLeoFrame < 1)
		{
			leoFrame_Helicopter = 0;
		}
	}

	//Dirigible
	if (keys[GLFW_KEY_TAB])
	{
		if (reproduciranimacion_Dirigible < 1)
		{
			if (play_Dirigible == false && (FrameIndex_Dirigible > 1))
			{
				resetElements_Dirigible();
				//First Interpolation				
				interpolation_Dirigible();
				play_Dirigible = true;
				playIndex_Dirigible = 0;
				i_curr_steps_Dirigible = 0;
				reproduciranimacion_Dirigible++;
				printf("\nPresiona BACKSPACE para habilitar reproducir de nuevo la animacion'\n");
				habilitaranimacion_Dirigible = 0;

			}
			else
			{
				play_Dirigible = false;
			}
		}
	}
	if (keys[GLFW_KEY_BACKSPACE])
	{
		if (habilitaranimacion_Dirigible < 1)
		{
			reproduciranimacion_Dirigible = 0;
		}
	}

	if (keys[GLFW_KEY_PERIOD])
	{
		if (guardoFrame_Dirigible < 1)
		{
			saveFrame_Dirigible();
			printf("\nPresiona , (coma) para habilitar guardar otro frame\n");
			guardoFrame_Dirigible++;
			reinicioFrame_Dirigible = 0;
		}
	}
	if (keys[GLFW_KEY_COMMA])
	{
		if (reinicioFrame_Dirigible < 1)
		{
			guardoFrame_Dirigible = 0;
		}
	}

	if (keys[GLFW_KEY_LEFT])
	{
		if (ciclo_Dirigible < 1)
		{
			movd_x -= 5.0f;
			printf("movd_x es: %f\n", movd_x);
			ciclo_Dirigible++;
			ciclo2_Dirigible = 0;
			printf("reinicia con F1\n");
		}
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		if (ciclo_Dirigible < 1)
		{
			movd_x += 5.0f;
			printf("movd_x es: %f\n", movd_x);
			ciclo_Dirigible++;
			ciclo2_Dirigible = 0;
			printf("reinicia con F1\n");
		}
	}

	if (keys[GLFW_KEY_UP])
	{
		if (ciclo_Dirigible < 1)
		{
			movd_y -= 5.0f;
			printf("movd_y es: %f\n", movd_y);
			ciclo_Dirigible++;
			ciclo2_Dirigible = 0;
			printf("reinicia con F1\n");
		}
	}
	if (keys[GLFW_KEY_DOWN])
	{
		if (ciclo_Dirigible < 1)
		{
			movd_y += 5.0f;
			printf("movd_y es: %f\n", movd_y);
			ciclo_Dirigible++;
			ciclo2_Dirigible = 0;
			printf("reinicia con F1\n");
		}
	}

	if (keys[GLFW_KEY_G])
	{
		if (ciclo_Dirigible < 1)
		{
			girod += 90.0f;
			printf("giroh es: %f\n", girod);
			ciclo_Dirigible++;
			ciclo2_Dirigible = 0;
			printf("reinicia con F1\n");
		}
	}

	if (keys[GLFW_KEY_F])
	{
		if (ciclo_Dirigible < 1)
		{
			girod -= 90.0f;
			printf("giroh es: %f\n", girod);
			ciclo_Dirigible++;
			ciclo2_Dirigible = 0;
			printf("reinicia con F1\n");
		}
	}


	if (keys[GLFW_KEY_F1])
	{
		if (ciclo2_Dirigible < 1)
		{
			ciclo_Dirigible = 0;
		}
	}


	if (keys[GLFW_KEY_INSERT])
	{
		if (leoFrame_Dirigible < 1)
		{
			printf("\nLeyendo Frames\n");
			readFrame_Dirigible();
			printf("\nPresiona DELETE para habilitar leer nuevamente el archivo\n");
			leoFrame_Dirigible++;
			reinicioLeoFrame_Dirigible = 0;
		}
	}
	if (keys[GLFW_KEY_DELETE])
	{
		if (reinicioLeoFrame_Dirigible < 1)
		{
			leoFrame_Dirigible = 0;
		}
	}

}


