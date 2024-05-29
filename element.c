#include <ui/element.h>

int create_element ( ui_element **pp_element )
{

    // Argument check
    if ( pp_element == (void *) 0 ) goto no_element;

    // Initialized data
    ui_element *p_element = calloc(1, sizeof(ui_element));
    
    // Error check
    if ( p_element == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_element = p_element;

    // Success
    return 1;

    // Error checking
    {

        // Argument errors
        {
            no_element:
                #ifndef NDEBUG
                    ui_print_error("[UI] [Element] Null pointer provided for \"element\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    ui_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_element_as_json_value(ui_element **pp_element, json_value *p_value)
{
    
    // Argument check
    if ( p_value == (void *) 0 ) goto no_value;

    // Initialized data
    json_value *p_type = 0,
               *p_name = 0;

    // Get properties from the dictionary
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        p_type = dict_get(p_dict, "type");
        p_name = dict_get(p_dict, "name");

        // Check for missing properties
        if ( ! ( p_type && p_name ) )
            goto missing_properties;
    }

    // Construct the element
    if ( p_type->type == JSON_VALUE_STRING )
    {
        
        // Initialized data
        int (*constructor)(void*, void*) = dict_get(load_lut, p_type->string);

        // Error checking
        if ( constructor == (void *) 0 ) goto not_implemented;

        // Call the element constructor for the specific type
        (*constructor)((void*) &p_value, p_value);

        // Construct the element
        construct_element(pp_element, p_name->string, p_type->string, p_value);
    }

    // Success
    return 1;

    missing_properties:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    ui_print_error("[UI] [Element] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            out_of_memory:
                #ifndef NDEBUG
                    ui_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Interface errors
        {
            not_implemented:
                #ifndef NDEBUG
                    ui_print_error("[UI] [Element] Failed to call constructor for type \"%s\" in call to function \"%s\"\n", p_type->string, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        no_name:
        no_type:
            return 0;
    }
}

int construct_element(ui_element **pp_element, char *p_name, char *p_type, void* element_data)
{
    
    // Argument check
    if ( element_data == (void *) 0 ) goto no_element_data;

    // Initialized data
    ui_element *p_element = 0;
    
    // Allocate memory for an element
    if ( create_element(&p_element) == 0 ) goto failed_to_allocate_element;

    // Construct the element
    {

        // Initialized data
        char *name = 0,
             *type = 0;
               
        // Set the name
        {

            // Initialized data
            size_t len = strlen(p_name);

            // Allocate memory for the name
            name = calloc(len + 1, sizeof(u8));

            // Error handling
            if ( name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(name, p_name, len);
        }

        // Set the type
        {

            // Initialized data
            size_t len = strlen(p_type);
            
            // Allocate memory for the name
            type = calloc(len + 1, sizeof(u8));

            // Error handling
            if ( type == (void *) 0 ) goto no_mem;

            // Copy the type
            strncpy(type, p_type, len);
        }

        // Populate the element
        *p_element = (ui_element)
        {
            .name = name,
            .type = type,
            .draw = true,
            .button = element_data // Not really a button. Rather, a pointer to the element 
        };
    }

    // Return a pointer to the caller 
    *pp_element = p_element;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_element_data:
            invalid_type:
            failed_to_allocate_element:
            no_mem:
                return 0;
        }
    }
}

int click_element ( ui_element* element, ui_mouse_state mouse_state )
{

    // TODO: Argument check
	//

    // Initialized data
    ui_instance *instanace = ui_get_active_instance();
    int (*click)(void*, ui_mouse_state) = dict_get(click_lut, element->type);

    // Set the last element
    instanace->active_window->last = element;
    
    // Call the element click function for the element type
    return (*click)((void*)element->label, mouse_state); 

    // TODO: Error handling
    {

    }
}

int hover_element ( ui_element* element, ui_mouse_state mouse_state )
{

    // TODO: Argument check
	//

    // Initialized data
    int (*hover)(void*, ui_mouse_state) = dict_get(hover_lut, element->type);

    // Call the element hover function for the specific type
    return (*hover)((void*)element->label, mouse_state);

    // TODO: Error handling
    {

    }
}

int release_element ( ui_element* element, ui_mouse_state mouse_state )
{

    // TODO: Argument check
	//

    // Initialized data
    ui_instance *instanace = ui_get_active_instance();
    int (*release)(void*, ui_mouse_state) = dict_get(release_lut, element->type);

    // Set the last element
    instanace->active_window->last = element;

    // Call the element release function for the specific type
    return (*release)((void*)element->label, mouse_state);

    // TODO: Error handling
    {

    }
}

int add_click_callback_element(ui_element* element, void(*callback)(ui_element*, ui_mouse_state))
{

    // TODO: Argument check
    //

    // Initialized data
    int (*add_click_callback)(void*, void(*callback)(ui_element*, ui_mouse_state)) = dict_get(add_click_lut, element->type);

    // Call the element add click function for the specific type
    return (*add_click_callback)((void*)element->label, callback);

    // TODO: Error handling
}

int add_hover_callback_element(ui_element* element, void(*callback)(ui_element*, ui_mouse_state))
{

    // TODO: Argument check
    //

    // Initialized data
    int (*add_hover_callback)(void*, void(*callback)(ui_element*, ui_mouse_state)) = dict_get(add_hover_lut, element->type);

    // Call the element add hover function for the specific type
    return (*add_hover_callback)((void*)element->label, callback);

    // TODO: Error handling
    {

    }
}

int add_release_callback_element(ui_element* element, void(*callback)(ui_element*, ui_mouse_state))
{

    // TODO: Argument check
    //

    // Initialized data
    int (*add_release_callback)(void*, void(*callback)(ui_element*, ui_mouse_state)) = dict_get(add_release_lut, element->type);

    // Call the element add release function for the specific type
    return (*add_release_callback)((void*)element->label, callback);

    // TODO: Error handling
    {

    }
}

bool in_bounds ( ui_element* element, ui_mouse_state mouse_state )
{

    // TODO: Argument check
    //

    // Initialized data
    bool (*bounds)(void*, ui_mouse_state) = dict_get(bounds_lut, element->type);

    // Call the element in bounds function for the specific type
    return (*bounds)((void*)element->label, mouse_state);

    // TODO: Error handling
    {

    }
}

int draw_element( ui_window *window, ui_element* element )
{

    // TODO: Argument check
    //

    // Construct the element
    if ( element->draw )
    {

        // Initialized data
        int (*drawer)(void*, void*) = dict_get(draw_lut, element->type);

        // Call the element constructor for the specific type
        return (*drawer)((void*)window, element->label);

    }

    // Success
    return 1;

    // TODO: Error handling
}

int destroy_element(ui_element **pp_element)
{

    // Argument check
    if ( pp_element == (void *) 0 ) goto no_element;
    
    // Initialized data
    ui_element *p_element = *pp_element;
    
    // Free the element
    free(p_element->label);
    free(p_element->name);
    free(p_element);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_element:
                #ifndef NDEBUG
                    printf("[UI] [Element] Null pointer provided for \"pp_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
