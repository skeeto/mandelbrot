CFLAGS = -O2 -W -Wall
LDFLAGS = -lm

objects = bmpReader.o \
          bmpWriter.o \
          frac.o \
          Image.o \
          pnmReader.o \
          yuvReader.o \
          colormap.o

frac : $(objects)
	$(CC) $(LDFLAGS) $^ -o $@

frac.o : frac.c \
	Image.h \
	common.h
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

clean :
	$(RM) $(objects) frac
