#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gltf/tiny_gltf.h>


//float vertices[] = {
//    -0.5f, -0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     0.0f,  0.5f, 0.0f
//};

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

const char* vertex_shader_src = {
"#version 430 core\n"
"layout(location = 0) in vec3 attPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(attPos.x, attPos.y, attPos.z, 1.0);\n"
"}\n"
};

const char* frag_shader_src = {
"#version 430 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n"
};


bool check_compile_status(GLuint shader)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        GLint shader_type;
        glGetShaderiv(shader, GL_SHADER_TYPE, &shader_type);

        if (shader_type == GL_VERTEX_SHADER)
            std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILED:\n" << infoLog << std::endl;
        else if (shader_type == GL_FRAGMENT_SHADER)
            std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED:\n" << infoLog << std::endl;
    }

    return success;
}

bool check_link_status(GLuint shader_program)
{
    int  success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED:\n" << infoLog << std::endl;
    }

    return success;
}


int main(void)
{
    GLFWwindow* window;

    cs460::enable_glfw_debug_callbacks();

    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1280, 720, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    GLenum glewStatus = glewInit();

    if (glewStatus != GLEW_OK)
    {
        std::cout << "GLEW initialization failed\n";
        return 0;
    }

    // Enable v-sync
    cs460::enable_gl_debug_callbacks();
    glfwSwapInterval(1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::cout << glGetString(GL_VERSION) << std::endl;

    /*tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string errorStr;
    std::string warningStr;

    bool ret = loader.LoadASCIIFromFile(&model, &errorStr, &warningStr, "data/gltf/Sphere.gltf");
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

    if (!warningStr.empty())
        std::cout << "TINYGLTF WARNING: " << warningStr << std::endl;

    if (!errorStr.empty())
        std::cout << "TINYGLTF ERROR: " << errorStr << std::endl;


    if (!ret) {
        std::cout << "TINYGLTF: Failed to parse glTF\n";
        return -1;
    }*/


    // Generate and bind the Vertex Array Object to store the attribute and buffers configuration
    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the Vertex Buffer Object
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Upload the vertex data to the GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell opengl how to find the vertex attribute 0 in the buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Generate and bind the Element Buffer Object
    unsigned int ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // Upload the indices data to the GPU buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Create the vertex shader, copy the source code and compile it
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);

    // Check for vertex shader compiling errors
    check_compile_status(vertex_shader);
    
    // Create the fragment shader, copy the source code and compile it
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_src, nullptr);
    glCompileShader(frag_shader);

    // Check for fragment shader compiling errors
    check_compile_status(frag_shader);

    // Create the shader program object and attach the vertex and fragment objects to it
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);

    // Link the shaders in the shader program and check if the linking failed
    glLinkProgram(shader_program);
    check_link_status(shader_program);

    // Now that the program has been linked, dettach and delete the shaders as we no longer need them
    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, frag_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    // Bind the shader program so that we can draw
    glUseProgram(shader_program);
    

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the scene/do other logic
        glUseProgram(shader_program);
        glBindVertexArray(vao);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glUseProgram(0);
    glDeleteProgram(shader_program);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    return 0;
}