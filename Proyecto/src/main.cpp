#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"
#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
Shader shaderParticlesFire;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);
Box boxIntro;
Box boxViewDepth;

// Models
Model Pino;
Model Muro;
Model Hogera;
Model Casa;
Model Ventanas;

// Modelos animados
// Mayow
//Model mayowModelAnimate;
Model mainModelAnimate;
Model main1ModelAnimate;
Model main2ModelAnimate;
Model zombieModelAnimate;
Model zombie1ModelAnimate;

// Fountain
//Model modelFountain;
// Terrain model instance
Terrain terrain(-1, -1, 200, 5, "../Textures/Mapas_de_alturas/heightmap.png");

ShadowBox * shadowBox;

GLuint textureCespedID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureInit1ID,textureInit2ID,textureInit3ID,textureInit4ID,textureActivaID,textureScreen1ID,textureScreen2ID,textureScreen3ID;
GLuint textureParticleFountainID,textureParticleFireID,texId;

bool iniciaPartida = false, presionarOpcion = false, presionarEnter = false;; 

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/Skybox_bosque1/posx.tga",
		"../Textures/Skybox_bosque1/negx.tga",
		"../Textures/Skybox_bosque1/posy.tga",
		"../Textures/Skybox_bosque1/negy.tga",
		"../Textures/Skybox_bosque1/posz.tga",
		"../Textures/Skybox_bosque1/negz.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
//glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 modelMatrixMain = glm::mat4(1.0f);
glm::mat4 modelMatrixMain1 = glm::mat4(1.0f);
glm::mat4 modelMatrixMain2 = glm::mat4(1.0f);
glm::mat4 modelMatrixZombie = glm::mat4(1.0f);
glm::mat4 modelMatrixZombie1 = glm::mat4(1.0f);
//glm::mat4 modelMatrixFountain= glm::mat4(1.0f);

//Model matrix ecenario
glm::mat4 modelMatrixPino = glm::mat4(1.0f);
glm::mat4 modelMatrixCasa = glm::mat4(1.0f);
glm::mat4 modelMatrixVentanas = glm::mat4(1.0f);
glm::mat4 modelMatrixMuro = glm::mat4(1.0f);
glm::mat4 modelMatrixHogera = glm::mat4(1.0f);
//glm::mat4 modelMatrix = glm::mat4(1.0f);

//variables de animacion modelos
//int animationMayowIndex = 1;
int animationMainIndex = 1;
int animationMain1Index = 1;
int animationMain2Index = 2;
int animationZombieIndex = 1;
int animationZombie1Index = 1;
int modelSelected = 0,mapa = 0;
bool enableCountSelected = true;

//coordenadas de los arboles primera midad
std::vector<glm::vec3> PinoPos1 = {
	glm::vec3(-94.5,0,7.0),glm::vec3(-94.1,0,19.1),glm::vec3(-96.1,0,36.7),
	glm::vec3(-73.8,0,28.9),glm::vec3(-69.1,0,8.2),glm::vec3(-59.0,0,7.8),
	glm::vec3(-30.5,0,7.0),glm::vec3(-17.6,0,6.3),glm::vec3(-6.6,0,9.8),
	glm::vec3(-20.7,0,24.6),glm::vec3(-12.9,0,25.8),glm::vec3(-3.1,0,26.2),
	glm::vec3(14.8,0,18.0),glm::vec3(20.7,0,9.0),glm::vec3(27.7,0,10.9),
	glm::vec3(47.7,0,16.8),glm::vec3(-23.4,0,46.9),glm::vec3(-73.8,0,51.2),
	glm::vec3(-31.3,0,50.8),glm::vec3(-87.1,0,52.0),glm::vec3(-94.9,0,72.3),
	glm::vec3(-51.6,0,60.2),glm::vec3(-90.1,0,75.0),glm::vec3(-84.0,0,68.4),
	glm::vec3(-94.1,0,95.7),glm::vec3(-84.8,0,88.3),glm::vec3(-73.4,0,94.5),
	glm::vec3(-42.2,0,95.7),glm::vec3(-39.5,0,86.3),glm::vec3(-44.9,0,81.6),
	glm::vec3(-19.9,0,91.8),glm::vec3(-14.5,0,96.1),glm::vec3(-6.3,0,95.7),
	glm::vec3(-2.0,0,65.2),glm::vec3(-11.3,0,60.5),glm::vec3(-0.4,0,56.3),
	glm::vec3(18.4,0,94.1),glm::vec3(25.4,0,60.9),glm::vec3(17.6,0,71.1),
	glm::vec3(46.9,0,92.6),glm::vec3(42.2,0,83.2),glm::vec3(47.3,0,73.0),
	glm::vec3(57.8,0,84.0),glm::vec3(61.3,0,94.1),glm::vec3(66.0,0,86.3),
	glm::vec3(94.1,0,90.6),glm::vec3(93.8,0,80.9),glm::vec3(93.8,0,68.4),
	glm::vec3(73.0,0,63.7),glm::vec3(75.4,0,56.6)
};
//coordenadas segunda mitad del mapa
std::vector<glm::vec3> PinoPos2 = {
	glm::vec3(-93.8,0,-92.578125),glm::vec3(-94.5,0,-57.421875),glm::vec3(-91.4,0,-67.1875),
	glm::vec3(-70.7,0,-82.8125),glm::vec3(-58.2,0,-84.765625),glm::vec3(-53.5,0,-94.53125),
	glm::vec3(-31.6,0,-90.234375),glm::vec3(-25.0,0,-67.96875),glm::vec3(-24.2,0,-80.078125),
	glm::vec3(-7.8,0,-86.328125),glm::vec3(0.0,0,-59.375),glm::vec3(-2.0,0,-75.78125),
	glm::vec3(9.8,0,-62.5),glm::vec3(10.5,0,-76.5625),glm::vec3(19.5,0,-87.5),
	glm::vec3(23.4,0,-43.75),glm::vec3(29.7,0,-39.0625),glm::vec3(33.2,0,-94.53125),
	glm::vec3(67.2,0,-92.578125),glm::vec3(57.4,0,-81.25),glm::vec3(46.9,0,-76.5625),
	glm::vec3(63.3,0,-66.796875),glm::vec3(48.4,0,-57.03125),glm::vec3(56.3,0,-59.765625),
	glm::vec3(56.6,0,-46.09375),glm::vec3(65.2,0,-44.921875),glm::vec3(87.1,0,-95.3125),
	glm::vec3(92.6,0,-65.234375),glm::vec3(-95.3,0,-43.359375),glm::vec3(-89.1,0,-27.34375),
	glm::vec3(-64.1,0,-25.78125),glm::vec3(-56.3,0,-16.796875),glm::vec3(-49.2,0,-38.671875),
	glm::vec3(-37.9,0,-37.5),glm::vec3(-30.1,0,-11.71875),glm::vec3(-23.8,0,-22.65625),
	glm::vec3(-5.5,0,-12.890625),glm::vec3(3.5,0,-22.265625),glm::vec3(5.5,0,-9.375),
	glm::vec3(43.8,0,-12.109375),glm::vec3(52.3,0,-10.9375),glm::vec3(57.8,0,-18.75),
	glm::vec3(71.1,0,-11.328125),glm::vec3(79.3,0,-15.234375),glm::vec3(76.2,0,-24.21875),
	glm::vec3(93.8,0,-19.53125),glm::vec3(96.5,0,-27.734375)
};
//coordenadas de las hogeras y las luces
std::vector<glm::vec3> Hogeras = {
	glm::vec3(-82.4,0,78.1),
	glm::vec3(-73.4,0,23.4),
	glm::vec3(9.0,0,72.3),
	glm::vec3(-5.5,0,16.0),
	glm::vec3(79.3,0,52.3),
	glm::vec3(-73.4,0,-19.9),
	glm::vec3(-34.0,0,-82.8),
	glm::vec3(10.2,0,-52.7),
	glm::vec3(50.0,0,-18.8),
	glm::vec3(64.5,0,-59.4)
};

