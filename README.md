# un
<nobr>
File system generator. </br>
</br>
Project was written on Qt.</br>
It generates file system with the following architechture:</br>
 ___________________ </br>
<pre>|     Header        | Contains 5 values.  uint_32t each of them</br>
|___________________|</br>
| List of files     | Contains list of files headers insight of file system. </br>
| declaration       | It's following structure: 4 bytes of files offset by X, X is end of list, </br>
|__________________X| 4 bytes - size of file, 1 byte N (number) of symbols in name of file. N - array, name of file.</br>
|                   |</br>
|      Data         | This segment contains file's data.</br>
|___________________|</br></pre>
</br>
</br>
Usage:</br>
in main.c : add directory with your file system in your project, call the directory by 2 symbolic name, then compile
the project.
You'll get convinient file system with simple way to parse name of your files. </br>
Array N (name of file) doesn't contain name of directory with file system.</br>

Field of implemention:</br>
Use external flash memory in microprocessor with limitted storage.</br>
</nobr>
