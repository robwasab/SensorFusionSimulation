//
//  hello_triangles_main.c
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/23/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct DrawableArrays
{
    GLuint vertex_buffer_object;
    GLuint vertex_attrib_object;
    GLuint shader_program;
};

struct DrawableElements
{
    GLuint element_buffer_object;
    GLuint vertex_buffer_object;
    GLuint vertex_attrib_object;
    GLuint shader_program;
};

static void framebuffer_size_callback(GLFWwindow * window, int width, int height);
static void processInput(GLFWwindow * window);
static void initialize_triangle(struct DrawableArrays * triangle);
static void read_shader_file(const char * filename, char * buffer, size_t len);
static GLuint compile_shader_source(const char * filename, GLenum shader_source_type);
static GLuint link_shaders_to_program(GLuint vertex_shader, GLuint fragment_shader);
static GLuint load_shader_program(void);
static void draw_triangle(struct DrawableArrays * obj);
static void initialize_rectangle(struct DrawableElements * obj);
static void draw_rectangle(struct DrawableElements * obj);

int main(int argc, const char * argv[])
{
    // initialize glfw windowing framework
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow * window;
    window = glfwCreateWindow(800, 600, "2_HelloTriangle", NULL, NULL);
    
    if(window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    printf("Successfully initialized glfw!\n");
    
    // initialize glad function pointer manager
    if(0 == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // now we can talk to opengl
    glViewport(0, 0, 800, 600);
    
    // dark green bluish background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    struct DrawableArrays triangle = {0};
    initialize_triangle(&triangle);
    
    struct DrawableElements rectangle = {0};
    initialize_rectangle(&rectangle);
    
    while(GLFW_FALSE == glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);
        
        draw_triangle(&triangle);
        draw_rectangle(&rectangle);
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}

/**
 * Called when the framebuffer of the window has changed - window resize
 */
static void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
 * Called at the beginning of each frame refresh iteration to check user input.
 */
static void processInput(GLFWwindow * window)
{
    if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void draw_triangle(struct DrawableArrays * obj)
{
    glUseProgram(obj->shader_program);
    glBindVertexArray(obj->vertex_attrib_object);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

static void draw_rectangle(struct DrawableElements * obj)
{
    glUseProgram(obj->shader_program);
    glBindVertexArray(obj->vertex_attrib_object);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

static void initialize_rectangle(struct DrawableElements * obj)
{
    GLfloat vertices[] =
    {
        0.5, 0.5, 0.0,  // top right
        0.5,-0.5, 0.0,  // bot right
       -0.5,-0.5, 0.0,  // bot left
       -0.5, 0.5, 0.0,  // top left
    };
    
    GLuint indecies[] =
    {
        0, 1, 3,    // first triangle
        1, 2, 3,    // second triangle
    };
    
    GLuint shader_program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(/*vertex attr index*/ 0,
                          /*num components*/ 3,
                          /*data ype*/ GL_FLOAT,
                          /*normalized*/ GL_FALSE,
                          /*stride*/ sizeof(float) * 3,
                          /*offset*/ (void*) 0);

    shader_program = load_shader_program();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    obj->vertex_attrib_object = vao;
    obj->vertex_buffer_object = vbo;
    obj->element_buffer_object = ebo;
    obj->shader_program = shader_program;
}

/**
 * Called at program initialization once to load the vertices into the GPU.
 */
static void initialize_triangle(struct DrawableArrays * obj)
{
    float vertices[] =
    {
        -0.5f, -0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f,
        +0.0f, +0.5f, +0.0f,
    };
    
    // Create vertex attribute object.
    glGenVertexArrays(1, &obj->vertex_attrib_object);
    
    // Bind the newly created object so subsequent operations are performed on it.
    glBindVertexArray(obj->vertex_attrib_object);
    
    glGenBuffers(1, &obj->vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertex_buffer_object);
    
    // After binding vertex_buffer_object to GL_ARRAY_BUFFER, any calls made
    // using GL_ARRAY_BUFFER as the target will affect the bound buffer -
    // vertex_buffer_object.
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // load and compile the vertex and fragment shaders
    obj->shader_program = load_shader_program();
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(/*vertex attr index*/ 0,
                          /*num components*/ 3,
                          /*data ype*/ GL_FLOAT,
                          /*normalized*/ GL_FALSE,
                          /*stride*/ sizeof(float) * 3,
                          /*offset*/ (void*) 0);

    // unbind the attribute from the target.
    glBindVertexArray(0);

    // unbind the buffer from the target.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static GLuint load_shader_program(void)
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;
    
    char vertex_shader_filename[] =
    "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/2_hello_triangles_vertex_shader.fsh";
    
    vertex_shader = compile_shader_source(vertex_shader_filename, GL_VERTEX_SHADER);
    
    char fragment_shader_filename[] =
    "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/2_hello_triangles_fragment_shader.fsh";
    
    fragment_shader = compile_shader_source(fragment_shader_filename, GL_FRAGMENT_SHADER);
    
    shader_program = link_shaders_to_program(vertex_shader, fragment_shader);
    
    // delete shader objects once we've linked them into the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

static GLuint link_shaders_to_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint shader_program;
    shader_program = glCreateProgram();
    
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    char info_log[512] = {0};
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(0 == success)
    {
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        printf("------------------\n");
        printf("PROGRAM LINK ERROR\n");
        printf("------------------\n");
        printf("%s\n", info_log);
        exit(-1);
    }
    
    printf("Program link success!\n");
    
    return shader_program;
}

static GLuint compile_shader_source(const char * filename, GLenum shader_source_type)
{
    GLuint shader_id;
    GLchar info_log[512];
    GLchar shader_source[512];

    read_shader_file(filename, shader_source, sizeof(shader_source));
    
    shader_id = glCreateShader(shader_source_type);
    
    const GLchar * shader_source_ptr = shader_source;
    
    glShaderSource(shader_id, 1, &shader_source_ptr, NULL);
    glCompileShader(shader_id);
    
    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    
    if(0 == success)
    {
        glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
        printf("------------------------\n");
        printf("SHADER COMPILATION ERROR\n");
        printf("------------------------\n");
        printf("%s\n", info_log);
        exit(-1);
    }
    
    printf("Compile success!\n");
    return shader_id;
}

static void read_shader_file(const char * filename, char * buffer, size_t len)
{
    FILE * shader_file = fopen(filename, "r");
    assert(shader_file != NULL);
    
    memset(buffer, 0, len);
    
    size_t bytes_read = fread(buffer, len, 1, shader_file);
    
    assert(bytes_read <= len);
    
    printf("--------------------\n");
    printf("SHADER FILE CONTENTS\n");
    printf("--------------------\n");
    printf("%s\n", buffer);
}

