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

    }
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
