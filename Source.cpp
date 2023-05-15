#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include "Planet.h"

const unsigned int width = 1000;
const unsigned int height = 1000;

void createPlanet(Shader planetShader, Camera camera, Texture tex, Planet planet) {
	planetShader.Activate();
	planet.move();
	//shader'y default obs³uguja jednoczesnie kamere nwm czy mozna to rozdzielic na osobne shadery
	glUniform3f(glGetUniformLocation(planetShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(planetShader, "camMatrix");
	tex.Bind();
	planet.Draw(planetShader);
}

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	//SUN
	Shader planetShader("planet.vert", "planet.frag");
	Planet sun(2.5f, 32);


	Texture sunTexture("sun_2.jpg", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE);
	sunTexture.texUnit(planetShader, "tex2", 2);
	
	//MERCURY
	Shader mercuryShader("default.vert", "default.frag");
	Planet mercury(0.1f, 32);

	Texture mercuryTex("merkury.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	mercuryTex.texUnit(mercuryShader, "tex0", 0);

	//VENUS
	Shader venusShader("default.vert", "default.frag");
	Planet venus(0.28f, 32);

	Texture venusTex("wenus.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	venusTex.texUnit(venusShader, "tex0", 0);

	//EARTH
	Shader earthShader("default.vert", "default.frag");
	Planet earth(0.35f, 32);

	Texture earthTex("earth.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	earthTex.texUnit(earthShader, "tex0", 0);
	Texture specularMap("earthOmap.jpg", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	specularMap.texUnit(earthShader, "tex1", 1);

	//MARS
	Shader marsShader("default.vert", "default.frag");
	Planet mars(0.3f, 32);

	Texture marsTex("mars.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	marsTex.texUnit(marsShader, "tex0", 0);


	//JUPITER
	Shader jupiterShader("default.vert", "default.frag");
	Planet jupiter(0.9f, 32);

	Texture jupiterTex("jupiter.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	jupiterTex.texUnit(jupiterShader, "tex0", 0);

	//SATURN
	Shader saturnShader("default.vert", "default.frag");
	Planet saturn(0.8f, 32);

	Texture saturnTex("saturn.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	saturnTex.texUnit(saturnShader, "tex0", 0);

	//URAN
	Shader uranShader("default.vert", "default.frag");
	Planet uran(0.5f, 32);

	Texture uranTex("uran.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	uranTex.texUnit(uranShader, "tex0", 0);

	//NEPTUN
	Shader neptunShader("default.vert", "default.frag");
	Planet neptun(0.5f, 32);

	Texture neptunTex("neptun.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	neptunTex.texUnit(neptunShader, "tex0", 0);

	//GLuint uniID = glGetUniformLocation(planetShader.ID, "scale");

	//CAMERA
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 30.0f));

	//Translate
	glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	sun.ModelTranslate(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), sunPosition, glm::mat4(1.0f), 0.0f, 0.0f, 0.0f);

	mercury.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(3.0f, 0.0f, -1.0f), glm::mat4(1.0f), -3.0f, -4.65f, 2.5f);
	venus.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(4.0f, 0.0f, 13.0f), glm::mat4(1.0f), 4.0f, 3.6f, 0.4f);
	earth.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(6.0f, 0.0f, -3.0f), glm::mat4(1.0f), -6.0f, 5.5f, 0.3f);
	mars.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(8.0f, 0.0f, 4.0f), glm::mat4(1.0f), 8.0f, -6.4f, 0.4f);
	jupiter.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 0.0f, -12.0f), glm::mat4(1.0f), -10.0f, 6.4f, 0.8f);
	saturn.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(13.0f, 0.0f, 5.0f), glm::mat4(1.0f), 13.0f, 12.2f, 0.5f);
	uran.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(16.0f, 0.0f, 7.0f), glm::mat4(1.0f), -16.0f, -6.2f, 0.7f);
	neptun.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(18.0f, 0.0f, -2.0f), glm::mat4(1.0f), 18.0f, 7.4f, 0.7f);


	//conf
	sun.ShaderConfigure(planetShader);
	//glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//glUniform4f(glGetUniformLocation(planetShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	mercury.ShaderConfigureWithSunReflection(mercuryShader, sun);
	venus.ShaderConfigureWithSunReflection(venusShader, sun);
	earth.ShaderConfigureWithSunReflection(earthShader, sun);
	mars.ShaderConfigureWithSunReflection(marsShader, sun);
	jupiter.ShaderConfigureWithSunReflection(jupiterShader, sun);
	saturn.ShaderConfigureWithSunReflection(saturnShader, sun);
	uran.ShaderConfigureWithSunReflection(uranShader, sun);
	neptun.ShaderConfigureWithSunReflection(neptunShader, sun);
	

	glEnable(GL_DEPTH_TEST);


	glm::vec3 orbitCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat lastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.03f, 0.07f, 0.07f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 200.0f);

		//MERCURY
		createPlanet(mercuryShader, camera, mercuryTex, mercury);

		//VENUS
		createPlanet(venusShader, camera, venusTex, venus);

		//EARTH
		createPlanet(earthShader, camera, earthTex, earth);

		//MARS
		createPlanet(marsShader, camera, marsTex, mars);

		//JUPITER
		createPlanet(jupiterShader, camera, jupiterTex, jupiter);

		//SATURN
		createPlanet(saturnShader, camera, saturnTex, saturn);

		//URAN
		createPlanet(uranShader, camera, uranTex, uran);

		//NEPTUN
		createPlanet(neptunShader, camera, neptunTex, neptun);

		//SUN
		planetShader.Activate();
		camera.Matrix(planetShader, "camMatrix");
		sunTexture.Bind();
		sun.Draw(planetShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
		//glUniform1f(uniID, 0.5f); //nwm od czego to i co to robi - to chyba ustala skale obiektu, ale nam nie potrzebne chyba
	}


	mercuryTex.Delete();
	venusTex.Delete();
	earthTex.Delete();
	marsTex.Delete();
	jupiterTex.Delete();
	saturnTex.Delete();
	neptunTex.Delete();
	uranTex.Delete();
	sunTexture.Delete();

	specularMap.Delete();
	mercuryShader.Delete();
	venusShader.Delete();
	earthShader.Delete();
	marsShader.Delete();
	jupiterShader.Delete();
	saturnShader.Delete();
	uranShader.Delete();
	neptunShader.Delete();
	planetShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}