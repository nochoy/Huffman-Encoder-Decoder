
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

#include "code.h"
#include "io.h"

uint64_t bytes_read = 0;                  // track total bytes that are read
uint64_t bytes_written = 0;               // tracks total bytes that are written
static uint8_t read_buffer[BLOCK] = {0};  // buffer for bytes being read
static uint8_t write_buffer[BLOCK] = {0}; // bytes for bytes being written
static uint32_t buff_index = 0;           // tracks index in the buffer

// takes in infile descriptor, buffer, number of bytes
// read nbytes from infile into buf
// return the number of bytes read
int read_bytes(int infile, uint8_t *buf, int nbytes) {
  uint32_t bytes_read_here = 0;
  uint64_t bytes_counter = 0;
  if (nbytes == 0) {
    return 0;
  }
  while ((bytes_counter = read(infile, buf + bytes_read_here,
                               nbytes - bytes_read_here)) > 0) {
    bytes_read_here += bytes_counter;
    if (bytes_read == (uint64_t)nbytes) {
      break;
    }
  }
  bytes_read += bytes_read_here;
  return bytes_read_here;
}

// takes in outfile descriptor, buffer, number of bytes
// write nbytes from the buffer into outfile
// returns the number of bytes written
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  uint32_t bytes_written_here = 0;
  uint32_t bytes_counter = 0;
  if (nbytes == 0) {
    return 0;
  }
  while ((bytes_counter = write(outfile, buf + bytes_written_here,
                                nbytes - bytes_written_here)) > 0) {
    bytes_written_here += bytes_counter;
    if (bytes_written_here == (uint32_t)nbytes ||
        bytes_written_here == bytes_read) {
      break;
    }
  }
  bytes_written += bytes_written_here;
  return bytes_written_here;
}

// takes in infile descriptor, bit
// read bit from infile and return value through bit
// returns boolean if there are still bits to read
bool read_bit(int infile, uint8_t *bit) {
  static uint32_t index = 0;
  static uint32_t buff_size;
  if (index == 0) {
    buff_size = read_bytes(infile, read_buffer, BLOCK);
  }
  if (buff_size == 0) {
    return false;
  }
  *bit = (read_buffer[index / 8] >> (index % 8)) & 1;
  index += 1;
  if (index == buff_size * 8) {
    index = 0;
  }
  return true;
}

// takes in outfile descriptor, code c
// write bits from c into a static buffer and write out contents of buffer into
// outfile
void write_code(int outfile, Code *c) {
  static uint32_t buff_size = 0;
  for (uint32_t i = 0; i < code_size(c); i += 1) {
    uint8_t bit = code_get_bit(c, i);
    if (bit == 1) {
      write_buffer[buff_index / 8] |= (1 << buff_index % 8);
    } else {
      write_buffer[buff_index / 8] &= ~(1 << buff_index % 8);
    }
    buff_index += 1;
    if (buff_index == BLOCK * 8) {
      write_bytes(outfile, write_buffer, BLOCK);
      buff_size += BLOCK;
      buff_index = 0;
    }
  }
}

// takes in outfile descriptor
// write out any extra bits in buffer
void flush_codes(int outfile) {
  static uint32_t buff_size = 0;
  if (buff_index % 8 == 0) {
    write_bytes(outfile, write_buffer, buff_index / 8);
    buff_size += buff_index / 8;
  } else {
    write_buffer[buff_index / 8] &= ((1 << (buff_index % 8)) - 1);
    write_bytes(outfile, write_buffer, buff_index / 8 + 1);
  }
  buff_index = 0;
}
