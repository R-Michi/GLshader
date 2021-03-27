#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

bool init_OpenGL(GLFWwindow** window, int sizex, int sizey, int posx, int posy)
{
    glfwInit();
    glfwWindowHint(GLFW_RED_BITS, 24);              // set red bits
    glfwWindowHint(GLFW_GREEN_BITS, 24);            // set green bits
    glfwWindowHint(GLFW_BLUE_BITS, 24);             // set blue bits
    glfwWindowHint(GLFW_ALPHA_BITS, 24);            // set alpha bits
    glfwWindowHint(GLFW_DEPTH_BITS, 24);            // set depth bits
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // use OpenGL 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create window
    * window = glfwCreateWindow(sizex, sizey, "Shading test", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return false;
    }
    glfwSetWindowPos(*window, posx, posy);
    glfwMakeContextCurrent(*window);

    glfwSetCursorPos(*window, sizex / 2 + posx, sizey / 2 + posy);      // set mousepos to the center
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);   // disable cursor

    glfwSwapInterval(0);
    glewInit();
    return true;
}

int main()
{
    constexpr int win_width = 1920 / 2, win_height = 1080 / 2;
    constexpr int win_posx = 1920 / 4, win_posy = 1080 / 4;
    int width, height;

    GLFWwindow* window;
    init_OpenGL(&window, win_width, win_height, win_posx, win_posy);

    gl::Shader shader;
    gl::ShaderLoadError error = shader.load("test_vertex_shader.txt", "test_fragment_shader.txt");

    if(error & gl::ShaderLoadErrorType::VERTEX_SHADER_ERROR)
        std::cout << "Vertex Shader: " << shader.get_last_vertex_msg() << std::endl;
    if(error & gl::ShaderLoadErrorType::FRAGMENT_SHADER_ERROR)
        std::cout << "Fragment Shader: " << shader.get_last_fragment_msg() << std::endl;
    if(error & gl::ShaderLoadErrorType::SHADER_LINK_ERROR)
        std::cout << "Linker: " << shader.get_last_link_msg() << std::endl;

    float quad_vertices[12] = {
        0.5f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f
    };

    // generate buffer objects
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // generate vertex array objects
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float angle = 0.0f;

    while(!glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &width, &height);
        // if window height is 0 it will cause division by 0
        if(height < 1)
        {
            height = 1;
            glfwSetWindowSize(window, width, height);
        }
        const float size_ratio = (float)win_height / (float)height;
        const float aspect_ratio = (float)width / (float)height;

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f * aspect_ratio, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 resize_mat(1.0f);
        resize_mat = glm::scale(resize_mat, glm::vec3(size_ratio, size_ratio, 1.0f));
        glm::mat4 projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 mvp = projection * (model * resize_mat);

        //angle += 1.5f;

        glViewport(0, 0, width, height);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.uniform_4f("obj_color", 0.0f, 0.0f, 1.0f, 1.0f);
        shader.uniform_matrix_4x4f("mvp", 1, false, glm::value_ptr(mvp));
        shader.uniform_1f("size_ratio", size_ratio);
        glBindVertexArray(vao);
        glDrawArrays(GL_QUADS, 0, sizeof(quad_vertices) / 3);
        glBindVertexArray(0);
        shader.unuse();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
