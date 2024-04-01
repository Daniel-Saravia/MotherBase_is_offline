// Daniel Saravia
// g++ Sphere.cpp -lGL -lGLU -lglut -lGLEW -o Sphere
// 3/21/24
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
// Angle for rotating the sphere
float rotationAngle = 0.0f;

// Initial FOV
float fieldOfView = 60.0f; 

void reshape(int width, int height); // Forward declaration

// Vertex structure to hold position and texture coordinates
struct Vertex
{
    float x, y, z; // Position coordinates
    float u, v;    // Texture coordinates
};

std::vector<Vertex> vertices; // Container for all vertices of the sphere
std::vector<int> indices;     // Indices for drawing the sphere vertices
GLuint textureID;             // Texture ID for OpenGL

// Constants for sphere generation and math calculations
const float PI = 3.14159265358979323846f; // Mathematical constant PI
int segments = 64;                        // Number of segments for sphere longitude
int rings = 32;                           // Number of rings for sphere latitude
float radius = 2.0f;                      // Radius of the sphere

// Function to load the texture from an image file
void loadTexture()
{
    int width, height, channels;
    // Load the texture image data
    unsigned char *data = stbi_load("Earth.jpg", &width, &height, &channels, 0);

    if (data) // Check if the image data was loaded successfully
    {
        glGenTextures(1, &textureID);            // Generate a texture ID
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the texture as a 2D texture

        // Set texture wrapping and filtering modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture into OpenGL depending on the number of channels
        if (channels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (channels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps for the texture

        stbi_image_free(data); // Free the image data memory
    }
    else
    {
        std::cerr << "Error loading texture." << std::endl;
        exit(1); // Exit if there is an error loading the texture
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '+': // Zoom in
            fieldOfView -= 5;
            if (fieldOfView < 10) fieldOfView = 10; // Limit zoom in
            break;
        case '-': // Zoom out
            fieldOfView += 5;
            if (fieldOfView > 120) fieldOfView = 120; // Limit zoom out
            break;
    }
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); // Update the projection
}



// Use the fieldOfView variable to set the perspective in the reshape function.
void reshape(int width, int height)
{
    glViewport(0, 0, width, height); // Set the viewport to the new window size
    glMatrixMode(GL_PROJECTION);     // Change to projection matrix mode
    glLoadIdentity();                // Reset the projection matrix
    gluPerspective(fieldOfView, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
}


// Function to generate the vertices and indices for a sphere
void generateSphere(float radius, int segments, int rings)
{
    // Loop over latitude (rings)
    for (int lat = 0; lat <= rings; lat++)
    {
        float latAngle = PI * lat / rings; // Calculate the latitude angle
        float v = (float)lat / rings;      // Calculate the texture coordinate V

        // Loop over longitude (segments)
        for (int lon = 0; lon <= segments; lon++)
        {
            float lonAngle = 2 * PI * lon / segments; // Calculate the longitude angle
            float u = 1.0f - ((float)lon / segments); // Calculate the texture coordinate U, flipped horizontally

            // Calculate the vertex position based on spherical coordinates
            float x = radius * sin(latAngle) * cos(lonAngle);
            float y = radius * cos(latAngle);
            float z = radius * sin(latAngle) * sin(lonAngle);

            vertices.push_back({x, y, z, u, v}); // Add the vertex to the list
        }
    }

    // Generate indices for drawing the sphere with triangles
    for (int lat = 0; lat < rings; lat++)
    {
        for (int lon = 0; lon < segments; lon++)
        {
            int first = lat * (segments + 1) + lon;
            int second = first + segments + 1;

            // Define the vertices of each triangle
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

// Function to display the scene
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers
    glEnable(GL_TEXTURE_2D);                            // Enable texture mapping
    glBindTexture(GL_TEXTURE_2D, textureID);            // Bind the texture

    glMatrixMode(GL_MODELVIEW);           // Switch to modelview matrix mode
    glLoadIdentity();                     // Reset the modelview matrix
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0); // Set the camera position and orientation

    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Apply rotation to the sphere

    glBegin(GL_TRIANGLES); // Begin drawing triangles
    for (int i = 0; i < indices.size(); i++)
    {
        const Vertex &v = vertices[indices[i]]; // Get the vertex
        glTexCoord2f(v.u, v.v);                 // Set texture coordinates
        glVertex3f(v.x, v.y, v.z);              // Draw vertex
    }
    glEnd(); // End drawing triangles

    glutSwapBuffers(); // Swap the front and back buffers
}

// Function to initialize OpenGL settings
void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);        // Set the clear color for the background
    glEnable(GL_DEPTH_TEST);                 // Enable depth testing for 3D rendering
    loadTexture();                           // Load the texture for the sphere
    generateSphere(radius, segments, rings); // Generate the sphere data
}

// Function called at regular intervals to update the scene
void update(int value)
{
    rotationAngle += 0.8f;      // Increase the rotation angle to rotate the sphere
    if (rotationAngle > 360.0f) // Keep the angle within a 0-360 range
    {
        rotationAngle -= 360.0f;
    }

    glutPostRedisplay();          // Tell GLUT to redraw the screen
    glutTimerFunc(16, update, 0); // Re-register the update function to maintain a regular interval (~60 FPS)
}

// The main function where execution begins
int main(int argc, char **argv)
{
    std::cout << "Press + to zoom in and - to zoom out:" << std::endl;
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Set the display mode to double buffering, RGB color, and depth buffering
    glutInitWindowSize(2000, 2000); // Set the initial window size
    glutCreateWindow("Textured Sphere - Earth"); // Create the window with a title

    // Initialize GLEW, a library to manage OpenGL extensions
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return -1; // Return an error code if GLEW initialization fails
    }

    init(); // Call the initialization function

    // Set callback functions for display, window reshape, keyboard events, and updates
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); // Register keyboard callback for zooming
    glutTimerFunc(16, update, 0); // Register the update function to start the animation

    glutMainLoop(); // Start the GLUT event processing loop

    return 0; // Return 0 to indicate successful execution
}

