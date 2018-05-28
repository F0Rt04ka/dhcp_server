#include "custom_function.h"

enum colors {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Default = 9
};

void exit_error(char* message)
{
    error_message(message);
    exit(EXIT_FAILURE);
}

void info_message(char* message)
{
    printf("\E[3%dm%s\E[3%dm\n", Yellow, message, Default);
}

void error_message(char* message)
{
    printf("\E[3%dm%s\E[3%dm\n", Red, message, Default);
}