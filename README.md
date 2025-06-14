# alpha_vm

**Description**: A virtual machine for running Alpha code.

## Authors

* Angelos T. Dimoglis csd5078
* Panagiwths Antonakakhs csd5137
* Aris Patramanis csd5249

## Testing

Testing was done at kerasi.

## How to compile

```
make all
```
or just
```
make
```
(all is the default)

## Command line options

* An input file must be given, the program will abort otherwise.

* -i \<input file\> // meaning a binary or text .abc file

## Note on the directory structure

* bin/
    * all .o files
* lib/
    * all .h files
* src/
    * execute_function
        * all execute function definitions
    * .cpp files
* Makefile
* README.md

