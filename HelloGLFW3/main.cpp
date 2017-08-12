#include <iostream>
#include <unistd.h>

// OpenGL includes
#include <GL/glew.h> // this needs to be included before glfw3 headers
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <GL_Utils/Shader.h>

using namespace std;

int main(int argc, char* argv[]) {
    if(!glfwInit()) { // init GLFW3
        cerr << "Failed to initialize GLFW3" << endl;
        return -1;
    } else {
        cout << "GLFW3! ";
    }

    GLFWwindow* window = glfwCreateWindow(480, 640, "HelloGLFW3", NULL, NULL);

    if(!window) {
        cerr << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    } else {
        cout << "WINDOW! ";
    }

    glfwMakeContextCurrent(window); // init GLEW
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        return -1;
    } else {
        cout << "GLEW! ";
    }

    cout << "GO!" << endl;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // vertex array object setup
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID); // make this VAO the current one

    // triangle vertex setup
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    // give the triangle to OpenGL
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint shaderProgram = ShaderGenerator::createProgram("shader/vtx_shader.glsl", "shader/frag_shader.glsl");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // draw our triangle
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
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}
