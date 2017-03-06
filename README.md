# prisma

A small C library for playing with [ANSI escape sequences](http://en.wikipedia.org/wiki/ANSI_escape_code).  
It comes with a man page.  

prisma is released under the [CC0 license](http://creativecommons.org/about/cc0).

Please note that this is an unfinished project which goal was for me to learn how to create a library and document it.  
Its code is old and not so well-thought.

![Demo Screenshot](http://yoanlecoq.com/dl/dev/prisma/pr_rainbow.png)
![Man page](http://yoanlecoq.com/dl/dev/prisma/pr_man.png)


## About

prisma is a small C library, which allows easy Select Graphics Rendition 
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
* Small chat widgets;
* In-game command-lines;
* Informative loading screens;
* etc.


## Installing

You will need `make`, the GNU Compiler Collection and the GNU binutils.
Windows users will need something like TDM-GCC.

In your command line, you should enter :
```bash
$ make OS=<Your-OS-here> ARCH=<32-or-64-bits>
```
For linux users, this is also available :
```bash
$ sudo make install
```
This way you can install the library and get the man pages.

## Testing

If prisma was installed :
```bash
gcc examples/rainbow_demo.c -o rainbow_demo -lprisma
```
If prisma was not installed :
```bash
gcc examples/rainbow_demo.c -o rainbow_demo -Iinclude -Llib/<OS>/ -lprisma
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
