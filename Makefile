#OS can be set to win32, win64, macosx, linux32 or linux64.
#It should be set to the platform you're compiling on.
OS = win32
#LANG can be set to C or CPP.
LANG = CPP
#CC must be set to the appropriate compiler (gcc, or g++).
CC = g++


CCFLAGS = -Iinclude -O3
LDFLAGS = 
LDLIBS = -lm
OFILES = build/$(LANG)/$(OS)/prisma.o
CLEAN = rm -f $(OFILES)
CLEAN_WINDOWS = del /f /q build\$(LANG)\$(OS)\*

INSTALL =

COMMAND_WINDOWS = $(CC) $(CCFLAGS) -shared -o lib/$(LANG)/$(OS)/prisma.dll build/$(LANG)/$(OS)/prisma.o $(LDFLAGS) $(LDLIBS)

ifeq ($(OS), win32)
    CCFLAGS += -m32 -DBUILD_DLL
    COMMAND = $(COMMAND_WINDOWS)
    CLEAN = $(CLEAN_WINDOWS)
endif

ifeq ($(OS), win64)
    CCFLAGS += -m64 -DBUILD_DLL
    COMMAND = $(COMMAND_WINDOWS)
    CLEAN = $(CLEAN_WINDOWS)
endif

COMMAND_LINUX = $(CC) $(CCFLAGS) -shared build/$(LANG)/$(OS)/prisma.o -Wl,-soname,libprisma.so -o lib/$(LANG)/$(OS)/libprisma.so $(LDFLAGS) $(LDLIBS)
INSTALL_LINUX = cp ./lib/$(LANG)/$(OS)/libprisma.so /usr/local/lib/; ldconfig; cp ./include/* /usr/local/include; cp -pR doc/man? /usr/local/share/man/; mandb

ifeq ($(OS), macosx)
    CCFLAGS += -fPIC
    COMMAND = $(COMMAND_LINUX) -dynamiclib -flat_namespace
    INSTALL = $(INSTALL_LINUX)
endif

ifeq ($(OS), linux32)
    CCFLAGS += -m32 -fPIC
    COMMAND = $(COMMAND_LINUX)
    INSTALL = $(INSTALL_LINUX)
endif

ifeq ($(OS), linux64)
    CCFLAGS += -m64 -fPIC
    COMMAND = $(COMMAND_LINUX)
    INSTALL = $(INSTALL_LINUX)
endif

all : build/$(LANG)/$(OS)/prisma.o
	$(COMMAND)

build/$(LANG)/$(OS)/prisma.o : src/prisma.c include/prisma.h include/_prisma.h
	$(CC) $(CCFLAGS) -c src/prisma.c -o build/$(LANG)/$(OS)/prisma.o

install : all
	$(INSTALL)

clean :
	$(CLEAN)
mrproper : clean all
re : clean all


.PHONY : all clean mrproper re
