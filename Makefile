CC       = gcc
CFLAGS   = -Wall -O2
INCLUDES = -Ivendor/stb
LIBS     = -lGL -lGLEW -lglfw -lm -lcglm
SRC      = main.c shader.c glError.c camera.c mesh.c texture.c

all: output

output: $(SRC)
	$(CC) $(SRC) -o output $(CFLAGS) $(INCLUDES) $(LIBS)

run: output
	./output

debug: $(SRC)
	$(CC) $(SRC) -o output -Wall -g $(INCLUDES) $(LIBS)

run-debug: debug
	gdb ./output

clean:
	rm -f output