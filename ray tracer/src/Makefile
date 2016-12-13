CC = g++

RM = /bin/rm -f 
all: main

main: main.o shader.o Vectors.h Matrices.h Colors.h Shape.h Light.h
	$(CC) $(CFLAGS) -o main main.o shader.o $(INCFLAGS) $(LDFLAGS) 
main.o: Vectors.h Matrices.h Colors.h Shape.h main.cpp Light.h Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
matrices.o: Vectors.h Matrices.h Matrices.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c Matrices.cpp
shader.o: Vectors.h Shader.cpp Colors.h Shape.h Light.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Shader.cpp

clean:
	$(RM) *.o
