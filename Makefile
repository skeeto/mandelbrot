CFLAGS = -O2 -W -Wall
LDFLAGS = -lm

objects = bmpReader.o \
	bmpWriter.o \
	mandel.o \
	Image.o \
	pnmReader.o \
	yuvReader.o \
	colormap.o \
	main.o

# Main program linking
mandel : $(objects)
	$(CC) $(LDFLAGS) $^ -o $@

# Objects
mandel.o : mandel.c \
	Image.h \
	common.h \
	mandel.h
bmpReader.o: bmpReader.c \
	bmpFormat.h
bmpWriter.o: bmpWriter.c \
	bmpFormat.h
Image.o: Image.c \
	Image.h \
	bmpWriter.h \
	bmpReader.h \
	pnmReader.h \
	yuvReader.h
pnmReader.o: pnmReader.c \
	pnmReader.h
yuvReader.o: yuvReader.c
colormap.o: colormap.c \
	colormap.h \
	Image.h \
	common.h
main.o: main.c \
	common.h \
	colormap.h \
	mandel.h

# Clean target
clean :
	$(RM) $(objects) mandel
