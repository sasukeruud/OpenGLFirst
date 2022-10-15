//Section 1
#include <iostream>
#include <tclap/CmdLine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GeometricTools.h"

/*
*Method to get key input from user
* with esc input the window will be closed
*/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv)
{	
	//Section 2
	try {
		TCLAP::CmdLine cmd("Command descritption", ' ', "0.9");

		TCLAP::ValueArg<std::string> widthArg("w", "width=", "Width of window", false, "600","string");
		TCLAP::ValueArg<std::string> heightArg("H", "height=", "height of window", false, "600", "string");
		TCLAP::SwitchArg triangleArg("t", "triangle", "Draw a triangle",cmd, false);
		TCLAP::SwitchArg squareArg("s", "square", "Draw a square", cmd, false);

		cmd.add(widthArg);
		cmd.add(heightArg);

		TCLAP::SwitchArg consoleArg("c", "console", "console", cmd, false);

		cmd.parse(argc, argv);

		bool console = consoleArg.getValue();
		
		if (consoleArg) {
			std::cout << "Hello OpenGL: Console" << std::endl;
		}
		else {
			std::cout << "Hello OpenGL" << std::endl;
		}

	//Section 3
	auto value = glfwInit();

	if (value == GLFW_FALSE) {
		return EXIT_FAILURE;
	}

	GLFWwindow* window = glfwCreateWindow(stoi(widthArg.getValue()), stoi(heightArg.getValue()), "Window", NULL, NULL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	//
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	//Section 4
	gladLoadGL();

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//Section 5
	float triangle[3 * 2] = {
		-0.5f, -0.5f, 
		0.5f, -0.5f,
		0.0f, 0.5f
	};

	float square[6 * 2] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, 0.5f
	};

	float triangleColors[] = {
		0.01f,0.01f,0.60f,	
		0.01f,0.01f,0.60f,	
		0.01f,0.01f,0.60f	
	};

	float squareColors[] = {
		0.01f,0.01f,0.60f,	
		0.01f,0.01f,0.60f,	
		0.01f,0.01f,0.60f,
		0.01f,0.01f,0.60f	
	};

	//Create a vertex array
	GLuint vertexArrayId, vertexArrayId1;
	GLuint triangleColorBuffer, squareColorBuffer;
	glGenBuffers(1, &triangleColorBuffer);
	glGenBuffers(1, &squareColorBuffer);
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	if (!squareArg) {
		//Create a vertex buffer triangle
		GLuint vertexBufferTriangle;
		glGenBuffers(1, &vertexBufferTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferTriangle);

		// Populate the vertex buffer 
		glBufferData(GL_ARRAY_BUFFER, sizeof(GeometricTools::UnitTriangle2D), GeometricTools::UnitTriangle2D, GL_DYNAMIC_DRAW);
		// Set the layout of the bound buffer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);	//Location 0 on shader
		glEnableVertexAttribArray(0);													//Enable vertex atribute for cordinates												

		//Color
		//GLuint triangleColorBuffer;
		//glGenBuffers(1, &triangleColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, triangleColorBuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors), triangleColors, GL_DYNAMIC_DRAW);
		// Set the layout of the bound buffer
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);	//Location 1 on shader
		glEnableVertexAttribArray(1);													//Enable vertex atribute for color
	}else{
		glDeleteVertexArrays(1, &vertexArrayId);
	}

	//Create a vertex array
	glGenVertexArrays(1, &vertexArrayId1);
	glBindVertexArray(vertexArrayId1);

	if (!triangleArg) {
		//Create a vertex buffer
		GLuint vertexBufferSquare;
		glGenBuffers(1, &vertexBufferSquare);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferSquare);

		// Populate the vertex buffer 
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_DYNAMIC_DRAW);
		// Set the layout of the bound buffer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);	//Location 0 on shader
		glEnableVertexAttribArray(0);													//Enable vertex atribute for cordinates		

		//Color
		//GLuint squareColorBuffer;
		//glGenBuffers(1, &squareColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, squareColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareColors), squareColors, GL_DYNAMIC_DRAW);
		// Set the layout of the bound buffer
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);	//Location 1 on shader
		glEnableVertexAttribArray(1);													//Enable vertex atribute for color
	}else{
		glDeleteVertexArrays(1, &vertexArrayId1);
	}

	// Verex shader code
	const std::string vertexShaderSrc = R"(
	#version 430 core 

	Layout(location = 0) in vec2 position;

	void main(){
	//Final coordinate for gl_Position are expressed in 
	// 3D homogeneous coordinates (3D + 1) while our coordinates
	// were just 2D. Homogeneous coordiantes are used for perspective purposes
	// and will be studied later. For now we set the third coordinate (z) to t0.0
	// and the homogeneous component to 1.0.
	gl_Position = vec4(position, 0.0, 1.0);
	}
	)";

	const std::string vertexShaderSrc2 = R"(
	layout(location = 0) in vec2 position;
	layout(location = 1) in vec3 color; // this is the input color from the vertex specification

	out vec3 vs_color; //this newly created variable that will output color to the fragment shader
		               // as opposed to the gl_Position placeholder, you can name it and type it
			           // as you see fit.

	void main()
	{
	gl_Position = vec4(position, 0.0, 1.0);
	vs_color = color; // Here we do the color transfer;
	}

	)";

	// Fragment shader code
	const std::string fragmentShaderSrc = R"(
	#verison 430 core
	
	out vec4 color;
	void main(){
	color ) vec4(1.0,1.0,1.0,1.0);
	}
	)";

	const std::string fragmentShaderSrc2 = R"(
	//Fragment shader
	in vec3 vs_color; //The name should be the same as the corresponding output variable 
		              //in the vertex shader
	out vec3 finalColor;
	void main()
	{
	finalColor = vs_color;
	})";

	// Compile the vertex shader
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vss = vertexShaderSrc2.c_str();
	glShaderSource(vertexShader, 1, &vss, nullptr);
	glCompileShader(vertexShader);

	// Compile the fragment shader
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fss = fragmentShaderSrc2.c_str();
	glShaderSource(fragmentShader, 1, &fss, nullptr);
	glCompileShader(fragmentShader);

	// Create a shader program
	auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Activate the program 
	glUseProgram(shaderProgram);

	//Section 6
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

	float changeColorTriangle[] = { 0.0f, 0.0f, 0.0f};
	float changeColorSquare[] = { 0.0f, 0.0f, 0.0f };
	
	double a{};
	double b{};

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	double deltaTime{};
	
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		a += deltaTime * 5.0;
		b += deltaTime * 3.0;
		
		// Keep running
		glClear(GL_COLOR_BUFFER_BIT);
		if (!triangleArg) {
			glBindVertexArray(vertexArrayId1);
			glBindBuffer(GL_ARRAY_BUFFER, squareColorBuffer); //NEEDS TO BE REBIND 
			glDrawArrays(GL_QUADS, 0, 6); //Draws a square
			
			//For loop to change color
			if (a > 1) {
				for (auto &c : changeColorSquare) c = (rand() % 100) / 100.0f; //Loop to get a number between 0 and 99 and divided by 100
				a = 0;
				glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(float), 3 * sizeof(float), changeColorSquare);	//Area of buffer that will change (color area)
				glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float), 3 * sizeof(float), changeColorSquare);	//Area of buffer that will change (color area)
				glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), 3 * sizeof(float), changeColorSquare);	//Area of buffer that will change (color area)
				glBufferSubData(GL_ARRAY_BUFFER, 9 * sizeof(float), 3 * sizeof(float), changeColorSquare);	//Area of buffer that will change (color area)
			}
			
			glBindVertexArray(0);
		}

		//For drawing a triangle on the sceen with changing triangleColors
		if (!squareArg) {
			glBindVertexArray(vertexArrayId);
			glBindBuffer(GL_ARRAY_BUFFER, triangleColorBuffer); //NEEDS TO BE REBIND 
			glDrawArrays(GL_TRIANGLES, 0, 3); //Draws a triangle

			//For loop to change color
			if (b > 1) {
				for (auto &c : changeColorTriangle) c = (rand() % 100) / 100.0f; //Loop to get a number between 0 and 99 and divided by 100
				b = 0;
				glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(float), 3 * sizeof(float), changeColorTriangle);	//Area of buffer that will change (color area)
				glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float), 3 * sizeof(float), changeColorTriangle);	//Area of buffer that will change (color area)
				glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), 3 * sizeof(float), changeColorTriangle);	//Area of buffer that will change (color area)
			}
			glBindVertexArray(0);
		}
		

		glfwSwapBuffers(window);

		// Window will react to input from user
		glfwPollEvents();
	}


	// Section 7
	glfwDestroyWindow(window);
	glfwTerminate();

	}
	catch (TCLAP::ArgException& e)  // catch exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

	return EXIT_SUCCESS;
}
