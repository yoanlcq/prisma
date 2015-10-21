define OS_ARCH_ERROR


OS and ARCH are not defined. 
Please set them on the command line when calling make,
with one of the following values :

OS = windows | linux | osx
ARCH = 32 | 64 (don't specify for osx)

Like so : 
	    make OS=linux ARCH=64


endef

ifndef OS
$(error $(call OS_ARCH_ERROR))
endif

ifneq ($(OS),osx)
ifndef ARCH
$(error $(call OS_ARCH_ERROR))
endif
endif

ifndef ARCH
ARCH=
endif

CC = gcc

CCFLAGS = -Iinclude -O3
ifneq ($(ARCH),)
CCFLAGS += -m$(ARCH)
endif
LDFLAGS = 
LDLIBS = -lm
BUILDDIR=build/$(OS)$(ARCH)
LIBDIR=lib/$(OS)$(ARCH)
OFILES = $(BUILDDIR)/prisma.o
CLEAN = rm -f $(OFILES)
CLEAN_WINDOWS = del /f /q build\$(OS)$(ARCH)\*
MKDIR=mkdir
ifneq ($(OS),windows)
MKDIR += -p
endif

INSTALL =

COMMAND_WINDOWS = $(CC) $(CCFLAGS) -shared -o $(LIBDIR)/prisma.dll $(BUILDDIR)/prisma.o $(LDFLAGS) $(LDLIBS)


ifeq ($(OS),windows)
    CCFLAGS += -DBUILD_DLL
    COMMAND = $(COMMAND_WINDOWS)
    CLEAN = $(CLEAN_WINDOWS)
endif


COMMAND_LINUX = $(CC) $(CCFLAGS) -shared $(BUILDDIR)/prisma.o -Wl,-soname,libprisma.so -o $(LIBDIR)/libprisma.so $(LDFLAGS) $(LDLIBS)

INSTALL_LINUX = cp $(LIBDIR)/libprisma.so /usr/local/lib/; ldconfig; cp include/* /usr/local/include; cp -pR doc/man? /usr/local/share/man/; mandb

ifeq ($(OS),linux)
    CCFLAGS += -fPIC
    COMMAND = $(COMMAND_LINUX)
    INSTALL = $(INSTALL_LINUX)
endif

ifeq ($(OS),osx)
    CCFLAGS += -fPIC
    COMMAND = $(COMMAND_LINUX) -dynamiclib -flat_namespace
    INSTALL = $(INSTALL_LINUX)
endif

all : $(BUILDDIR) $(LIBDIR) $(BUILDDIR)/prisma.o
	$(COMMAND)

$(BUILDDIR) :
	$(MKDIR) $(BUILDDIR)

$(LIBDIR) :
	$(MKDIR) $(LIBDIR)


$(BUILDDIR)/prisma.o : src/prisma.c include/prisma.h include/_prisma.h
	$(CC) $(CCFLAGS) -c src/prisma.c -o $(BUILDDIR)/prisma.o

install : all
	$(INSTALL)

clean :
	$(CLEAN)
mrproper : clean all
re : clean all


.PHONY : all clean mrproper re
