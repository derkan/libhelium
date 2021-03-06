/*
 * Copyright (C) 2014 Helium Systems Inc.
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/**
   @file helium.h
   @authors the Helium team
   @copyright Helium Systems, 2014
*/

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdint.h>
#include <uv.h>

#ifndef HELIUM_API_H
#define HELIUM_API_H

#ifdef _WIN32
#  ifdef helium_EXPORTS
#    define MODULE_API __declspec(dllexport)
#  else
#    define MODULE_API __declspec(dllimport)
#  endif
#else
#  define MODULE_API
#endif

const char *libhelium_version();

/**
  @brief Turns on libhelium logging.
*/
MODULE_API void helium_logging_start(); /* debug */

/**
   @brief The type of Helium security tokens.

   Tokens are 16 byte AES-128 keys, in binary form.

   Please note that tokens are not null-terminated and may contain NUL characters.
*/
typedef unsigned char helium_token_t[16];

/**
  @brief An opaque type encapsulating a UDP connection over the Helium network. */
typedef struct helium_connection_s helium_connection_t;

/**
   @brief A Helium callback function invoked when connections receive data.
   @param conn The connection that received this message.
   @param sender_mac The MAC address of the device that sent this message.
   @param message A NULL-terminated message from the device.
   @param size The length of the message.
*/
typedef void (*helium_callback_t)(const helium_connection_t *conn, uint64_t sender_mac, char * const message, size_t count);

#if HAVE_BLOCKS
/**
  @brief Identical to helium_callback_t, but as a block rather than a function pointer.
*/
typedef void (^helium_block_t)(const helium_connection_t *conn, uint64_t sender_mac, char * const message, size_t count);
#endif


/**
   @brief Allocates a new libhelium connection.

   The result of this function must be passed to `helium_free`.
*/
MODULE_API helium_connection_t *helium_alloc(void);

/**
   @brief Frees a previously-allocation libhelium connection.

   @param conn The connection to free.

   If `conn` is `NULL`, this function has no effect.
*/
MODULE_API void helium_free(helium_connection_t *conn);


/**
    @brief Gets the `uv_loop_t` onto which Helium connections are added by default.
*/
uv_loop_t *helium_default_loop(void);

/**
   @brief Open a helium connection, receiving data with the provided callback.

   A `helium_open` operation must be balanced by a call to `helium_close()`.

   @param conn The connection to open.
   @param proxy_addr An optional IPv4 proxy to use. If `NULL`, IPv6 is used.
   @param callback A function pointer that will be invoked when this connection receives data.
*/
MODULE_API int helium_open(helium_connection_t *conn, const char *proxy_addr, helium_callback_t callback);

/**
   @brief Closes a helium connection.
   @param conn The connection to close.
   @return 0 on success, `UV_EALREADY` if the connection is already open.
*/
MODULE_API int helium_close(helium_connection_t *conn);

#if HAVE_BLOCKS
/**
   @brief Open a helium connection, receiving data with the provided callback.

   Identical to @helium_open()@, but takes a block rather than a function pointer.
*/
int helium_open_b(helium_connection_t *conn, char *proxy_addr, helium_block_t callback);
#endif

/**
   @brief Subscribe to messages from the specified device.
   @param conn The connection which will receive messages
   @param macaddr The MAC address of the device to subscribe to
   @param token The security token of this device
   @return 0 on success.
*/
MODULE_API int helium_subscribe(helium_connection_t *conn, uint64_t macaddr, helium_token_t token);

/**
   @brief Unsubscribe from messages from the specified device.
   @param conn The connection which will receive messages
   @param macaddr The MAC address of the device to subscribe to
   @return 0 on success.
*/
MODULE_API int helium_unsubscribe(helium_connection_t *conn, uint64_t macaddr);


/**
   @brief Send a given device a message.
   @param conn The connection that will send this message
   @param macaddr The MAC address of the destination device
   @param token A valid Helium security token
   @param message The message to send
   @param count The number of characters to send.
   @return 0 on success.
*/
MODULE_API int helium_send(helium_connection_t *conn, uint64_t macaddr, helium_token_t token, unsigned char *message, size_t count);

/**
   @brief Associate an arbitrary datum with this connection.
   @param conn The target connection
   @param context An arbitrary pointer
*/
MODULE_API void helium_set_context(helium_connection_t *conn, void *context);

/**
   @brief Retrieve the datum associated with the provided connection.
   @param conn The connection to query
   @return The associated value, or NULL if it never set
*/
MODULE_API void *helium_get_context(const helium_connection_t *conn);

/**
   @brief Decodes a Helium token from the provided base64 string.
   @param input The string to decode
   @param length The length of `input`
   @param token The token into which to decode
*/
MODULE_API int helium_base64_token_decode(const unsigned char *input, int length, helium_token_t token);


#endif /* HELIUM_API_H */
