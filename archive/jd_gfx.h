
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


#ifndef JD_GFX_H
#define JD_GFX_H


#ifdef JD_GFX_IMPLEMENTATION 
   #define JD static 
#else
   #define JD extern

#endif


#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


// Main Operations

typedef struct {
    void (*init)(); 
    void (*terminate)();
    void (*main_loop)(jd_window *window, void (*render_func)(jd_window*));
} jd_gfx_operations;


// Window

typedef GLFWwindow jd_window;

typedef struct {
    jd_window* window;
    void (*render_function)(jd_window*);
} WindowData;

typedef struct {
    jd_window* (*create)(const char *title, int width, int height);
    int (*should_close)(jd_window *window);
    void (*destroy)(jd_window *window);
} jd_gfx_window_operations;



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


// Color

typedef struct {
    unsigned char r, g, b, a;
} Color;


// Events

typedef struct {
    void (*poll)();
} jd_gfx_events_operations;


// ==========================================================
// Implementations
// ==========================================================


#ifdef JD_GFX_IMPLEMENTATION


void jd_gfx_initialize() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
}

jd_window* jd_gfx_window_create(const char *title, int width, int height) {
    jd_window* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window\n");
        exit(EXIT_FAILURE); // Exit if window creation fails
    }
    glfwMakeContextCurrent(window);
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

void jd_gfx_main_loop(jd_window *window, void (*render_func)(jd_window*)) {
    while (!jd_gfx_window_should_close(window)) {
        render_func(window); // Call the provided render function
        jd_gfx_buffers_swap(window);
        jd_gfx_events_poll();
    }
    jd_gfx_window_destroy(window); // Destroy the window after the loop ends
    jd_gfx_terminate(); // Terminate GLFW
}

void SetNormalizedColor(Color color) {
    const char* color_name = "Unknown Color";
    glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}


void jd_gfx_rect_easy(int posX, int posY, int width, int height, Color color) {
   
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


// ==========================================================

#endif // JD_GFX_IMPLEMENTATION
#endif // JD_GFX_H
