/* jd_gfx.h */

// ==========================================================
// JD_GFX Introduction
// ==========================================================

/**
 * @file jd_gfx.h
 * @brief Single file header, abstracting and wrapping GLFW API for simplicity.
 *
 * This header file provides an abstraction layer over GLFW to simplify its usage
 * in graphics programming. It aims to reduce the complexity of using GLFW directly
 * and to make the code more readable and maintainable.
 *
 * Usage:
 * Define JD_GFX_IMPLEMENTATION in one .c/.cpp file before including this header file
 * to create the implementation in that file.
 */

#define JD_DEBUG_IMPLEMENTATION

#ifndef JD_GFX_H
#define JD_GFX_H

#ifdef JD_GFX_IMPLEMENTATION
   #define JD static // Define functions as static in implementation files
#else
   #define JD extern // Define functions as extern in header files
#endif

#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jd_debug.h" // Debug utilities

// ==========================================================
// Colors
// ==========================================================

#define LIGHTGRAY  (Color){ 211, 211, 211, 255 }   // Light Gray
#define GRAY       (Color){ 128, 128, 128, 255 }   // Gray
#define DARKGRAY   (Color){ 169, 169, 169, 255 }   // Dark Gray
#define YELLOW     (Color){ 255, 210, 101, 255 }   // Yellow
#define GOLD       (Color){ 255, 223, 0, 255 }     // Gold
#define ORANGE     (Color){ 241, 156, 101, 255 }   // Orange
#define PINK       (Color){ 255, 105, 180, 255 }   // Pink
#define RED        (Color){ 206, 77, 69, 255 }     // Red
#define MAROON     (Color){ 128, 0, 0, 255 }       // Maroon
#define GREEN      (Color){ 42, 168, 118, 255 }    // Green
#define LIME       (Color){ 0, 255, 0, 255 }       // Lime
#define DARKGREEN  (Color){ 0, 100, 0, 255 }       // Dark Green
#define SKYBLUE    (Color){ 135, 206, 235, 255 }   // Sky Blue
#define BLUE       (Color){ 0, 0, 255, 255 }       // Blue
#define DARKBLUE   (Color){ 0, 0, 139, 255 }       // Dark Blue
#define PURPLE     (Color){ 128, 0, 128, 255 }     // Purple
#define VIOLET     (Color){ 238, 130, 238, 255 }   // Violet
#define DARKPURPLE (Color){ 148, 0, 211, 255 }     // Dark Purple
#define BEIGE      (Color){ 245, 245, 220, 255 }   // Beige
#define BROWN      (Color){ 165, 42, 42, 255 }     // Brown
#define DARKBROWN  (Color){ 101, 67, 33, 255 }     // Dark Brown

#define WHITE      (Color){ 255, 255, 255, 255 }   // White
#define BLACK      (Color){ 0, 0, 0, 255 }         // Black
#define BLANK      (Color){ 0, 0, 0, 0 }           // Blank (Transparent)

// ==========================================================
// Type Definitions and Enums
// ==========================================================

/* GLFW window type alias */
typedef GLFWwindow jd_window;

/* Context for window rendering and management */
typedef struct WindowContext {
    jd_window* window;                           // GLFW window instance
    void (*render_function)(jd_window*);         // Function pointer for rendering
    struct WindowContext* next;                  // Pointer to the next context (linked list)
} WindowContext;

/* Global head pointer for window contexts */
JD WindowContext* jd_window_context_head = NULL; // Global head for the list of window contexts

/* Operations for managing windows */
typedef struct {
    jd_window* (*create)(const char *title, int width, int height, void (*renderFunc)(jd_window*));
    int (*should_close)(jd_window *window);
    void (*destroy)(jd_window *window);
} jd_gfx_window_operations;

/* Operations for initializing and terminating graphics */
typedef struct {
    void (*init)();
    void (*terminate)();
    void (*main_loop)();
} jd_gfx_operations;

/* Coordinate system for screen rendering */
typedef enum {
    COORD_CARTESIAN,
    COORD_NORMALIZED,
    COORD_SCREEN
} jd_coordinate_system;

/* Operations for screen configuration */
typedef struct {
    void (*set_coordinate_system)(jd_coordinate_system system, int width, int height);
} jd_gfx_screen_operations;

