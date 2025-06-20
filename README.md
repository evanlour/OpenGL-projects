# Showcasing the OpenGL basic capabilities
This repository is a small chunk of independent projects in order to familiarize with the OpenGL graphics library. 
## How to compile the showcase programs
Using a UNIX environment, from the main folder run the following:
- g++ -g Showcase1/src/showcase1.cpp -o Showcase1/src/showcase1 -lglfw -lGL -lGLEW
- g++ -g Showcase2/src/showcase21.cpp Showcase2/src/imgui/*.cpp -o Showcase2/src/showcase21 -lglfw -lGL -lGLEW
- g++ -g Showcase2/src/showcase22.cpp Showcase2/src/imgui/*.cpp -o Showcase2/src/showcase22 -lglfw -lGL -lGLEW
- g++ -g Showcase2/src/showcase23.cpp Showcase2/src/imgui/*.cpp -o Showcase2/src/showcase23 -lglfw -lGL -lGLEW
- g++ -g Showcase2/src/showcase24.cpp Showcase2/src/imgui/*.cpp -o Showcase2/src/showcase24 -lglfw -lGL -lGLEW
- g++ -g Showcase3/src/showcase3.cpp Showcase3/src/imgui/*.cpp -o Showcase3/src/showcase3 -lglfw -lGL -lGLEW
After than, you can run each program and test its function.

The showcases start from quite a low level, rendering basic triangles, to outputing multiple moving objects, each bound to multiple static and moving, point and directional lights. You can run each program from its corresponding directory.