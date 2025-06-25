// src/wifi.c
// Low-level Wi-Fi and Ethernet interface skeleton for unxus kernel

#include "wifi.h"
#include <stddef.h>
#include <stdint.h>

// Data structure for a network device
typedef struct net_device {
    const char *name;
    int (*init)(void);
    int (*send)(const uint8_t *data, size_t len);
    int (*recv)(uint8_t *buf, size_t maxlen);
    int (*up)(void);
    int (*down)(void);
} net_device_t;

// Example stubs for Ethernet device
static int eth_init(void)   { /* hardware init here */ return 0; }
static int eth_send(const uint8_t *data, size_t len) { /* hardware send */ return (int)len; }
static int eth_recv(uint8_t *buf, size_t maxlen) { /* hardware recv */ return 0; }
static int eth_up(void)     { /* bring interface up */ return 0; }
static int eth_down(void)   { /* bring interface down */ return 0; }

// Example stubs for Wi-Fi device
static int wifi_init(void)  { /* hardware init here */ return 0; }
static int wifi_send(const uint8_t *data, size_t len) { /* hardware send */ return (int)len; }
static int wifi_recv(uint8_t *buf, size_t maxlen) { /* hardware recv */ return 0; }
static int wifi_up(void)    { /* bring interface up */ return 0; }
static int wifi_down(void)  { /* bring interface down */ return 0; }

// Network device registry
static net_device_t eth_device = {
    .name = "eth0",
    .init = eth_init,
    .send = eth_send,
    .recv = eth_recv,
    .up   = eth_up,
    .down = eth_down,
};

static net_device_t wifi_device = {
    .name = "wifi0",
    .init = wifi_init,
    .send = wifi_send,
    .recv = wifi_recv,
    .up   = wifi_up,
    .down = wifi_down,
};

// Register devices with the kernel network stack
void net_devices_init(void) {
    eth_device.init();
    wifi_device.init();
    // Add to global device list if needed
}

// Example API for higher-level code
int net_send(const char *dev_name, const uint8_t *data, size_t len) {
    if (dev_name == eth_device.name) return eth_device.send(data, len);
    if (dev_name == wifi_device.name) return wifi_device.send(data, len);
    return -1;
}

int net_recv(const char *dev_name, uint8_t *buf, size_t maxlen) {
    if (dev_name == eth_device.name) return eth_device.recv(buf, maxlen);
    if (dev_name == wifi_device.name) return wifi_device.recv(buf, maxlen);
    return -1;
}