std::vector<glm::vec3> Muros = {
	glm::vec3(0.0,0.0,-1.5),glm::vec3(20.0,0.0,-1.5),glm::vec3(40.0,0.0,-1.5),glm::vec3(60.0,0.0,-1.5),glm::vec3(80.0,0.0,-1.5),
	glm::vec3(100.0,0.0,-1.5),glm::vec3(-20.0,0.0,-1.5),glm::vec3(-40.0,0.0,-1.5),glm::vec3(-60.0,0.0,-1.5),glm::vec3(-80.0,0.0,-1.5)
	,glm::vec3(-100.0,0.0,-1.5)
};

std::vector<glm::vec3> Casas = {
	glm::vec3(32.8125,0,36.71875),
	glm::vec3(-73.828125,0,-51.171875)
};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"Casa", Casas[mapa]},//glm::vec3(32.8125,0,36.71875)}
		{"Hogera1", Hogeras[0]},
		{"Hogera2", Hogeras[1]},
		{"Hogera3", Hogeras[2]},
		{"Hogera4", Hogeras[3]},
		{"Hogera5", Hogeras[4]},
		{"Hogera6", Hogeras[5]},
		{"Hogera7", Hogeras[6]},
		{"Hogera8", Hogeras[7]},
		{"Hogera9", Hogeras[8]},
		{"Hogera10", Hogeras[9]}
};

double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// OpenAL Defines
#define NUM_BUFFERS 4
#define NUM_SOURCES 4
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0 ambiente bosque
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1 grito zombie
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2 sonido zombie
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3 sonido pisadas
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Source 4 sonido fogata
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, false, false, true, true};

// Framesbuffers
GLuint depthMap, depthMapFBO;

//definicion de variables para las particulas
//GLuint initVel, startTime;
//GLuint VAOParticles;
//FVAOParticlesFire/GLuint nParticles = 3000;
//double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.

void initParticleBuffers(){

	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for(unsigned int i = 0; i < nParticlesFire; i++ ) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine {};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", {"Position", "Velocity", "Age"});
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);
	
	//Pino
	Pino.loadModel("../models/arboles/arbolLowPoly/CartoonTree.obj");
	Pino.setShader(&shaderMulLighting);
	//muro
	Muro.loadModel("../models/Pared/CaveWalls3_C.obj");
	Muro.setShader(&shaderMulLighting);
	//hogera
	Hogera.loadModel("../models/Hogera/Hogera.obj");
	Hogera.setShader(&shaderMulLighting);
	//casa abandonada
	Casa.loadModel("../models/Casa_avandonada/CasaAban.obj");
	Casa.setShader(&shaderMulLighting);
	Ventanas.loadModel("../models/Casa_avandonada/CasaAbanVentanas.obj");
	Ventanas.setShader(&shaderMulLighting);

	// Mayow
/* 	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting); */
	//Main
	mainModelAnimate.loadModel("../models/Estudiante/1.fbx");
	mainModelAnimate.setShader(&shaderMulLighting);
	//Main1
	main1ModelAnimate.loadModel("../models/Guarda/1.fbx");
	main1ModelAnimate.setShader(&shaderMulLighting);
	//Main1
	main2ModelAnimate.loadModel("../models/Explorador/1.fbx");
	main2ModelAnimate.setShader(&shaderMulLighting);
	//Zombie
	zombieModelAnimate.loadModel("../models/zombie/1.fbx");
	zombieModelAnimate.setShader(&shaderMulLighting);
	//Zombie1
	zombie1ModelAnimate.loadModel("../models/zombie 1/1.fbx");
	zombie1ModelAnimate.setShader(&shaderMulLighting);

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
//	int imageWidth, imageHeight;
//	FIBITMAP *bitmap;
//	unsigned char *data;
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/Texturas_bosque/suelo/SueloBosque.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureG("../Textures/Texturas_bosque/suelo/Piedra.png");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/Texturas_bosque/suelo/Borde.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/Mapas_de_alturas/blendMap.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro1("../Textures/Intro.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
		textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/Personaje1.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
		textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro3("../Textures/Personaje2.png");
	textureIntro3.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit3ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit3ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro3.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro3.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro3.getWidth(), textureIntro3.getHeight(), 0,
		textureIntro3.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro3.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro3.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro4("../Textures/Personaje3.png");
	textureIntro4.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit4ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit4ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro4.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro4.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro4.getWidth(), textureIntro4.getHeight(), 0,
		textureIntro4.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro4.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro4.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen1("../Textures/Vida3.png");
	textureScreen1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen1.getWidth(), textureScreen1.getHeight(), 0,
		textureScreen1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen2("../Textures/Vida2.png");
	textureScreen2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen2.getWidth(), textureScreen2.getHeight(), 0,
		textureScreen2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen2.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen3("../Textures/Vida1.png");
	textureScreen3.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen3ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen3ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen3.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen3.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen3.getWidth(), textureScreen3.getHeight(), 0,
		textureScreen3.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen3.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen3.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureFire("../Textures/fuego2.png");
	textureFire.loadImage(); // Cargar la textura
	glGenTextures(1, &textureParticleFireID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureFire.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureFire.getChannels() == 3 ? GL_RGB : GL_RGBA, textureFire.getWidth(), textureFire.getHeight(), 0,
		textureFire.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureFire.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureFire.freeImage(); // Liberamos memoria

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for( int i = 0; i < randData.size(); i++ ) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f,0.1f,0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0,1,0);
	n = glm::cross(glm::vec3(1,0,0), v);
	if( glm::length(n) < 0.00001f ) {
		n = glm::cross(glm::vec3(0,1,0), v);
	}
	u = glm::cross(v,n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fireplace.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/night-forest.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/sound-of-walking.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/zombie.wav");
	buffer[4] = alutCreateBufferFromFile("../sounds/zombie-screaming.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}

	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 0.5f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[1]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 0.0);
 
 	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.5f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.5f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 1000);

	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 0.3f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 2000);

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFire.destroy();
	shaderParticlesFountain.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	rayModel.destroy();
	boxIntro.destroy();
	boxViewDepth.destroy();

	// Custom objects Delete
	//mayowModelAnimate.destroy();
	mainModelAnimate.destroy();
	main1ModelAnimate.destroy();
	main2ModelAnimate.destroy();
	zombieModelAnimate.destroy();
	zombie1ModelAnimate.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureScreen1ID);
	glDeleteTextures(1, &textureScreen2ID);
	glDeleteTextures(1, &textureScreen3ID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	//maquina de estados para inicio del juego
	if(!iniciaPartida){
		if (!presionarEnter && glfwGetKey(window, GLFW_KEY_ENTER)== GLFW_PRESS){
			presionarEnter = true;
			std::cout << "Enter true" << std::endl;
			if(textureActivaID == textureInit1ID ){
				textureActivaID = textureInit2ID;	
			}else if(textureActivaID == textureInit2ID){
				iniciaPartida = true;
				textureActivaID = textureScreen1ID;
				modelSelected = 0;
			}else if (textureActivaID == textureInit3ID ){
				iniciaPartida = true;
				textureActivaID = textureScreen1ID;
				modelSelected = 1;
			}else if (textureActivaID == textureInit4ID){
				iniciaPartida = true;
				textureActivaID = textureScreen1ID;
				modelSelected = 2;
			}
		}
		else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureActivaID == textureInit2ID){
				textureActivaID = textureInit3ID;
			}else if(textureActivaID == textureInit3ID){
				textureActivaID = textureInit4ID;
			}else if(textureActivaID == textureInit4ID){
				textureActivaID = textureInit2ID;
			}	
		}
		else if(glfwGetKey(window, GLFW_KEY_ENTER)== GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE){
			presionarEnter = false;
			presionarOpcion = false;
		}
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

