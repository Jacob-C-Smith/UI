#pragma once

#include <ui/ui.h>
#include <ui/label.h>

#define UI_ELEMENT_NAME_LENGTH_MAX 63 + 1

struct ui_element_s
{
    char  _name[UI_ELEMENT_NAME_LENGTH_MAX];
    void *p_element;
    ui_element_info *p_functions;
};

// Allocators 
/** !
 *  Allocate a UI Element
 *
 *  @param pp_element : Pointer to pointer to element
 *  
 *  @sa destroy_element
 *  
 *  @return 1 on success, 0 on error.  
 */
DLLEXPORT int create_element ( ui_element **pp_element );

// Constructors
/** !
*  Load a UI Button from a dict 
*
*  @param pp_element : Pointer to pointer to element
*  @param path       : Path to element file
* 
*  @sa load_element_as_json 
*  
*  @return 1 on success, 0 on error.  
*/
DLLEXPORT int load_element ( ui_element **pp_element, const char path[] );

/** !
*  Load a UI Button from a JSON value
*
*  @param pp_element : Pointer to pointer to element
*  @param p_value : JSON token text
* 
*  @return 1 on success, 0 on error.  
*/
DLLEXPORT int element_load_as_json_value ( ui_element **pp_element, json_value *p_value );

// Callbacks
DLLEXPORT int click_element   ( ui_element *element, ui_mouse_state mouse_state );
DLLEXPORT int hover_element   ( ui_element *element, ui_mouse_state mouse_state );
DLLEXPORT int release_element ( ui_element *element, ui_mouse_state mouse_state );

// Add callbacks
DLLEXPORT int add_click_callback_element   ( ui_element *element, void (*callback)(ui_element*, ui_mouse_state) );
DLLEXPORT int add_hover_callback_element   ( ui_element *element, void (*callback)(ui_element*, ui_mouse_state) );
DLLEXPORT int add_release_callback_element ( ui_element *element, void (*callback)(ui_element*, ui_mouse_state) );

// Bounds
DLLEXPORT bool in_bounds ( ui_element *element, ui_mouse_state mouse_state );

// Drawing
DLLEXPORT int draw_element ( ui_window *window, ui_element *element );

// Destructor
DLLEXPORT int destroy_element ( ui_element **pp_element );
