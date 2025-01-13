
/* app.c */
#define JD_GFX_IMPLEMENTATION
#include "jd_gfx.h"
#include "math.h"

void render(jd_window* window) {
    jd_gfx_buffers.clear(COLOR);

    jd_gfx_rect.outline(20, 20, 760, 760, GREEN);
    jd_gfx_rect.verticalGradient(60, 60, 680, 680, BLACK, GREEN);
    jd_gfx_rect.easy(100, 100, 600, 600, BLACK);

    Rectangle r = {140, 140, 400, 400}; // Example rectangle
    jd_gfx_rect.rounded(r, 0.25f, 16, GREEN); // Rounded rectangle with roundness of 0.1 and 16 segments
}

void debug_render(jd_window* window) {
    jd_gfx_buffers.clear(COLOR);
    jd_gfx_rect.easy(100, 100, 600, 600, BLACK);
}

int main() {

    const int WIDTH = 800;
    const int HEIGHT = 800;
    const char *title = "Jarom's Window";
    const char *debug_title = "Debug Window";

    jd_gfx.init(); 
    jd_window* window = jd_gfx_window.create(title, WIDTH, HEIGHT);
    jd_window* debug_window = jd_gfx_window.create(debug_title, WIDTH, HEIGHT);

    jd_gfx_screen.set_coordinate_system(COORD_SCREEN, WIDTH, HEIGHT);

    jd_gfx.main_loop(window, render);
    jd_gfx.main_loop(debug_window, debug_render);
    jd_gfx_window.destroy(window);
    jd_gfx_window.destroy(debug_window);
    jd_gfx.terminate();

    return 0;
}