/* 		if(modelSelected == 0){
			if(fabs(axes[1]) > 0.2){
				modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0, 0, -axes[1] * 0.1));
				animationMayowIndex = 1;
			}if(fabs(axes[0]) > 0.2){
				modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
				animationMayowIndex = 1;
			}
		} */

		//movimiento personajes con joistick control
		if(modelSelected == 0){
			if(fabs(axes[1]) > 0.2){
				modelMatrixMain = glm::translate(modelMatrixMain, glm::vec3(0, 0, -axes[1] * 0.1));
				animationMainIndex = 2;
			}if(fabs(axes[0]) > 0.2){
				modelMatrixMain = glm::rotate(modelMatrixMain, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
				animationMainIndex = 2;
			}		
		}
		if (modelSelected == 1){
			if(fabs(axes[1]) > 0.2){
				modelMatrixMain1 = glm::translate(modelMatrixMain1, glm::vec3(0, 0, -axes[1] * 0.1));
				animationMain1Index = 2;
			}if(fabs(axes[0]) > 0.2){
				modelMatrixMain1 = glm::rotate(modelMatrixMain1, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
				animationMain1Index = 2;
			}
		}
		if (modelSelected == 2){
			if(fabs(axes[1]) > 0.2){
				modelMatrixMain2 = glm::translate(modelMatrixMain2, glm::vec3(0, 0, -axes[1] * 0.1));
				animationMain2Index = 1;
			}if(fabs(axes[0]) > 0.2){
				modelMatrixMain2 = glm::rotate(modelMatrixMain2, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
				animationMain2Index = 1;
			}
		}

		if(fabs(axes[3]) > 0.2){
			camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
		}if(fabs(axes[4]) > 0.2){
			camera->mouseMoveCamera(0.0, axes[4], deltaTime);
		}

		const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount << std::endl;
		if(buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if(!isJump && buttons[0] == GLFW_PRESS){
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;
	//ejemplo mayow
/* 		// Controles de mayow
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			modelMatrixMayow = glm::rotate(modelMatrixMayow, 0.1f, glm::vec3(0, 1, 0));
			animationMayowIndex = 0;
			alSourceStop(source[2]);
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			modelMatrixMayow = glm::rotate(modelMatrixMayow, -0.1f, glm::vec3(0, 1, 0));
			animationMayowIndex = 0;
			alSourceStop(source[2]);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 0.06));
			animationMayowIndex = 0;
			alSourceStop(source[2]);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, -0.06));
			animationMayowIndex = 0;
			alSourceStop(source[2]);
		} */

	if(modelSelected == 0){
		// Controles de mayow
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			modelMatrixMain = glm::rotate(modelMatrixMain, 0.1f, glm::vec3(0, 1, 0));
			animationMainIndex = 2;
			
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			modelMatrixMain = glm::rotate(modelMatrixMain, -0.1f, glm::vec3(0, 1, 0));
			animationMainIndex = 2;
			
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			modelMatrixMain = glm::translate(modelMatrixMain, glm::vec3(0.0, 0.0, 0.06));
			animationMainIndex = 2;
			
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			modelMatrixMain = glm::translate(modelMatrixMain, glm::vec3(0.0, 0.0, -0.06));
			animationMainIndex = 2;
			
		}
	}else if(modelSelected == 1){
		// Controles de mayow
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			modelMatrixMain1 = glm::rotate(modelMatrixMain1, 0.1f, glm::vec3(0, 1, 0));
			animationMain1Index = 1;
			
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			modelMatrixMain1 = glm::rotate(modelMatrixMain1, -0.1f, glm::vec3(0, 1, 0));
			animationMain1Index = 1;
			
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			modelMatrixMain1 = glm::translate(modelMatrixMain1, glm::vec3(0.0, 0.0, 0.06));
			animationMain1Index = 2;
			
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			modelMatrixMain1 = glm::translate(modelMatrixMain1, glm::vec3(0.0, 0.0, -0.06));
			animationMain1Index = 2;
			
		}
	}else if(modelSelected == 2){
		// Controles de mayow
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			modelMatrixMain2 = glm::rotate(modelMatrixMain2, 0.1f, glm::vec3(0, 1, 0));
			animationMain2Index = 1;
			
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			modelMatrixMain2 = glm::rotate(modelMatrixMain2, -0.1f, glm::vec3(0, 1, 0));
			animationMain2Index = 1;
			
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			modelMatrixMain2 = glm::translate(modelMatrixMain2, glm::vec3(0.0, 0.0, 0.06));
			animationMain2Index = 1;
			
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			modelMatrixMain2 = glm::translate(modelMatrixMain2, glm::vec3(0.0, 0.0, -0.06));
			animationMain2Index = 1;
			
		}
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void prepareScene(){

	terrain.setShader(&shaderTerrain);
/* 
		//Mayow
		mayowModelAnimate.setShader(&shaderMulLighting);
	 */

	if(modelSelected == 0){
		//Main
		mainModelAnimate.setShader(&shaderMulLighting);
	}
	if (modelSelected == 1){
		//Main1
		main1ModelAnimate.setShader(&shaderMulLighting);
	}
	if(modelSelected == 2){
		//Main2
		main2ModelAnimate.setShader(&shaderMulLighting);
	}
	//Zombie
	zombieModelAnimate.setShader(&shaderMulLighting);
	//Zombie1
	zombie1ModelAnimate.setShader(&shaderMulLighting);

	//casa
	Casa.setShader(&shaderMulLighting);
	//muro
	Muro.setShader(&shaderMulLighting);
	//hogera
	Hogera.setShader(&shaderMulLighting);
	//arboles
	Pino.setShader(&shaderMulLighting);
	// Fountain
	//modelFountain.setShader(&shaderMulLighting);
}

void prepareDepthScene(){

	terrain.setShader(&shaderDepth);
 	if(modelSelected == 0){
/*	//Mayow
	mayowModelAnimate.setShader(&shaderDepth); */
	//Main
	mainModelAnimate.setShader(&shaderDepth);
	//Main1
	}if(modelSelected == 1){
	main1ModelAnimate.setShader(&shaderDepth);
	//Main2
	}if (modelSelected == 2){
	main2ModelAnimate.setShader(&shaderDepth);
	}
	//Zombie
	zombieModelAnimate.setShader(&shaderDepth);
	//Zombie1
	zombie1ModelAnimate.setShader(&shaderDepth);
	//casa
	Casa.setShader(&shaderDepth);
	//muro
	Muro.setShader(&shaderDepth);
	//hogera
	Hogera.setShader(&shaderDepth);
	//arboles
	Pino.setShader(&shaderDepth);
	// Fountain
	//modelFountain.setShader(&shaderDepth);
}

void renderSolidScene(){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del agua
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	shaderTerrain.setInt("backgroundTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
	terrain.setPosition(glm::vec3(100, 0, 100));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/

	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Se regresa el cull faces IMPORTANTE para la capa
	//glEnable(GL_CULL_FACE);
	if (mapa == 0){
		//primera mitad arboles
		for(int i = 0; i< PinoPos1.size() ; i++ ){
			PinoPos1[i].y =terrain.getHeightTerrain(PinoPos1[i].x,PinoPos1[i].z);
			Pino.setPosition(PinoPos1[i]);
			Pino.setScale(glm::vec3(3.0));			
			Pino.render();
		}
	}else{
		for(int i = 0; i< PinoPos2.size() ; i++ ){
			PinoPos2[i].y =terrain.getHeightTerrain(PinoPos2[i].x,PinoPos2[i].z);
			Pino.setPosition(PinoPos2[i]);
			Pino.setScale(glm::vec3(3.0));			
			Pino.render();
		}
	}
	//hogeras
	if (mapa == 0){
		for(int i = 0; i< Hogeras.size() - 5 ; i++ ){
			Hogeras[i].y =terrain.getHeightTerrain(Hogeras[i].x,Hogeras[i].z);
			Hogera.setPosition(Hogeras[i]);
			Hogera.setScale(glm::vec3(1.0));			
			Hogera.render();
		}
	}else{
		for(int i = 5; i< Hogeras.size() ; i++ ){
			Hogeras[i].y =terrain.getHeightTerrain(Hogeras[i].x,Hogeras[i].z);
			Hogera.setPosition(Hogeras[i]);
			Hogera.setScale(glm::vec3(1.0));			
			Hogera.render();
		}
	}
	//muro
	for(int i = 0; i< Muros.size() ; i++ ){
		Muros[i].y =terrain.getHeightTerrain(Muros[i].x,Muros[i].z);
		Muro.setPosition(Muros[i]);
		Muro.setScale(glm::vec3(0.08));			
		Muro.render();
	}

	//casas
	glDisable(GL_CULL_FACE);
	modelMatrixCasa[3][1] = terrain.getHeightTerrain(modelMatrixCasa[3][0], modelMatrixCasa[3][2]);
	Casa.render(modelMatrixCasa);
	glEnable(GL_CULL_FACE);

	/*****************************************
	 * Objetos animados por huesos
	 * **************************************/
	float speed = 0.5f; // velocidad del Zombie
	void updateZombiePosition(glm::mat4 &modelMatrixZombie, glm::mat4 &modelMatrixMayow, float speed, float deltaTime, Terrain &terrain);
	//ejemplo mayow
	/* 	glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]));
		glm::vec3 ejex = glm::vec3(modelMatrixMayow[0]);
		glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
		ejex = glm::normalize(glm::cross(ejey, ejez));
		modelMatrixMayow[0] = glm::vec4(ejex, 0.0);
		modelMatrixMayow[1] = glm::vec4(ejey, 0.0);
		modelMatrixMayow[2] = glm::vec4(ejez, 0.0);
		modelMatrixMayow[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		tmv = currTime - startTimeJump;
		if(modelMatrixMayow[3][1] < terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2])){
			isJump = false;
			modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		}
		glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
		modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021f));
		mayowModelAnimate.setAnimationIndex(animationMayowIndex);
		mayowModelAnimate.render(modelMatrixMayowBody);
		animationMayowIndex = 1; */

	if(modelSelected == 0){

		glm::vec3 ejey1 = glm::normalize(terrain.getNormalTerrain(modelMatrixMain[3][0], modelMatrixMain[3][2]));
		glm::vec3 ejex1 = glm::vec3(modelMatrixMain[0]);
		glm::vec3 ejez1 = glm::normalize(glm::cross(ejex1, ejey1));
		ejex1 = glm::normalize(glm::cross(ejey1, ejez1));
		modelMatrixMain[0] = glm::vec4(ejex1, 0.0);
		modelMatrixMain[1] = glm::vec4(ejey1, 0.0);
		modelMatrixMain[2] = glm::vec4(ejez1, 0.0);
		modelMatrixMain[3][1] = terrain.getHeightTerrain(modelMatrixMain[3][0], modelMatrixMain[3][2]);
		glm::mat4 modelMatrixMainBody = glm::mat4(modelMatrixMain);
		modelMatrixMainBody = glm::scale(modelMatrixMainBody, glm::vec3(0.015f));
		mainModelAnimate.setAnimationIndex(animationMainIndex);
		mainModelAnimate.render(modelMatrixMainBody);
		//animationMainIndex = 2; //El 2 es la siguiente animacion, 0 la primera 

	}
	if (modelSelected == 1){
		glm::vec3 ejey3 = glm::normalize(terrain.getNormalTerrain(modelMatrixMain1[3][0], modelMatrixMain1[3][2]));
		glm::vec3 ejex3 = glm::vec3(modelMatrixMain1[0]);
		glm::vec3 ejez3 = glm::normalize(glm::cross(ejex3, ejey3));
		ejex3 = glm::normalize(glm::cross(ejey3, ejez3));
		modelMatrixMain1[0] = glm::vec4(ejex3, 0.0);
		modelMatrixMain1[1] = glm::vec4(ejey3, 0.0);
		modelMatrixMain1[2] = glm::vec4(ejez3, 0.0);
		modelMatrixMain1[3][1] = terrain.getHeightTerrain(modelMatrixMain1[3][0], modelMatrixMain1[3][2]);
		glm::mat4 modelMatrixMain1Body = glm::mat4(modelMatrixMain1);
		modelMatrixMain1Body = glm::scale(modelMatrixMain1Body, glm::vec3(0.015f));
		main1ModelAnimate.setAnimationIndex(animationMain1Index);
		main1ModelAnimate.render(modelMatrixMain1Body);
		//animationMain1Index = 2; //El 2 es la siguiente animacion, 0 la primera
	}
	if (modelSelected == 2){
		glm::vec3 ejey4 = glm::normalize(terrain.getNormalTerrain(modelMatrixMain2[3][0], modelMatrixMain2[3][2]));
		glm::vec3 ejex4 = glm::vec3(modelMatrixMain2[0]);
		glm::vec3 ejez4 = glm::normalize(glm::cross(ejex4, ejey4));
		ejex4 = glm::normalize(glm::cross(ejey4, ejez4));
		modelMatrixMain2[0] = glm::vec4(ejex4, 0.0);
		modelMatrixMain2[1] = glm::vec4(ejey4, 0.0);
		modelMatrixMain2[2] = glm::vec4(ejez4, 0.0);
		modelMatrixMain2[3][1] = terrain.getHeightTerrain(modelMatrixMain2[3][0], modelMatrixMain2[3][2]);
		glm::mat4 modelMatrixMain2Body = glm::mat4(modelMatrixMain2);
		modelMatrixMain2Body = glm::scale(modelMatrixMain2Body, glm::vec3(0.015f));
		main2ModelAnimate.setAnimationIndex(animationMain2Index);
		main2ModelAnimate.render(modelMatrixMain2Body);
		//animationMain2Index = 2; //El 2 es la siguiente animacion, 0 la primera
	}
	if (modelSelected == 0)
		updateZombiePosition(modelMatrixZombie, modelMatrixMain, speed, deltaTime, terrain);
	if (modelSelected == 1)
		updateZombiePosition(modelMatrixZombie, modelMatrixMain1, speed, deltaTime, terrain);
	if (modelSelected)
		updateZombiePosition(modelMatrixZombie, modelMatrixMain2, speed, deltaTime, terrain);
	
	glm::vec3 ejey2 = glm::normalize(terrain.getNormalTerrain(modelMatrixZombie[3][0], modelMatrixZombie[3][2]));
	glm::vec3 ejex2 = glm::vec3(modelMatrixZombie[0]);
	glm::vec3 ejez2 = glm::normalize(glm::cross(ejex2, ejey2));
	ejex2 = glm::normalize(glm::cross(ejey2, ejez2));
	modelMatrixZombie[0] = glm::vec4(ejex2, 0.0);
	modelMatrixZombie[1] = glm::vec4(ejey2, 0.0);
	modelMatrixZombie[2] = glm::vec4(ejez2, 0.0);
	modelMatrixZombie[3][1] = terrain.getHeightTerrain(modelMatrixZombie[3][0], modelMatrixZombie[3][2]);
	glm::mat4 modelMatrixZombieBody = glm::mat4(modelMatrixZombie);
	modelMatrixZombieBody = glm::scale(modelMatrixZombieBody, glm::vec3(0.015f));
	zombieModelAnimate.setAnimationIndex(animationZombieIndex);
	zombieModelAnimate.render(modelMatrixZombieBody);
	animationZombieIndex = 2; //El 2 es la siguiente animacion, 0 la primera

	glm::vec3 ejey5 = glm::normalize(terrain.getNormalTerrain(modelMatrixZombie1[3][0], modelMatrixZombie1[3][2]));
	glm::vec3 ejex5 = glm::vec3(modelMatrixZombie1[0]);
	glm::vec3 ejez5 = glm::normalize(glm::cross(ejex5, ejey5));
	ejex5 = glm::normalize(glm::cross(ejey5, ejez5));
	modelMatrixZombie1[0] = glm::vec4(ejex5, 0.0);
	modelMatrixZombie1[1] = glm::vec4(ejey5, 0.0);
	modelMatrixZombie1[2] = glm::vec4(ejez5, 0.0);
	modelMatrixZombie1[3][1] = terrain.getHeightTerrain(modelMatrixZombie1[3][0], modelMatrixZombie1[3][2]);
	glm::mat4 modelMatrixZombie1Body = glm::mat4(modelMatrixZombie1);
	modelMatrixZombie1Body = glm::scale(modelMatrixZombie1Body, glm::vec3(0.015f));
	zombie1ModelAnimate.setAnimationIndex(animationZombie1Index);
	zombie1ModelAnimate.render(modelMatrixZombie1Body);
	animationZombie1Index = 2; //El 2 es la siguiente animacion, 0 la primera

	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void renderAlphaScene(bool render = true){
	/**********
	 * Update the position with alpha objects
	 */
	// Update the Casa
	blendingUnsorted.find("Casa")->second = glm::vec3(modelMatrixVentanas[3]);
	blendingUnsorted.find("Hogera1")->second = glm::vec3(-82.4,0,78.1);
	blendingUnsorted.find("Hogera2")->second = glm::vec3(-73.4,0,23.4);
	blendingUnsorted.find("Hogera3")->second = glm::vec3(9.0,0,72.3);
	blendingUnsorted.find("Hogera4")->second = glm::vec3(-5.5,0,16.0);
	blendingUnsorted.find("Hogera5")->second = glm::vec3(79.3,0,52.3);
	blendingUnsorted.find("Hogera6")->second = glm::vec3(-73.4,0,-19.9);
	blendingUnsorted.find("Hogera7")->second = glm::vec3(-34.0,0,-82.8);
	blendingUnsorted.find("Hogera8")->second = glm::vec3(10.2,0,-52.7);
	blendingUnsorted.find("Hogera9")->second = glm::vec3(50.0,0,-18.8);
	blendingUnsorted.find("Hogera10")->second = glm::vec3(64.5,0,-59.4);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);//GL_ONE);//GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
	//glDisable(GL_CULL_FACE);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		if(it->second.first.compare("Casa") == 0){
			// Render for the casa model
			glm::mat4 modelMatrixCasaBlending = glm::mat4(modelMatrixVentanas);
			modelMatrixCasaBlending[3][1] = terrain.getHeightTerrain(modelMatrixCasaBlending[3][0], modelMatrixCasaBlending[3][2]);
			Ventanas.render(modelMatrixCasaBlending);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (it->second.first.compare("Hogera1") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);; //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera2") == 0){
		/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();			

			}else if (it->second.first.compare("Hogera3") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera4") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera5") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera6") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera7") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera8") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera9") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}else if (it->second.first.compare("Hogera10") == 0){
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0); //modelMatrixDoor;
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(1.0, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

		}
	}

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if(render){
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		modelText->render("Texto en OpenGL", -1, 0);
	}
}

