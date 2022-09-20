
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#define OPTIONS "hvi:o:"

// file descriptors for infile and outfile
static int fd_in = STDIN_FILENO;
static int fd_out = STDOUT_FILENO;

// prints help statement
void print_help() {
  printf("SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the "
         "Huffman coding "
         "algorithm.\n\n");
  printf("USAGE\n  ./encode [-h] [-i infile] [-o outfile]\n\n");
  printf("OPTIONS\n");
  printf("  -h             Program usage and help.\n");
  printf("  -v             Print compression statistics\n");
  printf("  -i infile      Input file to compress.\n");
  printf("  -o outfile     Output of compressed data.\n");
}

// constructs the histogram to store the frequencies of each unique symbol
uint32_t create_histogram(int infile, uint8_t *read_buffer,
                          uint64_t histogram[static ALPHABET], int bytes) {
  uint32_t unique_symbols = 0;
  while ((bytes = read_bytes(infile, read_buffer, BLOCK)) > 0) {
    for (int i = 0; i < bytes; i += 1) {
      if (histogram[read_buffer[i]] == 0) {
        unique_symbols += 1;
      }
      histogram[read_buffer[i]] += 1;
    }
  }
  return unique_symbols;
}

// main function to encode infile and write to outfile
int main(int argc, char **argv) {
  static uint32_t bytes = 0;
  char *infile;
  char *outfile;
  int temp_file = 0;
  bool v_case = false;
  bool i_case = false;
  bool o_case = false;
  int32_t opt = 0;
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      print_help();
      return 1;
      break;
    case 'v':
      v_case = true;
      break;
    case 'i':
      infile = optarg;
      i_case = true;
      break;
    case 'o':
      outfile = optarg;
      o_case = true;
      break;
    default:
      print_help();
      return 1;
      break;
    }
  }

  // open infile
  if (i_case) {
    fd_in = open(infile, O_RDONLY);
    if (fd_in == -1) {
      fprintf(stderr, "Error: failed to open infile\n");
      return 1;
    }
  } else { // reading from stdin
    uint8_t buffer[BLOCK] = {0};
    temp_file = open("/tmp/temp_file", O_CREAT | O_RDWR | O_TRUNC, 0600);
    while ((bytes = read_bytes(fd_in, buffer, BLOCK)) > 0) {
      write_bytes(temp_file, buffer, BLOCK);
    }
    fd_in = temp_file;
  }

  // go back to beginning of infile
  lseek(fd_in, 0, SEEK_SET);

  // open outfile
  if (o_case) {
    fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd_out == -1) {
      fprintf(stderr, "Error: failed to open outfile\n");
      return 1;
    }
  }

  // create histogram
  uint64_t hist[ALPHABET] = {0};
  uint8_t read_buffer[BLOCK] = {0};
  uint32_t unique_symbols = 2;
  bytes = 0;
  hist[0] += 1;
  hist[255] += 1;
  unique_symbols += create_histogram(fd_in, read_buffer, hist, bytes);

  // get stats for infile and set permissions of outfile to the same as infile
  struct stat infile_stats;
  fstat(fd_in, &infile_stats);
  fchmod(fd_out, infile_stats.st_mode);

  // construct Huffman Tree
  Node *huff_tree = build_tree(hist);

  // build code table
  Code code_table[ALPHABET] = {0};
  build_codes(huff_tree, code_table);

  // create header
  Header header;
  header.magic = MAGIC;
  header.permissions = infile_stats.st_mode;
  header.tree_size = (unique_symbols * 3) - 1;
  header.file_size = infile_stats.st_size;
  write_bytes(fd_out, (uint8_t *)&header, sizeof(header));

  // dump tree to outfile
  dump_tree(fd_out, huff_tree);

  // go back to beginning of infile
  lseek(fd_in, 0, SEEK_SET);

  // write codes to outfile
  while ((bytes = read_bytes(fd_in, read_buffer, BLOCK)) > 0) {
    for (uint32_t i = 0; i < bytes; i += 1) {
      write_code(fd_out, &code_table[read_buffer[i]]);
    }
  }
  flush_codes(fd_out);

  // print statistics
  if (v_case) {
    fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n",
            infile_stats.st_size);
    fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2f%%\n",
            (1 - ((double)bytes_written / infile_stats.st_size)) * 100);
  }

  // cleanup time
  if (i_case) {
    close(fd_in);
  }
  if (o_case) {
    close(fd_out);
  }
  if (temp_file) {
    unlink("tmp/temp_file");
  }
  delete_tree(&huff_tree);

  return 0;
}
