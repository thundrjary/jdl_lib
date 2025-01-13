/* app.c */

#define JD_DEBUG_IMPLEMENTATION
#define JD_GFX_IMPLEMENTATION
#include "jd_gfx.h"
#include "math.h"
#include <assert.h>

void render(jd_window* window) {
    assert(glfwGetCurrentContext() != NULL);  // Ensure a valid OpenGL contex
    assert(glfwGetCurrentContext() == window);
    DEBUG_PRINT("Entered `render` for window: %p\n", (void *)window);

    DEBUG_PRINT("Clearing buffers.\n");
    jd_gfx_buffers.clear(COLOR|DEPTH|STENCIL);

    DEBUG_PRINT("Current OpenGL Context in render: %p for window %p\n", glfwGetCurrentContext(), (void *)window);
    assert(glfwGetCurrentContext() == window);

    DEBUG_PRINT("Drawing easy rectangle.\n");
    jd_gfx_rect.easy(100, 100, 600, 600, RED);

    DEBUG_PRINT("Exiting `render`.\n");
}

void debug_render(jd_window* window) {
    assert(glfwGetCurrentContext() != NULL);  // Ensure a valid OpenGL context
    DEBUG_PRINT("Entered `debug_render` for window: %p\n", (void *)window);

    DEBUG_PRINT("Clearing buffers.\n");
    jd_gfx_buffers.clear(COLOR|DEPTH|STENCIL);

    DEBUG_PRINT("Current OpenGL Context in debug_render: %p for window %p\n", glfwGetCurrentContext(), (void *)window);
    assert(glfwGetCurrentContext() == window);

    DEBUG_PRINT("Drawing easy rectangle for debug.\n");
    jd_gfx_rect.easy(100, 100, 600, 600, BLUE);

    DEBUG_PRINT("Exiting `debug_render`.\n");
}

int main() {
    const int WIDTH = 800;
    const int HEIGHT = 800;

    jd_gfx.init();
    jd_gfx_window.create("Main Window", WIDTH, HEIGHT, render);
    //jd_gfx_window.create("Debug Window", WIDTH, HEIGHT, debug_render);
    jd_gfx_screen.set_coordinate_system(COORD_SCREEN, WIDTH, HEIGHT);
    jd_gfx.main_loop();
    jd_gfx.terminate();

    return 0;
}