void renderScene(){
	renderSolidScene();
	renderAlphaScene(false);
}

void applicationLoop() {
	//float speed = 0.5f; // velocidad del Zombie
	void updateZombiePosition(glm::mat4 &modelMatrixZombie, glm::mat4 &modelMatrixMayow, float speed, float deltaTime, Terrain &terrain);

	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;
	
	//casa
	if(mapa == 0){
		modelMatrixCasa = glm::translate(modelMatrixCasa,glm::vec3(32.8125,0,36.71875));
		modelMatrixVentanas = glm::translate(modelMatrixVentanas,glm::vec3(32.8125,0,36.71875));
	}else{
		modelMatrixCasa = glm::translate(modelMatrixCasa,glm::vec3(-73.828125,0,-51.171875));
		modelMatrixVentanas = glm::translate(modelMatrixVentanas,glm::vec3(-73.828125,0,-51.171875));
	}
	modelMatrixCasa = glm::scale(modelMatrixCasa,glm::vec3(3.0f));
	modelMatrixVentanas = glm::scale(modelMatrixVentanas,glm::vec3(3.0f));

	//personaje
/* 	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(70.3,0,52.3));//glm::vec3(-10.0f, 0.0f, -20.0f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f), glm::vec3(0, 1, 0)); */

	modelMatrixMain = glm::translate(modelMatrixMain, glm::vec3(10.0f, 0.0f, 0.0f));
	modelMatrixMain = glm::rotate(modelMatrixMain, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	
	modelMatrixMain1 = glm::translate(modelMatrixMain1, glm::vec3(70.3,0.0,52.3));
	modelMatrixMain1 = glm::rotate(modelMatrixMain1, glm::radians(30.0f), glm::vec3(0, 1, 0));

	modelMatrixMain2 = glm::translate(modelMatrixMain2, glm::vec3(70.3,0.0,52.3));
	modelMatrixMain2 = glm::rotate(modelMatrixMain2, glm::radians(-90.0f), glm::vec3(0, 1, 0));


	float speed = 0.5f; // velocidad del Zombie

	modelMatrixZombie = glm::translate(modelMatrixZombie, glm::vec3(-15.6f, 0.0f, -18.9f));
	modelMatrixZombie = glm::rotate(modelMatrixZombie, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixZombie1 = glm::translate(modelMatrixZombie1, glm::vec3(-15.6f, 0.0f, -18.9f));
	modelMatrixZombie1 = glm::rotate(modelMatrixZombie1, glm::radians(-90.0f), glm::vec3(0, 1, 0));


	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureInit1ID;

	// Time for the particles animation
	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		//maynow ejemplo
					/* axis = glm::axis(glm::quat_cast(modelMatrixMayow));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
			target = modelMatrixMayow[3]; */
		if(modelSelected == 0){
			axis = glm::axis(glm::quat_cast(modelMatrixMain));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMain));
			target = modelMatrixMain[3];
		}
		if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixMain1));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMain1));
			target = modelMatrixMain1[3];
		}
		if (modelSelected == 2){
			axis = glm::axis(glm::quat_cast(modelMatrixMain2));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMain2));
			target = modelMatrixMain2[3];
		}

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		glm::mat4 view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para las hogeras
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.04, 0.04, 0.04)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.04, 0.04, 0.04)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);
		shaderTerrain.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", Hogeras.size());
		shaderTerrain.setInt("pointLightCount", Hogeras.size());
		for(int i = 0; i < Hogeras.size(); i++){
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(Hogeras[i]));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(Hogeras[i]));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		/************Render de imagen de frente**************/
		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*****************************************
		 * Objetos animados por huesos
		 * **************************************/
