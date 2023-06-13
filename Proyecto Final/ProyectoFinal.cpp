/*
Semestre 2023-2
Nombres: 
Arroyo Quiroz José Miguel
Pérez Quintana Arturo
Proyecto Final: Diorama de la Vida Cotidiana (Lucario y Frijolito)
*/
//para cargar imagen
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
#include <ctime>
#include <iostream>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include <chrono>

//Para audio
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSorce.h"

const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float toffsetu;
float toffsetv;

bool dia;
float movVol;
float movVolOffset;
int rotVol;
int rotVolOffset;
bool arriba;
float movOnix;
float movOnixOffset;
int rotOnix;
int rotOnixOffset;
bool avanzaOnix;
float movXLuc;
float movXLucOffset;
float movZLuc;
float movZLucOffset;
int rotCuerLuc;
int rotLuc;
int rotLucOffset;
bool avanza;
bool gira;

float rotColumpio, rotSyB;
float rotColumpioOffset, rotSyBOffset;
bool BanColumpio, BanSyB;
float incRot;

//Animacion autobus
glm::vec3 PosIni(70.0f, 2.16f, -80.0f);

//Movimiento del autobus
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;
float movKitXOffset = 0.4;
float movKitZOffset = 0.4;
float rotKitOffset = 0.4;

bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;

//Movimiento frijolito
float angleFri = 0.1;
float angleFriOffset = 0.01;
float radius = 8.0;
float movFriX = radius * cos(angleFri);
float movFriZ = radius * sin(angleFri);
float movFriExtremidad = 0.0;
float movFriExtremidadOffset = 0.65;
bool BanFrijolito = true;


//Banderas
int bandia = 0;
bool audioBandera = false;
bool audioBandera2 = true;
bool banAntena = false;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camIso;

Texture plainTexture;
Texture pisoTexture;
Texture humo;
Texture Lago;


Model Pueblo;
Model Arboles;
Model Voltorb;
Model Electrode;
Model Onix;

//Avatar
Model LucCuerpo;
Model LucCabeza;
Model LucCola;
Model LucBraDer;
Model LucBraIzq;
Model LucPierDer;
Model LucPierIzq;


Model Farola;
Model Poste;

Model CasaParque;
Model Fuente;
Model Dulceria;
Model AreaInf;
Model Reja;
Model Columpio;
Model SyB;
Model Entrada;
Model Casa2;
Model Personaje2;
Model arbol;
Model arbusto;

//Frijolito
Model Ring;
Model Gradas;
Model CasaFrijolito;
Model Autobus;
Model AutobusLlanta;
Model Escuela;
Model Estanque;
Model Frijolito;
Model FrijolitoPiernaDer;
Model FrijolitoPiernaIzq;
Model FrijolitoBrazoDer;
Model FrijolitoBrazoIzq;
Model AntenaBrazoInf;
Model AntenaBrazoSup;
Model Antena;


//Skybox
Skybox skyboxDia;
Skybox skyboxNoche;


//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//Luz direccional
DirectionalLight mainLight;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Audio
SoundDevice* mysounddevice = SoundDevice::get();
uint32_t /*ALunit*/ soundFrijolito = SoundBuffer::get()->addSoundEffect("FrijolitoTema.wav");
uint32_t /*ALunit*/ soundAmbiental = SoundBuffer::get()->addSoundEffect("PokemonIntro.wav");
SoundSorce mySpeaker;

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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



///////////////////////////////KEYFRAMES/////////////////////

float rotBrazoInfZ = 0, rotBrazoInfY = 0, rotBrazoSupZ = 0, rotBrazoSupY = 0, rotAntena = 0;

