# 🚀 Huffman Encoder & Decoder

**High-performance lossless data compression using Huffman coding algorithm**  

## 🎬 Live Demo
![demo](https://github.com/user-attachments/assets/f518d44d-c7e7-4218-b8c6-bec9a8e834c5)

## 🎯 Features

- ✅ **Lossless Compression**: Perfect reconstruction of original data
- ✅ **High Performance**: Optimized C implementation with `O(n log n)` complexity
- ✅ **Cross-Platform**: Works on Linux and macOS
- ✅ **Memory Efficient**: Minimal RAM usage with streaming I/O
- ✅ **CLI Interface**: Unix-style command-line arguments
- ✅ **Verbose Mode**: Detailed compression statistics
- ✅ **Error Handling**: Comprehensive input validation

## 🏗️ Architecture

### Core Components

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Frequency     │    │   Huffman       │    │   Bit I/O       │
│   Analysis      │───▶│   Tree          │───▶│   Operations    │
│   (O(n))        │    │   Construction  │    │   (Streaming)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Data Structures

- **Hash Table**: Fast symbol frequency counting
- **Min-Heap Priority Queue**: Efficient node selection `(O(log n))`
- **Huffman Tree**: Optimal prefix code generation
- **Bit Buffer**: Memory-efficient bit-level I/O operations

## 🔧 Installation

### Prerequisites
- GCC compiler (≥4.8) or Clang
- Make build system
- Standard C library

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/nochoy/Huffman-Encoder-Decoder.git

# Open repository
cd Huffman-Encoder-Decoder

# Standard build
make

# Clean build
make clean && make
```

## 📖 Usage Guide

#### Encoding (Compression)
```bash
./encode [OPTIONS] -i INPUT -o OUTPUT

Options:
  -i, --input FILE    Input file to compress
  -o, --output FILE   Output compressed file
  -v, --verbose       Show compression statistics
  -h, --help          Display help message
```

#### Decoding (Decompression)
```bash
./decode [OPTIONS] -i INPUT -o OUTPUT

Options:
  -i, --input FILE    Input compressed file
  -o, --output FILE   Output decompressed file
  -v, --verbose       Show decompression statistics
  -h, --help          Display help message
```

### Examples

#### Basic Compression
```bash
# Compress a text file
./encode -i examples/sample.txt -o sample.huff

# Decompress back to original
./decode -i sample.huff -o sample-restored.txt

# Verify integrity
diff examples/sample.txt sample-restored.txt
# No output = files are identical
```

#### Verbose Mode with Statistics
```bash
./encode -i examples/lorem.txt -o lorem-encoded -v
# Output:
# Uncompressed file size: 635 bytes
# Compressed file size: 468 bytes
# Space saving: 26.3%
```

## 🧪 Testing & Demo

### Interactive Demo
Run the interactive demonstration script:
```bash
./demo.sh
```

This will:
1. Build the project
2. Create sample files
3. Demonstrate compression/decompression
4. Show compression statistics
5. Verify data integrity

### Manual Testing
```bash
# Test with different file types
./encode -i /etc/passwd -o passwd-encoded
./decode -i passwd-encoded -o passwd-decoded
diff /etc/passwd passwd-decoded
```

## 📁 Project Structure

```
Huffman-Encoder-Decoder/
├── encode.c              # Main encoding program
├── decode.c              # Main decoding program
├── huffman.c/.h          # Core Huffman algorithm
├── code.c/.h             # Bit vector Huffman codes
├── pq.c/.h               # Priority queue implementation
├── io.c/.h               # Bit-level I/O operations
├── node.c/.h             # Tree node structures
├── stack.c/.h            # Stack for tree traversal
├── examples/             # Sample files for testing
├── Makefile              # Build configuration
├── demo.sh               # Interactive demonstration
└── README.md             # This file
```

## 🧮 Algorithm Details

### Huffman Coding Process

1.  **Frequency Analysis**: Count the frequency of each character in the input file.
2.  **Priority Queue**: Build a min-heap priority queue of nodes, where each node represents a character and its frequency.
3.  **Tree Construction**: Build the Huffman tree by repeatedly extracting the two nodes with the lowest frequencies from the priority queue and joining them into a new parent node.
4.  **Code Generation**: Traverse the Huffman tree to generate a unique prefix code for each character.
5.  **Encoding**: Replace each character in the input file with its corresponding prefix code.
6.  **Decoding**: Reconstruct the original data by reading the prefix codes and traversing the Huffman tree.

### Complexity Analysis

Let **N** be the number of bytes in the input file and **k** be the number of unique symbols (at most 256).

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Encoding** |
| Frequency Count | O(N) | O(k) |
| Tree Construction | O(k log k) | O(k) |
| Write to File | O(N) | O(k) |
| **Overall Encoding** | **O(N + k log k)** | **O(k)** |
| **Decoding** |
| Tree Reconstruction| O(k) | O(k) |
| Read from File | O(N) | O(k) |
| **Overall Decoding** | **O(N + k)** | **O(k)** |

*Note: Since k is a constant (≤256), the complexities can be simplified to O(N) time and O(1) space.*

## 🐛 Troubleshooting

### Common Issues

**Build Errors**
```bash
# If make fails, try:
gcc -o encode encode.c huffman.c pq.c io.c node.c stack.c -lm
gcc -o decode decode.c huffman.c pq.c io.c node.c stack.c -lm
```

**Permission Errors**
```bash
# On Unix systems
chmod +x demo.sh
```


---
NOTE: This program was modified from a Computer Systems and C Programming course assignment. All header files were provided by Professor Darrell Long at UC Santa Cruz.
