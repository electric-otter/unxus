// src/https.h
// Low-level HTTPS interface for unxus kernel

#ifndef UNXUS_HTTPS_H
#define UNXUS_HTTPS_H

#include <stddef.h>
#include <stdint.h>

// HTTPS connection handle structure (opaque to user)
typedef struct https_conn https_conn_t;

// Initializes the HTTPS subsystem (load keys, etc.)
int https_init(void);

// Opens a new HTTPS connection to host:port
https_conn_t *https_connect(const char *host, uint16_t port);

// Sends data over an HTTPS connection
ssize_t https_send(https_conn_t *conn, const uint8_t *data, size_t len);

// Receives data from an HTTPS connection
ssize_t https_recv(https_conn_t *conn, uint8_t *buf, size_t maxlen);

// Closes an HTTPS connection and frees resources
void https_close(https_conn_t *conn);

#endif // UNXUS_HTTPS_H
