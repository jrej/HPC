/* -------------- */
/* --- main.c --- */
/* -------------- */

#include <stdio.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "test_matrix.h"

#include "mymacro.h"

// ============
void info(void)
// ============
{
#ifdef ENABLE_BENCHMARK
    puts("#############################");
    puts("mode Benchmark ON & DEBUG OFF");
    puts("#############################");
#else
    puts("#############################");
    puts("mode Benchmark OFF & DEBUG ON");
    puts("#############################");
#endif
}
// =============================
int main(int argc, char *argv[])
// =============================
{
    info();
    //main_matrix(argc, argv);
    main_stencil(argc, argv);

    return 0;   
}