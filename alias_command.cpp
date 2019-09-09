#include "alias_command.h"

void alias_cmd(char** vect, int len)
{
    int fd = open("aliasfile", O_RDWR | O_APPEND, 0774);
    
}