/** !
 * Main header for ui library

 * @file ui/ui.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard Library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// sync module
#include <sync/sync.h>

// dict module
#include <dict/dict.h>

// array module
#include <dict/dict.h>

// json module
#include <json/json.h>

// parallel module
#include <parallel/parallel.h>
#include <parallel/thread.h>
#include <parallel/schedule.h>

// ui module
#include <ui/typedef.h>
#include <ui/window.h>

// SDL
#include <SDL.h>
#undef main

// Preprocessor definitions
#define UI_M1     0x01
#define UI_M2     0x02
#define UI_M3     0x04
#define UI_M4     0x08
#define UI_M5     0x10
#define UI_SWUP   0x20
#define UI_SWDOWN 0x40

#define UI_INSTANCE_NAME_LENGTH_MAX 255 + 1
#define UI_INSTANCE_WINDOW_MAX      16

#define BUILD_SYNC_WITH_MONITOR
#define BUILD_SYNC_WITH_MONITOR

// Set the reallocator for the dict module
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array module
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the json module
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the parallel module
#ifdef PARALLEL_REALLOC
    #undef PARALLEL_REALLOC
    #define PARALLEL_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef UI_REALLOC
    #define UI_REALLOC(p, sz) realloc(p,sz)
#endif

// Mouse event callback parameter
struct ui_mouse_state_s
{

    // The position of the cursor
    s32 x, y;
    
    // Bitmask mouse keys and scroll wheel
    u8 button;
};

// UI Instance. Contains a list of windows and an active theme.
struct ui_instance_s
{
    char _name[UI_INSTANCE_NAME_LENGTH_MAX];
    bool running;

    struct 
    {
        size_t     count;
        dict      *lookup;
        ui_window *active,
                  *_list[UI_INSTANCE_WINDOW_MAX];
    } windows;

    struct
    {
        color primary,
              background, 
              accent_1,
              accent_2,
              accent_3;
    } theme;
};

// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
DLLEXPORT void ui_init ( void ) __attribute__((constructor));

DLLEXPORT int ui_window_add ( ui_window **pp_window, const char *const path, fn_window_constructor pfn_window_constructor );
 

/** !
 *  Initialize UI. If path is null, a config file will be loaded from the user's home directory. 
 *  If no config file exists, a default config file will be generated.
 *
 *  @param pp_instance : Pointer to pointer to UI instance
 *  @param path        : Path to config file or null
 *
 *  @sa ui_exit
 *
 *  @return 1 on success, 0 on error.
 */
//DLLEXPORT int ui_init ( ui_instance **pp_instance, const char *path );

// Text drawing
/** !
  *  Graphical printf
  *
  *  @param format : printf format text
  *  @param window : The window to print to
  *  @param x      : The x offset in pixels
  *  @param y      : The y offset in pixels
  *  @param size   : 1 is 8px height, 2 is 16px, 3 is 24px, etc
  *  @param ...    : varadic arguments
  *
  *  @sa ui_draw_text
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int ui_draw_format_text ( ui_window *p_window, int x, int y, int size, const char* const format, ... );

// Text drawing
/** !
  *  Graphical puts
  *
  *  @param format : printf format text
  *  @param window : The window to print to
  *  @param x      : The x offset in pixels
  *  @param y      : The y offset in pixels
  *  @param size   : 1 is 8px height, 2 is 16px, 3 is 24px, etc
  *
  *  @sa ui_draw_format_text
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int ui_draw_text ( const char* const text, ui_window *p_window, int x, int y, int size );

/** !
  *  Draw a circle
  *
  *  @param radius : printf format text
  *  @param window : The window to draw to
  *  @param x      : The x offset in pixels
  *  @param y      : The y offset in pixels
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int ui_draw_circle ( ui_window *p_window, int radius, int x_center, int y_center );

// Window operations
/** !
  *  Append a window to an instance
  *
  *  @param instance : Pointer to instance
  *  @param window   : The window to append
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int ui_append_window ( ui_instance *p_instance, ui_window *p_window );

/** !
  *  Remove a window
  *
  *  @param instance : Pointer to instance
  *  @param name     : The name of the window
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT ui_window *ui_remove_window ( ui_instance *p_instance, const char *name );

/** !
  *  Process active window input
  *
  *  @param instance : Pointer to instance
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int ui_process_input ( ui_instance *p_instance );

/** !
  *  Draw the active window
  *
  *  @param instance : Pointer to instance
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int ui_draw ( ui_instance *p_instance );

// Image drawing
/** !
  *  Get the active instance
  *
  *  @return active instance for this process ID
  */
DLLEXPORT ui_instance *ui_get_active_instance ( void );

// Function declarations
size_t ui_load_file ( const char *path, void *buffer, bool binary );

// Exit
/** !
  * This gets called at runtime after main. 
  *
  *  @param void
  *
  *  @return void
  */
DLLEXPORT void ui_exit ( void ) __attribute__((destructor));
