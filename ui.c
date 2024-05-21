/** !
 * UI library
 * 
 * @file ui.c
 * 
 * @author Jacob Smith
 */

// Header
#include <ui/ui.h>

// Preprocessor definitions
#define UI_CONFIG_FILE_LENGTH_MAX 2048

// Data
// Each u64 encodes an 8x8 glyph. ASCII values can be used to index characters. Add whatever you want so long as you keep the length under 255
u64 font[137] = {
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x183C3C1818001800, 0x3636000000000000, 0x36367F367F363600, 0x0C3E031E301F0C00, 0x006333180C666300, 0x1C361C6E3B336E00, 0x0606030000000000,
    0x180C0606060C1800, 0x060C1818180C0600, 0x00663CFF3C660000, 0x000C0C3F0C0C0000, 0x00000000000C0C06, 0x0000003F00000000, 0x00000000000C0C00, 0x6030180C06030100,
    0x3E63737B6F673E00, 0x0C0E0C0C0C0C3F00, 0x1E33301C06333F00, 0x1E33301C30331E00, 0x383C36337F307800, 0x3F031F3030331E00, 0x1C06031F33331E00, 0x3F3330180C0C0C00,
    0x1E33331E33331E00, 0x1E33333E30180E00, 0x000C0C00000C0C00, 0x000C0C00000C0C06, 0x180C0603060C1800, 0x00003F00003F0000, 0x060C1830180C0600, 0x1E3330180C000C00,
    0x3E637B7B7B031E00, 0x0C1E33333F333300, 0x3F66663E66663F00, 0x3C66030303663C00, 0x1F36666666361F00, 0x7F46161E16467F00, 0x7F46161E16060F00, 0x3C66030373667C00,
    0x3333333F33333300, 0x1E0C0C0C0C0C1E00, 0x7830303033331E00, 0x6766361E36666700, 0x0F06060646667F00, 0x63777F7F6B636300, 0x63676F7B73636300, 0x1C36636363361C00,
    0x3F66663E06060F00, 0x1E3333333B1E3800, 0x3F66663E36666700, 0x1E33070E38331E00, 0x3F2D0C0C0C0C1E00, 0x3333333333333F00, 0x33333333331E0C00, 0x6363636B7F776300,
    0x6363361C1C366300, 0x3333331E0C0C1E00, 0x7F6331184C667F00, 0x1E06060606061E00, 0x03060C1830604000, 0x1E18181818181E00, 0x081C366300000000, 0x00000000000000FF,
    0x0C0C180000000000, 0x00001E303E336E00, 0x0706063E66663B00, 0x00001E3303331E00, 0x3830303e33336E00, 0x00001E333f031E00, 0x1C36060f06060F00, 0x00006E33333E301F,
    0x0706366E66666700, 0x0C000E0C0C0C1E00, 0x300030303033331E, 0x070666361E366700, 0x0E0C0C0C0C0C1E00, 0x0000337F7F6B6300, 0x00001F3333333300, 0x00001E3333331E00,
    0x00003B66663E060F, 0x00006E33333E3078, 0x00003B6E66060F00, 0x00003E031E301F00, 0x080C3E0C0C2C1800, 0x0000333333336E00, 0x00003333331E0C00, 0x0000636B7F7F3600,
    0x000063361C366300, 0x00003333333E301F, 0x00003F190C263F00, 0x380C0C070C0C3800, 0x1818180018181800, 0x070C0C380C0C0700, 0x6E3B000000000000, 0x0000000000000000,
    0x80C0E070371E1C08, 0x00001c3e3e3e1c00, 0x040C1C3C3C1C0C04, 0x0000ff4224180000, 0x7E7E7E7E7E7E3C18, 0x0709FF81818181FF, 0x784C46424242427E, 0x183C7E1818181818,
    0xC3E77e3c3c7eE7C3};

const char _default_config[] = "{\n"\
                               "    \"name\"       : \"Default\",\n"\
                               "    \"primary\"    : [ 0, 0, 0 ],\n"\
                               "    \"accent 1\"   : [ 192, 192, 192 ],\n"\ 
                               "    \"accent 2\"   : [ 128, 128, 128 ],\n"\
                               "    \"accent 3\"   : [ 0, 128, 255 ],\n"\
                               "    \"background\" : [ 255, 255, 255 ]\n"\ 
                               "}\n";