#define MAX_FRAMES 11
int i_max_steps = 1500;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float rotBrazoInfZ;		//Variable para Rotacion del brazo inferior
	float rotBrazoInfY;
	float rotBrazoSupZ;		//Variable para Rotacion del brazo superior
	float rotBrazoSupY;
	float rotAntena;		//Variable para la rotacion de la antena

	//Respectivos incrementos
	float rotIncBraInfZ;
	float rotIncBraInfY;
	float rotIncBraSupZ;
	float rotIncBraSupY;
	float rotIncAntena;

}FRAME;


FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos navegando entre los frames
bool play = false;
int playIndex = 0;			//Jalo las animaciones


GLfloat frames[] = {
		 0.0,	      0.0,		  0.0,	    0.0,         0.0,		//Frame 0 de parttida
		17.600029,  150.099960,  43.999905, 0.000000,  -30.0,		//Frame 1
		29.900076,  -43.099918,  62.199326, 0.000000,   30.0,		//Frame 2
		19.400036, -131.098816,  -7.100001, 0.000000,  -30.0,		//Frame 3
		39.499969,  -15.600037,  59.599365, 0.000000,   30.0,		//Frame 4
		12.900015,  113.198837, -18.400040, 4.370000,  -30.0,		//Frame 5
		17.000027,  150.099960, -17.200035, 0.000000,   30.0,		//Frame 6
		 7.799997,  -39.899967, -20.900049, 0.000000,  -30.0,		//Frame 7
		17.500029, -107.298943,  25.700035,-4.370000,   30.0,		//Frame 8
		17.600029,  150.099960,  33.999905, 0.000000,  -30.0,		//Frame 9
		 0.0,	      0.0,		  0.0,	    0.0,         0.0		//Regresa a la posicion original
};


//Cargando datos para keyframes
void saveFrame(void)
{
	//Para los brazos
	for (int i = 0;i < MAX_FRAMES * 5; i += 5) {
		KeyFrame[FrameIndex].rotBrazoInfZ = frames[i];
		KeyFrame[FrameIndex].rotBrazoInfY = frames[i + 1];
		KeyFrame[FrameIndex].rotBrazoSupZ = frames[i + 2];
		KeyFrame[FrameIndex].rotBrazoSupY = frames[i + 3];

		//Para la antena
		KeyFrame[FrameIndex].rotAntena = frames[i + 4];

		FrameIndex++;
	}

}

void resetElements(void)
{
	rotBrazoInfZ = KeyFrame[0].rotBrazoInfZ;
	rotBrazoInfY = KeyFrame[0].rotBrazoInfY;

	rotBrazoSupZ = KeyFrame[0].rotBrazoSupZ;
	rotBrazoSupY = KeyFrame[0].rotBrazoSupY;

	rotAntena = KeyFrame[0].rotAntena;
}

void interpolation(void)
{
	KeyFrame[playIndex].rotIncBraInfZ = (KeyFrame[playIndex + 1].rotBrazoInfZ - KeyFrame[playIndex].rotBrazoInfZ) / i_max_steps;
	KeyFrame[playIndex].rotIncBraSupZ = (KeyFrame[playIndex + 1].rotBrazoSupZ - KeyFrame[playIndex].rotBrazoSupZ) / i_max_steps;

	KeyFrame[playIndex].rotIncBraInfY = (KeyFrame[playIndex + 1].rotBrazoInfY - KeyFrame[playIndex].rotBrazoInfY) / i_max_steps;
	KeyFrame[playIndex].rotIncBraSupY = (KeyFrame[playIndex + 1].rotBrazoSupY - KeyFrame[playIndex].rotBrazoSupY) / i_max_steps;

	KeyFrame[playIndex].rotIncAntena = (KeyFrame[playIndex + 1].rotAntena - KeyFrame[playIndex].rotAntena) / i_max_steps;

}

void animacion()
{

	//Movimiento del personaje

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Fin secuencia por keyframes\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			rotBrazoInfZ += KeyFrame[playIndex].rotIncBraInfZ;
			rotBrazoSupZ += KeyFrame[playIndex].rotIncBraSupZ;

			rotBrazoInfY += KeyFrame[playIndex].rotIncBraInfY;
			rotBrazoSupY += KeyFrame[playIndex].rotIncBraSupY;

			rotAntena += KeyFrame[playIndex].rotIncAntena;


			i_curr_steps++;
		}

	}
}



