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

#define ASPECT_RATIO ((float)window_width/(float)window_height)

const int window_width  = 640;
const int window_height = 480;

using namespace std;

int main(int argc, char* argv[]) {
    // initialization functions
    GLFWwindow* window = GLFW_Init(window_width, window_height, "TextureMapping", 3, 3, GL_TRUE);
    TextureLoader::InitIL(); // allows building of textures later on

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    string res_loc("../assets/shader/"); // prefix for shaders
    string img_loc("../assets/img/");    // prefix for images

    Util::Shader shader_color(res_loc, "vtx_color.glsl", "frag_color.glsl");
    Util::Shader shader_texture(res_loc, "vtx_texture.glsl", "frag_texture.glsl");

    //Util::Texture IMG_plant(img_loc, "shaggy-this-isnt-weed.jpg");
    Util::Texture IMG_plant(img_loc, "is-that-chicken.jpg");
    IMG_plant.bind();

    Util::Texture IMG_grass(img_loc, "grass-1.jpg");

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    Util::Camera cam;
    cam.setLocation(glm::vec3(1.0f, 0.0f, 3.0f));
    cam.setLookingAt(glm::vec3(0.0f, 0.0f, 0.0f));

    // triangle vertex setup
    static const GLfloat g_vertex_buffer_data[] = {
        // first triangle
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        // second triangle
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
    };

    // triangle color setup
    static GLfloat g_color_buffer_data[9]; // one RGB triplet per vertex
    for(int i = 0; i < 9; i++)
        g_color_buffer_data[i] = randColor(); // fill with new random colors every time

    // texture coordinates setup
    static GLfloat g_uv_buffer_data[12] = {
        // first triangle
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        // second triangle
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    // give various bits of data to OpenGL
    Util::VertexArrayObject vao;
    vao.bind();
        // position data
        Util::VertexBufferObject vbo_pos;
        vbo_pos.bind();
        vbo_pos.bufferData(g_vertex_buffer_data, 18*sizeof(GLfloat));
        vbo_pos.setAttribPtrData(0, 3, GL_FLOAT);

        // color data
        Util::VertexBufferObject vbo_col;
        vbo_col.bind();
        vbo_col.bufferData(g_color_buffer_data, 9*sizeof(GLfloat));
        vbo_col.setAttribPtrData(1, 3, GL_FLOAT);

        // uv coordinate data
        Util::VertexBufferObject vbo_uv;
        vbo_uv.bind();
        vbo_uv.bufferData(g_uv_buffer_data, 12*sizeof(GLfloat));
        vbo_uv.setAttribPtrData(2, 2, GL_FLOAT);

    // setup ModelViewProjection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, 0.1f, 100.0f);
    glm::mat4 View = cam.getViewTf();
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 mvp = Projection * View * Model; // final MVP matrix

    GLuint MatrixID       = shader_texture.getUniformLocation("MVP");
    GLuint SamplerID      = shader_texture.getUniformLocation("TEX");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.bind();

        shader_texture.bind();

        // send transformation to the currently bound shader (MVP uniform)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glActiveTexture(GL_TEXTURE0);
        IMG_plant.bind();
        glUniform1i(SamplerID, 0);

        vao.enableAttribute(0);
        vbo_pos.bind();
        vbo_pos.generateAttribPointer();

        vao.enableAttribute(1);
        vbo_col.bind();
        vbo_col.generateAttribPointer();

        vao.enableAttribute(2);
        vbo_uv.bind();
        vbo_uv.generateAttribPointer();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader_color.bind();

        vao.disableAttribute(0);
        vao.disableAttribute(1);
        vao.disableAttribute(2);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}

