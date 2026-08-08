CC       = gcc
CFLAGS   = -Wall -O2
INCLUDES = -Ivendor/stb -Ientities
LIBS     = -lGL -lGLEW -lglfw -lm -lcglm
SRC      = main.c shader.c glError.c texture.c handler.c entities/camera.c entities/mesh.c 
OBJ      = $(SRC:.c=.o)

all: output

output: $(OBJ)
	$(CC) $(OBJ) -o output $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: output
	./output

debug: $(SRC)
	$(CC) $(SRC) -o output -Wall -g -fsanitize=address $(INCLUDES) $(LIBS)

run-debug: debug
	gdb ./output

clean:
	rm -f output $(OBJ)