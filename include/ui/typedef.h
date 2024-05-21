/** !
 * Type definitions for ui library

 * @file ui/typedef.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// json module
#include <json/json.h>

// Platform dependent macros
#ifdef _WIN64
    #define DLLEXPORT extern __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

// Forward declarations
struct ui_label_s;
struct ui_button_s;
struct ui_mouse_state_s;
struct ui_checkbox_s;
struct ui_dropdown_s;
struct ui_image_s;
struct ui_label_s;
struct ui_radio_button_s;
struct ui_slider_s;
struct ui_text_input_s;
struct ui_element_s;
struct ui_window_s;
struct ui_instance_s;
struct ui_element_info_s;

// Type definitions
typedef unsigned char        u8;
typedef signed   char        s8;
typedef          char        i8;
typedef unsigned short       u16;
typedef signed   short       s16;
typedef          short       i16;
typedef unsigned long        u32;
typedef signed   long        s32;
typedef          long        i32;
typedef unsigned long long   u64;
typedef signed   long long   s64;
typedef          long long   i64;
typedef float                f32;
typedef double               f64;
typedef long     double      f80;

typedef struct
{
    u8 r : 8,
       g : 8,
       b : 8,
       a : 8;    
} color;

typedef struct ui_label_s        ui_label;
typedef struct ui_button_s       ui_button;
typedef struct ui_mouse_state_s  ui_mouse_state;
typedef struct ui_checkbox_s     ui_checkbox;
typedef struct ui_dropdown_s     ui_dropdown;
typedef struct ui_image_s        ui_image;
typedef struct ui_radio_button_s ui_radio_button;
typedef struct ui_slider_s       ui_slider;
typedef struct ui_text_input_s   ui_text_input;
typedef struct ui_element_s      ui_element;
typedef struct ui_window_s       ui_window;
typedef struct ui_instance_s     ui_instance;
typedef struct ui_element_info_s ui_element_info;

typedef int(*fn_window_constructor)(ui_window *p_window);

typedef int(*fn_element_load)       ( void **const pp_element, const json_value *const p_value );
typedef int(*fn_element_draw)       ( ui_window *p_window, void *p_element );
typedef int(*fn_element_bounds)     ( void  *p_element, ui_mouse_state mouse_state );
typedef int(*fn_element_destructor) ( void **pp_element );
typedef int(*fn_element_click)      ( void  *const p_element, ui_mouse_state mouse_state );
typedef int(*fn_element_hover)      ( void  *const p_element, ui_mouse_state mouse_state );
typedef int(*fn_element_release)    ( void  *const p_element, ui_mouse_state mouse_state );
typedef int(*fn_element_add_click)  ( void  *const p_element, fn_element_click pfn_element_click );
typedef int(*fn_element_add_hover)  ( void  *const p_element, fn_element_click pfn_element_hover );
typedef int(*fn_element_add_release)( void  *const p_element, fn_element_click pfn_element_release );
