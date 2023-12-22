/* 
  Filename: Main.cpp
  Authors: Gary M. Zoppetti, Ph.D. & Chad Hogg
  Course: CSCI375
  Assignment: A01Primitives
  Description: A beginning OpenGL program that uses OpenGL 3.3 to draw a 
    triangle and point
 */

/******************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdexcept>

// Use GLEW so we can access the latest OpenGL functionality
// Always include GLEW before GLFW!
#include <glad/glad.h>
// Use GLFW so we can create windows and handle events in a
//   platform-independent way
#include <GLFW/glfw3.h>

/******************************************************************/
// Local includes
#include "ShaderProgram.hpp"

/******************************************************************/
// Type declarations/globals variables/prototypes

// One VAO for the triangle, one for the point
#define NUMBER_OF_VAOS 2

// The VAOs
GLuint g_vaos[NUMBER_OF_VAOS];

// Need a shader program to transform and light the primitives
ShaderProgram* g_shaderProgram;

/******************************************************************/

// Initializes all of the libraries.
void
init(GLFWwindow*& window);

// Initializes the GLFW library -- should only be called by init().
void
initGlfw();

// Initializes the GLEW library -- should only be called by init().
void
initGlad();

// Creates and initializes the window -- should only be called by init().
void
initWindow(GLFWwindow*& window);

// Re-renders the window -- should be called when size changes.
void
resetViewport(GLFWwindow* window, int width, int height);

// Creates the geometric objects -- should only be called by init().
void
initScene();

// Sets up our shader programs -- should only be called by init().
void
initShaders();

// Move geometric objects around using game logic.
void
updateScene(double time);

// Draws the scene onto the window.
void
drawScene(GLFWwindow* window);

// Respond to any user input.
void
processKeys(GLFWwindow* window, int key, int scanCode, int action,
	     int modifiers);

// Clean up as program exits.
void
releaseGlResources();

// Prints an error from GLFW.
void
outputGlfwError(int error, const char* description);

/******************************************************************/

int
main(int argc, char* argv[])
{
    GLFWwindow* window;
    init(window);

    // Game/render loop
    double previousTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
    double currentTime = glfwGetTime();
    // Compute frame times, which we can use later for frame rate computation,
    //   animation, and physics.
    double deltaTime = currentTime - previousTime;
    previousTime = currentTime;
    updateScene(deltaTime);
    drawScene(window);
    // Process events in the event queue, which results in callbacks
    //   being invoked.
    glfwPollEvents();
    }

    releaseGlResources();
    // Destroying the window destroys the OpenGL context
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

/******************************************************************/

void
init(GLFWwindow*& window)
{
    // Always initialize GLFW before GLEW
    initGlfw();
    initWindow(window);
    initShaders();
    initScene();
}

/******************************************************************/

void
initGlfw()
{
    glfwSetErrorCallback(outputGlfwError);
    if(!glfwInit())
    {
    fprintf(stderr, "Failed to init GLFW -- exiting\n");
    exit(EXIT_FAILURE);
    }
}

/******************************************************************/

void
initWindow(GLFWwindow*& window)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    // Necessary on MacOS!
    // Set forward compatibility bit
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Request core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    window = glfwCreateWindow(1200, 900, "VenusEngine", nullptr, nullptr);
    if(window == nullptr)
    {
    fprintf(stderr, "Failed to init the window -- exiting\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
    }
    glfwSetWindowPos(window, 200, 100);

    glfwMakeContextCurrent(window);
    // Swap buffers after 1 frame
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, processKeys);
    glfwSetFramebufferSizeCallback(window, resetViewport);

    initGlad();

    // Specify background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Enable depth testing so occluded surfaces aren't drawn
    glEnable(GL_DEPTH_TEST);
    // Enable the culling of back-facing triangles
    // A triangle must be wound CCW in window coordinates to be front facing
    // If it is wound CW it is back facing and will NOT be displayed!
    glEnable(GL_CULL_FACE);
    // The next two setting are default, but we'll be explicit.
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // Set initial viewport size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

/******************************************************************/

void
initGlad()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("failed to initialize GLAD");
    }
}

/******************************************************************/

void
resetViewport(GLFWwindow* window, int width, int height)
{
  // Render into entire window
  // Origin for window coordinates is lower-left of window
  glViewport(0, 0, width, height);
}

/******************************************************************/