/* FIN KEYFRAMES*/
//Cargando datos para keyframes



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	//Declarando ambas camaras
	camera = Camera(glm::vec3(-60.0f, 5.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.5f, 0.5f);			//Ligada al planoXZ
	camIso = Camera(glm::vec3(-150.0f, 150.0f, 150.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, -45.0f, 0.5f, 0.5f);	//Isometrica
	
	//Original
	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	humo = Texture("Textures/humo.tga");
	humo.LoadTextureA();
	Lago = Texture("Textures/water.jpg");
	Lago.LoadTextureA();


	Pueblo = Model();
	Pueblo.LoadModel("Models/Pueblo.obj");
	Arboles = Model();
	Arboles.LoadModel("Models/Arboles.obj");
	Voltorb = Model();
	Voltorb.LoadModel("Models/Voltorb.obj");
	Electrode = Model();
	Electrode.LoadModel("Models/Electrode.obj");
	Onix = Model();
	Onix.LoadModel("Models/Onix.obj");

	//Avatar
	LucCuerpo = Model();
	LucCuerpo.LoadModel("Models/LucarioCuerpo.obj");
	LucCabeza = Model();
	LucCabeza.LoadModel("Models/LucarioCabeza.obj");
	LucCola = Model();
	LucCola.LoadModel("Models/LucarioCola.obj");
	LucBraDer = Model();
	LucBraDer.LoadModel("Models/LucarioBraDer.obj");
	LucBraIzq = Model();
	LucBraIzq.LoadModel("Models/LucarioBraIzq.obj");
	LucPierDer = Model();
	LucPierDer.LoadModel("Models/LucarioPierDer.obj");
	LucPierIzq = Model();
	LucPierIzq.LoadModel("Models/LucarioPierIzq.obj");

	Farola = Model();
	Farola.LoadModel("Models/Farola.obj");
	Poste = Model();
	Poste.LoadModel("Models/Poste.obj");


	CasaParque = Model();
	CasaParque.LoadModel("Models/CasaParque.obj");
	Fuente = Model();
	Fuente.LoadModel("Models/FuenteParque.obj");
	Dulceria = Model();
	Dulceria.LoadModel("Models/SnackArea.obj");
	AreaInf = Model();
	AreaInf.LoadModel("Models/AreaInfantil.obj");
	Columpio = Model();
	Columpio.LoadModel("Models/Columpio.obj");
	SyB = Model();
	SyB.LoadModel("Models/SubeyBaja.obj");
	Reja = Model();
	Reja.LoadModel("Models/Reja.obj");
	Entrada = Model();
	Entrada.LoadModel("Models/Entrada.obj");
	Personaje2 = Model();
	Personaje2.LoadModel("Models/Mordecai.obj");
	arbol = Model();
	arbol.LoadModel("Models/Arbol2.obj");
	arbusto = Model();
	arbusto.LoadModel("Models/Arbusto.obj");

	//-----------Frijolito---------------//
	Ring = Model();
	Ring.LoadModel("Models/Ring.obj");
	Gradas = Model();
	Gradas.LoadModel("Models/Gradas.obj");
	CasaFrijolito = Model();
	CasaFrijolito.LoadModel("Models/CasaFrijolito.obj");
	Autobus = Model();
	Autobus.LoadModel("Models/Autobus.obj");
	AutobusLlanta = Model();
	AutobusLlanta.LoadModel("Models/AutobusLlanta.obj");
	Escuela = Model();
	Escuela.LoadModel("Models/Escuela.obj");
	Estanque = Model();
	Estanque.LoadModel("Models/Lago.obj");
	Frijolito = Model();
	Frijolito.LoadModel("Models/Frijolito.obj");
	FrijolitoPiernaDer = Model();
	FrijolitoPiernaDer.LoadModel("Models/FrijolitoPiernaDer.obj");
	FrijolitoPiernaIzq = Model();
	FrijolitoPiernaIzq.LoadModel("Models/FrijolitoPiernaIzq.obj");
	FrijolitoBrazoDer = Model();
	FrijolitoBrazoDer.LoadModel("Models/FrijolitoBrazoDer.obj");
	FrijolitoBrazoIzq = Model();
	FrijolitoBrazoIzq.LoadModel("Models/FrijolitoBrazoIzq.obj");
	AntenaBrazoInf = Model();
	AntenaBrazoInf.LoadModel("Models/brazoInferior.obj");
	AntenaBrazoSup = Model();
	AntenaBrazoSup.LoadModel("Models/brazoSuperior.obj");
	Antena = Model();
	Antena.LoadModel("Models/antena.obj");



	std::vector<std::string> skyboxFacesDia;

	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_rt.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_lf.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_dn.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_up.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_bk.tga");
	skyboxFacesDia.push_back("Textures/Skybox/Day-Skybox_ft.tga");
	

	std::vector<std::string> skyboxFacesNoche;

	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_rt.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_lf.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_dn.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_up.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_bk.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/Night-Skybox_ft.tga");

	skyboxDia = Skybox(skyboxFacesDia);
	skyboxNoche = Skybox(skyboxFacesNoche);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.55f, 0.62f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		2.5f, 3.3f,
		40.0f, 15.0f, -50.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		2.5f, 3.3f,
		40.0f, 15.0f, 40.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f,
		2.5f, 3.3f,
		-35.0f, 15.0f, 40.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		30.0f, 20.0f, -88.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		-48.0f, 20.0f, -20.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		85.0f, 20.0f, -7.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Inicialización de Variables de Animación
	movVol = 0.0;
	movVolOffset = 0.1;
	rotVol = 0;
	rotVolOffset = 5;
	arriba = false;
	movOnix = 90.0f;
	movOnixOffset = 0.5f;
	rotOnix = 0;
	rotOnixOffset = 10;
	avanzaOnix = false;
	movXLuc = 0.0;
	movXLucOffset = 0.1;
	movZLuc = 0.0;
	movZLucOffset = 0.1;
	rotCuerLuc = 0;
	rotLuc = 0;
	rotLucOffset = 1;
	avanza = false;
	gira = false;
	

	rotColumpio = 0.0f;
	rotColumpioOffset = 0.2f;
	BanColumpio = BanSyB = true;
	rotSyB = 0.0f;
	rotSyBOffset = 0.23f;
	incRot = 10.0f;
	
	// Obtención del tiempo actual
	auto tiempo_anterior = std::chrono::steady_clock::now();

	//Cargando Keyframes
	saveFrame();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		animacion();

		//Obtiene el tiempo Stranscurrido actual
		auto tiempo_actual = std::chrono::steady_clock::now();

		//Obtiene la diferencia de tiempo
		auto diferencia = std::chrono::duration_cast<std::chrono::milliseconds>
			(tiempo_actual - tiempo_anterior).count();

		//Verifica que pasen 60 segundos y cambia
		if (diferencia >= 60000) {
			dia = !dia;
			tiempo_anterior = tiempo_actual;
		}
		
		//Audio bandera sirve para llamar solo una vez la funcion Play
		if (!audioBandera && mainWindow.getSoundtrack() == true) {
			mySpeaker.Play(soundFrijolito, 1, soundAmbiental);		// Parametro 1 inicia o continua la musica K
			audioBandera = true;
		}

		if (mainWindow.getSoundtrack() == false) {
			mySpeaker.Play(soundFrijolito, 2,soundAmbiental);		//Parametro 2 pausa L
			audioBandera = false;
		}


		//Animación Voltorb y Electrode
		if (movVol < 10.0f && arriba == false && mainWindow.getBanOnAnim() == true) {
			movVol += movVolOffset * deltaTime;
			rotVol += rotVolOffset * deltaTime;
			if (movVol < 10.5f && movVol > 9.5f)
				arriba = true;
		}
		else if (movVol > 0.0f && arriba == true && mainWindow.getBanOnAnim() == true) {
			movVol -= movVolOffset * deltaTime;
			rotVol -= rotVolOffset * deltaTime;
			if (movVol < 0.5f && movVol > -0.5f)
				arriba = false;
		}

		//Animacion Onix
		if (movOnix > -90.5f && avanzaOnix == false && mainWindow.getBanOnAnim() == true) {
			movOnix -= movOnixOffset * deltaTime;
			rotOnix += rotOnixOffset * deltaTime;
			if (movOnix > -91.0f && movOnix < -90.0f) {
				avanzaOnix = true;
			}
		}
		else if (movOnix < 90.5f && avanzaOnix == true && mainWindow.getBanOnAnim() == true) {
			movOnix += movOnixOffset * deltaTime;
			rotOnix -= rotVolOffset * deltaTime;
			if (movOnix < 91.0f && movOnix > 90.0f)
				avanzaOnix = false;
		}

		//Animación Avatar
		if (rotLuc < 21 && gira == false && mainWindow.getBanOnAnim() == true) {
			rotLuc += rotLucOffset ;
			//std::cout << rotLuc << std::endl;
			if (rotLuc < 23 && rotLuc > 19) {
				gira = true;
			}
		}
		else if (rotLuc > -21 && gira == true && mainWindow.getBanOnAnim() == true) {
			rotLuc -= rotLucOffset ;
			//std::cout << rotLuc << std::endl;
			if (rotLuc < -19 && rotLuc > -23) {
				gira = false;
			}
		}


		if (movXLuc < 100.5f && avanza == false && mainWindow.getBanOnAnim() == true) {
			movXLuc += movXLucOffset * deltaTime;
			if (movXLuc < 101.0f && movXLuc > 100.0f ) {
				rotCuerLuc = 1;
				avanza = true;
			}
		}
		else if (movZLuc < 120.5f && rotCuerLuc == 1 && mainWindow.getBanOnAnim() == true) {
			movZLuc += movVolOffset * deltaTime;
			if (movZLuc < 121.0f && movZLuc > 120.0f) {
				rotCuerLuc = 2;
			}
		}
		else if (movXLuc > 0.0f && rotCuerLuc == 2 && mainWindow.getBanOnAnim() == true) {
			movXLuc -= movXLucOffset * deltaTime;
			if (movXLuc < 0.5f && movXLuc > -0.5f) {
				rotCuerLuc = 3;
			}
		}
		else if (movZLuc > 0.0f && rotCuerLuc == 3 && mainWindow.getBanOnAnim() == true) {
			movZLuc -= movVolOffset * deltaTime;
			if (movZLuc < 0.5f && movZLuc > -0.5f) {
				rotCuerLuc = 0;
				avanza = false;
			}
		}

		// ANIMACION SIMPLE: Columpio
		if (mainWindow.getBanOnAnim()) { // Rota
			if (rotColumpio < incRot && BanColumpio == true)
				rotColumpio += rotColumpioOffset * deltaTime;
			else if (rotColumpio > -incRot && BanColumpio == false)
				rotColumpio -= rotColumpioOffset * deltaTime;
			else {
				BanColumpio = !BanColumpio;
				if (incRot < 60.0f)
					incRot += 5.0f;
			}
		}
		else { // Se detiene y regresa al punto incial
			if (rotColumpio < -0.1f) {
				rotColumpio += rotColumpioOffset * deltaTime;
			}
			else if (rotColumpio > 0.1f) {
				rotColumpio -= rotColumpioOffset * deltaTime;
			}
			incRot = 0.0f;
		}

		// ANIMACION SIMPLE: Sube y Baja
		if (mainWindow.getBanOnAnim()) { // Rota
			if (rotSyB < 26.0f && BanSyB == true)
				rotSyB += rotSyBOffset * deltaTime;
			else if (rotSyB > 0.0f && BanSyB == false)
				rotSyB -= rotSyBOffset * deltaTime;
			else {
				BanSyB = !BanSyB;
			}
		}
		else { // Se detiene y regresa al punto incial
			if (rotSyB < -0.1f) {
				rotSyB += rotColumpioOffset * deltaTime;
			}
			else if (rotColumpio > 0.1f) {
				rotSyB -= rotColumpioOffset * deltaTime;
			}
		}

		//-----------------------------------//
		//--------Animacion autobus----------//
		//-----------------------------------//
		if (mainWindow.getCircuito())
		{		
			if (recorrido1)
			{
				movKitZ += movKitZOffset * deltaTime;
				if (movKitZ > 200)		//
				{
					recorrido1 = false;
					recorrido2 = true;
				}
			}
			if (recorrido2)
			{
				rotKit = -90;
				movKitX -= movKitXOffset * deltaTime;
				if (movKitX < -180)		//
				{
					recorrido2 = false;
					recorrido3 = true;

				}
			}

			if (recorrido3)
			{
				rotKit = 180;
				movKitZ -= movKitZOffset * deltaTime;
				if (movKitZ < 0)		//
				{
					recorrido3 = false;
					recorrido4 = true;
				}
			}

			if (recorrido4)
			{
				rotKit = 90;
				movKitX += movKitXOffset * deltaTime;
				if (movKitX > 0)		//
				{
					recorrido4 = false;
					recorrido5 = true;

				}
			}
			if (recorrido5)
			{
				rotKit = 0;
				movKitZ += movKitZOffset * deltaTime;
				if (movKitZ > 200)		//
				{
					recorrido5 = false;
					recorrido1 = true;
				}
			}
		}


		//Animacion Frijolito rotacion
		if (mainWindow.getSoundtrack()) {
			angleFri += angleFriOffset * deltaTime;
			movFriX = radius * cos(angleFri);
			movFriZ = radius * sin(angleFri);
		}
		

		//Animacion piernas frijolito
		if (mainWindow.getSoundtrack()) {
			if (movFriExtremidad < 16.0f && BanFrijolito == true)
				movFriExtremidad += movFriExtremidadOffset * deltaTime;

			else if (movFriExtremidad > -16.0f && BanFrijolito == false)
				movFriExtremidad -= movFriExtremidadOffset * deltaTime;

			else 
				BanFrijolito = !BanFrijolito;
		}
		else { 
			if (movFriExtremidad < -0.1f)
				movFriExtremidad += movFriExtremidadOffset * deltaTime;

			else if (movFriExtremidad > 0.1f)
				movFriExtremidad -= movFriExtremidadOffset * deltaTime;
		}
		

		//Animacion por keyfames ANTENA
		if (!banAntena && mainWindow.getAnimKeyFrames() == true)
		{
			
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();

				play = true;
				playIndex = 0;
				i_curr_steps = 0;
			}
			else
			{
				play = false;
			}

			banAntena = true;
		}
		if (mainWindow.getAnimKeyFrames() == false) {
			banAntena = false;
		}

		//Recibir eventos del usuario

		//---------------------------------//
		//-----------CAMERAS---------------//
		//---------------------------------//

		glfwPollEvents();
		if (mainWindow.getCameraInfo()) {
			camIso.keyControl(mainWindow.getsKeys(), deltaTime);
			camIso.mouseControl(0.0f, 0.0f);
		}
		else {
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), 0.0f);
		}
		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Cambio entre día y noche
		if (dia) {
			skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
			mainLight.SetInten(0.55f, 0.62f);
			pointLightCount = 0;
		}
		else {
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
			mainLight.SetInten(0.2f, 0.2f);
			pointLightCount = 3;
		}

		if (mainWindow.getBanluz()) {
			spotLightCount = 3;
		}
		else {
			spotLightCount = 0;
		}
			

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//---------------------------------//
		//-----------CAMERAS---------------//
		//---------------------------------//

		if (mainWindow.getCameraInfo()) {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camIso.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camIso.getCameraPosition().x, camIso.getCameraPosition().y, camIso.getCameraPosition().z);
		}
		else {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		}		
		


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelLuc(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelSyB(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);//Se declara al inicio para que las texturas no se muevan

		//-------------Modelos-----------//
		
		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(16.0f, 1.0f, 16.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		//Pueblo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pueblo.RenderModel();

		//********Arboles Externos********

		//Arboles Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-128.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles Enfrente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 140.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles Atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, -165.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//*********Modelos de Iluminación***********

		//Farola Atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 0.1f, -100.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.1f, -20.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.1f, -7.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Poste Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste.RenderModel();

		//Poste Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste.RenderModel();

		//Poste Atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -50.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste.RenderModel();

		//Voltorb
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, movVol, 40.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, rotVol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Voltorb.RenderModel();

		//Electrode
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, movVol, 40.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180+rotVol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Electrode.RenderModel();

		//Onix
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movOnix, 3.0f, 105.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		if(avanzaOnix == false)
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		else
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotOnix * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Onix.RenderModel();

		
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f+movXLuc, 4.0f, 50.0f-movZLuc));
		if(rotCuerLuc == 0)
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		else if(rotCuerLuc == 1)
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		else if (rotCuerLuc == 2)
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		else if (rotCuerLuc == 3)
			model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelLuc = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucCuerpo.RenderModel();

		//Cabeza
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, -rotLuc * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucCabeza.RenderModel();

		//Cola
		model = glm::mat4(1.0);
		model = modelLuc;
		model = glm::rotate(model, rotLuc * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucCola.RenderModel();

		//Brazo Derecho
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, rotLuc * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucBraDer.RenderModel();

		//Brazo Izquierdo
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, -rotLuc * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucBraIzq.RenderModel();

		//Pierna Derecha
		model = glm::mat4(1.0);
		model = modelLuc;
		model = glm::rotate(model, rotLuc * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucPierDer.RenderModel();

		//Pierna Izquierda
		model = glm::mat4(1.0);
		model = modelLuc;
		model = glm::rotate(model, -rotLuc * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucPierIzq.RenderModel();

		//##########################//
		//####	  Mordecai      ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 25.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Personaje2.RenderModel();

		//##########################//
		//#### Casa del Parque  ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaParque.RenderModel();

		//##########################//
		//#### Fuente del Parque####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(-15.0f, -0.2f, 15.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fuente.RenderModel();

		//##########################//
		//####     Dulcería	    ####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dulceria.RenderModel();

		//##########################//
		//#### Area Infantil    ####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		modelSyB = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AreaInf.RenderModel();

		//Sube y Baja 1
		model = modelSyB;
		model = glm::translate(model, glm::vec3(-0.715f, 0.633f, 2.798f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, rotSyB * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SyB.RenderModel();

		//Sube y Baja 2
		model = modelSyB;
		model = glm::translate(model, glm::vec3(-0.697, 0.633f, 4.172f));
		model = glm::rotate(model, (-rotSyB + 26.0f) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SyB.RenderModel();

		//Columpios 1
		model = modelSyB;
		model = glm::translate(model, glm::vec3(0.6f, 3.442f, -0.919f));
		model = glm::rotate(model, rotColumpio * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Columpio.RenderModel();

		//Columpios 2
		model = modelSyB;
		model = glm::translate(model, glm::vec3(-1.302f, 3.442f, -0.919f));
		model = glm::rotate(model, -rotColumpio * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Columpio.RenderModel();

		//##########################//
		//#### Rejas del parque ####//
		//##########################// Cada barra mide 0.188f
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 45.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f + 0.376f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		//model = glm::rotate(model, 90* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Entrada.RenderModel();

		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f + 0.376f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		//######################//
		//#### Flora		####//
		//######################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-33.398f, 0.0f, 8.775f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-34.398f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-33.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -35.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 17 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -55.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 17 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(23.0f, 0.0f, -8.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 0.0f, 8.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.398f, 0.0f, 35.775f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 74 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.398f, 0.0f, 35.775f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.398f, 0.0f, 40.775f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 43 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -45.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, -46.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 74 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-18.0f, 0.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 23 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-33.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 0.0f, 38.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 74 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();


		//##########################//
		//#### Frijolito modelos  ####//
		//##########################//

		//Escuela
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escuela.RenderModel();

		//Casa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(125.0f, 0.0f, 75.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaFrijolito.RenderModel();

		//Ring
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 85.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ring.RenderModel();
		
		//Gradas
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -40.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gradas.RenderModel();
		model = glm::translate(model, glm::vec3(45.0f, 0.0f, 40.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gradas.RenderModel();

		//Autobus
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, -0.8, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		//model = glm::translate(model, glm::vec3(70.0f, 2.16f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Autobus.RenderModel();

		//Estanque
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(100.0f, 0.1f, 90.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Estanque.RenderModel();

		//Frijolito
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-80.0f + movFriX, 4.35f, 85.0f + movFriZ));
		model = glm::rotate(model, glm::radians(-angleFri*60), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Frijolito.RenderModel();

		//Piernas Frijolito
		model = glm::translate(model, glm::vec3(0.7f , 2.327f, 0.0f));
		model = glm::rotate(model, glm::radians(movFriExtremidad), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoPiernaIzq.RenderModel();

		model = glm::rotate(model, glm::radians(-movFriExtremidad), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.4f , 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-movFriExtremidad), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoPiernaDer.RenderModel();
	
		//Brazos Frijolito
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-80.0f + movFriX, 4.2f, 85.0f + movFriZ));
		model = glm::rotate(model, glm::radians(-angleFri * 60), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.6f, 3.96f, 0.0f));
		model = glm::rotate(model, glm::radians(-movFriExtremidad), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoBrazoIzq.RenderModel();

		model = glm::rotate(model, glm::radians(movFriExtremidad), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.3f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movFriExtremidad), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoBrazoDer.RenderModel();

		//Antena
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-62.5f, 24.1f, 37.0f));
		model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, glm::radians(rotBrazoInfY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBrazoInfZ-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AntenaBrazoInf.RenderModel();

		model = glm::translate(model, glm::vec3(-0.516f, 0.674f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBrazoSupY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBrazoSupZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AntenaBrazoSup.RenderModel();

		model = glm::translate(model, glm::vec3(-0.429f, 0.269f, 0.0f));
		model = glm::rotate(model, glm::radians(rotAntena), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Antena.RenderModel();


		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);//Para indicar trasparencia y traslucidez
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Va antes de la textura

		//Para texturas con movimiento
		toffsetu += 0.0005 * deltaTime;
		toffsetv += 0.0003 * deltaTime;
		if (toffsetu > 1.0)
			toffsetu = 0.0;
		if (toffsetv > 1.0) {
			toffsetv -= 0.0003 * deltaTime;
			if (toffsetv < 0.0)
				toffsetv += 0.0003 * deltaTime;
		}
		toffset = glm::vec2(toffsetu, toffsetv);

		//------Textura con movimiento del humo------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.972f, 28.592f, 3.27f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		humo.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-24.772f, 31.392f, 3.27f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		humo.UseTexture();
		meshList[4]->RenderMesh();

		//----- Textura con movimiento del Lago  -------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.2f, 90.0f));
		model = glm::scale(model, glm::vec3(19.0f, 15.0f, 8.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lago.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		
		glDisable(GL_BLEND);//Desactiva el blender

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
