# un
File system generator. 

Project was written on Qt.
It generates file system with the following architechture:
 ___________________
|   Header          | Contains 5 values.  uint_32t each of them
|___________________|
| List of files     | Contains list of files headers insight of file system. 
| declaration       | It's following structure: 4 bytes of files offset by X, X is end of list, 
|__________________X| 4 bytes - size of file, 1 byte N (number) of symbols in name of file. N - array, name of file.
|                   |
|      Data         | This segment contains file's data.
|___________________|


Usage:
in main.c : add directory with your file system in your project, call the directory by 2 symbolic name, then compile
the project.
You'll get convinient file system with simple way to parse name of your files. 
Array N (name of file) doesn't contain name of directory with file system.

Field of implemention:
Use external flash memory in microprocessor with limitted storage.
