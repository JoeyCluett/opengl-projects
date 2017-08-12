#include <iostream>
#include <unistd.h>

// OpenGL includes
#include <GL/glew.h> // this needs to be included before glfw3 headers
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS // suppresses a bunch of compiler warnings
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// CustomLibs
#include <GL_Utils/Shader.h>   // ShaderGenerator::*
#include <GL_Utils/InitGlfw.h> // GLFW_Init(...)

#define ASPECT_RATIO ((float)window_width/(float)window_height)

using namespace std;

const int window_width  = 640;
const int window_height = 480;

int main(int argc, char* argv[]) {
    GLFWwindow* window = GLFW_Init(640, 480, "HelloGLFW", 3, 3, GL_TRUE);

    // vertex array object setup
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID); // make this VAO the current one

    GLuint shaderProgram;

    try {
        //GLuint shaderProgram = ShaderGenerator::createProgram("shader/vtx_shader.glsl", "shader/frag_shader.glsl");
        shaderProgram = ShaderGenerator::createProgram("shader/vtx_color.glsl", "shader/frag_shader.glsl");
    } catch(ifstream::failure e) {
        cout << e.what() << endl;
        return -1;
    }

    // hand the MVP matrix to GLSL
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");

    // triangle vertex setup
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    static const GLfloat g_color_buffer_data[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // triangle color setup
    //static GLfloat g_color_buffer_data[9]; // one RGB triplet per vertex
    //for(int i = 0; i < 9; i++)
        //g_color_buffer_data[i] = randColor(); // fill with new random colors every time

    // give the triangle to OpenGL
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // give color data to OpenGL
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // setup ModelViewProjection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), ASPECT_RATIO, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(4, 3, 3), // position
            glm::vec3(0, 0, 0), // looking at
            glm::vec3(0, 1, 0)  // up
    );
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 mvp = Projection * View * Model; // final MVP matrix

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // send transformation to the currently bound shader (MVP uniform)
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

        // 1st attribute: vertices
        glEnableVertexAttribArray(0); // zero is arbitrary but must match everywhere its used
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,        // attribute 0
                3,        // size
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0,        // stride
                (void*)0  // array buffer offset
        );

        // 2nd attribute: colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,        // attribute 1
                3,        // size
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0,        // stride
                (void*)0  // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // disable every enabled vertex attribute
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    // delete various stuff from OpenGL
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VertexArrayID);

    glfwTerminate();
    return 0;
}
