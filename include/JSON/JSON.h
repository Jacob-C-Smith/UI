#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <dict/dict.h>

#define JSON_VALUE( token, t ) (token) ? (token->type==t) ? token->value.n_where : 0 : 0;

// Enumerations

// Enumeration for the types of JSON objects
enum JSONValueType_e
{
    JSONobject    = 0, // Anything within a "{ ... }"
    JSONarray     = 1, // Any array
    JSONstring    = 2, // Any string
    JSONprimative = 3  // Any other value
};

// Unions and structs

// This union is used to differentiate pointers form double pointers, 
// or more simply, arrays from non arrays
union JSONContent_u {
    void  *n_where;       // Pointer to object
    void **a_where;       // Double pointer array of objects
};

// Container for the JSON key/value pair
struct JSONToken_s
{
    char                 *key;   // The key
    union JSONContent_u   value; // The token
    enum JSONValueType_e  type;  // The type.
};

// Type definitions
typedef union  JSONContent_u   JSONContent_t;
typedef enum   JSONValueType_e JSONValueType_t;
typedef struct JSONToken_s     JSONToken_t;

static const char *token_types[] = {
    "object", 
    "array",
    "string",
    "primative"
};

// Function definitions
#define DLLEXPORT extern __declspec(dllexport)

// Parse json into a dictionary
DLLEXPORT int parse_json ( char *token_text, size_t len, dict   **dictionary );

DLLEXPORT int encode_json ( FILE *buffer, size_t count, JSONToken_t* tokens );
