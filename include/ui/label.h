#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <json/json.h>

#include <ui/ui.h>
#include <ui/typedef.h>

struct ui_label_s
{
    char    *text;
    i32      x, y, width, height, size;
    
    // Callbacks
    size_t   on_click_count,   on_click_max,
             on_hover_count,   on_hover_max,
             on_release_count, on_release_max;
    
    void   **on_click,
           **on_hover,
           **on_release;
    
};

// Allocators
/** !
 * Allocate memory for a label
 * 
 * @param pp_label return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int label_create ( ui_label **const pp_label );

// Constructors
/** !
 * Construct a label from a json value
 * 
 * @param pp_label return
 * @param p_value  the json value
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int label_load_as_json_value ( ui_label **const pp_label, const json_value *const p_value );

// Draw
DLLEXPORT int  label_draw                 ( ui_window  *p_window, ui_label       *p_label );

// Callbacks
DLLEXPORT int  label_click                ( ui_label   *p_label, ui_mouse_state  mouse_state );
DLLEXPORT int  label_hover                ( ui_label   *p_label, ui_mouse_state  mouse_state );
DLLEXPORT int  label_release              ( ui_label   *p_label, ui_mouse_state  mouse_state );

// Add callbacks
DLLEXPORT int  label_add_click   ( ui_label   *p_label, void           ( *callback ) ( ui_label*, ui_mouse_state) );
DLLEXPORT int  label_add_hover   ( ui_label   *p_label, void           ( *callback ) ( ui_label*, ui_mouse_state) );
DLLEXPORT int  label_add_release ( ui_label   *p_label, void           ( *callback ) ( ui_label*, ui_mouse_state) );

// Bounds
DLLEXPORT bool label_in_bounds            ( ui_label   *p_label, ui_mouse_state  mouse_state );

// Printers
DLLEXPORT int  print_label_to_file        ( ui_label   *p_label, FILE *f, char *name );

// Deallocators
DLLEXPORT int label_destroy              ( ui_label   **pp_label );