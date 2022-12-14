# Huffman Coder

NOTE: This program was modified from a Computer Systems and C Programming course assignment. All header files were provided by Professor Darrell Long at UC Santa Cruz. 
<br><br>
This program implements Huffman Coding to compress and decompress files. The encoder and decoder read from `stdin` and `stdout` by default, but can read and write from input and output files using command line arguments. 

## How It Works
The Huffman encoder reads in an input file and compresses the contents. It marks down the frequency of each unique symbol and constructs a Huffman tree based on the frequencies. The Huffman tree is represented using a priority queue (implemented by a min heap) that stores nodes each containing a symbol and its frequency. Nodes with lower frequencies have a higher priority. Codes are then assigned to each unique symbol, using less bits to represent higher frequency symbols and a greater number of bits to represent lower frequency symbols. The encoder outputs the Huffman Tree and the codes for each symbol. 

The decoder reads in a compressed input file and expands it to its original size. It uses a stack to reconstruct the Huffman tree. Using the codes to traverse the tree, the original symbols will be sent to the output. 

## How To Build
Build the program with:
```
$ make all
```
Clean binary and object files with:
```
$ make clean
```


## How To Run
### The program supports any combination of the following command-line options:

To read from an input file, run the encoder or decoder with a `-i` flag:
```
$ ./encode -i [file name]
$ ./decode -i [file name]
```

To print to an output file, run the encoder or decoder with a `-o` flag:

```
$ ./encode -i [file name] -o [file name]
$ ./decode -i [file name] -o [file name]
```
To print the usage and help page, run the encoder or decoder with a `-h` flag:
```
$ ./encode -h
$ ./decode -h
```
To print compression statistics, run the encoder or decoder with a `-v` flag:
```
$ ./encode -v
$ ./decode -v
```