/* 			glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]));
			glm::vec3 ejex = glm::vec3(modelMatrixMayow[0]);
			glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
			ejex = glm::normalize(glm::cross(ejey, ejez));
			modelMatrixMayow[0] = glm::vec4(ejex, 0.0);
			modelMatrixMayow[1] = glm::vec4(ejey, 0.0);
			modelMatrixMayow[2] = glm::vec4(ejez, 0.0);
			modelMatrixMayow[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
			tmv = currTime - startTimeJump;
			if(modelMatrixMayow[3][1] < terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2])){
				isJump = false;
				//alSourceStop(source[0]);
				modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
			}
			glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
			modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021f));
			mayowModelAnimate.setAnimationIndex(animationMayowIndex);
			mayowModelAnimate.render(modelMatrixMayowBody);
			animationMayowIndex = 1; */
		if (modelSelected == 0){
			glm::vec3 ejey1 = glm::normalize(terrain.getNormalTerrain(modelMatrixMain[3][0], modelMatrixMain[3][2]));
			glm::vec3 ejex1 = glm::vec3(modelMatrixMain[0]);
			glm::vec3 ejez1 = glm::normalize(glm::cross(ejex1, ejey1));
			ejex1 = glm::normalize(glm::cross(ejey1, ejez1));
			modelMatrixMain[0] = glm::vec4(ejex1, 0.0);
			modelMatrixMain[1] = glm::vec4(ejey1, 0.0);
			modelMatrixMain[2] = glm::vec4(ejez1, 0.0);
			modelMatrixMain[3][1] = terrain.getHeightTerrain(modelMatrixMain[3][0], modelMatrixMain[3][2]);
			glm::mat4 modelMatrixMainBody = glm::mat4(modelMatrixMain);
			modelMatrixMainBody = glm::scale(modelMatrixMainBody, glm::vec3(0.021f));
			mainModelAnimate.setAnimationIndex(animationMainIndex);
			mainModelAnimate.render(modelMatrixMainBody);
			//animationMainIndex = 2; //El 2 es la siguiente animacion, 0 la primera 

		} if (modelSelected == 1){
			glm::vec3 ejey3 = glm::normalize(terrain.getNormalTerrain(modelMatrixMain1[3][0], modelMatrixMain1[3][2]));
			glm::vec3 ejex3 = glm::vec3(modelMatrixMain1[0]);
			glm::vec3 ejez3 = glm::normalize(glm::cross(ejex3, ejey3));
			ejex3 = glm::normalize(glm::cross(ejey3, ejez3));
			modelMatrixMain1[0] = glm::vec4(ejex3, 0.0);
			modelMatrixMain1[1] = glm::vec4(ejey3, 0.0);
			modelMatrixMain1[2] = glm::vec4(ejez3, 0.0);
			modelMatrixMain1[3][1] = terrain.getHeightTerrain(modelMatrixMain1[3][0], modelMatrixMain1[3][2]);
			glm::mat4 modelMatrixMain1Body = glm::mat4(modelMatrixMain1);
			modelMatrixMain1Body = glm::scale(modelMatrixMain1Body, glm::vec3(0.021f));
			main1ModelAnimate.setAnimationIndex(animationMain1Index);
			main1ModelAnimate.render(modelMatrixMain1Body);
			//animationMain1Index = 2; //El 2 es la siguiente animacion, 0 la primera

		}
		if (modelSelected == 2){
			glm::vec3 ejey4 = glm::normalize(terrain.getNormalTerrain(modelMatrixMain2[3][0], modelMatrixMain2[3][2]));
			glm::vec3 ejex4 = glm::vec3(modelMatrixMain2[0]);
			glm::vec3 ejez4 = glm::normalize(glm::cross(ejex4, ejey4));
			ejex4 = glm::normalize(glm::cross(ejey4, ejez4));
			modelMatrixMain2[0] = glm::vec4(ejex4, 0.0);
			modelMatrixMain2[1] = glm::vec4(ejey4, 0.0);
			modelMatrixMain2[2] = glm::vec4(ejez4, 0.0);
			modelMatrixMain2[3][1] = terrain.getHeightTerrain(modelMatrixMain2[3][0], modelMatrixMain2[3][2]);
			glm::mat4 modelMatrixMain2Body = glm::mat4(modelMatrixMain2);
			modelMatrixMain2Body = glm::scale(modelMatrixMain2Body, glm::vec3(0.021f));
			main2ModelAnimate.setAnimationIndex(animationMain2Index);
			main2ModelAnimate.render(modelMatrixMain2Body);
			//animationMain2Index = 2; //El 2 es la siguiente animacion, 0 la primera
		}

		if (modelSelected == 0)
			updateZombiePosition(modelMatrixZombie, modelMatrixMain, speed, deltaTime, terrain);
		if (modelSelected == 1)
			updateZombiePosition(modelMatrixZombie, modelMatrixMain1, speed, deltaTime, terrain);
		if (modelSelected)
			updateZombiePosition(modelMatrixZombie, modelMatrixMain2, speed, deltaTime, terrain);
		glm::vec3 ejey2 = glm::normalize(terrain.getNormalTerrain(modelMatrixZombie[3][0], modelMatrixZombie[3][2]));
		glm::vec3 ejex2 = glm::vec3(modelMatrixZombie[0]);
		glm::vec3 ejez2 = glm::normalize(glm::cross(ejex2, ejey2));
		ejex2 = glm::normalize(glm::cross(ejey2, ejez2));
		modelMatrixZombie[0] = glm::vec4(ejex2, 0.0);
		modelMatrixZombie[1] = glm::vec4(ejey2, 0.0);
		modelMatrixZombie[2] = glm::vec4(ejez2, 0.0);
		modelMatrixZombie[3][1] = terrain.getHeightTerrain(modelMatrixZombie[3][0], modelMatrixZombie[3][2]);
		glm::mat4 modelMatrixZombieBody = glm::mat4(modelMatrixZombie);
		modelMatrixZombieBody = glm::scale(modelMatrixZombieBody, glm::vec3(0.021f));
		zombieModelAnimate.setAnimationIndex(animationZombieIndex);
		zombieModelAnimate.render(modelMatrixZombieBody);
		animationZombieIndex = 2; //El 2 es la siguiente animacion, 0 la primera

		glm::vec3 ejey5 = glm::normalize(terrain.getNormalTerrain(modelMatrixZombie1[3][0], modelMatrixZombie1[3][2]));
		glm::vec3 ejex5 = glm::vec3(modelMatrixZombie1[0]);
		glm::vec3 ejez5 = glm::normalize(glm::cross(ejex5, ejey5));
		ejex5 = glm::normalize(glm::cross(ejey5, ejez5));
		modelMatrixZombie1[0] = glm::vec4(ejex5, 0.0);
		modelMatrixZombie1[1] = glm::vec4(ejey5, 0.0);
		modelMatrixZombie1[2] = glm::vec4(ejez5, 0.0);
		modelMatrixZombie1[3][1] = terrain.getHeightTerrain(modelMatrixZombie1[3][0], modelMatrixZombie1[3][2]);
		glm::mat4 modelMatrixZombie1Body = glm::mat4(modelMatrixZombie1);
		modelMatrixZombie1Body = glm::scale(modelMatrixZombie1Body, glm::vec3(0.021f));
		zombie1ModelAnimate.setAnimationIndex(animationZombie1Index);
		zombie1ModelAnimate.render(modelMatrixZombie1Body);
		animationZombie1Index = 2; //El 2 es la siguiente animacion, 0 la primera

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		//Arboles colliders
		for (int i = 0; i < PinoPos1.size(); i++){
			AbstractModel::OBB pinoCollider;
			glm::mat4 modelMatrixColliderPino= glm::mat4(1.0);
			modelMatrixColliderPino = glm::translate(modelMatrixColliderPino, PinoPos1[i]);
			addOrUpdateColliders(collidersOBB, "Pino-" + std::to_string(i), pinoCollider, modelMatrixColliderPino);
			// Set the orientation of collider before doing the scale
			pinoCollider.u = glm::quat_cast(modelMatrixColliderPino);
			modelMatrixColliderPino = glm::scale(modelMatrixColliderPino, glm::vec3(0.5, 0.5, 0.5));
			modelMatrixColliderPino = glm::translate(modelMatrixColliderPino, Pino.getObb().c);
			pinoCollider.c = glm::vec3(modelMatrixColliderPino[3]);
			pinoCollider.e = Pino.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("Pino-" + std::to_string(i))->second) = pinoCollider;
		}
		//Hogeras colliders
		for (int i = 0; i < Hogeras.size(); i++){
			AbstractModel::OBB hogeraCollider;
			glm::mat4 modelMatrixColliderHogera= glm::mat4(1.0);
			modelMatrixColliderHogera = glm::translate(modelMatrixColliderHogera, Hogeras[i]);
			addOrUpdateColliders(collidersOBB, "Hogera-" + std::to_string(i), hogeraCollider, modelMatrixColliderHogera);
			// Set the orientation of collider before doing the scale
			hogeraCollider.u = glm::quat_cast(modelMatrixColliderHogera);
			modelMatrixColliderHogera = glm::scale(modelMatrixColliderHogera, glm::vec3(0.5, 0.5, 0.5));
			modelMatrixColliderHogera = glm::translate(modelMatrixColliderHogera, Pino.getObb().c);
			hogeraCollider.c = glm::vec3(modelMatrixColliderHogera[3]);
			hogeraCollider.e = Hogera.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("Hogera-" + std::to_string(i))->second) = hogeraCollider;
		}		
		//Muros colliders
		for (int i = 0; i < Muros.size(); i++){
			AbstractModel::OBB muroCollider;
			glm::mat4 modelMatrixColliderMuro= glm::mat4(1.0);
			modelMatrixColliderMuro = glm::translate(modelMatrixColliderMuro, Muros[i]);
			//modelMatrixColliderMuro = glm::rotate(modelMatrixColliderMuro, glm::radians(lamp1Orientation[i]),glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Muro-" + std::to_string(i), muroCollider, modelMatrixColliderMuro);
			// Set the orientation of collider before doing the scale
			muroCollider.u = glm::quat_cast(modelMatrixColliderMuro);
			modelMatrixColliderMuro = glm::scale(modelMatrixColliderMuro, glm::vec3(0.078, 0.078, 0.078));
			modelMatrixColliderMuro = glm::translate(modelMatrixColliderMuro, Muro.getObb().c);
			muroCollider.c = glm::vec3(modelMatrixColliderMuro[3]);
			muroCollider.e = Muro.getObb().e * glm::vec3(0.078, 0.078, 0.078);
			std::get<0>(collidersOBB.find("Muro-" + std::to_string(i))->second) = muroCollider;
		}
		// Collider de mayow