ui_instance _active_instance = { 0 };
static bool initialized = false;

// Function declarations
size_t ui_load_file ( const char *path, void *buffer, bool binary );

void ui_init ( void )
{

    // State check
    if ( initialized ) return;

    // Initialized data
    FILE *p_config_file = (void *) 0;
    const char *home = (void *) 0;
    char _config_file_path[1023+1] = { 0 };
    char _config_file_contents[UI_CONFIG_FILE_LENGTH_MAX] = { 0 };

    // Initialize modules
    {

        // Initialize the log library
        log_init();

        // Initialize the sync library
        sync_init();

        // Initialize the hash cache library
        hash_cache_init();

        // Initialize the array library
        array_init();

        // Initialize the dict library
        dict_init();

        // Initialize the queue library
        queue_init();

        // Initialize the json library
        json_init();

        // Initialize the parallel library
        parallel_init();
    }

    // Locate the config file
    {

        // Locate the config file
        home = getenv("HOME");

        // Error check
        if ( home == (void *) 0 ) goto failed_to_find_home;

        // Build the config file path
        snprintf(&_config_file_path, 1023, "%s/ui_config.json", home);

        // Open the config file
        p_config_file = fopen(_config_file_path, "r");

        // Error check
        if ( p_config_file == (void *) 0 ) goto construct_config_file;

        config_file_present:
        
        // Log the config file path to standard out
        log_info("[ui] Loading config from \"%s\"\n", _config_file_path);
    }

    // Load the config file
    {
        
        // Initialized data
        size_t len = ui_load_file(_config_file_path, 0, false);

        // Error check
        if ( len > UI_CONFIG_FILE_LENGTH_MAX - 1 ) goto config_file_too_long;

        // Load the file
        ui_load_file(_config_file_path, _config_file_contents, false);
    }

    // Parse the config file
    {
        
        // Initialized data
        const json_value *p_value      = (void *) 0,
                         *p_name       = (void *) 0,
                         *p_primary    = (void *) 0,
                         *p_accent_1   = (void *) 0,
                         *p_accent_2   = (void *) 0,
                         *p_accent_3   = (void *) 0,
                         *p_background = (void *) 0;
        dict *p_dict = 0;

        // Parse the config file into a json value
        if ( json_value_parse(&_config_file_contents, 0, &p_value) == 0 ) goto failed_to_parse_json;

        // Error check
        if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

        // Store the dictionary
        p_dict = p_value->object;

        // Store the properties
        p_name       = dict_get(p_dict, "name");
        p_primary    = dict_get(p_dict, "primary");
        p_accent_1   = dict_get(p_dict, "accent 1");
        p_accent_2   = dict_get(p_dict, "accent 2");
        p_accent_3   = dict_get(p_dict, "accent 3");
        p_background = dict_get(p_dict, "background");

        // Error check
        if ( (
                p_name       &&
                p_primary    &&
                p_accent_1   &&
                p_accent_2   &&
                p_accent_3   &&
                p_background
        ) == false )
            goto missing_properties;

        // Error check
        if ( p_name->type != JSON_VALUE_STRING ) goto wrong_name_type;

        // Store the name
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Error check
            if ( len > UI_INSTANCE_NAME_LENGTH_MAX - 1 ) goto name_too_long;

            // Copy the name
            strncpy(_active_instance._name, p_name->string, len);
        }

        // Set the color theme
        {

            // Set the primary color
            if ( p_primary->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t array_len = 0;
                json_value *pp_array[3] = {0, 0, 0};

                // Get the array contents
                {

                    // Get the length of the array
                    array_get(p_primary->list, 0, &array_len);

                    // Check for a valid array length
                    if ( array_len != 3 ) goto failed_primary;

                    // Get the R, G, and B values
                    array_get(p_primary->list, pp_array, &array_len);
                }

                // Store the color
                _active_instance.theme.primary = (color)
                {
                    .r = pp_array[0]->integer,
                    .g = pp_array[1]->integer,
                    .b = pp_array[2]->integer,
                    .a = 0xFF
                };
            }
            // Default
            else
                goto wrong_primary_type;

            // Set accent color 1
            if ( p_accent_1->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t array_len = 0;
                json_value *pp_array[3] = {0, 0, 0};

                // Get the contents of the array
                {

                    // Get the length of the array
                    array_get(p_accent_1->list, 0, &array_len);

                    // Check for a valid array length
                    if ( array_len != 3 ) goto failed_accent_1;

                    // Get the R, G, and B values
                    array_get(p_accent_1->list, pp_array, &array_len);
                }

                // Store the color
                _active_instance.theme.accent_1 = (color)
                {
                    .r = pp_array[0]->integer,
                    .g = pp_array[1]->integer,
                    .b = pp_array[2]->integer,
                    .a = 0xFF
                };
            }
            // Default
            else
                goto wrong_accent_1_type;

            // Set accent color 2
            if ( p_accent_2->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t array_len = 0;
                json_value *pp_array[3] = {0, 0, 0};

                // Get the contents of the array
                {
                    // Get the length of the array
                    array_get(p_accent_2->list, 0, &array_len);

                    // Check for a valid array length
                    if ( array_len != 3 ) goto failed_accent_2;

                    // Get the R, G, and B values
                    array_get(p_accent_2->list, pp_array, &array_len);
                }

                // Store the color
                _active_instance.theme.accent_2 = (color)
                {
                    .r = pp_array[0]->integer,
                    .g = pp_array[1]->integer,
                    .b = pp_array[2]->integer,
                    .a = 0xFF
                };
            }
            // Default
            else
                goto wrong_accent_2_type;

            // Set accent color 3
            if( p_accent_3->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t array_len = 0;
                json_value *pp_array[3] = {0, 0, 0};

                // Dump the contents of the array
                {
                    // Get the length of the array
                    array_get(p_accent_3->list, 0, &array_len);

                    // Check for a valid array length
                    if ( array_len != 3 ) goto failed_accent_3;

                    // Get the R, G, and B values
                    array_get(p_accent_3->list, pp_array, &array_len);
                }

                // Store the color
                _active_instance.theme.accent_3 = (color)
                {
                    .r = pp_array[0]->integer,
                    .g = pp_array[1]->integer,
                    .b = pp_array[2]->integer,
                    .a = 0xFF
                };
            }
            // Default
            else
                goto wrong_accent_3_type;

            // Set background
            if ( p_background->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t array_len = 0;
                json_value *pp_array[3] = {0, 0, 0};

                // Dump the contents of the array
                {

                    // Get the length of the array
                    array_get(p_background->list, 0, &array_len);

                    // Check for a valid array length
                    if ( array_len != 3 ) goto failed_background;

                    // Get the R, G, and B values
                    array_get(p_background->list, pp_array, &array_len);
                }

                // Store the color
                _active_instance.theme.background = (color)
                {
                    .r = pp_array[0]->integer,
                    .g = pp_array[1]->integer,
                    .b = pp_array[2]->integer,
                    .a = 0xFF
                };
            }

            // Default
            else goto wrong_background_type;
        }
    
        // Clean up
        json_value_free(p_value);
    }

    // Initialize SDL2
    {

        // Fixes blurry rendering on Mac OS
        #ifdef __APPLE__
            SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        #endif

        // Initialize SDL
        if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0 ) goto failed_to_init_sdl2;
    }

    // Initialize the window system
    dict_construct(&_active_instance.windows.lookup, UI_INSTANCE_WINDOW_MAX, 0);

    // Initialize the element system
    {

        // TODO
        //
    }

    // Set the initialized flag
    initialized = true;

    // Set the running flag
    _active_instance.running = true;

    // Done
    return;

    construct_config_file:

        // Write an error message to standard out
        log_warning("[ui] Failed to find config file\n");

        // Reopen the file handle in write mode
        p_config_file = fopen(_config_file_path, "w+");

        // Write the config to the file
        fwrite(_default_config, sizeof(char), sizeof(_default_config), p_config_file);

        // Reopen the file handle in read mode
        p_config_file = freopen(0, "r", p_config_file);

        // Write an error message to standard out
        log_warning("[ui] Generated new config file at \"%s\"\n", _config_file_path);

        // Done
        goto config_file_present;

    // TODO:
    wrong_name_type:
    name_too_long:
    failed_primary:
    wrong_primary_type:
    failed_accent_1:
    wrong_accent_1_type:
    failed_accent_2:
    wrong_accent_2_type:
    failed_accent_3:
    wrong_accent_3_type:
    failed_background:
    wrong_background_type:
    failed_to_init_sdl2:
        
        // Abort
        exit(EXIT_FAILURE);

    // Error handling
    {

        // Environment errors
        {
            failed_to_find_home:
            
                // Write an error message to standard out
                log_error("[ui] Failed to find home directory in call to function \"%s\"\n", __FUNCTION__);

                // Abort execution
                exit(EXIT_FAILURE);
        }

        // json errors
        {
            wrong_type:

                // Write an error message to standard out
                log_error("[ui] Config file must be of type [ object ] in call to function \"%s\"\n", _config_file_path, __FUNCTION__);

                // Abort execution
                exit(EXIT_FAILURE);

            missing_properties:

                // Write an error message to standard out
                log_error("[ui] Config file is missing required properties in call to function \"%s\"\n", _config_file_path, __FUNCTION__);

                // Abort execution
                exit(EXIT_FAILURE);
        }

        // ui errors
        {
            failed_to_parse_json:

                // Write an error message to standard out
                log_error("[ui] Failed to parse config file \"%s\" in call to function \"%s\"\n", _config_file_path, __FUNCTION__);

                // Abort execution
                exit(EXIT_FAILURE);

            config_file_too_long:

                // Write an error message to standard out
                log_error("[ui] Config file must be less than %d characters in call to function \"%s\"\n", UI_CONFIG_FILE_LENGTH_MAX, __FUNCTION__);

                // Abort execution
                exit(EXIT_FAILURE);
        }
    }
}

