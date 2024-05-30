# Need CMake 3.20
cmake_minimum_required (VERSION 3.20)

# The name of the repository
project ("ui")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

IF(WIN32)
    find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
ELSEIF(LINUX)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
ELSEIF(APPLE)

ENDIF(WIN32)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Compiler warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wextra -Wno-declaration-after-statement -O3 -Wl,--no-as-needed")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Build sync with semaphore
add_compile_definitions(BUILD_SYNC_WITH_SEMAPHORE)

# Build sync with monitor
add_compile_definitions(BUILD_SYNC_WITH_MONITOR)

# Find the log module
if ( NOT "${HAS_LOG}")

    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the array module
if ( NOT "${HAS_ARRAY}")

    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_ARRAY true PARENT_SCOPE)
        set(ARRAY_INCLUDE_DIR ${ARRAY_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the hash cache module
if ( NOT "${HAS_HASH_CACHE}")

    # Add the hash cache directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/hash-cache ${CMAKE_SOURCE_DIR}/build/hash-cache)

    # Let any parent project know that this project has hash cache
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_HASH_CACHE true PARENT_SCOPE)
        set(HASH_CACHE_INCLUDE_DIR ${HASH_CACHE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")

    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_DICT true PARENT_SCOPE)
        set(DICT_INCLUDE_DIR ${DICT_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the json module
if ( NOT "${HAS_JSON}")

    # Add the json directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_JSON true PARENT_SCOPE)
        set(JSON_INCLUDE_DIR ${JSON_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the parallel module
if ( NOT "${HAS_PARALLEL}")

    # Add the parallel directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/parallel ${CMAKE_SOURCE_DIR}/build/parallel)

    # Let any parent project know that this project has parallel
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_PARALLEL true PARENT_SCOPE)
        set(PARALLEL_INCLUDE_DIR ${PARALLEL_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the tuple module
if ( NOT "${HAS_TUPLE}")

    # Add the tuple directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/tuple ${CMAKE_SOURCE_DIR}/build/tuple)

    # Let any parent project know that this project has tuple
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_TUPLE true PARENT_SCOPE)
        set(TUPLE_INCLUDE_DIR ${TUPLE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the ui project
if ( NOT "${HAS_UI}")

    # Log
    message("[ui] Adding ui project")

    set(UI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the ui project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_UI true PARENT_SCOPE)
        set(UI_INCLUDE_DIR ${UI_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add source to this project's executable.
add_executable (ui_example "main.c")
add_dependencies(ui_example ui parallel tuple json array dict hash_cache queue sync )
target_include_directories(ui_example PUBLIC include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${UI_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR}) 
target_link_libraries(ui_example PUBLIC ui parallel tuple tuple json array dict hash_cache queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# add_library (ui_canvas SHARED "canvas.c")       
# add_dependencies(ui_canvas ui parallel tuple json array dict hash_cache queue sync)
# target_include_directories(ui_canvas PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${UI_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR}) 
# target_link_libraries(ui_canvas PUBLIC ui parallel tuple json array dict hash_cache queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# Add source to this project's library
#add_library (ui SHARED "ui.c" "fileselector.c" "window.c" "button.c" "image.c" "checkbox.c" "dropdown.c" "element.c" "radiobutton.c" "label.c" "slider.c" "textinput.c" "table.c") #      
add_library (ui SHARED "ui.c" "window.c" "element.c" "label.c" "button.c") 
add_dependencies(ui parallel tuple json array dict hash_cache queue sync)
target_include_directories(ui PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${UI_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${PARALLEL_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${TUPLE_INCLUDE_DIR}) 
target_link_libraries(ui PUBLIC parallel tuple json array dict hash_cache queue sync  ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
