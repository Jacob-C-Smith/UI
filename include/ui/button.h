/** !
 * Header file for button element
 * 
 * @file ui/button.h
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
struct ui_button_s
{
    char *text;
    i32   x, y, width, height, size;
    bool  depressed;
};

// Allocators
/** !
 * Allocate memory for a button
 * 
 * @param pp_button return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int button_create ( ui_button **const pp_button );

// Constructors
/** !
 * Construct a button from a json value
 * 
 * @param pp_button return
 * @param p_value  the json value
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int button_load_as_json_value ( ui_button **const pp_button, const json_value *const p_value );

// Draw
/** !
 * Draw a button on a window
 * 
 * @param p_window the window
 * @param p_button  the button
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int button_draw ( ui_window *p_window, ui_button *p_button );

// Bounds
/** !
 * Test if the cursor is over a button
 * 
 * @param p_button     the button
 * @param mouse_state the cursor state
 * 
 * @return true if cursor is over button else false
 */
DLLEXPORT bool button_in_bounds ( ui_button *p_button, ui_mouse_state mouse_state );

// Serializer
/** !
 * Serialize a button to a file
 * 
 * @param p_button the button
 * @param p_f     the file handle
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int button_print_to_file ( const ui_button *const p_button, FILE *p_f );

// Deallocators
/** !
 * Free a button
 * 
 * @param pp_button pointer to button pointer
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int button_destroy ( ui_button **pp_button );
