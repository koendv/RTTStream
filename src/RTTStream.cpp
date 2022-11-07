/*
 * Arduino stream wrapper for Segger RTT.
 * 2021 Koen De Vleeschauwer
 * No copyright asserted.
 */

#include <RTTStream.h>
#include <SEGGER_RTT.h>
#include "SEGGER_RTT_Peek.h"

RTTStream::RTTStream() {
  SEGGER_RTT_Init();
}

RTTStream::~RTTStream() {
}

void RTTStream::blockUpBufferFull() {
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
}

void RTTStream::skipUpBufferFull() {
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void RTTStream::trimUpBufferFull() {
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
}

void RTTStream::blockDownBufferFull() {
  SEGGER_RTT_ConfigDownBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
}

void RTTStream::skipDownBufferFull() {
  SEGGER_RTT_ConfigDownBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void RTTStream::trimDownBufferFull() {
  SEGGER_RTT_ConfigDownBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
}

void RTTStream::echo() {
  echo_on = true;
}

void RTTStream::noEcho() {
  echo_on = false;
}

int RTTStream::available() {
  int bytes_in_buffer = SEGGER_RTT_GetBytesInDownBuffer(0);
  return bytes_in_buffer;
}

int RTTStream::read() {
  int ch;
  int bytes_read = SEGGER_RTT_Read(0, &ch, 1);
  if (echo_on && bytes_read == 1)
    SEGGER_RTT_PutChar(0, ch);
  if (bytes_read == 1)
    return ch;
  return -1;
};

size_t RTTStream::readBytes(char *buffer, size_t length) {
  int bytes_read = SEGGER_RTT_Read(0, buffer, length);
  if (echo_on && bytes_read > 0)
    SEGGER_RTT_Write(0, buffer, bytes_read);
  return bytes_read;
};

int RTTStream::peek() {
  int peekch;
  peekch = SEGGER_RTT_Peek(0);
  return peekch;
}

size_t RTTStream::write(uint8_t ch) {
  int bytes_written = SEGGER_RTT_PutChar(0, ch);
  return bytes_written;
}

size_t RTTStream::write(const uint8_t *buffer, size_t size) {
  size_t bytes_written = SEGGER_RTT_Write(0, buffer, size);
  return bytes_written;
}

void RTTStream::flush() {
  while (SEGGER_RTT_GetBytesInBuffer(0) != 0) {
    delay(1);
  }
}

int RTTStream::printf(const char *format, ...) {
  int retval;
  va_list args;
  va_start(args, format);
  retval = SEGGER_RTT_vprintf(0, format, &args);
  va_end(args);
  return retval;
}

/* compatibility with Serial */

void RTTStream::begin(unsigned long baudrate) {
  (void)baudrate;
}

RTTStream::operator bool() {
  return true;
}

/* not truncated */