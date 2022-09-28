////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////
// Objects to render
Plain* Window::plain;
//Sphere* Window::sphere;
Ball* Window::ball;
Cube* Window::cube;
std::vector<Plain*> Window::plains;

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Starter";

glm::vec3 Window::windSpeed = glm::vec3(-1.0, -1.0, -1.0);

//particle system
glm::vec3 Window::defaultPos = glm::vec3(0);
glm::vec3 Window::defaultVelocity = glm::vec3(10,-5,5);
glm::vec3 Window::defaultWindVelocity = glm::vec3(0,0,0);

//time
GLfloat Window::lastFrameTime = 0.0f;
GLfloat Window::thisFrameTime = 0.0f;
GLfloat Window::deltaTime = 0.0f;
GLfloat Window::calTime = 0.0f;
GLfloat Window::renderRate = 60.0f;
GLfloat Window::simulationRate = 200.0f;
GLfloat Window::renderTimeStep = 1.0f / renderRate;
GLfloat Window::renderTime = renderTimeStep;
GLfloat Window::simTimeStep = 1.0f / simulationRate;
ParticleSystem* psb;

bool Window::simStart = false;
// The shader program id
GLuint Window::shaderProgram;
GLuint Window::pointShaderProgram;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Create a shader program with a vertex shader and a fragment shader.
	pointShaderProgram = LoadShaders("shaders/pointShader.vert", "shaders/pointShader.frag");

	// Check the shader program.
	if (!pointShaderProgram)
	{
		std::cerr << "Failed to initialize point shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube
	//plain = new Plain(10.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.4f));
	//sphere = new Sphere(1.0, glm::vec3(0.3,0.3,0.4));
	
	cube = new Cube(10.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	ball = new Ball(defaultPos, defaultVelocity, defaultWindVelocity, 
					GRAV_COE, DEFAULT_AIRRESIST, DEFAULT_RADIUS, 
					DEFAULT_ELASTIC, DEFAULT_FRICTION, cube);
	std::vector<Plain*>p;
	psb = new ParticleSystem(p, 1.0f, 0.1f, 3.0f, 0.5f);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete plain;
	delete ball;
	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
	
	//calculate time
	if (simStart) {
		thisFrameTime = glfwGetTime();
		deltaTime = thisFrameTime - lastFrameTime;
		lastFrameTime = thisFrameTime;
		calTime += deltaTime;
		renderTime = renderTime - deltaTime;
		//when reaching the render time
		if (renderTime < EPSILON) {
			ball->renderUpdate();
			renderTime = renderTimeStep - renderTime;//reset timer
		}
		// Perform any updates as necessary.
		while (calTime > simTimeStep) {
			calTime -= simTimeStep;

			GLfloat restTime = 0.0f;
			ball->update(simTimeStep, restTime);
			psb->update(simTimeStep, ball->GetPos(), ball->GetVelocity(), ball->GetRadius());
			calTime += restTime;
			
		}
	}
	else {
		lastFrameTime = glfwGetTime();
		ball->renderUpdate();
	}

	//update values
	Cam->Update();
	//partSys->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
	ball->draw(Cam->GetView(), Cam->GetProject(), Window::shaderProgram, Window::pointShaderProgram);
	psb->draw(Cam->GetView(), Cam->GetProject(), Window::pointShaderProgram);
	cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
	drawGUI();
	
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;

		case GLFW_KEY_UP:
			break;
		case GLFW_KEY_DOWN:
			break;
		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
void Window::drawGUI() {

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::GetIO().WantCaptureMouse = true;
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Windspeed show
	ImGui::Begin("PARTICLE SYSTEM");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SetWindowSize(ImVec2(350, Window::height));
	ImGui::SetWindowPos(ImVec2(Window::width - 350, 0));

	if (ImGui::Button("Start")) {
		if (simStart) {
			simStart = false;
		}
		else {
			simStart = true;
		}
	}
	if (ImGui::TreeNode("Ball Property")) {

		ImGui::SliderFloat("Radius", ball->getSphereRadius(), 0.5, 3);
		ImGui::SliderFloat("Mass", ball->getMass(), 1, 5);
		ImGui::SliderFloat("Elasticity", ball->getElastic(), 0.1, 1);

		if (ImGui::TreeNode("Initial position")) {
			ImGui::SliderFloat("X", ball->getPosX(), 
				cube->getLeft()->getOrigin().x + ball->getRadius() + TOLERANCE, 
				cube->getRight()->getOrigin().x - ball->getRadius() - TOLERANCE);
			ImGui::SliderFloat("Y", ball->getPosY(), 
				cube->getBot()->getOrigin().y + ball->getRadius() + TOLERANCE,
				cube->getTop()->getOrigin().y - ball->getRadius() - TOLERANCE);
			ImGui::SliderFloat("Z", ball->getPosZ(),
				cube->getBack()->getOrigin().z + ball->getRadius() + TOLERANCE,
				cube->getFront()->getOrigin().z - ball->getRadius() - TOLERANCE);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Initial Velocity")) {
			ImGui::SliderFloat("X", ball->getVelX(), -5, 5);
			ImGui::SliderFloat("Y", ball->getVelY(), -5, 5);
			ImGui::SliderFloat("Z", ball->getVelZ(), -5, 5);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Environment")) {
		ImGui::SliderFloat("Gravity", ball->getGravMult(), -1, 3);
		ImGui::SliderFloat("Air Resist", ball->getAirResist(), 0, 1);
		ImGui::SliderFloat("Friction", ball->getFriction(), 0, 0.9);
		if (ImGui::TreeNode("Initial Wind Velocity")) {
			ImGui::SliderFloat("X", ball->getWindX(), -5, 5);
			ImGui::SliderFloat("Y", ball->getWindY(), -5, 5);
			ImGui::SliderFloat("Z", ball->getWindZ(), -5, 5);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	if (ImGui::Button("Restore to Default")) {
		ball->restoreDefault();
		simStart = false;
	}
	//draw imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}