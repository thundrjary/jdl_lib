
/* app.c */
#define JD_DEBUG_IMPLEMENTATION
#define JD_GFX_IMPLEMENTATION
#include "jd_gfx.h"
#include "math.h"
#include <assert.h>

void render(jd_window* window) {
    assert(glfwGetCurrentContext() != NULL);  // Ensure a valid OpenGL contex
					      //
    assert(glfwGetCurrentContext() == window);
    DEBUG_PRINT("Entered `render` for window: %p\n", (void *)window);

    DEBUG_PRINT("Clearing buffers.\n");
    jd_gfx_buffers.clear(COLOR|DEPTH|STENCIL);

    assert(glfwGetCurrentContext() == window);
    DEBUG_PRINT("Drawing outline.\n");
    jd_gfx_rect.outline(20, 20, 760, 760, GREEN);

    DEBUG_PRINT("Drawing vertical gradient.\n");
    jd_gfx_rect.verticalGradient(60, 60, 680, 680, BLACK, GREEN);

    assert(glfwGetCurrentContext() == window);
    DEBUG_PRINT("Drawing easy rectangle.\n");
    jd_gfx_rect.easy(100, 100, 600, 600, BLACK);

    Rectangle r = {140, 140, 520, 520}; // Example rectangle
    DEBUG_PRINT("Drawing rounded rectangle at x: %f, y: %f, width: %f, height: %f.\n", r.x, r.y, r.width, r.height);
    jd_gfx_rect.rounded(r, 0.25f, 16, GREEN); // Rounded rectangle with roundness of 0.1 and 16 segments

    DEBUG_PRINT("Exiting `render`.\n");
}

void debug_render(jd_window* window) {
    assert(glfwGetCurrentContext() != NULL);  // Ensure a valid OpenGL context
    DEBUG_PRINT("Entered `debug_render` for window: %p\n", (void *)window);

    assert(glfwGetCurrentContext() == window);
    DEBUG_PRINT("Clearing buffers.\n");
    jd_gfx_buffers.clear(COLOR|DEPTH|STENCIL);

    assert(glfwGetCurrentContext() == window);
    DEBUG_PRINT("Drawing easy rectangle for debug.\n");
    jd_gfx_rect.easy(100, 100, 600, 600, BLACK);

    DEBUG_PRINT("Exiting `debug_render`.\n");
}

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 800;

    jd_gfx.init(); 
    jd_gfx_window.create("Main Window", WIDTH, HEIGHT, render);
    jd_gfx_window.create("Debug Window", WIDTH, HEIGHT, debug_render);

    jd_gfx_screen.set_coordinate_system(COORD_SCREEN, WIDTH, HEIGHT);

    jd_gfx.main_loop();

    jd_gfx.terminate();

    return 0;
}
