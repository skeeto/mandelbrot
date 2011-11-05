CC = gcc
CFLAGS = -O2 -W -Wall
LDLIBS = -lm

objects = bmpWriter.o \
	mandel.o \
	Image.o \
	colormap.o \
	main.o \
	parse.o

# Main program linking
mandel : $(objects)

# Objects
mandel.o : mandel.c \
	Image.h \
	common.h \
	mandel.h
bmpWriter.o: bmpWriter.c \
	bmpFormat.h
Image.o: Image.c \
	Image.h \
	bmpWriter.h 
colormap.o: colormap.c \
	colormap.h \
	Image.h \
	common.h
main.o: main.c \
	common.h \
	colormap.h \
	mandel.h \
	parse.h
parse.o: parse.c \
	parse.h \
	common.h \
	colormap.h

# Clean target
clean :
	$(RM) $(objects) mandel
