/*
Semestre 2023-2
Nombres: 
Arroyo Quiroz José Miguel
Hernández Hernández Cristian
Proyecto Final: Diorama de la Vida Cotidiana (Lucario y Mordecai)
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
glm::vec3 PosIni(70.0f, 2.16f, -55.0f);

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

//Banderas
int bandia = 0;
/*unsigned t0, t1;
double time;*/

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camIso;

Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
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


//Frijolito
Model Ring;
Model Gradas;
Model CasaFrijolito;
Model Autobus;
Model AutobusLlanta;
Model Escuela;
Model Estanque;

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
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
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

	//-----------Frijolito---------------//
	Ring = Model();
	Ring.LoadModel("Models/Ring.obj");
	Gradas = Model();
	Gradas.LoadModel("Models/Gradas.obj");
	CasaFrijolito = Model();
	CasaFrijolito.LoadModel("Models/Casa/CasaFrijolito.obj");
	Autobus = Model();
	Autobus.LoadModel("Models/Autobus.obj");
	AutobusLlanta = Model();
	AutobusLlanta.LoadModel("Models/AutobusLlanta.obj");
	Escuela = Model();
	Escuela.LoadModel("Models/Escuela.obj");
	Estanque = Model();
	Estanque.LoadModel("Models/Lago.obj");
	



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
		-40.0f, 15.0f, -40.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		2.5f, 3.3f,
		40.0f, 15.0f, 40.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f,
		2.5f, 3.3f,
		-40.0f, 15.0f, 40.0f,
		1.0f, 0.5f, 0.0f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		0.0f, 20.0f, -68.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		-48.0f, 20.0f, -10.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		70.0f, 20.0f, -2.0f,
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
	rotLucOffset = 2;
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

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
			//rotOnix += rotOnixOffset * deltaTime;
			if (movOnix > -91.0f && movOnix < -90.0f) {
				avanzaOnix = true;
			}
		}
		else if (movOnix < 90.5f && avanzaOnix == true && mainWindow.getBanOnAnim() == true) {
			movOnix += movOnixOffset * deltaTime;
			//rotOnix -= rotVolOffset * deltaTime;
			if (movOnix < 91.0f && movOnix > 90.0f)
				avanzaOnix = false;
		}

		//Animación Avatar
		if (rotLuc < 20 && gira == false && mainWindow.getBanOnAnim() == true) {
			rotLuc += rotLucOffset * deltaTime;
			if (rotLuc < 21 && rotLuc > 19) {
				gira = true;
			}
		}
		else if (rotLuc > -20 && gira == true && mainWindow.getBanOnAnim() == true) {
			rotLuc -= rotLucOffset * deltaTime;
			if (rotLuc < -19 && rotLuc > -21) {
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

		//-----------------------------------//
		//--------Animacion autobus----------//
		//-----------------------------------//
		if (mainWindow.getCircuito())
		{		
			if (recorrido1)
			{
				movKitZ += movKitZOffset * deltaTime;
				if (movKitZ > 85)
				{
					recorrido1 = false;
					recorrido2 = true;
				}
			}
			if (recorrido2)
			{
				rotKit = -90;
				movKitX -= movKitXOffset * deltaTime;
				if (movKitX < -98)
				{
					recorrido2 = false;
					recorrido3 = true;

				}
			}

			if (recorrido3)
			{
				rotKit = 180;
				movKitZ -= movKitZOffset * deltaTime;
				if (movKitZ < 0)
				{
					recorrido3 = false;
					recorrido4 = true;
				}
			}

			if (recorrido4)
			{
				rotKit = 90;
				movKitX += movKitXOffset * deltaTime;
				if (movKitX > 0)
				{
					recorrido4 = false;
					recorrido5 = true;

				}
			}
			if (recorrido5)
			{
				rotKit = 0;
				movKitZ += movKitZOffset * deltaTime;
				if (movKitZ > 85)
				{
					recorrido5 = false;
					recorrido1 = true;
				}
			}
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
		
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		//skyboxDia.DrawSkybox(camIso.calculateViewMatrix(), projection);

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
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 14.0f));
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

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 115.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, -140.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Farola1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -80.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.1f, -10.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Farola3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 0.1f, -2.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		//Poste1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste.RenderModel();

		//Poste2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste.RenderModel();

		//Poste3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -50.0f));
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
		model = glm::translate(model, glm::vec3(movOnix, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotOnix * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Onix.RenderModel();

		
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f+movXLuc, 0.0f, 50.0f-movZLuc));
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
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucBraDer.RenderModel();

		//Brazo Izquierdo
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucBraIzq.RenderModel();

		//Pierna Derecha
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucPierDer.RenderModel();

		//Pierna Izquierda
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucPierIzq.RenderModel();


		//##########################//
		//#### Frijolito modelos  ####//
		//##########################//

		//Escuela
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Escuela.RenderModel();

		//Casa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(120.0f, 0.0f, 75.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaFrijolito.RenderModel();

		//Ring
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-85.0f, 0.0f, 85.0f));
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
		//model = glm::translate(model, glm::vec3(70.0f, 2.16f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Autobus.RenderModel();

			//--------Llantas--------//
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(model, glm::vec3(5.4f, 0.0f, 24.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AutobusLlanta.RenderModel();

		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(model, glm::vec3(5.4f, 0.0f, -15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AutobusLlanta.RenderModel();

		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(model, glm::vec3(-5.4f, 0.0f, -15.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AutobusLlanta.RenderModel();

		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(model, glm::vec3(-5.4f, 0.0f, 24.8f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AutobusLlanta.RenderModel();

		//Estanque
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(-80.0f, 0.5f, 80.0f));
		model = glm::translate(model, glm::vec3(95.0f, 0.1f, 90.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Estanque.RenderModel();




		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);//Para indicar trasparencia y traslucidez
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Va antes de la textura

		//textura con movimiento del humo
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
		model = glm::translate(model, glm::vec3(95.0f, 0.2f, 90.0f));
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
