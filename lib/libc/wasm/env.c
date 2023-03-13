

#include <stdio.h>

char *__progname;

char **environ;


FILE *_nsyyin;

static int __errno_location_priv = 0;

int *__errno_location(void)
{
    return &__errno_location_priv;
}