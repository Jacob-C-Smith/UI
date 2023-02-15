#include <UI/UICheckbox.h>

int create_checkbox              ( UICheckbox_t **checkbox )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(checkbox == (void *)0)
                goto no_checkbox;
        #endif
    }

    // Allocate for a checkbox
    UICheckbox_t* i_checkbox = calloc(1, sizeof(UICheckbox_t));

    // Check memory
    {
        if(i_checkbox == (void *)0)
            goto no_mem;
    }

    *checkbox = i_checkbox;

    return 1;

    // TODO: Error handling
    {
        // Argument errors
        {
            no_checkbox:
            #ifndef NDEBUG

            #endif
            return 0;
        }

        // Standard library errors
        {
            no_mem:
                ui_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                return 0;
        }
    }
}

int load_checkbox_as_dict ( UICheckbox_t **checkbox, dict *dictionary )
{

    // TODO: Argument check
    {
        #ifndef NDEBUG
            if(checkbox == (void *)0)
                goto no_tokens;
            if (dictionary == 0)
                goto no_token_count;
        #endif
    }

    // Initialized data
    UICheckbox_t *ret           = 0;
    size_t        j             = 0,
                  label_count   = 0,
                  longest_label = 0;
    char        **labels        = 0,
                 *x             = 0,
                 *y             = 0,
                **checked       = 0;
    bool          hidden        = false;

    // Get data for the constructor
    {
        JSONToken_t *token = 0;

        token   = dict_get(dictionary, "labels");
        labels  = JSON_VALUE(token, JSONarray);

        token   = dict_get(dictionary, "x");
        x       = JSON_VALUE(token, JSONprimative);

        token   = dict_get(dictionary, "y");
        y       = JSON_VALUE(token, JSONprimative);

        token   = dict_get(dictionary, "hidden");
        hidden  = JSON_VALUE(token, JSONprimative);

        token   = dict_get(dictionary, "checked");
        checked = JSON_VALUE(token, JSONarray);

    }

    construct_checkbox(checkbox, labels, checked, atoi(x), atoi(y));

    return 1;

    // Error handling
    {
        notACheckbox:
            ui_print_error("[UI] [Checkbox] NOT A CHECKBOX\n");

            return 0;

        // Standard library errors
        {
        out_of_memory:
            ui_print_error("[Standard library] Out of memory in call to function \"%s\"\n");
            return 0;
        }

        // Argument errors
        {
            no_tokens:
            ui_print_error("[UI] [Checkbox] Null pointer provided for \"tokens\" in call to function \"%s\"\n", __FUNCTION__);
            return 0;

        no_token_count:
            ui_print_error("[UI] [Checkbox] \"token_count\" is zero in call to function \"%s\"\n", __FUNCTION__);
            return 0;
        }


        // Missing JSON errors
        {
        no_checkbox_name:
            ui_print_error("[UI] [Checkbox] No \"name\" in \"token\" in call to function \"%s\"\n", __FUNCTION__);
            return 0;
        no_checkbox_text:
            ui_print_error("[UI] [Checkbox] No \"labels\" in \"token\" in call to function \"%s\"\n", __FUNCTION__);
            return 0;
        no_checkbox_x:
            ui_print_error("[UI] [Checkbox] No \"x\" in \"token\" in call to function \"%s\"\n", __FUNCTION__);
            return 0;
        no_checkbox_y:
            ui_print_error("[UI] [Checkbox] No \"y\" in \"token\" in call to function \"%s\"\n", __FUNCTION__);
            return 0;
        }
    }
   
}

int construct_checkbox(UICheckbox_t** checkbox, char** labels, bool* checked, i32 x, i32 y)
{
    // TODO: Argument check
    {

    }

    // Initialized data
    UICheckbox_t *i_checkbox = 0;

    create_checkbox(checkbox);
    
    i_checkbox = *checkbox;

    // Construct the checkbox
    {

        // Initialized data
        size_t label_count   = 0,
               longest_label = 0;

        // Count labels
        while (labels[++label_count]);

        i_checkbox->label_count = label_count;

        dict_construct(&i_checkbox->labels, label_count);

        // Allocate for and copy labels, check states
        for (size_t i = 0; i < label_count; i++)
        {

            // Initialized data
            char   *label   = labels[i],
                   *i_label = 0;
            size_t  i_length = strlen(label);

            
            longest_label = (i_length > longest_label) ? i_length : longest_label;


            // Allocate for the option
            i_label = calloc(i_length + 1, sizeof(char));

            // TOOD: Memory check

            // Copy the option string
            strncpy(i_label, label, i_length);

            i_checkbox->labels[i] = i_label;

        }

        i_checkbox->longest_label = longest_label;
        i_checkbox->checks        = calloc(label_count, sizeof(bool));

        // Set x
        i_checkbox->x = x;

        // Set y
        i_checkbox->y = y;

    }

    return 1;

    // TODO: Error handling

}

