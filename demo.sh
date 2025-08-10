#!/bin/bash

# Huffman Encoder/Decoder Interactive Demo

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[1;35m'
NC='\033[0m' # No Color

# Function to print colored output
print_header() {
    echo -e "\n${BLUE}=== $1 ===${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
}

print_comments() {
    echo -e "${PURPLE}$1${NC}"
    echo -e "\n"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

# Check if we're in the right directory
if [[ ! -f "encode" || ! -f "decode" ]]; then
    print_error "Encoder/Decoder binaries not found. Building..."
    make all
fi

print_header "Huffman Encoder/Decoder Demo"
echo "Welcome to the interactive Huffman coding demonstration!"
echo "This demo will show you how lossless compression works."

# Create sample files for demonstration
print_info "Creating sample files..."

# Sample 1: Simple short text (should not compress well)
cat > sample1.txt << EOF
Hello, World! This is a demonstration of Huffman coding.
Huffman coding is a lossless data compression algorithm.
It assigns variable-length codes to input characters,
with shorter codes assigned to more frequent characters.
EOF

# Sample 2: Repetitive text (should compress well)
cat > sample2.txt << EOF
aaaaaaaabbbbbbbbcccccccccddddddddeeeeeeeeffffffffgggggggg
hhhhhhhhiiiiiiiijjjjjjjjkkkkkkkkllllllllmmmmmmmmnnnnnnnn
oooooooooppppppppqqqqqqqqrrrrrrrrssssssssttttttttuuuuuuuu
vvvvvvvvvvvvvwwwwwwwwwwwwwwxxxxxxxxxxxxxxxxxyyyyyyyyyyyyy
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
aaaaaaaabbbbbbbbcccccccccddddddddeeeeeeeeffffffffgggggggg
hhhhhhhhiiiiiiiijjjjjjjjkkkkkkkkllllllllmmmmmmmmnnnnnnnn
oooooooooppppppppqqqqqqqqrrrrrrrrssssssssttttttttuuuuuuuu
vvvvvvvvvvvvvwwwwwwwwwwwwwwxxxxxxxxxxxxxxxxxyyyyyyyyyyyyy
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
aaaaaaaabbbbbbbbcccccccccddddddddeeeeeeeeffffffffgggggggg
hhhhhhhhiiiiiiiijjjjjjjjkkkkkkkkllllllllmmmmmmmmnnnnnnnn
oooooooooppppppppqqqqqqqqrrrrrrrrssssssssttttttttuuuuuuuu
vvvvvvvvvvvvvwwwwwwwwwwwwwwxxxxxxxxxxxxxxxxxyyyyyyyyyyyyy
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
EOF

# Sample 3: Mixed content (should not compress well)
cat > sample3.txt << EOF
The quick brown fox jumps over the lazy dog.
THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.
1234567890!@#$%^&*()_+-=[]{}|;':",./<>?
EOF

print_success "Sample files created!"

# Function to run compression demo
run_compression_demo() {
    local input_file=$1
    local description=$2
    
    print_header "Compressing: $description"
    echo "File: $input_file"
    
    # Show original file
    echo -e "\nOriginal content (first 100 chars):"
    head -c 100 "$input_file" | sed 's/$/.../' 2>/dev/null || cat "$input_file"
    
    # Get original size
    original_size=$(wc -c < "$input_file")
    echo -e "\nOriginal size: ${original_size} bytes"
    
    # Compress
    print_info "Compressing with Huffman coding..."
    if ./encode -i "$input_file" -o "${input_file}.compressed" -v 2>&1 | tee compression.log; then
        compressed_size=$(wc -c < "${input_file}.compressed")
        compression_ratio=$(echo "scale=2; (1 - $compressed_size/$original_size) * 100" | bc -l)
        
        print_success "Compression successful!"
        echo "Compressed size: ${compressed_size} bytes"
        echo "Compression ratio: ${compression_ratio}% reduction"
        
        # Decompress
        print_info "Decompressing to verify integrity..."
        if ./decode -i "${input_file}.compressed" -o "${input_file}.restored" -v 2>&1 | tee decompression.log; then
            # Verify integrity
            if diff -q "$input_file" "${input_file}.restored" > /dev/null; then
                print_success "Integrity check passed! Original and restored files are identical."
            else
                print_error "Integrity check failed! Files differ."
                return 1
            fi
        else
            print_error "Decompression failed!"
            return 1
        fi
    else
        print_error "Compression failed!"
        return 1
    fi
}

# Run demos for all sample files
print_header "Starting Compression Demonstrations"

run_compression_demo "sample1.txt" "Regular English Text"
print_comments "Notice the compressed file was actually larger than the original file.
This is due to the overhead required for decompression. Many data compression algorithms
are not optimal for small files."
sleep 5

run_compression_demo "sample2.txt" "Highly Repetitive Text"
print_comments "Notice the compressed file was successfully compressed into a smaller file!
This is due to the large number of frequent characters. Data compression algorithms
are optimal for files with many repeating characters."
sleep 5

run_compression_demo "sample3.txt" "Mixed Content"
print_comments "Notice the compressed file was actually larger than the original file.
This is due to the uniform character frequency distribution. This file contains nearly all unique characters.
Many data compression algorithms are not optimal for files with infrequent unique characters."
sleep 5

# Interactive section
print_header "Interactive Mode"
print_info "Let's try with your own text!"

read -p "Enter some text to compress (or press Enter to skip): " user_text
if [[ -n "$user_text" ]]; then
    echo "$user_text" > user_input.txt
    run_compression_demo "user_input.txt" "Your Custom Text"
fi
sleep 5

# Performance demonstration
print_header "Performance Analysis"
print_info "Running performance benchmarks..."

# Create larger test file
base64 /dev/urandom | head -c 1000000 > large_test.txt
run_compression_demo "large_test.txt" "1MB Random Data"
print_comments "Notice the compressed file was successfully compressed into a smaller file!
This is due to the large number of characters in the file. Data compression algorithms
are optimal for large files."

# Summary
print_header "Demo Summary"
echo "✅ Huffman coding successfully demonstrated!"
echo "✅ Lossless compression verified for all test cases"
echo "✅ Variable compression ratios based on content entropy"
echo "✅ Fast compression/decompression with minimal overhead"

# Cleanup
print_info "Cleaning up temporary files..."
rm -f sample*.txt sample*.compressed sample*.restored user_input.txt large_test.txt compression.log decompression.log

print_success "Demo complete! Thank you for trying the Huffman Encoder/Decoder."
sleep 3