void
initScene()
{
    // GENERAL GEOMETRY SETUP LOGIC
    // ============================
    // Generate vertex array objects
    // For each vertex array object:
    //   Bind it
    //   Generate buffer object
    //   Bind it
    //   Fill bound buffer with data
    //   For each vertex attribute:
    //     Enable the vertex attribute array
    //     Specify the attribute pointer(describes the data)
    //   Unbind vertex array object

    // These control how our C++ program communicates with the shaders
    const GLint POSITION_ATTRIB_INDEX = 0;
    const GLint COLOR_ATTRIB_INDEX = 1;

    // Creates VAOs in array
    glGenVertexArrays(NUMBER_OF_VAOS, g_vaos);

    //////////////////////////////
    // Set up triangle geometry
    glBindVertexArray(g_vaos[0]);
    GLuint triVbo;
    glGenBuffers(1, &triVbo);
    glBindBuffer(GL_ARRAY_BUFFER, triVbo);
    // 3 2D points, followed by 3 RGB colors
    std::vector<float> triVertices {
        0.0f, 0.5f,         // 2-d coordinates of first vertex(X, Y)
        -0.5f, -0.5f,       // 2-d coordinates of second vertex(X, Y)
        0.5f, -0.5f,        // 2-d coordinates of third vertex(X, Y)
        1.0f, 0.0f, 0.0f,   // color of first vertex(R, G, B)
        0.0f, 1.0f, 0.0f,   // color of second vertex(R, G, B)
        0.0f, 0.0f, 1.0f    // color of third vertex(R, G, B)
    };
  
    glBufferData(GL_ARRAY_BUFFER, triVertices.size() * sizeof(float),
	    triVertices.data(),
	    GL_STATIC_DRAW);
    // Tell the shaders how the data in the array is laid out
    glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
    // Positions have 2 parts, each are floats, and start at beginning of array
    glVertexAttribPointer(POSITION_ATTRIB_INDEX, 2, GL_FLOAT, GL_FALSE, 0,
			    reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(COLOR_ATTRIB_INDEX);
    // Colors have 3 parts, each are floats, and start at 7th position in array
    glVertexAttribPointer(COLOR_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 0,
			    reinterpret_cast<void*>(6 * sizeof(float)));
    glBindVertexArray(0);

    ///////////////////////////////////////////////////////////////////
    // Use a second VAO for point geometry
    glPointSize(50);
    glBindVertexArray(g_vaos[1]);
    GLuint pointVbo;
    glGenBuffers(1, &pointVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pointVbo);
    // 1 2D point, followed by 1 RGB color
    float pointVertices[] = {
    0.8f, 0.8f,         // 2-d coordinates of only vertex(X, Y)
    1.0f, 0.0f, 1.0f    // color of only vertex(R, G, B)
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), pointVertices,
	    GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
    // Positions have 2 parts, each are floats, and start at beginning of aray
    glVertexAttribPointer(POSITION_ATTRIB_INDEX, 2, GL_FLOAT, GL_FALSE, 0,
			    reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(COLOR_ATTRIB_INDEX);
    // Colors have 3 parts, each are floats, and start at 3rd position in array
    glVertexAttribPointer(COLOR_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 0,
			    reinterpret_cast<void*>(2 * sizeof(float)));

    glBindVertexArray(0);
}

/******************************************************************/

void
initShaders()
{
    // Create shader programs, which consist of linked shaders.
    // No need to use the program until we draw.
    g_shaderProgram = new ShaderProgram();
    g_shaderProgram->createVertexShader("../Core/Vec2.vert");
    g_shaderProgram->createFragmentShader("../Core/Vec2.frag");
    g_shaderProgram->link();
}

/******************************************************************/

void
updateScene(double time)
{
    // No updates in this simple program.
}

/******************************************************************/

void
drawScene(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Iterate over each object in scene and draw it

    // RENDER LOGIC
    // ============
    // For each drawable
    //   Use associated shader program
    //   Bind drawable-s VAO
    //   Set uniforms for shader program(if applicable)
    //   Issue draw call
    //   Unbind vertex array object
    //   Un-use shader program

    g_shaderProgram->enable();
    // Set shader uniforms if applicable(none here)
    glBindVertexArray(g_vaos[0]);
    // Draw triangle primitives, starting with index 0, and including
    //   3 indices
    // Other primitive types include GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
    //   GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, and GL_TRIANGLES(and a few others)
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Now draw the point
    glBindVertexArray(g_vaos[1]);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);

    g_shaderProgram->disable();

    // Swap the front and back buffers.
    // We draw to the back buffer, which is then swapped with the front
    //   for display.
    glfwSwapBuffers(window);
}

/******************************************************************/

void
processKeys(GLFWwindow* window, int key, int scanCode, int action,
	     int modifiers)
{
    // Exit if ESC is pressed
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

/******************************************************************/

void
releaseGlResources()
{
    // Delete OpenGL resources, particularly important if program will
    //   continue running
    delete g_shaderProgram;
    glDeleteVertexArrays(NUMBER_OF_VAOS, g_vaos);
}

/******************************************************************/

void
outputGlfwError(int error, const char* description)
{
    fprintf(stderr, "GLFW error: %s, code %d\n", description, error);
}

/******************************************************************/