/* Buffers to be cleared during rendering */
typedef enum {
    COLOR= GL_COLOR_BUFFER_BIT,
    DEPTH= GL_DEPTH_BUFFER_BIT,
    STENCIL= GL_STENCIL_BUFFER_BIT
} jd_clearable_buffers;

/* Operations for buffer management */
typedef struct {
    void (*swap)(jd_window *window);
    void (*clear)(jd_clearable_buffers buffers_to_clear);
} jd_gfx_buffers_operations;

/* Color structure */
typedef struct {
    unsigned char r, g, b, a;
} Color;

/* Rectangle structure for rendering shapes */
typedef struct {
    float x, y;
    float width, height;
} Rectangle;

/* Operations for rendering rectangles */
typedef struct {
    void (*easy)(int posX, int posY, int width, int height, Color color);

} jd_gfx_rectangle_operations;

/* Event handling operations */
typedef struct {
    void (*poll)();
} jd_gfx_events_operations;

#ifdef JD_GFX_IMPLEMENTATION

// ==========================================================
// Implementations
// ==========================================================

/* Callback function for GLFW errors */
void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

/* Initialize graphics framework */
void jd_gfx_initialize() {

    /* Before calling glfwInit() */
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
}

/* Create a GLFW window with the specified settings */
jd_window* jd_gfx_window_create(const char *title, int width, int height, void (*renderFunc)(jd_window*)) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    DEBUG_PRINT("Entering jd_gfx_window_create with title: '%s', width: %d, height: %d\n", title, width, height);
    DEBUG_PRINT("Attempting to create GLFW window.\n");
    jd_window* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        DEBUG_PRINT("Failed to create GLFW window.\n");

        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window\n");
        exit(EXIT_FAILURE);
    } else {
        DEBUG_PRINT("GLFW window created successfully.\n");
    }

    DEBUG_PRINT("Making context current for the new window.\n");

    glfwMakeContextCurrent(window);
    assert(glfwGetCurrentContext() == window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    DEBUG_PRINT("Allocating new WindowContext.\n");
    WindowContext* newContext = (WindowContext*)malloc(sizeof(WindowContext));
    if (!newContext) {
        DEBUG_PRINT("Failed to allocate memory for WindowContext.\n");
        // Handle memory allocation failure
        // ...
    }

    DEBUG_PRINT("Setting up new WindowContext.\n");
    newContext->window = window;
    newContext->render_function = renderFunc;
    newContext->next = jd_window_context_head;
    jd_window_context_head = newContext;

    DEBUG_PRINT("New window context added to the global list.\n");
    DEBUG_PRINT("Exiting jd_gfx_window_create.\n");

    return window;
}

int jd_gfx_window_should_close(jd_window *window) {
    return glfwWindowShouldClose(window);
}

void jd_gfx_buffers_swap(jd_window *window) {
    glfwSwapBuffers(window);
}

void jd_gfx_clear_buffers(jd_clearable_buffers buffers_to_clear) {
    const char* buffer_name;
    glClear(buffers_to_clear);
}

void jd_gfx_events_poll() {
    glfwPollEvents();
}

void jd_gfx_window_destroy(jd_window *window) {
    glfwDestroyWindow(window);
}

void jd_gfx_terminate() {
    glfwTerminate();
}


