#include <stdio.h>
#include "yqsh.h"

int main(int argc, char const *argv[])
{
    // Initialization
    
    // TODO: read config file
    printf("yqsh version %s\n", YQSH_VERSION);   
    // Main loop
    yqsh_loop(argc, argv);
    // Cleanup
    
    return EXIT_SUCCESS;
}
