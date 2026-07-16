CC     = gcc
CFLAGS = -Wall -O2
LIBS   = -lGL -lGLEW -lglfw -lm -lcglm
SRC    = main.c shader.c glError.c camera.c mesh.c

all: output

output: $(SRC)
	$(CC) $(SRC) -o output $(CFLAGS) $(LIBS)

run: output
	./output

debug: $(SRC)
	$(CC) $(SRC) -o output -Wall -g $(LIBS)

run-debug: debug
	gdb ./output

clean:
	rm -f output