/* 			AbstractModel::OBB mayowCollider;
			glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
			modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
					glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
			modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
					glm::vec3(mayowModelAnimate.getObb().c.x,
							mayowModelAnimate.getObb().c.y,
							mayowModelAnimate.getObb().c.z));
			mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
			addOrUpdateColliders(collidersOBB, "mayow", mayowCollider, modelMatrixMayow); */
		if (modelSelected == 0){
			AbstractModel::OBB mainCollider;
			glm::mat4 modelmatrixColliderMain = glm::mat4(modelMatrixMain);
			modelmatrixColliderMain = glm::rotate(modelmatrixColliderMain,
					glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			mainCollider.u = glm::quat_cast(modelmatrixColliderMain);
			modelmatrixColliderMain = glm::scale(modelmatrixColliderMain, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderMain = glm::translate(modelmatrixColliderMain,
					glm::vec3(mainModelAnimate.getObb().c.x,
							mainModelAnimate.getObb().c.y,
							mainModelAnimate.getObb().c.z));
			mainCollider.e = mainModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			mainCollider.c = glm::vec3(modelmatrixColliderMain[3]);
			addOrUpdateColliders(collidersOBB, "main", mainCollider, modelMatrixMain);
		}
		if (modelSelected == 1){
			AbstractModel::OBB main1Collider;
			glm::mat4 modelmatrixColliderMain1 = glm::mat4(modelMatrixMain1);
			modelmatrixColliderMain1 = glm::rotate(modelmatrixColliderMain1,
					glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			main1Collider.u = glm::quat_cast(modelmatrixColliderMain1);
			modelmatrixColliderMain1 = glm::scale(modelmatrixColliderMain1, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderMain1 = glm::translate(modelmatrixColliderMain1,
					glm::vec3(main1ModelAnimate.getObb().c.x,
							main1ModelAnimate.getObb().c.y,
							main1ModelAnimate.getObb().c.z));
			main1Collider.e = main1ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			main1Collider.c = glm::vec3(modelmatrixColliderMain1[3]);
			addOrUpdateColliders(collidersOBB, "main1", main1Collider, modelMatrixMain1);
		}
		if (modelSelected == 2){
			AbstractModel::OBB main2Collider;
			glm::mat4 modelmatrixColliderMain2 = glm::mat4(modelMatrixMain2);
			modelmatrixColliderMain2 = glm::rotate(modelmatrixColliderMain2,
					glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			main2Collider.u = glm::quat_cast(modelmatrixColliderMain2);
			modelmatrixColliderMain2 = glm::scale(modelmatrixColliderMain2, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderMain2 = glm::translate(modelmatrixColliderMain2,
					glm::vec3(main2ModelAnimate.getObb().c.x,
							main2ModelAnimate.getObb().c.y,
							main2ModelAnimate.getObb().c.z));
			main2Collider.e = main2ModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			main2Collider.c = glm::vec3(modelmatrixColliderMain2[3]);
			addOrUpdateColliders(collidersOBB, "main2", main2Collider, modelMatrixMain2);
		}
		//colliders Zombies
/* 		for(int i=0 ; i < zombies.size(); i++){
			AbstractModel::OBB mayowCollider;
			glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
			modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
			modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.021, 0.021, 0.021));
			modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
			glm::vec3(mayowModelAnimate.getObb().c.x,mayowModelAnimate.getObb().c.y,mayowModelAnimate.getObb().c.z));
			mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
			addOrUpdateColliders(collidersOBB, "mayow", mayowCollider, modelMatrixMayow);
		} */

		/**********Render de transparencias***************/
		renderAlphaScene();

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); 
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, 
				glm::mat4, glm::mat4>>::iterator sbbBuscado = 
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second){
					addOrUpdateColliders(collidersOBB, itCollision->first);
				}else{
					if (itCollision->first.compare("main") == 0){
						//if (modelSelected == 0)
							modelMatrixMain = std::get<1>(obbBuscado->second);
					}
					if (itCollision->first.compare("main1") == 0){
						//if (modelSelected == 1)
							modelMatrixMain1 = std::get<1>(obbBuscado->second);
					}
					if (itCollision->first.compare("main2") == 0){
						//if (modelSelected == 2)
							modelMatrixMain2 = std::get<1>(obbBuscado->second);
						
					}
				}
			}
		}

		// Constantes de animaciones
		//animationMayowIndex = 1;
		animationMainIndex = 1;
		animationMain1Index = 1;
		animationMain2Index = 2;

		glfwSwapBuffers(window);
		/****************************+
		 * Open AL sound data
		 */

		if (modelSelected == 0){
			// Listener for the Thris person camera
			listenerPos[0] = modelMatrixMain[3].x;
			listenerPos[1] = modelMatrixMain[3].y;
			listenerPos[2] = modelMatrixMain[3].z;
			alListenerfv(AL_POSITION, listenerPos);

			glm::vec3 upModel = glm::normalize(modelMatrixMain[1]);
			glm::vec3 frontModel = glm::normalize(modelMatrixMain[2]);

			listenerOri[0] = frontModel.x;
			listenerOri[1] = frontModel.y;
			listenerOri[2] = frontModel.z;
			listenerOri[3] = upModel.x;
			listenerOri[4] = upModel.y;
			listenerOri[5] = upModel.z;
		}
		if (modelSelected == 1){
			// Listener for the Thris person camera
			listenerPos[0] = modelMatrixMain1[3].x;
			listenerPos[1] = modelMatrixMain1[3].y;
			listenerPos[2] = modelMatrixMain1[3].z;
			alListenerfv(AL_POSITION, listenerPos);

			glm::vec3 upModel = glm::normalize(modelMatrixMain1[1]);
			glm::vec3 frontModel = glm::normalize(modelMatrixMain1[2]);

			listenerOri[0] = frontModel.x;
			listenerOri[1] = frontModel.y;
			listenerOri[2] = frontModel.z;
			listenerOri[3] = upModel.x;
			listenerOri[4] = upModel.y;
			listenerOri[5] = upModel.z;

		}
		if (modelSelected == 2){
			// Listener for the Thris person camera
			listenerPos[0] = modelMatrixMain2[3].x;
			listenerPos[1] = modelMatrixMain2[3].y;
			listenerPos[2] = modelMatrixMain2[3].z;
			alListenerfv(AL_POSITION, listenerPos);

			glm::vec3 upModel = glm::normalize(modelMatrixMain2[1]);
			glm::vec3 frontModel = glm::normalize(modelMatrixMain2[2]);

			listenerOri[0] = frontModel.x;
			listenerOri[1] = frontModel.y;
			listenerOri[2] = frontModel.z;
			listenerOri[3] = upModel.x;
			listenerOri[4] = upModel.y;
			listenerOri[5] = upModel.z;
		}

		//sinido zombie
		source3Pos[0] = modelMatrixZombie[3].x;
		source3Pos[1] = modelMatrixZombie[3].y;
		source3Pos[2] = modelMatrixZombie[3].z;
		alSourcefv(source[3], AL_POSITION, source3Pos);

		//sonido pasos
		if(modelSelected == 0){
			source3Pos[0] = modelMatrixMain[3].x;
			source3Pos[1] = modelMatrixMain[3].y;
			source3Pos[2] = modelMatrixMain[3].z;
			alSourcefv(source[3], AL_POSITION, source3Pos);
		}
		if(modelSelected == 1){
			source3Pos[0] = modelMatrixMain1[3].x;
			source3Pos[1] = modelMatrixMain1[3].y;
			source3Pos[2] = modelMatrixMain1[3].z;
			alSourcefv(source[3], AL_POSITION, source3Pos);
		}if(modelSelected == 2){
			source3Pos[0] = modelMatrixMain2[3].x;
			source3Pos[1] = modelMatrixMain2[3].y;
			source3Pos[2] = modelMatrixMain2[3].z;
			alSourcefv(source[3], AL_POSITION, source3Pos);
		}

/* 		// Listener for the First person camera
		 listenerPos[0] = camera->getPosition().x;
		 listenerPos[1] = camera->getPosition().y;
		 listenerPos[2] = camera->getPosition().z;
		 alListenerfv(AL_POSITION, listenerPos);
		glm::vec3 upModel = camera->getUp();
		glm::vec3 frontModel = camera->getFront();
		 listenerOri[0] = camera->getFront().x;
		 listenerOri[1] = camera->getFront().y;
		 listenerOri[2] = camera->getFront().z;
		 listenerOri[3] = camera->getUp().x;
		 listenerOri[4] = camera->getUp().y;
		 listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri); */

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
				sourcesPlay[i] = false;
			}
		}
	}
}