int           hover_checkbox               ( UICheckbox_t *checkbox, ui_mouse_state_t mouse_state)
{
    s32 x = mouse_state.x - checkbox->x,
        y = mouse_state.y - checkbox->y;

    if (y >= 0)
    {
        checkbox->hover_index = (y / 15) ;
        if (checkbox->hover_index > checkbox->label_count)
            checkbox->hover_index = checkbox->label_count - 1;
    }
    else
    {
        checkbox->hover_index = -1;
    }

    
    return 0;
}

int           click_checkbox               ( UICheckbox_t *checkbox, ui_mouse_state_t mouse_state)
{
    if(checkbox->hover_index != -1)
        checkbox->checks[checkbox->hover_index] ^= true;

    // Iterate through callbacks
    for (size_t i = 0; i < checkbox->on_click_count; i++)
    {
        // Define the callback function
        void (*callback)(UICheckbox_t*, ui_mouse_state_t) = checkbox->on_click[i];

        // Call the callback function
        (*callback)(checkbox, mouse_state);

    }

    return 0;
}

int           release_checkbox              ( UICheckbox_t *checkbox, ui_mouse_state_t mouse_state )
{
    // Iterate through callbacks
    for (size_t i = 0; i < checkbox->on_release_count; i++)
    {
        // Define the callback function
        void (*callback)(UICheckbox_t*, ui_mouse_state_t) = checkbox->on_release[i];


        // Call the callback function
        if (callback)
            (*callback)(checkbox, mouse_state);

    }

    return 0;
}

int add_click_callback_checkbox(UICheckbox_t* checkbox, void(*callback)(UICheckbox_t*, ui_mouse_state_t))
{
    // TODO:
    return 0;
}

int add_hover_callback_checkbox(UICheckbox_t* checkbox, void(*callback)(UICheckbox_t*, ui_mouse_state_t))
{
    // TODO:
    return 0;
}

int add_release_callback_checkbox(UICheckbox_t* checkbox, void(*callback)(UICheckbox_t*, ui_mouse_state_t))
{
    // TODO:
    return 0;
}

int           draw_checkbox                ( UIWindow_t   *window  , UICheckbox_t *checkbox )
{
    UIInstance_t* instance = ui_get_active_instance();
    SDL_Rect r = { checkbox->x, checkbox->y, 12, 12 };

    checkbox->width  = 12,
    checkbox->height = 18*checkbox->label_count;

    for (size_t i = 0; i < checkbox->label_count; i++)
    {
        SDL_SetRenderDrawColor(window->renderer, (u8)instance->primary, (u8)(instance->primary >> 8), (u8)(instance->primary >> 16), 0xff);
        SDL_RenderDrawRect(window->renderer, &r);

        bool *v = checkbox->checks;

        if (v[i] == true)
            ui_draw_text("\200", window, r.x + 2, r.y + 1, 1);

        ui_draw_text(checkbox->labels[i], window, r.x + 14, r.y + 2, 1);

        r.y += 15;
    }

    return 0;

}


bool       checkbox_in_bounds  ( UICheckbox_t  *checkbox, ui_mouse_state_t mouse_state)
{
	// Initialized data
	i32  x = checkbox->x,
		 y = checkbox->y,
		 w = checkbox->width,
		 h = checkbox->height;
	
	// Check for bounds
	if (mouse_state.x >= x && mouse_state.y >= y && mouse_state.x <= x + w && mouse_state.y <= y + h)
		return true;

	return false;

}


int           destroy_checkbox             ( UICheckbox_t *checkbox )
{
    free(checkbox);
    return 0;
}
