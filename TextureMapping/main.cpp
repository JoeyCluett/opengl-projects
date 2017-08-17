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
#include <GL_Utils/VAO.h>      // VertexArrayObject objects
#include <GL_Utils/VBO.h>      // VertexBufferObject objects
#include <GL_Utils/FloatCam.h> // FloatCam objects (floating camera)

// Model(s) in CustomLibs
#include <GL_Utils/Model/Panel.h>
#include <GL_Utils/Model/SimpleCube.h>

#include <MISC/TimeClass.h>
#include "proj-tool.h"

#define ASPECT_RATIO ((float)window_width/(float)window_height)

#define MAP_SCALE_X 26.7734
#define MAP_SCALE_Y 52.40

const int window_width  = 800;
const int window_height = 600;

using namespace std;

int main(int argc, char* argv[]) {
    // initialization functions
    GLFWwindow* window = GLFW_Init(window_width, window_height, "TextureMapping", 3, 3, GL_TRUE, GL_FALSE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    TextureLoader::InitIL(); // allows building of textures later on

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    string res_loc("../assets/shader/"); // prefix for shaders
    Util::Shader shader_texture(res_loc, "vtx_texture.glsl", "frag_texture.glsl");
    //Util::Shader shader_texture(res_loc, "vtx-track-intersect.glsl", "frag-track-intersect.glsl");

    string img_loc("../assets/img/");    // prefix for images
    //Util::Texture IMG_grass(  img_loc, "grass-1.jpg");
    Util::Texture IMG_grass(  img_loc, "minecraft-dirt-top.png");
    Util::Texture IMG_weed(   img_loc, "shaggy-this-isnt-weed.jpg");
    Util::Texture IMG_chicken(img_loc, "is-that-chicken.jpg");
    Util::Texture IMG_os(     img_loc, "os_users_as_seen_by.jpg");
    Util::Texture IMG_brakes( img_loc, "oil-your-brakes.jpg");
    Util::Texture IMG_ground( img_loc, "ndsu_map.jpg");

    Util::Texture IMG_minecraft_top (img_loc, "minecraft-dirt-top.png");
    Util::Texture IMG_minecraft_side(img_loc, "minecraft-dirt-side.jpg");
    Util::Texture IMG_minecraft_bott(img_loc, "minecraft-dirt-bottom.jpg");

    // needed for Model/View/Projection transformation
    glm::mat4 Projection = glm::perspective(glm::radians(60.0f), ASPECT_RATIO, 0.1f, 100.0f);

    Util::FloatCam fCamera(
            glm::vec3(0, 2, 3),
            6.0,           // movement speed
            window_width,  // window width
            window_height, // window height
            0.11,          // mouse sensitivity
            window         // ptr to GLFWwindow
    );
    fCamera.setOrientation(M_PI, 0.0f);

    GLfloat ndsu_uv[8] = {
        0.448, 0.315,
        0.8,   0.315,
        0.8,   0.805,
        0.443, 0.803
    };

    Model::Panel ground_panel;
    ground_panel.setRotation(M_PI/-2.0f, 1, 0, 0);
    //ground_panel.setTranslation(MAP_SCALE_X/2, 0, MAP_SCALE_Y/2);
    ground_panel.setScale(MAP_SCALE_X, MAP_SCALE_Y, 1);
    ground_panel.setShaderTexture(shader_texture, IMG_ground);
    ground_panel.setUVCoordinates(ndsu_uv);
    ground_panel.finalize();


    Model::SimpleCube cube_1(10, 10, 10);
    cube_1.applyTranslation(0, 8, 0);
    cube_1.setTranslation(0, 0, 0);
    cube_1.setShader(shader_texture);
    cube_1.setTextureTop(IMG_minecraft_top);
    cube_1.setTextureSides(IMG_minecraft_side);
    cube_1.setTextureBottom(IMG_minecraft_bott);
    cube_1.finalize();

    TimeClass tc;

    do {
        glm::vec3 cPos = fCamera.getPosition();
        glm::vec3 cDir = fCamera.getDirection();

        double delta_time = tc.getElapsedSecondsUpdate();
        static double cube_rot = 0.0;
        cube_rot += delta_time;
        cube_1.setRotation(cube_rot, 1, 0, 0);

        if(cPos.y > 0.1)
            fCamera.update((float)delta_time);
        else
            fCamera.setPosition(glm::vec3(cPos.x, 0.15, cPos.z));

        glm::vec3 pt_i = Proj_Tools::getIntersect(cPos, cDir);

        ground_panel.setVec4Uniform(glm::vec4(pt_i.x, pt_i.y, pt_i.z, 1), 0);

        glm::mat4 View = fCamera.getTf();

        ground_panel.setViewProjection(View, Projection);
        cube_1.setViewProjection(View, Projection);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: render a sphere where the line intersects the ground

        ground_panel.render();
        cube_1.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}

