//Section 1
#include <iostream>
#include <tclap/CmdLine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

	//Create a vertex array
	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	//Create a vertex buffer
	GLuint vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBufferId);

	// Populate the vertex buffer 
	if (triangleArg) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	}
	else if (squareArg) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	}

	// Set the layout of the bound buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
	glEnableVertexAttribArray(0);

	// Verex sahder code
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

	// Fragment shader code
	const std::string fragmentShaderSrc = R"(
	#verison 430 core
	
	out vec4 color;
	void main(){
	color ) vec4(1.0,1.0,1.0,1.0);
	}
	)";

	// Compile the vertex shader
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vss = vertexShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vss, nullptr);
	glCompileShader(vertexShader);

	// Compile the fragment shader
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fss = fragmentShaderSrc.c_str();
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

	while (!glfwWindowShouldClose(window))
	{
		// Keep running
		glClear(GL_COLOR_BUFFER_BIT);
		if (triangleArg) {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else if (squareArg) {
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glfwSwapBuffers(window);
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
