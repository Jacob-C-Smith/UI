#include <ui/window.h>

int window_create ( ui_window **pp_window )
{

	// Argument check
	if ( pp_window == (void *) 0 ) goto no_window;

	// Initialized data
	ui_window *p_window = UI_REALLOC(0, sizeof(ui_window));

	// Check memory
	if ( p_window == (void *) 0 ) goto no_mem;

	// Return a pointer to the caller
	*pp_window = p_window;

	// Initialize
	memset(p_window, 0, sizeof(ui_window));
	
	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_window:
				#ifndef NDEBUG
					log_error("[ui] [window] Null pointer provided for \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error 
				return 0;
		}
	}
}

int window_load ( ui_window **pp_window, const char *p_path )
{

	// Argument check
	if( p_path    == (void *) 0 ) goto no_path;
	if( pp_window == (void *) 0 ) goto no_window;

	// Initialized data
	size_t  len  = ui_load_file(p_path, 0, false);
	char   *data = UI_REALLOC(0, len * sizeof(u8));

	// Error checking
	if ( data == (void *) 0 ) goto no_mem;

	// Load the file
	if ( ui_load_file(p_path, data, false) == 0 ) goto failed_to_load_file;
	
	// Load the window as json text
	if ( window_load_as_json(pp_window, data) == 0 ) goto failed_to_load_window;
	
	// Cleanup
	UI_REALLOC(data, 0);
	
	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_window:
				#ifndef NDEBUG
					log_error("[ui] [window] Null pointer provided for \"pp_window\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_path:
				#ifndef NDEBUG
					log_error("[ui] [window] Null pointer provided for \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}

		// Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

		// ui errors
		{
			failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[ui] [window] Failed to load file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

			failed_to_load_window:
                #ifndef NDEBUG
                    log_error("[ui] [window] Failed to load window in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
		}
	}
}

int window_load_as_json ( ui_window **pp_window, char *text )
{

	// Argument check
	if ( pp_window == (void *) 0 ) goto no_window;
	if ( text      == (void *) 0 ) goto no_text;

	// Initialized data
	ui_instance *p_instance    = ui_get_active_instance();
	ui_window   *p_window      = 0;
	dict        *p_dict        = 0;
    json_value  *p_value       = 0,
	            *p_name        = 0,
	            *p_title       = 0,
	            *p_width       = 0,
	            *p_height      = 0,
	            *p_elements    = 0;

    // Parse the window file into a JSONValue
    if ( json_value_parse(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Is the json value the right type?
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;
	
	// Store the dictionary
	p_dict = p_value->object;

	// Store properties
	p_name     = dict_get(p_dict, "name");
	p_title    = dict_get(p_dict, "title");
	p_width    = dict_get(p_dict, "width");
	p_height   = dict_get(p_dict, "height");
	p_elements = dict_get(p_dict, "elements");
	
	// Error check
	if ( ! ( p_name && p_title && p_width && p_height && p_elements ) ) goto missing_elements;

	// Error check
	if ( p_name->type     != JSON_VALUE_STRING  ) goto wrong_name_type;
	if ( p_width->type    != JSON_VALUE_INTEGER ) goto wrong_width_type;
	if ( p_height->type   != JSON_VALUE_INTEGER ) goto wrong_height_type;
	if ( p_title->type    != JSON_VALUE_STRING  ) goto wrong_title_type;
	if ( p_elements->type != JSON_VALUE_OBJECT  ) goto wrong_elements_type;

	// Allocate memory for a UI window
	if ( window_create(&p_window) == 0 ) goto failed_to_allocate_window;

	// Set the name
	{

		// Initialized data
		size_t len = strlen(p_name->string);

		// Error checking
		if ( len < 1 )                         goto name_too_short;
		if ( len > UI_WINDOW_NAME_LENGTH_MAX ) goto name_too_long;

		// Copy the name
		strncpy(p_window->attributes._name, p_name->string, len);

		// Store a null terminator
		p_window->attributes._name[len] = '\0';
	}

	// Set the title
	{

		// Initialized data
		size_t len = strlen(p_title->string);

		// Error checking
		if ( len < 1 )                         goto title_too_short;
		if ( len > UI_WINDOW_NAME_LENGTH_MAX ) goto title_too_long;

		// Copy the title
		strncpy(p_window->attributes._title, p_title->string, len);

		// Store a null terminator
		p_window->attributes._title[len] = '\0';
	}

	// Set the width
	p_window->attributes.width = p_width->integer;

	// Set the height
	p_window->attributes.height = p_height->integer;

	// Create an SDL2 window
	p_window->sdl2.window = SDL_CreateWindow(p_window->attributes._name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_window->attributes.width, p_window->attributes.height, SDL_WINDOW_SHOWN);

	// These are required for window functionality
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

	// Create an SDL2 renderer
	p_window->sdl2.renderer = SDL_CreateRenderer(p_window->sdl2.window, -1, SDL_RENDERER_ACCELERATED);

	// Create a monitor
	monitor_create(&p_window->_monitor);

	p_instance->windows.active = p_window;

	// Construct window elements
	{
		
		// Initialized data
		size_t  element_count = 0;
		dict   *p_dict        = p_elements->object;
		const char *p_element_keys[64] = { 0 };
		json_value *p_element_values[64] = { 0 };

		// Store the size of the array
		element_count = dict_keys(p_dict, 0);

		dict_keys(p_dict, &p_element_keys);
		dict_values(p_dict, &p_element_values);

		dict_construct(&p_window->elements.lookup, element_count, 0);

		// Construct each element
		for (size_t i = 0; i < element_count; i++)
		{
			
			// Initialized data
			ui_element *p_element       = (void *) 0;
			json_value *p_element_value = 0;

			// Construct the element
			element_load_as_json_value(&p_element, p_element_values[i]);

			// Copy the name
			strncpy(p_element->_name, p_element_keys[i], 63);

			// Add the element to the lookup
			dict_add(p_window->elements.lookup, &p_element->_name, p_element);

			// Add the element to the list
			p_window->elements._data[i] = p_element;

			// Update the element counter
			p_window->elements.count++;
		}
	}
	
	// Return a pointer to the caller
	*pp_window = p_window;

	// Success
	return 1;
	
	// TODO: 
	missing_elements:
	wrong_name_type:
	failed_to_allocate_window:
	wrong_width_type:
	failed_to_parse_json:
	no_mem:
	failed_to_load_element:
	wrong_height_type:
	wrong_title_type:
	name_too_long:
	name_too_short:
	title_too_long:
	title_too_short:
	name_type_error:
	width_type_error:
	height_type_error:
	element_type_error:
	missing_properties:
	no_elements:
	wrong_elements_type:
	no_name:
	empty_name:
	wrong_type:

		// Error
		return 0;
		
	// Error handling
	{

		// Argument errors
		{
			no_window:
				#ifndef NDEBUG
					log_error("[UI] [Window] Null pointer provided for parameter \"pp_window\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					log_error("[UI] [Window] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// json errors
		{
			
		}
	}
}
 
int window_draw ( const ui_window *const p_window )
{

	// Argument check
	if ( p_window == (void *)0 ) goto no_window;

	// Initialized data
	ui_instance *instance = ui_get_active_instance();

	// color <- background
	SDL_SetRenderDrawColor(
		p_window->sdl2.renderer,
		(u8)instance->theme.background.r,
		(u8)instance->theme.background.g,
		(u8)instance->theme.background.b,
		0
	);

	// Clear the window
	SDL_RenderClear(p_window->sdl2.renderer);
	
	// State check
	if ( p_window->elements.count == 0 ) return 1;
	
	// Draw UI elements
	for (size_t i = 0; i < p_window->elements.count; i++)
	{

		// Initialized data
		ui_element *p_element = p_window->elements._data[i];

		// Draw the element
		p_element->p_functions->draw(p_window, p_element->p_element);
	}

	// Prevent overdraw by redrawing the active element
	// if ( p_window->last )
	//	draw_element(p_window, p_window->last);

	// Success
	return 1;

	// TODO: Error handling
	{

		// Argument errors
		{
			no_window:

				// Error
				return 0;
		}
	}
}

/*
int append_element_to_window ( ui_window *p_window, ui_element *element )
{

	// TODO: Argument check	

	dict_add(p_window->elements, element->name, element);
	
	if (p_window->element_count >= p_window->element_data_max)
		goto resize;

	resize_done:

	p_window->element_data[p_window->element_count] = element;

	p_window->element_count++;

	// Success
	return 1;
	// TODO: Error handling

	resize:
		p_window->element_data_max *= 2;
		p_window->element_data = realloc(p_window->element_data, sizeof(void *) * p_window->element_data_max);
		goto resize_done;
}

ui_element *find_element ( ui_window *p_window, char *name )
{

	// TODO: Argument check
	//

	// Initialized data
	ui_element *ret = dict_get(p_window->elements, name);
	
	// Return
	return ret;
}

int resize_window ( ui_window *p_window )
{
	
	// TODO: Argument check
	//

	// Set the window size
	SDL_SetWindowSize(p_window->window, p_window->width, p_window->height);

	// Success
	return 1;
}

int set_file_drop_operation ( ui_window *p_window, int (*callback_function)(ui_window *p_window, char *path))
{

	// Argument check
	if ( p_window          == (void *) 0 ) goto no_window;
	if ( callback_function == (void *) 0 ) goto no_callback_function;

	// Set the path drop function
	p_window->path_drop_function = callback_function;

	// Success
	return 1;

	// Error handling
	{
		
		// Argument errors
		{
			no_window:
				#ifndef NDEBUG
					log_error("[UI] [Window] Null pointer provided for parameter \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_callback_function:
				#ifndef NDEBUG
					log_error("[UI] [Window] Null pointer provided for parameter \"callback_function\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}
	}
}



int process_window_input ( ui_window *p_window )
{

	// Argument check
	if ( p_window == (void *) 0 ) goto no_window;

	// Initialized data
	ui_instance *instance = ui_get_active_instance();
	SDL_Event     e;

	dict_values(p_window->elements, p_window->element_data);

	// Process events
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{

		case SDL_QUIT:
			p_window->is_open = false;
			break;

		case SDL_KEYDOWN:
		{
			// const  u8* keyboardState = SDL_GetKeyboardState(NULL);

			// if (keyboardState[SDL_SCANCODE_F2])
			// {
			// 	destroy_window(window);
			// 	window = load_window("UI/UI-Window.json");
			// 	instance->windows = window;
			// 	window->is_open = true;
			// }
			if (e.key.keysym.sym == SDLK_BACKSPACE)
			{
				if (p_window->last)
				{
					if (strcmp(p_window->last->type, "TEXT INPUT") == 0)
					{
						ui_textinput* text_input = p_window->last->text_input;
						size_t text_len = strlen(text_input->text);
						
						text_input->text[text_len - 1] = (char)0;

						text_input->width = 8 + (8 * text_len - 1);

					}
				}
				else if (strcmp(p_window->last->type, "TABLE") == 0)
				{
					p_window->last->table->last_x++;
				}
			}
			else if (e.key.keysym.sym == SDLK_TAB)
			{
				if (p_window->last)
					if (strcmp(p_window->last->type, "TEXT INPUT") == 0)
					{
						ui_textinput *next       = 0;
						size_t last_i    = 0,
							   current_i = 0;

						// Find the current window->last index in the element list
						for (size_t i = 0; i < p_window->element_count; i++)
							if (p_window->element_data[i] == p_window->last)
								last_i = i;

						for (size_t i = p_window->element_count; i > 0; i--)
							if (strcmp(p_window->element_data[(last_i + i) % p_window->element_count]->type, "TEXT INPUT") == 0)
							{
								if ( p_window->element_data[(last_i + i) % p_window->element_count]->draw == false )
									continue;
									
								p_window->last = p_window->element_data[(last_i + i) % p_window->element_count];
							}
					}
			}

		}
		break;

		case SDL_MOUSEBUTTONUP:
		{
			ui_mouse_state mouse_state = { 0,0,0 };

			// Create the mouse_state struct
			{
				if (e.button.button == SDL_BUTTON_LEFT)
					mouse_state.button |= UI_M1;
				if (e.button.button == SDL_BUTTON_MIDDLE)
					mouse_state.button |= UI_M3;
				if (e.button.button == SDL_BUTTON_RIGHT)
					mouse_state.button |= UI_M2;
				if (e.wheel.y > 0)
				{
					mouse_state.button |= UI_SWUP;
				}
				if (e.wheel.y < 0)
				{
					mouse_state.button |= UI_SWDOWN;
				}
				mouse_state.x = e.button.x;
				mouse_state.y = e.button.y;
			}
			
			release_window(p_window, mouse_state);

		}
		break;

		case SDL_MOUSEBUTTONDOWN:
		{
			ui_mouse_state mouse_state = { 0,0,0 };

			// Create the mouse_state struct
			{
				if (e.button.button == SDL_BUTTON_LEFT)
					mouse_state.button |= UI_M1;
				if (e.button.button == SDL_BUTTON_MIDDLE)
					mouse_state.button |= UI_M3;
				if (e.button.button == SDL_BUTTON_RIGHT)
					mouse_state.button |= UI_M2;
				if (e.wheel.y > 0)
					mouse_state.button |= UI_SWUP;
				if (e.wheel.y < 0)
					mouse_state.button |= UI_SWDOWN;
				mouse_state.x = e.button.x;
				mouse_state.y = e.button.y;
			}

			click_window(p_window, mouse_state);
		}
		break;

		case SDL_MOUSEMOTION:
		{

			// Is the window being dragged?
			// TODO: Fix mouse cursor snapping to top left corner of window
			if (p_window->drag)
			{
				p_window->rx += e.motion.xrel,
				p_window->ry += e.motion.yrel;
			}

			// This is used for the oncoming callbacks
			ui_mouse_state mouse_state = { 0,0,0 };

			// Set the button state
			{

				// Left mouse click?
				if (e.button.button == SDL_BUTTON_LEFT)
					mouse_state.button |= UI_M1;

				// Right mouse click?
				if (e.button.button == SDL_BUTTON_RIGHT)
					mouse_state.button |= UI_M2;

				// Middle mouse click?
				if (e.button.button == SDL_BUTTON_MIDDLE)
					mouse_state.button |= UI_M3;

				// Scroll wheel?
				if (e.type == SDL_MOUSEWHEEL)
				{
					if (e.wheel.y > 0)
						mouse_state.button |= UI_SWUP;
					else if (e.wheel.y < 0)
						mouse_state.button |= UI_SWDOWN;
					else;
				}

				// Set the mouse location
				mouse_state.x = e.button.x;
				mouse_state.y = e.button.y;
			}


			hover_window(p_window, mouse_state);
		}
		break;

		case SDL_TEXTINPUT:
			if(p_window->last)
				if (strcmp(p_window->last->type, "TEXT INPUT") == 0)
				{//
					ui_textinput* text_input = p_window->last->text_input;
					strcat(text_input->text, e.text.text);
					text_input->width = 8 + (8 * strlen(text_input->text));
				}
			break;
		case SDL_WINDOWEVENT:
			break;
		case SDL_DROPFILE:
		{
			if ( p_window->path_drop_function )
				p_window->path_drop_function(p_window, e.drop.file);
		}
		default:
			break;
		}
	}
	
	if (p_window->drag)
	{
		int x = 0, y = 0;

		SDL_GetGlobalMouseState(&x, &y);
		SDL_SetWindowPosition(p_window->window, x, y);
	}
	return 0;
	
	// TODO: Error handling
	{
		no_mem:
		no_window:
			return 0;
	}
}

int click_window ( ui_window *p_window, ui_mouse_state mouse_state )
{

	// TODO: Argument check
	//

	// Initialized data
	dict          *elements = p_window->elements;
	void         **values   = 0;
	ui_instance  *instance = ui_get_active_instance();

	// Did the user click on the element on the iterator?
	for (size_t i = 0; i < p_window->element_count; i++)
		if ( in_bounds(p_window->element_data[i], mouse_state) )
			click_element(p_window->element_data[i], mouse_state);

	// Close window?
	{
		int w, h;

		SDL_GetWindowSize(p_window->window, &w, &h);
		
		if (mouse_state.x >= w - 48 && mouse_state.y >= 0 && mouse_state.x <= w - 20 && mouse_state.y <= 11)
		{

			// Left clicks minimize the window
			if (mouse_state.button & UI_M2)
				SDL_MinimizeWindow(p_window->window);

			// Right clicks close the window
			else if (mouse_state.button & UI_M1)
				p_window->is_open = false;
		}
	}
	
	// Drag window?
	if(p_window->drag == false) {
		int w, h;

		SDL_GetWindowSize(p_window->window, &w, &h);
		p_window->rx = 0;
		p_window->ry = 0;
		if (mouse_state.x >= 0 && mouse_state.y >= 0 && mouse_state.x <= w - 48 && mouse_state.y <= 11)
		{
			// Drag the window
			if (mouse_state.button & UI_M1)
			{
				
				p_window->rx = mouse_state.x,
				p_window->ry = mouse_state.y;
				p_window->drag = true;
			}
		}
	}

	// Success
	return 1;

	// TODO: Error handling
}

int hover_window ( ui_window *p_window, ui_mouse_state mouse_state )
{

	// TODO: Argument check
	//

	// Initialized data
	dict         *elements = p_window->elements;
	ui_instance *instance = ui_get_active_instance();

	if (p_window->drag)
		SDL_SetWindowPosition(p_window, p_window->rx+mouse_state.x, p_window->ry+mouse_state.y);

	// Iterate over elements

	// Did the user click on the element on the iterator?
	for (size_t i = 0; i < p_window->element_count; i++)
		if (in_bounds(p_window->element_data[i], mouse_state))
			hover_element(p_window->element_data[i], mouse_state);

	// Success
	return 1;
}

int release_window ( ui_window *p_window, ui_mouse_state mouse_state )
{

	// TODO: Argument check
	//

	// Initialized data
	dict* elements = p_window->elements;
	ui_instance* instance = ui_get_active_instance();

	if (p_window->drag) 
		p_window->drag = false;

	// Iterate over elements

	// Did the user click on the element on the iterator?
	for (size_t i = 0; i < p_window->element_count; i++)
		if (in_bounds(p_window->element_data[i], mouse_state))
			release_element(p_window->element_data[i], mouse_state);

	// Success
	return 1;

	// TODO: Error handling
}

int destroy_window ( ui_window *p_window )
{

	// Argument check
	if ( p_window == (void *) 0 ) goto no_window;

	// Close the window
	p_window->is_open = false;

	// Free the window name
	free(p_window->name);

	// Free the window title
	free(p_window->title);

	// Free the window elements
	//dict_free_clear(p_window->elements, destroy_element);
	free(p_window->element_data);

	// Free the SDL2 renderer
	SDL_DestroyRenderer(p_window->renderer);

	// Free the SDL2 window
	SDL_DestroyWindow(p_window->window);

	// Free the UI Window
	free(p_window);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_window:
				#ifndef NDEBUG
					printf("[UI] [Window] Null pointer provided for parameter \"p_window\" in call to function \"%s\"\n");
				#endif

				// Error
				return 0;
		}
	}
}
*/
