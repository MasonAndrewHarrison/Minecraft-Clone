#include "state.h"


State* initState(GLFWwindow* window, const GLFWvidmode* mode, bool startWithInfiniteWorldGen){

    State* state = malloc(sizeof(State));
    state->wireFrame = 0;
    state->needOfUpdate = 0;
    state->window = window;
    state->cam = cameraInit(mode->height, mode->width);
    state->infiniteWorldGen = startWithInfiniteWorldGen;
    state->tabWasPressed = 0;

    return state;
}