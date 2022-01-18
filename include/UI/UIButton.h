#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <JSON/JSON.h>

#include <UI/UItypedef.h>
#include <UI/UIWindow.h>
#include <UI/UIElement.h>
#include <UI/UI.h>

struct UIButton_s
{
    // ID
    char   *ID;

    // Renderer
    char   *text;
    i32     width,
            height,
            x,
            y;
    int     text_color,
            back_color,
            border_color;
    u8      text_align;
    i32     border_width;
    bool    hidden,
            depressed;

    // Callbacks
    void   **on_click,
           **on_hover;
    size_t   on_click_count,
             on_click_max,
             on_hover_count,
             on_hover_max;

};


// Allocators
UIElement_t *create_button                ( void );

// Constructors
UIButton_t  *load_button_as_json_tokens   ( JSONToken_t *tokens, size_t token_count );

// Callbacks
int         hover_button                 ( UIButton_t *button    , mouse_state_t mouse_state );
int         click_button                 ( UIButton_t *button    , mouse_state_t mouse_state );
int         release_button               ( UIButton_t *button    , mouse_state_t mouse_state );

// Add callbacks
int         add_hover_callback_button    ( UIButton_t* button, void (*callback)(UIButton_t*, mouse_state_t));

// Drawing
int         draw_button                  ( UIWindow_t *window    , UIButton_t   *button );

// Deallocators
int         destroy_button               ( UIButton_t *button );