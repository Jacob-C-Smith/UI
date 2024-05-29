#include <ui/element.h>

int element_create ( ui_element **pp_element )
{

    // Argument check
    if ( pp_element == (void *) 0 ) goto no_element;

    // Initialized data
    ui_element *p_element = UI_REALLOC(0, sizeof(ui_element));
    
    // Error check
    if ( p_element == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_element, 0, sizeof(ui_element));

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
                    log_error("[ui] [element] Null pointer provided for \"pp_element\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int element_load_as_json_value ( ui_element **pp_element, json_value *p_value )
{
    
    // Argument check
    if ( p_value == (void *) 0 ) goto no_value;

    // Initialized data
    ui_element *p_element = (void *) 0;
    json_value *p_type = 0;

    // Get properties from the dictionary
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Store the type
        p_type = dict_get(p_dict, "type");

        // Check for missing properties
        if ( p_type == (void *) 0 ) goto missing_properties;
    }

    // Allocate memory for the element
    element_create(&p_element);

    // Construct the element
    if ( p_type->type == JSON_VALUE_STRING )
    {
        
        // External data
        extern dict *p_element_infos;

        p_element->p_functions = dict_get(p_element_infos, p_type->string);

        // Call the element constructor
        // TODO: Error check
        p_element->p_functions->load(&p_element->p_element, p_value);
    }

    // Return a pointer to the caller
    *pp_element = p_element;

    // Success
    return 1;

    missing_properties:
    no_name:
    no_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    log_error("[ui] [element] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            out_of_memory:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Interface errors
        {
            not_implemented:
                #ifndef NDEBUG
                    log_error("[UI] [Element] Failed to call constructor for type \"%s\" in call to function \"%s\"\n", p_type->string, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_element(ui_element **pp_element)
{

    // Argument check
    if ( pp_element == (void *) 0 ) goto no_element;
    
    // Initialized data
    ui_element *p_element = *pp_element;
    
    // Cleanup
    UI_REALLOC(p_element, 0);

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
