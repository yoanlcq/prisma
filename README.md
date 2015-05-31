# prisma

A small C-library for playing with [ANSI escape sequences](http://en.wikipedia.org/wiki/ANSI_escape_code).

prisma is released under the [CC0 license](see http://creativecommons.org/about/cc0).

Anyone may freely build upon, enhance and reuse prisma for any 
purposes without restriction under copyright or database law.


## About

prisma is a small C/C++ library, which allows easy Select Graphics Rendition 
(SGR) ANSI escape sequences generation and interpretation, with no other dependency than the C standard library.
You may see it as a poor man's `ncurses`, except that it does not specifically target text interfaces.

In short, this is useful for storing style and colors as a text to 
reinterpret it back, and display it with your graphical application (using 
SDL, SFML, etc) or on your console.

[Currently supported SGR ANSI escape sequences](http://en.wikipedia.org/wiki/ANSI_escape_code#graphics)

prisma provides functions to adapt such escape sequences to some terminals,
and fake their behaviour when they are not supported.

prisma also provides some funny functions to manipulate colors, convert them,
and generate gradients.

Applications that would make a great use of prisma would be :
-> Small chat widgets;
-> In-game command-lines;
-> Informative loading screens;
-> etc.


## Installing

You will need `make`, the GNU Compiler Collection and the GNU binutils.
Windows users will need something like TDM-GCC.

First, edit the `Makefile`, to set the first three variables to values that suit
 your needs.

Example : I am currently under Linux 64 bits, and I want to build prisma for 
use with C++ programs. This will be my Makefile's head :

```Makefile
OS = linux64
LANG = CPP
CC = g++
```

In your command line, you should enter :
```bash
$ sudo make install
```
For Windows users :
```powershell
> <your_make_executable> install
```
But unlike in Unix systems, it doesn't actually install the library.
You need to copy the files under `include/` to the appropriate folder in your 
project.
Then, copy the generated `prisma.dll` you can retrieve under `lib/` and the 
appropriate subfolders to the appropriate folder in your project. 
Often, you should copy it to a `lib/` directory for compile-time, and to your executable's folder for run-time.


## Testing

Let's assume here you're using the C version of prisma.
If you're using C++, you should use g++ instead of gcc.

If prisma was installed :
```bash
gcc examples/rainbow_demo.c -o rainbow_demo -lprisma
```
If prisma was not installed :
```bash
gcc examples/rainbow_demo.c -o rainbow_demo -Iinclude -Llib/<LANGUAGE>/<OS>/ -lprisma
```
Windows users will need to make sure that `lib/<LANGUAGE>/<OS>/prisma.dll` is also in the executable's folder.

## Documentation

prisma's documentation is available in text format under `doc/txt`.

Linux/OSX users can access them with the usual 'man' command, like so :
If prisma was installed :
```bash
man prisma
```
If prisma was not installed :
```bash
$ man -M :doc prisma
```
