#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <dict/dict.h>
#include <json/json.h>

#include <ui/typedef.h>
#include <ui/ui.h>

// #include <ui/button.h>
// #include <ui/UICanvas.h>
// #include <ui/checkbox.h>
// #include <ui/UIChart.h>
// #include <ui/dropdown.h>
// #include <ui/image.h>
// #include <ui/radiobutton.h>
// #include <ui/slider.h>
// #include <ui/table.h>
// #include <ui/textinput.h>

struct UIWindow_s
{
	bool               is_open,
	                   drag;
	char              *name,
	                  *title;
	size_t             width,
		               height,
		               element_count,
		               element_data_max;
	i32                rx,
		               ry;
	dict              *elements;
	ui_element      **element_data;
	ui_element       *last;
	SDL_Window        *window;
	SDL_Renderer      *renderer;
	int (*path_drop_function)(ui_window *p_window, char *path);
};

// Allocators
/** !
 *  Allocate a window
 * 
 * @param pp_window : return
 * 
 * @sa destroy_window
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_window ( ui_window **pp_window );
 
// Constructors
DLLEXPORT int load_window         ( ui_window **pp_window, const char *path );
DLLEXPORT int load_window_as_json ( ui_window **pp_window, char       *text );
DLLEXPORT int construct_window    ( ui_window **pp_window, char       *title, size_t width, size_t height, size_t element_count );

// Element operations
DLLEXPORT int          append_element_to_window ( ui_window *p_window, ui_element *element );
DLLEXPORT ui_element *find_element             ( ui_window *p_window, char *name );

DLLEXPORT int resize_window ( ui_window *p_window );

DLLEXPORT int set_file_drop_operation ( ui_window *p_window, int (*callback_function)(ui_window *p_window, char *path));

// User interaction
DLLEXPORT int process_window_input ( ui_window *p_window );
DLLEXPORT int click_window         ( ui_window *p_window, ui_mouse_state mouse_state );
DLLEXPORT int hover_window         ( ui_window *p_window, ui_mouse_state mouse_state );
DLLEXPORT int release_window       ( ui_window *p_window, ui_mouse_state mouse_state );

// Drawing 
DLLEXPORT int draw_window ( ui_window *p_window );

// Destruction
DLLEXPORT int destroy_window ( ui_window *p_window );

