# Textured Sphere - Earth Simulation

This project showcases a textured sphere rendering in OpenGL, simulating the Earth. It includes texture loading, dynamic rotation, and an optional explosion effect to depict the planet's destruction.

## Compilation and Execution

Compile the program using the following command:

g++ Sphere.cpp -lGL -lGLU -lglut -lGLEW -o Sphere

Run the compiled executable:

./Sphere

## Features

- Texture Loading: Utilizes the STB image library to load a JPEG texture onto the sphere.
- Dynamic Rotation: Implements a rotation effect to spin the sphere, mimicking Earth's rotation.
- Explosion Effect: Enhances the simulation with an effect that makes the sphere seem to explode, showcasing vertex manipulation (optional).

## Requirements

- OpenGL
- GLUT
- GLEW
- STB Image (Header file included in the project)

## Implementation Details

- loadTexture(): Loads the texture from a JPEG file and applies it to the sphere.
- generateSphere(float radius, int segments, int rings): Generates the vertices and indices for a sphere mesh.
- display(): Renders the sphere with the applied texture and manages the rotation and explosion effects.
- update(int value): Updates the rotation angle and triggers scene redraw.
- main(): Initializes the OpenGL context, sets up the window, and enters the GLUT main loop.

## Customization

- Adjust the rotationAngle increment in update() to alter the rotation speed.
- To enable the explosion effect, integrate a factor that increases vertex displacement over time in the display() function.

## Notes

- Ensure that the Earth2.jpg texture file is in the same directory as the executable or update the path in loadTexture().
- The explosion effect is conceptual; for a more realistic appearance, consider employing advanced graphics techniques such as particle systems or shaders.

Authored by Daniel Saravia, 3/21/24


