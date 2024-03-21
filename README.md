# Textured Sphere - Earth Simulation

This project demonstrates a textured sphere rendering in OpenGL, simulating the Earth. It features texture loading, dynamic rotation, and an explosion effect to simulate the planet's destruction.

## Compilation and Running

To compile the program, use the following command:

```bash
g++ Sphere.cpp -lGL -lGLU -lglut -lGLEW -o Sphere

## Run the compiled executable with:

```bash
./Sphere

## Features

* Texture Loading: Utilizes STB image library to load a JPEG texture onto the sphere.
* Dynamic Rotation: Implements a rotation effect to spin the sphere, simulating the Earth's rotation.
* Explosion Effect: (Optional) Enhances the simulation with an effect that makes the sphere appear to explode, demonstrating vertex manipulation.

## Requirements

* OpenGL
* GLUT
* GLEW
* STB Image (Header file included in the project)

## Implementation Details

* loadTexture(): Loads the texture from a JPEG file and applies it to the sphere.
* generateSphere(float radius, int segments, int rings): Generates the vertices and * indices for a sphere mesh.
* display(): Renders the sphere with the applied texture and handles the rotation and explosion effects.
* update(int value): Updates the rotation angle and triggers the redraw of the scene.
* main(): Initializes the OpenGL context, sets up the window, and enters the GLUT main loop.

## Customization

* Modify the rotationAngle increment in update() to change the rotation speed.
* To enable the explosion effect, integrate a factor that increases vertex displacement over time in the display() function.

## Notes

* Ensure that the Earth2.jpg texture file is present in the same directory as the executable or adjust the path in loadTexture() accordingly.
* The explosion effect is conceptual and can be enhanced with more sophisticated graphics techniques like particle systems or shaders for a more realistic appearance.

Written by Daniel Saravia, 3/21/24

