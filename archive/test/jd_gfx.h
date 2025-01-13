
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
   #define JD static 
#else
   #define JD extern
#endif

#include <assert.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jd_debug.h"

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


// Window

typedef GLFWwindow jd_window;

typedef struct WindowContext {
    jd_window* window;
    void (*render_function)(jd_window*);
    struct WindowContext* next; // For linked list
} WindowContext;

JD WindowContext* jd_window_context_head = NULL; // Global head for the list of window contexts

typedef struct {
    jd_window* (*create)(const char *title, int width, int height, void (*renderFunc)(jd_window*));
    int (*should_close)(jd_window *window);
    void (*destroy)(jd_window *window);
} jd_gfx_window_operations;


// Main Operations

typedef struct {
    void (*init)(); 
    void (*terminate)();
    void (*main_loop)();
} jd_gfx_operations;


// Screen

typedef enum {
    COORD_CARTESIAN,
    COORD_NORMALIZED,
    COORD_SCREEN
} jd_coordinate_system;

typedef struct {
    void (*set_coordinate_system)(jd_coordinate_system system, int width, int height); 
} jd_gfx_screen_operations;


// Buffers

typedef enum {
    COLOR= GL_COLOR_BUFFER_BIT,
    DEPTH= GL_DEPTH_BUFFER_BIT,
    STENCIL= GL_STENCIL_BUFFER_BIT
} jd_clearable_buffers;

typedef struct {
    void (*swap)(jd_window *window);
    void (*clear)(jd_clearable_buffers buffers_to_clear);
} jd_gfx_buffers_operations;


// Color

typedef struct {
    unsigned char r, g, b, a;
} Color;


// Shapes

typedef struct {
    float x, y;
    float width, height;
} Rectangle;

typedef struct {
    void (*quad)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void (*rect)(float x, float y, float width, float height);
} jd_gfx_primitives_operations;

typedef struct {
    void (*easy)(int posX, int posY, int width, int height, Color color);
    void (*verticalGradient)(int posX, int posY, int width, int height, Color color1, Color color2);
    void (*outline)(int posX, int posY, int width, int height, Color color);
    void (*rounded)(Rectangle rec, float roundness, int segments, Color color);

} jd_gfx_rectangle_operations;


// Events

typedef struct {
    void (*poll)();
} jd_gfx_events_operations;



#ifdef JD_GFX_IMPLEMENTATION

// ==========================================================
// Implementations
// ==========================================================



void jd_gfx_initialize() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    assert(glfwInit() == GLFW_TRUE);
}

jd_window* jd_gfx_window_create(const char *title, int width, int height, void (*renderFunc)(jd_window*)) {
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

            DEBUG_PRINT("Making context current for window %p.\n", (void *)current->window);
            glfwMakeContextCurrent(current->window);

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


void jd_gfx_rect_easy(int posX, int posY, int width, int height, Color color) {
    DEBUG_PRINT("Trying to create jd_gfx_rect_easy\n");
    SetNormalizedColor(color);  // Set the color using normalized values 
                               
    glBegin(GL_QUADS);
        glVertex2f(posX, posY);
        glVertex2f(posX + width, posY);
        glVertex2f(posX + width, posY + height);
        glVertex2f(posX, posY + height);
    glEnd();
}

void jd_gfx_rect_verticalGradient(int posX, int posY, int width, int height, Color color1, Color color2) {

    glBegin(GL_QUADS);
        SetNormalizedColor(color1);

        glVertex2f(posX, posY);
        glVertex2f(posX + width, posY);

        SetNormalizedColor(color2);
        glVertex2f(posX + width, posY + height);
        glVertex2f(posX, posY + height);
    glEnd();
}

void jd_gfx_rect_outline(int posX, int posY, int width, int height, Color color) {

    SetNormalizedColor(color);
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(posX, posY);
        glVertex2f(posX + width, posY);
        glVertex2f(posX + width, posY + height);
        glVertex2f(posX, posY + height);
    glEnd();
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

// Helper function to draw a rounded corner
void drawCorner(float cx, float cy, float radius, int segments) {
    float theta = 2 * 3.1415926f / (float)segments;
    float c = cosf(theta); // Precompute the sine and cosine
    float s = sinf(theta);
    float t;

    float x = radius; // We start at angle = 0 
    float y = 0;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of fan

    for (int i = 0; i <= segments; i++) {
        glVertex2f(x + cx, y + cy);

        // Apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

void jd_gfx_rect_rounded(Rectangle rec, float roundness, int segments, Color color) {
    if (segments <= 0) segments = 1;
    if (roundness > 1.0f) roundness = 1.0f;

    float radius = roundness * fminf(rec.width, rec.height) / 2;
    if (radius <= 0.0f) {
        jd_gfx_rect_easy(rec.x, rec.y, rec.width, rec.height, color); // Draw a regular rectangle
        return;
    }

    SetNormalizedColor(color);

    // Draw each corner
    drawCorner(rec.x + radius, rec.y + radius, radius, segments); // Top-left corner
    drawCorner(rec.x + rec.width - radius, rec.y + radius, radius, segments); // Top-right corner
    drawCorner(rec.x + rec.width - radius, rec.y + rec.height - radius, radius, segments); // Bottom-right corner
    drawCorner(rec.x + radius, rec.y + rec.height - radius, radius, segments); // Bottom-left corner

    // Draw the sides and the center
    jd_gfx_rect_easy(rec.x, rec.y + radius, rec.width, rec.height - 2 * radius, color);
    jd_gfx_rect_easy(rec.x + radius, rec.y, rec.width - 2 * radius, radius, color);
    jd_gfx_rect_easy(rec.x + radius, rec.y + rec.height - radius, rec.width - 2 * radius, radius, color);
}

// ==========================================================
// Global Variables Declarations
// ==========================================================


const jd_gfx_operations jd_gfx = {
    .init = jd_gfx_initialize,
    .terminate = jd_gfx_terminate,
    .main_loop = jd_gfx_main_loop
};

jd_gfx_screen_operations jd_gfx_screen = {
    .set_coordinate_system = jd_gfx_set_coordinate_system
};


jd_gfx_window_operations jd_gfx_window = {
    .create = jd_gfx_window_create,
    .should_close = jd_gfx_window_should_close,
    .destroy = jd_gfx_window_destroy
};

jd_gfx_buffers_operations jd_gfx_buffers = {
    .swap = jd_gfx_buffers_swap,
    .clear = jd_gfx_clear_buffers
};

jd_gfx_events_operations jd_gfx_events = {
    .poll = jd_gfx_events_poll
};

jd_gfx_rectangle_operations jd_gfx_rect = {
    .easy = jd_gfx_rect_easy,
    .verticalGradient = jd_gfx_rect_verticalGradient,
    .outline = jd_gfx_rect_outline,
    .rounded = jd_gfx_rect_rounded
};




#endif // JD_GFX_IMPLEMENTATION
#endif // JD_GFX_H
