#include <ui/label.h>

int label_create ( ui_label **pp_label )
{

	// Argument check
	if ( pp_label == (void *) 0 ) goto no_label;

	// Initialized data
	ui_label *p_label = calloc(1, sizeof(ui_label));
	
	// Error checking
	if ( p_label == (void *) 0 ) goto no_mem;
	
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
					log_error("[UI] [Label] Null pointer provided for \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error 
				return 0;
		}

		// Standard Library errors
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

int label_load_as_json_value (ui_label**const pp_label, const json_value *const p_value)
{

	// Argument check
	if ( pp_label == (void *) 0 ) goto no_label;
	if ( p_value  == (void *) 0 ) goto no_value;

	// Initialized data
	ui_instance *p_instance = ui_get_active_instance();
	ui_label    *p_label    = 0;
	json_value   *p_text     = 0,
	             *p_x        = 0,
		         *p_y        = 0,
		         *p_size     = 0,
	             *p_color    = 0;

	// Get properties from the dictionary
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

		// Initialized data
		dict *p_dict = p_value->object;

        p_text  = dict_get(p_dict, "text");
        p_x     = dict_get(p_dict, "x");
		p_y     = dict_get(p_dict, "y");
		p_size  = dict_get(p_dict, "size");
		p_color = dict_get(p_dict, "color");

		// Error checking
		if ( ! ( p_text && p_x && p_y && p_size ) )
			goto missing_properties;
    }

	// Construct the label
	{

		// Allocate a label
		if ( label_create(&p_label) == 0 ) goto failed_to_allocate_label;

		// Copy the label text
		if ( p_text->type == JSON_VALUE_STRING )
		{

			// Initialized data
			size_t label_text_len = strlen(p_text->string);

			// Allocate memory for the label text
			p_label->text = calloc(label_text_len+1, sizeof(char));

			// Error check
			if ( p_label->text == (void *) 0 ) goto no_mem;

			// Copy the string
			strncpy(p_label->text, p_text->string, label_text_len);
		}

		// Set the x
		if ( p_x->type == JSON_VALUE_INTEGER)
			p_label->x = p_x->integer;
		// Default
		else
			goto wrong_x_type;

		// Set the y
		if ( p_y->type == JSON_VALUE_INTEGER)
			p_label->y = p_y->integer;
		// Default
		else
			goto wrong_y_type;

		// Set the size
		if ( p_size->type == JSON_VALUE_INTEGER)
			p_label->size = p_size->integer;
		// Default
		else
			goto wrong_size_type;
	}

	// Return
	*pp_label = p_label;

	// Success
	return 1;

	wrong_color_length:
	wrong_x_type:
	wrong_y_type:
	wrong_size_type:
	no_mem:
		return 0;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					log_error("[UI] [Label] Null pointer provided for \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_value:
				#ifndef NDEBUG
					log_error("[UI] [Label] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// TODO:
		failed_to_allocate_label:

		// Insufficent data error
		{
			// TODO: 
			missing_properties:
			no_x:
			no_y:
				return 0;
		}
	}
}

int label_draw ( ui_window *p_window, ui_label* p_label )
{

	// Argument check
	if ( p_window == (void *) 0 ) goto no_window;
	if ( p_label  == (void *) 0 ) goto no_label;

	// Initialized data
	ui_instance *p_instance = ui_get_active_instance();

	// Set the draw color
	SDL_SetRenderDrawColor(
		p_window->sdl2.renderer,
		p_instance->theme.primary.r,
		p_instance->theme.primary.g,
		p_instance->theme.primary.b,
		0xff
	);

	// Draw the label text
	ui_draw_text(p_label->text, p_window, p_label->x, p_label->y, p_label->size);

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
					log_error("[UI] [Label] Null pointer provided for parameter \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_label:
				#ifndef NDEBUG
					log_error("[UI] [Label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int label_click ( ui_label* p_label, ui_mouse_state mouse_state )
{

	// Success
	return 1;
}

int label_hover ( ui_label* p_label, ui_mouse_state mouse_state)
{

	// Success
	return 1;
}

int label_release ( ui_label* p_label, ui_mouse_state mouse_state)
{

	// Success
	return 1;
}

bool label_in_bounds ( ui_label  *p_label, ui_mouse_state mouse_state)
{

	// Argument check
	if ( p_label == (void *) 0 ) goto no_label;

	// Initialized data
	i32  x = p_label->x,
		 y = p_label->y,
		 w = p_label->width,
		 h = p_label->height;
	
	// Success
	return ( mouse_state.x >= x && mouse_state.y >= y && mouse_state.x <= x + w && mouse_state.y <= y + h );

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					log_error("[ui] [label] Null pointer provided for parameter \"p_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int print_label_to_file ( ui_label *p_label, FILE *p_f, char *name )
{

	// Initialized data
	json_value _value = { 0 },
			   _type  = { 0 },
			   _name  = { 0 },
			   _text  = { 0 },
			   _x     = { 0 },
			   _y     = { 0 },
			   _size  = { 0 };

	// Set the type
	_value.type = JSON_VALUE_OBJECT;

	// Construct a dictionary 
	dict_construct(&_value.object, 5, 0);

	// Populate the type struct
	_type = (json_value)
	{ 
		.type   = JSON_VALUE_STRING,
		.string = "LABEL"
	};

	// Populate the name struct
	_name = (json_value)
	{
		.type   = JSON_VALUE_STRING,
		.string = name
	};

	// Populate the text struct
	_text = (json_value)
	{
		.type   = JSON_VALUE_STRING,
		.string = p_label->text
	};

	// Populate the x struct
	_x = (json_value)
	{
		.type    = JSON_VALUE_INTEGER,
		.integer = p_label->x
	};

	// Populate the y struct
	_y = (json_value)
	{
		.type    = JSON_VALUE_INTEGER,
		.integer = p_label->y
	};

	// Populate the size struct
	_size = (json_value)
	{
		.type    = JSON_VALUE_INTEGER,
		.integer = p_label->size
	};

	// Add each property to the value
	dict_add(_value.object, "type", &_type);
	dict_add(_value.object, "text", &_text);
	dict_add(_value.object, "x"   , &_x);
	dict_add(_value.object, "y"   , &_y);
	dict_add(_value.object, "size", &_size);

	// If a name was supplied, add it
	dict_add(_value.object, "name", &_name);

	// Print the JSON value to the file
	json_value_fprint(&_value, p_f);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					log_error("[UI] [Label] Null pointer provided for parameter \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_file:
				#ifndef NDEBUG
					log_error("[UI] [Label] Null pointer provided for parameter \"p_f\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int label_destroy ( ui_label **pp_label )
{

	// Argument check
	if ( pp_label == (void *) 0 ) goto no_label;

	// Initialized data
	ui_label *p_label = *pp_label;

	// Clean up
	free(p_label);
	
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_label:
				#ifndef NDEBUG
					log_error("[UI] [Label] Null pointer provided for parameter \"pp_label\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}
