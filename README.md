# Huffman Encoder Decoder

NOTE: This program was modified from a Computer Systems and C Programming course assignment. All header files were provided by Professor Darrel Long at UC Santa Cruz. 
<br><br>
This program implements Huffman Coding to compress and decompress files. The encoder and decoder read from `stdin` and `stdout` by default, but can be read from input files and output to specified files using command line arguments. 

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