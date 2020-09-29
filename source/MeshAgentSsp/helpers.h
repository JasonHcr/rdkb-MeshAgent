/*
 * Copyright 2020 Comcast Cable Communications Management, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <msgpack.h>

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */
#define match(p, s) strncmp((p)->key.via.str.ptr, s, (p)->key.via.str.size)
#define member_size(type, member) sizeof(((type *)0)->member)

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
typedef struct {
    bool         mesh_enable;
    bool         ethernetbackhaul_enable;
    char         * subdoc_name;
    uint32_t     version;
    uint16_t     transaction_id;
} meshbackhauldoc_t;

enum {
    HELPERS_OK = 0,
    HELPERS_OUT_OF_MEMORY,
    HELPERS_INVALID_FIRST_ELEMENT,
    HELPERS_MISSING_WRAPPER
};

typedef int (*process_fn_t)(void *, int, ...);
typedef void (*destroy_fn_t)(void *);

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

/**
 *  Simple helper function that decodes the msgpack, then checks for a few
 *  sanity items (including an optional wrapper map) before calling the process
 *  argument passed in.  This also allocates the structure for the caller.
 *
 *  @param buf          the buffer to decode
 *  @param len          the length of the buffer in bytes
 *  @param struct_size  the size of the structure to allocate and pass to process
 *  @param wrapper      the optional wrapper to look for & enforce
 *  @param expect_type  the type of object expected
 *  @param optional     if the inner wrapper layer is optional
 *  @param process      the process function to call if successful
 *  @param destroy      the destroy function to call if there was an error
 *
 *  @returns the object after process has done it's magic to it on success, or
 *           NULL on error
 */
void* helper_convert( const void *buf, size_t len,
                      size_t struct_size, const char *wrapper,
                      msgpack_object_type expect_type, bool optional,
                      process_fn_t process,
                      destroy_fn_t destroy );

size_t b64_decode( const uint8_t *input,
                      const size_t input_size,
                      uint8_t *output );

/**
 *  This function converts a msgpack buffer into an meshbackhauldoc_t structure
 *  if possible.
 *
 *  @param buf the buffer to convert
 *  @param len the length of the buffer in bytes
 *
 *  @return NULL on error, success otherwise
 */
meshbackhauldoc_t* meshbackhauldoc_convert( const void *buf, size_t len );

/**
 *  This function destroys an meshbackhauldoc_t object.
 *
 *  @param e the meshbackhauldoc to destroy
 */
void meshbackhauldoc_destroy( meshbackhauldoc_t *d );

/**
 *  This function returns a general reason why the conversion failed.
 *
 *  @param errnum the errno value to inspect
 *
 *  @return the constant string (do not alter or free) describing the error
 */
const char* meshbackhauldoc_strerror( int errnum );

#endif
