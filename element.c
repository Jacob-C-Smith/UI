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
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // External data
    extern dict *p_element_infos;
    
    // Initialized data
    ui_element *p_element = (void *) 0;
    dict       *p_dict    = p_value->object;
    json_value *p_type    = dict_get(p_dict, "type");

    // Check for missing properties
    if ( p_type == (void *) 0 ) goto missing_properties;

    // Type check
    if ( p_type->type != JSON_VALUE_STRING ) goto wrong_type_type;

    // Allocate memory for the element
    element_create(&p_element);

    // Store the element constructor
    p_element->p_functions = dict_get(p_element_infos, p_type->string);

    // Call the element constructor
    if ( p_element->p_functions->load(&p_element->p_element, p_value) == (void *) 0 ) goto failed_to_construct_element;

    // Return a pointer to the caller
    *pp_element = p_element;

    // Success
    return 1;

    missing_properties:
    failed_to_construct_element:
    no_name:
    no_type:
    wrong_type_type:
    wrong_type:
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
                    log_error("[standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Interface errors
        {
            not_implemented:
                #ifndef NDEBUG
                    log_error("[ui] [element] Failed to call constructor for type \"%s\" in call to function \"%s\"\n", p_type->string, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_element ( ui_element **pp_element )
{

    // Argument check
    if ( pp_element == (void *) 0 ) goto no_element;
    
    // Initialized data
    ui_element *p_element = *pp_element;
    
    // Call the element destructor
    if ( p_element->p_functions->destructor(&p_element->p_element) == 0 ) goto failed_to_free;

    // Cleanup
    if ( UI_REALLOC(p_element, 0) ) goto failed_to_free;

    // Success
    return 1;

    // Error handling
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
            failed_to_free:
                #ifndef NDEBUG
                    log_error("[ui] [element] Failed to free memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
