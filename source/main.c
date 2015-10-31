#define NO_MAIN_WRAPPER
#include <bk.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    void* stack_bottom = NULL;
    gc_init(&stack_bottom);
    atexit(gc_deinit);
    return user_main(argc, argv);
}
