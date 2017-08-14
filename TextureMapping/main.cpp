/*
    TextureMapping proj
*/

#include <iostream>
#include <string.h>
#include <string>

// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS // suppresses a bunch of compiler warnings
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// CustomLibs
#include <GL_Utils/Shader.h>   // Shader objects
#include <GL_Utils/InitGlfw.h> // GLFW_Init(...)
#include <GL_Utils/Texture.h>  // Texture objects
#include <GL_Utils/Camera.h>   // Camera objects

#define ASPECT_RATIO ((float)window_width/(float)window_height)

const int window_width  = 640;
const int window_height = 480;

using namespace std;


int main(int argc, char* argv[]) {
    // initialization functions
    GLFWwindow* window = GLFW_Init(window_width, window_height, "HelloGLFW", 3, 3, GL_TRUE);
    TextureLoader::InitIL(); // allows building of textures later on

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LESS);

    string res_loc("../assets/shader/"); // prefix for shaders

    Shader shader_color(res_loc, "vtx_color.glsl", "frag_color.glsl");
    Shader shader_frag(res_loc, "vtx_texture.glsl", "frag_texture.glsl");



    return 0;
}

