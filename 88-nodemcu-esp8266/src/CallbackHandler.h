#ifndef CALLBACK_HANDLER_H
#define CALLBACK_HANDLER_H

#include <Arduino.h>

// Callback function declaration
void onDataReceive(uint8_t *mac, uint8_t *data, uint8_t len);

#endif // CALLBACK_HANDLER_H
