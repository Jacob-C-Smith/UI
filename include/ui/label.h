/** !
 * Header file for label element
 * 
 * @file ui/label.h
 * 
 * @author Jacob Smith
 */

// Header guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// log module
#include <log/log.h>

// json module
#include <json/json.h>

// ui
#include <ui/ui.h>
#include <ui/typedef.h>

// Structure definitions
struct ui_label_s
{
    char *text;
    i32   x, y, width, height, size;    
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
/** !
 * Draw a label on a window
 * 
 * @param p_window the window
 * @param p_label  the label
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int label_draw ( ui_window *p_window, ui_label *p_label );

// Bounds
/** !
 * Test if the cursor is over a label
 * 
 * @param p_label     the label
 * @param mouse_state the cursor state
 * 
 * @return true if cursor is over label else false
 */
DLLEXPORT bool label_in_bounds ( ui_label *p_label, ui_mouse_state mouse_state );

// Serializer
/** !
 * Serialize a label to a file
 * 
 * @param p_label the label
 * @param p_f     the file handle
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int label_print_to_file ( const ui_label *const p_label, FILE *p_f );

// Deallocators
/** !
 * Free a label
 * 
 * @param pp_label pointer to label pointer
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int label_destroy ( ui_label **pp_label );