void jd_gfx_main_loop() {
    DEBUG_PRINT("Entering jd_gfx_main_loop.\n");

    while (jd_window_context_head) {
        DEBUG_PRINT("Starting new iteration over window contexts.\n");

        WindowContext* current = jd_window_context_head;
        while (current) {
            DEBUG_PRINT("Processing window: %p\n", (void *)current->window);

            if (glfwWindowShouldClose(current->window)) {
                DEBUG_PRINT("Window %p should close, destroying window.\n", (void *)current->window);
                glfwDestroyWindow(current->window);
                // Remove from list and free memory
                // ...
                continue;
            }

            DEBUG_PRINT("Current OpenGL Context before switch: %p\n", glfwGetCurrentContext());
            DEBUG_PRINT("Switching OpenGL Context to window %p.\n", (void *)current->window);

            glfwMakeContextCurrent(current->window);
            if (glfwGetCurrentContext() != current->window) {
                DEBUG_PRINT("Error: Failed to make context current for window %p.\n", (void *)current->window);
            } else {
                DEBUG_PRINT("Context successfully switched to window %p.\n", (void *)current->window);
            }

            DEBUG_PRINT("Current OpenGL Context after switch: %p\n", glfwGetCurrentContext());
            DEBUG_PRINT("Calling render function for window %p.\n", (void *)current->window);
            current->render_function(current->window);

            // Check for OpenGL errors
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                DEBUG_PRINT("OpenGL error: %u\n", err);
                // Handle or log the error
            }

            DEBUG_PRINT("Swapping buffers for window %p.\n", (void *)current->window);
            glfwSwapBuffers(current->window);

            current = current->next;
        }

        DEBUG_PRINT("Polling for events.\n");
        glfwPollEvents();
    }

    DEBUG_PRINT("Terminating GLFW.\n");
    glfwTerminate();

    DEBUG_PRINT("Exiting jd_gfx_main_loop.\n");
}

void SetNormalizedColor(Color color) {
    const char* color_name = "Unknown Color";
    glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main() {\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

void jd_gfx_rect_easy(int posX, int posY, int width, int height, Color color) {

    // Compile and setup the shader
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors...
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors...
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors...
    glUseProgram(shaderProgram);

    // Set the color
    int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(colorLoc, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    // Define the rectangle coordinates
    float vertices[] = {
        posX, posY,
        posX + width, posY,
        posX + width, posY + height,
        posX, posY + height
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw the rectangle
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
}


void jd_gfx_set_coordinate_system(jd_coordinate_system system, int width, int height) {

    // Adjust the projection and modelview matrices based on the chosen system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    switch (system) {
        case COORD_CARTESIAN:

            // Set up an orthographic projection with a Cartesian coordinate system
            // You might allow for customizable bounds here
            glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1, 1);
            break;
        case COORD_NORMALIZED:

            // Default NDC, no changes needed
            break;
        case COORD_SCREEN:

            // Set up an orthographic projection with pixel-based coordinates
            glOrtho(0, width, height, 0, -1, 1);
            break;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ==========================================================
// Global Variables Declarations
// ==========================================================

const jd_gfx_operations jd_gfx = {
        .init                   = jd_gfx_initialize,
        .terminate              = jd_gfx_terminate,
        .main_loop              = jd_gfx_main_loop
};

jd_gfx_screen_operations jd_gfx_screen = {
        .set_coordinate_system  = jd_gfx_set_coordinate_system
};


jd_gfx_window_operations jd_gfx_window = {
        .create                 = jd_gfx_window_create,
        .should_close           = jd_gfx_window_should_close,
        .destroy                = jd_gfx_window_destroy
};

jd_gfx_buffers_operations jd_gfx_buffers = {
        .swap                   = jd_gfx_buffers_swap,
        .clear                  = jd_gfx_clear_buffers
};

jd_gfx_events_operations jd_gfx_events = {
        .poll                   = jd_gfx_events_poll
};

jd_gfx_rectangle_operations jd_gfx_rect = {
        .easy                   = jd_gfx_rect_easy,
};
//
// jd_gfx_namespace jd_gfx = {
//     .operations = {
//         .init           = jd_gfx_initialize,
//         .terminate      = jd_gfx_terminate,
//         .main_loop      = jd_gfx_main_loop,
//     },
//     .screen_operations = {
//         .set_coordinate_system = jd_gfx_set_coordinate_system,
//     },
//     .window_operations = {
//         .create         = jd_gfx_window_create,
//         .should_close   = jd_gfx_window_should_close,
//         .destroy        = jd_gfx_window_destroy,
//     },
//     .buffer_operations = {
//         .swap           = jd_gfx_buffers_swap,
//         .clear          = jd_gfx_buffers_clear,
//     },
//     .event_operations = {
//         .poll           = jd_gfx_events_poll,
//     },
//     .rectange_operations = {
//         .easy           = jd_gfx_rect_easy,
//     }
// };
//
#endif // JD_GFX_IMPLEMENTATION
#endif // JD_GFX_
