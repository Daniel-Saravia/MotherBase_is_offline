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
float rotationAngle = 0.0f;


struct Vertex
{
    float x, y, z, u, v;
};

std::vector<Vertex> vertices;
std::vector<int> indices;
GLuint textureID;

const float PI = 3.14159265358979323846f;
int segments = 64;
int rings = 32;
float radius = 2.0f;

void loadTexture()
{
    int width, height, channels;
    unsigned char *data = stbi_load("Earth.jpg", &width, &height, &channels, 0);
    if (data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (channels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Error loading texture." << std::endl;
        exit(1);
    }
}

void generateSphere(float radius, int segments, int rings)
{
    for (int lat = 0; lat <= rings; lat++)
    {
        float latAngle = PI * lat / rings;
        float v = (float)lat / rings;

        for (int lon = 0; lon <= segments; lon++)
        {
            float lonAngle = 2 * PI * lon / segments;
            float u = 1.0f - ((float)lon / segments); // Flipping the texture horizontally

            float x = radius * sin(latAngle) * cos(lonAngle);
            float y = radius * cos(latAngle);
            float z = radius * sin(latAngle) * sin(lonAngle);

            vertices.push_back({x, y, z, u, v});
        }
    }

    for (int lat = 0; lat < rings; lat++)
    {
        for (int lon = 0; lon < segments; lon++)
        {
            int first = lat * (segments + 1) + lon;
            int second = first + segments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // Apply rotation
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < indices.size(); i++)
    {
        const Vertex &v = vertices[indices[i]];
        glTexCoord2f(v.u, v.v);
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();

    glutSwapBuffers();
}



void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    loadTexture();
    generateSphere(radius, segments, rings);
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
}

void update(int value)
{
    rotationAngle += 0.8f; // Adjust this value for speed
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }

    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, update, 0); // Re-register the update function to run every ~16ms (about 60 FPS)
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(2000, 2000);
    glutCreateWindow("Textured Sphere - Earth");

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return -1;
    }

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0); // Register the update function to start the animation
    glutMainLoop();

    return 0;
}