int ui_window_run ( ui_window *p_window )
{

    // Argument check
    if ( p_window == (void *) 0 ) goto no_window;

    // Main loop
    while ( p_window->context.is_open )
    {

        // Wait 
        monitor_wait(&p_window->_monitor);

        // Repaint
        window_draw(p_window);
    }

    // Success
    return 1;

    // Error handling
    {
        no_window:
            #ifndef NDEBUG
                log_error("[ui] Null pointer provided for parameter \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int ui_window_add ( ui_window **pp_window, const char *const path, fn_window_constructor pfn_window_constructor )
{

    // Initialized data
    ui_window *p_window = (void *) 0;

    // Load the ui window
    if ( window_load(&p_window, path) == 0 ) goto failed_to_load_window;

    // Call the user's setup
    if ( pfn_window_constructor(p_window) == 0 ) goto failed_to_set_up_window;

    // Set the open flag
    p_window->context.is_open = true;

    // Store the window
    _active_instance.windows._list[_active_instance.windows.count] = p_window;

    // Register the window
    dict_add(_active_instance.windows.lookup, p_window->attributes._name, p_window);

    // Update the active window
    _active_instance.windows.active = p_window;

    // Return a pointer to the caller
    *pp_window = p_window;

    // Start a new thread for this window
    parallel_thread_start(&p_window->p_thread, ui_window_run, p_window);

    // Success
    return 1;

    // Error handling
    {

        // ui error
        {
            failed_to_load_window:
                #ifndef NDEBUG
                    log_error("[ui] Failed to load window in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // User error
        {
            failed_to_set_up_window:
                #ifndef NDEBUG
                    log_error("[ui] User provided setup function returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int ui_draw_format_text ( ui_window *p_window, int x, int y, int size, const char *const format, ... )
{

    // Argument check
    if ( p_window == (void *) 0 ) goto no_window;
    if ( format   == (void *) 0 ) goto no_format; 

    // We use the varadic argument list in vprintf
    va_list aList;
    va_start(aList, format);

    // TODO: Dynamically allocate size
    char *buffer = calloc(2048, sizeof(u8));

    // TODO: Check memory

    vsprintf(buffer, format, aList);

    ui_draw_text(buffer, p_window, x, y, size);

    free(buffer);

    va_end(aList);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_window:
                #ifndef NDEBUG
                    printf("[UI] Null pointer provided for \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_format:
                #ifndef NDEBUG
                    printf("[UI] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void ui_draw_char(char c, ui_window *p_window, int x, int y, int size)
{

    // Argument check
    if ( p_window == (void *) 0 ) goto no_window;

    // Initialized data
    u64 glyph = font[(u8)c];

    // Iterate over the width and the height of the glyph
    for (s32 h = (x + (8 * size)); h > x; h -= size)
        for (s32 w = y; w < (y + (8 * size)); w += size)
        {

            // Draw a pixel in the glyph
            if (glyph & 1)

                // Of a specified size
                for (s32 i = 0; i < size; i++)
                    for (s32 j = 0; j < size; j++)

                        // Draw the pixel
                        SDL_RenderDrawPoint(p_window->sdl2.renderer, w + i, h + j);

            // Draw the next part of the glyph
            glyph >>= 1;
        }

    // Return
    return;

    // Error handling
    {
        
        // Argument errors
        {
            no_window:
                #ifndef NDEBUG
                    printf("[UI] Null pointer provided for \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return;
        }
    }
}

int ui_draw_text(const char *const text, ui_window *p_window, int x, int y, int size)
{

    // Argument check
    if ( p_window == (void *) 0 ) goto no_window;

    size_t len = strlen(text);

    int lx = x,
        ly = y;

    for (size_t i = 0; i < len; i++)
        ui_draw_char(text[i], p_window, ly, lx + (i * (8 * size)), size);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_window:
                #ifndef NDEBUG
                    printf("[UI] Null pointer provided for \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int ui_draw_circle(ui_window *p_window, int radius, int x_center, int y_center)
{

    // Argument check
    if ( p_window == (void *) 0 ) goto no_window;
    
    // Initialized data
    i32 diameter = (radius * 2),
        x        = (radius - 1),
        y        = 0,
        tx       = 1,
        ty       = 1,
        error    = (tx - diameter);

    // Draw the circle
    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center + x, y_center - y);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center + x, y_center + y);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center - x, y_center - y);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center - x, y_center + y);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center + y, y_center - x);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center + y, y_center + x);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center - y, y_center - x);
        SDL_RenderDrawPoint(p_window->sdl2.renderer, x_center - y, y_center + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_window:
                #ifndef NDEBUG
                    printf("[UI] Null pointer provided for \"p_window\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

ui_instance *ui_get_active_instance ( void )
{

    // Success
    return &_active_instance;
}

void ui_info ( ui_instance *p_instance )
{

    // Print the name of the UI instance
    log_info(" - UI info - \n");

    // Print the color theme
    log_info("    Theme: \"%s\"\n", p_instance->_name);

    // Print the UI theme
    log_info(
        "         - primary   : #%02x%02x%02x\n",
        p_instance->theme.primary.r,
        p_instance->theme.primary.g,
        p_instance->theme.primary.b
    );
    log_info(
        "         - accent 1  : #%02x%02x%02x\n",
        p_instance->theme.accent_1.r,
        p_instance->theme.accent_1.g,
        p_instance->theme.accent_1.b
    );
    log_info(
        "         - accent 2  : #%02x%02x%02x\n",
        p_instance->theme.accent_2.r,
        p_instance->theme.accent_2.g,
        p_instance->theme.accent_2.b
    );
    log_info(
        "         - accent 3  : #%02x%02x%02x\n",
        p_instance->theme.accent_3.r,
        p_instance->theme.accent_3.g,
        p_instance->theme.accent_3.b
    );
    log_info(
        "         - background: #%02x%02x%02x\n",
        p_instance->theme.background.r,
        p_instance->theme.background.g,
        p_instance->theme.background.b
    );

    // Print window information
    log_info("    Windows [%d]:\n", 2);

    // Done
    return;
}

void ui_exit ( )
{

    // Print a message to standard out
    log_info("[ui] Shutting down\n");

    // Done
    return;
}

size_t ui_load_file ( const char *path, void *buffer, bool binary )
{
    
    // Argument checking
    if ( path == 0 ) goto no_path;

    // Initialized data
    size_t ret = 0;
    FILE *f = fopen(path, (binary) ? "rb" : "r");

    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // We no longer need the file
    fclose(f);

    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[Standard library] Null path provided to funciton \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

                // Error
                return 0;
        }
    }
}