void updateZombiePosition(glm::mat4 &modelMatrixZombie, glm::mat4 &modelMatrixMain, float speed, float deltaTime, Terrain &terrain) {
	// Obtener la posición actual de Mayow y Zombie
	glm::vec3 positionMayow = glm::vec3(modelMatrixMain[3]);
	glm::vec3 positionZombie = glm::vec3(modelMatrixZombie[3]);

	// Calcular la dirección desde el Zombie hacia Mayow
	glm::vec3 direction = glm::normalize(positionMayow - positionZombie);

	// Calcular la nueva posición del Zombie
	glm::vec3 newPosition = positionZombie + direction * speed * deltaTime;

	// Actualizar la posición del Zombie en el modelo
	modelMatrixZombie[3] = glm::vec4(newPosition, 1.0f);

	// Calcular la orientación del Zombie
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 right = glm::normalize(glm::cross(up, direction)); 
	glm::vec3 adjustedUp = glm::normalize(glm::cross(direction, right)); 

	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix[0] = glm::vec4(right, 0.0f);
	rotationMatrix[1] = glm::vec4(adjustedUp, 0.0f);
	rotationMatrix[2] = glm::vec4(direction, 0.0f);

	modelMatrixZombie = rotationMatrix;
	modelMatrixZombie[3] = glm::vec4(newPosition, 1.0f);

}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}