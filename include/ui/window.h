/** !
 * UI window header

 * @file ui/window.h
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

// ui module
#include <ui/typedef.h>
#include <ui/ui.h>
#include <ui/element.h>

// SDL
#include <SDL.h>
#undef main

// Preprocessor definitions
#define UI_WINDOW_NAME_LENGTH_MAX  255 + 1
#define UI_WINDOW_TITLE_LENGTH_MAX 255 + 1

struct ui_window_s
{
	struct
	{
		char _name [UI_WINDOW_NAME_LENGTH_MAX],
		     _title[UI_WINDOW_TITLE_LENGTH_MAX];
		size_t width, height;
	} attributes;

	struct 
	{
		bool is_open, drag;
		i32 rx, ry;
	} context;
	
	struct
	{
		size_t count, max;
		dict        *lookup;
		ui_element **data,
		            *last;
	} elements;
	
	struct
	{
		SDL_Window   *window;
		SDL_Renderer *renderer;		
	} sdl2;
	
	parallel_thread *p_thread;
	monitor          _monitor;
	int (*path_drop_function)(ui_window *p_window, char *path);
};

// Allocators
/** !
 *  Allocate a window
 * 
 * @param pp_window return
 * 
 * @sa window_destroy
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int window_create ( ui_window **const pp_window );
 
// Constructors
DLLEXPORT int window_load         ( ui_window **pp_window, const char *path );
DLLEXPORT int window_load_as_json ( ui_window **pp_window, char       *text );
DLLEXPORT int window_construct    ( ui_window **pp_window, char       *title, size_t width, size_t height );

// Accessors
//

// Mutators
DLLEXPORT int window_resize ( ui_window *const p_window );

// User interaction
DLLEXPORT int window_input_process ( ui_window *const p_window );
DLLEXPORT int window_click         ( ui_window *p_window, ui_mouse_state mouse_state );
DLLEXPORT int window_hover         ( ui_window *p_window, ui_mouse_state mouse_state );
DLLEXPORT int window_release       ( ui_window *p_window, ui_mouse_state mouse_state );

// Drawing 
DLLEXPORT int window_draw ( const ui_window *const p_window );

// Destructors
DLLEXPORT int window_destroy ( ui_window **pp_window );

