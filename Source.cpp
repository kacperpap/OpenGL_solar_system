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

const unsigned int width = 800;
const unsigned int height = 800;


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




	Shader earthShader("default.vert", "default.frag");
	Planet earth(0.4f, 32);

	Texture container("container.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	container.texUnit(earthShader, "tex0", 0);
	Texture specularMap("containerAOmap.jpg", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	specularMap.texUnit(earthShader, "tex1", 1);
	

	Shader planetShader("planet.vert", "planet.frag");
	Planet sun(1.0f, 32);

	Texture sunTexture("sun_2.jpg", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE);
	sunTexture.texUnit(planetShader, "tex2", 2);
	

	//GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));

	sun.ModelTranslate(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f));

	earth.ModelTranslate(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::mat4(1.0f));

	sun.ShaderConfigure(planetShader);
	
	earth.ShaderConfigureWithSunReflection(earthShader, sun);
	

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		earthShader.Activate();
		//shader'y default obs³uguja jednoczesnie kamere nwm czy mozna to rozdzielic na osobne shadery
		glUniform3f(glGetUniformLocation(earthShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(earthShader, "camMatrix");
		//glUniform1f(uniID, 1.5f); //nwm od czego to i co to robi
		container.Bind();
		specularMap.Bind();
		earth.Draw();


		planetShader.Activate();
		camera.Matrix(planetShader, "camMatrix");
		sunTexture.Bind();
		sun.Draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	container.Delete();
	specularMap.Delete();
	earthShader.Delete();
	planetShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}