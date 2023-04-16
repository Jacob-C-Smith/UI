#include <UI/UILabel.h>

int create_label ( UILabel_t **pp_label )
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_label == (void *)0)
				goto no_label;
		#endif
	}

	// Initialized data
	UILabel_t *p_label = calloc(1, sizeof(UILabel_t));
	
	// Error checking
	{
		#ifndef NDEBUG
			if (p_label == (void*)0)
				goto no_mem;
		#endif
	}
	
	// Return
	*pp_label = p_label;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error 
				return 0;
		}

		// Standard Library errors
		{
			no_mem:
				#ifndef NDEBUG
					ui_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error 
				return 0;
		}
	}
}

int load_label_as_json_value (UILabel_t** pp_label, JSONValue_t *p_value)
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_label == (void *)0)
				goto no_label;
			if (p_value == (void*)0)
				goto no_value;
		#endif
	}

	// Initialized data
	UILabel_t *p_label = 0;
	char      *text    = 0;
	signed     x       = 0,
		       y       = 0,
		       size    = 0;

	// Get properties from the dictionary
    if (p_value->type == JSONobject)
    {

        // Initialized data
        dict *p_dict = p_value->object;

        text = JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "text")), JSONstring);
        x    = JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "x"))   , JSONinteger);
		y    = JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "y"))   , JSONinteger);
		size = JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "size")), JSONinteger);
    }

	// Error checking
	{
		#ifndef NDEBUG
			if(!(text && x && y && size))
				goto no_text;
		#endif
	}

	// Construct the label
	{

		// Allocate a label
		if ( create_label(&p_label) == 0)
			goto failed_to_allocate_label;

		// Copy the label text
		{

			// Initialized data
			size_t label_text_len = strlen(text);

			// Allocate memory for the label text
			p_label->text = calloc(label_text_len+1, sizeof(char));

			// TODO: Check memory

			// Copy the string
			strncpy(p_label->text, text, label_text_len);
		}

		// Set the label x, y, and size
		p_label->x    = x;
		p_label->y    = y;
		p_label->size = size;
	}

	// Return
	*pp_label = p_label;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
			no_value:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// TODO:
		failed_to_allocate_label:

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

int draw_label ( UIWindow_t *p_window, UILabel_t* p_label )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_window == (void *) 0 )
				goto no_window;
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Initialized data
	UIInstance_t *p_instance = ui_get_active_instance();

	// Don't draw a hidden label
	if (p_label->hidden == false)
	{

		// Set the draw color
		SDL_SetRenderDrawColor(p_window->renderer, (u8)p_instance->primary, (u8)(p_instance->primary >> 8), (u8)(p_instance->primary >> 16), 0xff);

		// Draw the label text
		ui_draw_text(p_label->text, p_window, p_label->x, p_label->y, p_label->size);
	}

	// Update the width and the height
	p_label->width  = strlen(p_label->text) * 8 * p_label->size;
	p_label->height = 8 * p_label->size;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_window:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int click_label ( UILabel_t* p_label, ui_mouse_state_t mouse_state)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Iterate through callbacks
	for (size_t i = 0; i < p_label->on_click_count; i++)
	{
		// Define the callback function
		void (*callback)(UILabel_t*, ui_mouse_state_t) = p_label->on_click[i];

		// Call the callback function
		(*callback)(p_label, mouse_state);

	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int hover_label ( UILabel_t* p_label, ui_mouse_state_t mouse_state)
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}
	
	// Iterate through callbacks
	for (size_t i = 0; i < p_label->on_hover_count; i++)
	{

		// Define the callback function
		void (*callback)(UILabel_t*, ui_mouse_state_t) = p_label->on_hover[i];

		// Call the callback function
		if (callback)
			(*callback)(p_label, mouse_state);

	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int release_label ( UILabel_t* p_label, ui_mouse_state_t mouse_state)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Iterate through callbacks
	for (size_t i = 0; i < p_label->on_release_count; i++)
	{
		// Define the callback function
		void (*callback)(UILabel_t*, ui_mouse_state_t) = p_label->on_release[i];


		// Call the callback function
		if (callback)
			(*callback)(p_label, mouse_state);

	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int add_click_callback_label ( UILabel_t* p_label, void(*callback)(UILabel_t*, ui_mouse_state_t))
{

    // Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

    // If this is the first callback, set the max to 1 and 
    if (p_label->on_click_max == 0)
    {
		p_label->on_click_max = 1;
		p_label->on_click = calloc(1, sizeof(void*));
    }

    // Simple heuristic that doubles callbacks lists length when there is no space to store the callback pointer
    if (p_label->on_click_count + 1 > p_label->on_click_max)
    {
        // Double the max
		p_label->on_click_max *= 2;

        realloc(p_label->on_click, p_label->on_click_max);
		
		if (p_label->on_click == (void *)0)
			goto no_mem;
    }

    // Increment the callback counter and install the new callback
	p_label->on_click[p_label->on_click_count++] = callback;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					ui_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error 
				return 0;
		}
	}
}

int add_hover_callback_label ( UILabel_t* p_label, void(*callback)(UILabel_t*, ui_mouse_state_t))
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int add_release_callback_label ( UILabel_t* p_label, void(*callback)(UILabel_t*, ui_mouse_state_t))
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

bool label_in_bounds ( UILabel_t  *p_label, ui_mouse_state_t mouse_state)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Initialized data
	i32  x = p_label->x,
		 y = p_label->y,
		 w = p_label->width,
		 h = p_label->height;
	
	// Check for intersection
	if (mouse_state.x >= x && mouse_state.y >= y && mouse_state.x <= x + w && mouse_state.y <= y + h)

		// Intersect
		return true;

	// No intersect
	return false;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int  print_label_to_file ( UILabel_t *p_label, FILE *f, char *name )
{
	JSONValue_t *p_value = calloc(1, sizeof(JSONValue_t));
	
	p_value->type = JSONobject;
	dict_construct(&p_value->object,5);

	{

		// Initialized data
		JSONValue_t *p_type = calloc(1, sizeof(JSONValue_t)),
		            *p_name = calloc(1, sizeof(JSONValue_t)),
		            *p_text = calloc(1, sizeof(JSONValue_t)),
		            *p_x    = calloc(1, sizeof(JSONValue_t)),
		            *p_y    = calloc(1, sizeof(JSONValue_t)),
		            *p_size = calloc(1, sizeof(JSONValue_t));

		{

			p_type->type = JSONstring;
			p_type->string = "LABEL";
			
			p_name->type = JSONstring;
			p_name->string = name;

			p_text->type = JSONstring;
			p_text->string = p_text;

			p_x->type = JSONstring;
			p_x->string = p_text;

			p_y->type = JSONstring;
			p_y->string = p_text;

			p_size->type = JSONstring;
			p_size->string = p_text;

			dict_add(p_value->object, "type", p_type);
			dict_add(p_value->object, "name", p_name);
			dict_add(p_value->object, "text", p_text);
			dict_add(p_value->object, "x", p_x);
			dict_add(p_value->object, "y", p_y);
			dict_add(p_value->object, "size", p_size);
		}

		print_json_value(p_value, f);

	}
}

void destroy_label ( UILabel_t  **pp_label )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_label == (void *) 0 )
				goto no_label;
		#endif
	}

	// Initialized data
	UILabel_t *p_label = pp_label;

	// Free the label text and the callbacks
	free(p_label->text);
	free(p_label->on_click);
	free(p_label->on_hover);
	free(p_label->on_release);

	// Free the label
	free(p_label);

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					ui_print_error("[UI] [Label] Null pointer provided for parameter \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}
