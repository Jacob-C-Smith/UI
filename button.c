// Header file
#include <ui/button.h>

int button_create ( ui_button **pp_button )
{

	// Argument check
    if ( pp_button == (void *) 0 ) goto no_button;

    // Initialized data
    ui_button* p_button = UI_REALLOC(0, sizeof(ui_button));

    // Check memory
    if ( p_button == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_button, 0, sizeof(ui_button));

    // Return a pointer to the caller
    *pp_button = p_button;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_button:
                #ifndef NDEBUG
                    log_error("[ui] [button] Null pointer provided for \"pp_button\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int button_load_as_json_value ( ui_button **const pp_button, const json_value *const p_value )
{

    // Argument check
    if ( pp_button == (void *) 0 ) goto no_button;
	if ( p_value   == (void *) 0 ) goto no_value;

	// Initialized data
    ui_instance *p_instance = ui_get_active_instance();
	ui_button *p_button  = 0;
	json_value *p_label   = 0,
	           *p_x       = 0,
		       *p_y       = 0;
    SDL_Surface *p_surface = (void *) 0;

	// Get properties from the dictionary
    if (p_value->type == JSON_VALUE_OBJECT)
    {

        // Initialized data
        dict *p_dict = p_value->object;

        p_label = dict_get(p_dict, "text");
        p_x     = dict_get(p_dict, "x");
		p_y     = dict_get(p_dict, "y");
    }

	// Construct the label
	{

		// Allocate a label
		if ( button_create(&p_button) == 0 ) goto failed_to_allocate_button;

		// Copy the label text
		if ( p_label->type == JSON_VALUE_STRING )
        {

			// Initialized data
			size_t label_text_len = strlen(p_label->string);

			// Allocate memory for the label text
			p_button->text = calloc(label_text_len+1, sizeof(char));

			// Error checking
            if ( p_button->text == (void *) 0 ) goto no_mem;

			// Copy the string
			strncpy(p_button->text, p_label->string, label_text_len);
		}
        // Default
        else
            goto wrong_label_type;

		// Set the x
        if ( p_x->type == JSON_VALUE_INTEGER )
		    p_button->x = p_x->integer;
        else
            goto wrong_x_type;
        
		// Set the y
		if ( p_y->type == JSON_VALUE_INTEGER )
		    p_button->y = p_y->integer;
        else
            goto wrong_y_type;
	}

	p_surface = TTF_RenderText_Blended
	(
		p_instance->sdl2.font,
		p_button->text,
		(SDL_Color)
		{
			.r = p_instance->theme.primary.r,
			.g = p_instance->theme.primary.g,
			.b = p_instance->theme.primary.b,
			.a = p_instance->theme.primary.a,
		}
	);

	TTF_MeasureUTF8(p_instance->sdl2.font, p_button->text, 10000, &p_button->width, 0);

	p_button->sdl2.texture = SDL_CreateTextureFromSurface(
		p_instance->windows.active->sdl2.renderer,
		p_surface
	);

	SDL_FreeSurface(p_surface);

	// Return
	*pp_button = p_button;

	// Success
	return 1;
    
    no_mem:
    wrong_label_type:
    wrong_x_type:
    wrong_y_type:
        return 0;
	// Error handling
	{

		// Argument errors
		{
			no_button:
				#ifndef NDEBUG
					log_error("[UI] [Button] Null pointer provided for \"pp_button\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
                
			no_value:
				#ifndef NDEBUG
					log_error("[UI] [Button] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// TODO:
		failed_to_allocate_button:

		// Insufficent data error
		{
			// TODO: 
			no_text:
			no_x:
			no_y:
				return 0;
		}
	}
}

int button_hover ( ui_button *const p_button, ui_mouse_state mouse_state)
{

    // Argument check
    if ( p_button == (void *) 0 ) goto no_button;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_button:
                #ifndef NDEBUG
                    log_error("[UI] [Button] Null pointer provided for \"button\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int button_click ( ui_button *const p_button, ui_mouse_state mouse_state)
{

    // Argument check
    if ( p_button == (void *) 0 ) goto no_button;

    // Set the depressed flag
    // TODO: Figure out how to clear the depressed flag in my brain
    p_button->depressed = true;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_button:
                #ifndef NDEBUG
                    log_error("[UI] [Button] Null pointer provided for \"p_button\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int button_release ( ui_button *const p_button, ui_mouse_state mouse_state )
{

    // Argument check
    if ( p_button == (void *) 0 ) goto no_button;

    // Clear the depressed flag
    p_button->depressed = false;

    // Success
    return 1;
    
    // Error handling
    {

        // Argument errors
        {
            no_button:
                #ifndef NDEBUG
                    log_error("[UI] [Button] Null pointer provided for \"p_button\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int button_draw ( ui_window *p_window, ui_button *p_button )
{

    // TODO: Argument check
    //

    // Initialized data
    ui_instance *p_instance = ui_get_active_instance();
    size_t       l          = strlen(p_button->text);
    SDL_Rect     r = 
    {
        p_button->x,
        p_button->y,
        p_button->width + (p_instance->font.size / 5),
        p_instance->font.size + (p_instance->font.size / 5)
    };
    SDL_Rect d = { 0 };

    if ( p_button->depressed == false )
    {

        // Set the draw color
        SDL_SetRenderDrawColor(
            p_window->sdl2.renderer,
            p_instance->theme.accent_2.r,
            p_instance->theme.accent_2.g,
            p_instance->theme.accent_2.b,
            0xff
        );

        for (signed i = 0; i < (p_instance->font.size / 5); i++)
        {
            SDL_RenderDrawLine(p_window->sdl2.renderer, r.x + r.w + i, r.y + i, r.x + r.w + i, r.y + r.h + i);
            SDL_RenderDrawLine(p_window->sdl2.renderer, r.x + i, r.y + r.h + i, r.x + r.w + i, r.y + r.h + i);
        }
        
        // Set the draw color
        SDL_SetRenderDrawColor(
            p_window->sdl2.renderer,
            p_instance->theme.primary.r,
            p_instance->theme.primary.g,
            p_instance->theme.primary.b,
            0xff
        );
        
        SDL_RenderDrawLine(p_window->sdl2.renderer, r.x + r.w, r.y, r.x + r.w + (p_instance->font.size / 5), r.y + (p_instance->font.size / 5));
        SDL_RenderDrawLine(p_window->sdl2.renderer, r.x + r.w, r.y + r.h, r.x + r.w + (p_instance->font.size / 5), r.y + r.h + (p_instance->font.size / 5));
        SDL_RenderDrawLine(p_window->sdl2.renderer, r.x, r.y + r.h, r.x + (p_instance->font.size / 5), r.y + r.h + (p_instance->font.size / 5));
        SDL_RenderDrawLine(p_window->sdl2.renderer, r.x + (p_instance->font.size / 5), r.y + r.h + (p_instance->font.size / 5), r.x + r.w + (p_instance->font.size / 5), r.y + r.h + (p_instance->font.size / 5));
        SDL_RenderDrawLine(p_window->sdl2.renderer, r.x + r.w + (p_instance->font.size / 5), r.y + (p_instance->font.size / 5), r.x + r.w + (p_instance->font.size / 5), r.y + r.h + (p_instance->font.size / 5));

    }

    // Set the draw color
    SDL_SetRenderDrawColor(
        p_window->sdl2.renderer,
        p_instance->theme.accent_1.r,
        p_instance->theme.accent_1.g,
        p_instance->theme.accent_1.b,
        0xff
    );
    
    SDL_RenderFillRect(p_window->sdl2.renderer, &r);

    // Set the draw color
    SDL_SetRenderDrawColor(
        p_window->sdl2.renderer,
        p_instance->theme.primary.r,
        p_instance->theme.primary.g,
        p_instance->theme.primary.b,
        0xff
    );

    SDL_RenderDrawRect(p_window->sdl2.renderer, &r);
    
	d = (SDL_Rect)
	{
		.x = p_button->x + (p_instance->font.size / 10),
		.y = p_button->y - (p_instance->font.size / 20),
		.w = p_button->width,
		.h = p_instance->font.size + (p_instance->font.size / 5)
	};

	SDL_RenderCopy(p_window->sdl2.renderer, p_button->sdl2.texture, NULL, &d);

    //ui_draw_text(p_button->text, p_window, r.x + 5, r.y + 3, 1);

    // Success
    return 1;
}

bool button_in_bounds ( ui_button *button, ui_mouse_state mouse_state )
{

    // TODO: Argument check
    //

    // Initialized data
	i32  x = button->x,
		 y = button->y,
		 w = button->width,
		 h = button->height;
	
	// Bounds check
	return ( mouse_state.x >= x && mouse_state.y >= y && mouse_state.x <= x + w && mouse_state.y <= y + h );

    // TODO: Error check
}

int button_destroy ( ui_button **const pp_button )
{

    // Argument check
    if ( pp_button == (void *) 0 ) goto no_button;

    // Initialized data
    ui_button *p_button = *pp_button;

    // No more pointer for caller
    *pp_button = (void *) 0;

    // Free label string
    UI_REALLOC(p_button->text, 0);

    // Free the button memory
    UI_REALLOC(p_button, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_button:
                #ifndef NDEBUG
                    log_error("[ui] [button] Null pointer provided for \"pp_button\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
