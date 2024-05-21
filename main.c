/** !
 *  UI library example
 * 
 *  @author Jacob Smith
 * 
 *  @file main.c
*/

// Standard library
#include <stdio.h>

// sync
#include <sync/sync.h>

// ui
#include <ui/ui.h>

// Function declarations
int setup_window ( ui_window *p_window );

// Entry point
int main ( int argc, const char **argv )
{
    
    // Suppress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    ui_instance *p_ui_instance = 0;
    ui_window   *p_ui_window1  = 0;
    ui_window   *p_ui_window2  = 0;
    static size_t c = 0;

    // Get the active instance
    p_ui_instance = ui_get_active_instance();

    // Add a window to the instsance
    ui_window_add(&p_ui_window1, "window1.json", setup_window);

    // Print information about the UI instance
    ui_info(p_ui_instance);
    
    // UI Loop
    while (p_ui_window1->context.is_open)
    {
        SDL_Event e;

        monitor_notify(&p_ui_window1->_monitor);

        // Present the window
        SDL_RenderPresent(p_ui_window1->sdl2.renderer);

        // Process events
        SDL_WaitEvent(&e);

        switch (e.type)
        {

            case SDL_QUIT:
                p_ui_window1->context.is_open = false;
                break;

            default:
                break;
        }
    }

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        
        failed_to_load_window:

            // Print an error to standard out
            log_error("Error: Failed load \"window.json\"!\n");

            // Error
            return EXIT_FAILURE;
    }
}

int setup_window ( ui_window *p_window )
{

    // TODO: Set up the window

    // Success
    return 1;
}
