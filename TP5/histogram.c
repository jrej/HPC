
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>


/* Wrapper to check for errors */
#define CHECK_ERROR(a)                        \
if (a) {                                      \
    fprintf(stderr, "*** Error :\n" #a "\n"); \
    exit(1);                                  \
}


#define cas32(_a, _o, _n) ({                   \
        __typeof__(_o) __o = _o;               \
        __asm__ __volatile__(                  \
            "lock cmpxchg %3,%1"               \
          : "=a" (__o), "=m" (*(volatile uint32_t *)(_a)) \
          : "0"  (__o), "r" (_n));             \
        __o;                                   \
        })


#define MAX_THREADS 16
int32_t id_table[MAX_THREADS];
pthread_t pthread_table[MAX_THREADS];

pthread_mutex_t lock;
pthread_mutex_t lockb;
pthread_mutex_t lockr;
pthread_mutex_t lockv;

#define DEFAULT_NTHREADS 1

#define IMG_DATA_OFFSET_POS 10
#define BITS_PER_PIXEL_POS 28
#define PAGE_SIZE 0x1000

#define array_size(tab) (sizeof(tab) / sizeof(tab[0]))

#define CREATE(f, n) do {                         \
   for (int32_t _i = 1; _i < (n); _i += 1) {      \
      int32_t error = pthread_create(&pthread_table[_i], NULL, f, (void *) &id_table[_i]); \
      if (error != 0) {                           \
         printf("*** Error in pthread_create\n"); \
         exit(-1);                                \
      }                                           \
   }                                              \
   f(&id_table[0]);                               \
} while (false)

#define WAIT_FOR_END(n) do {                                 \
   for (int32_t _i = 1; _i < (n); _i += 1) {                 \
      int32_t error = pthread_join(pthread_table[_i], NULL); \
      if (error != 0) {                                      \
         printf("*** Error in pthread_join\n");              \
         exit(-1);                                           \
      }                                                      \
   }                                                         \
} while (false)



typedef void * (*calc_func_t)(void *);


bool swap; // to indicate if we need to swap byte order of header information

typedef struct {
    int32_t align_buff_s[16];
    uint8_t * data;
    int32_t start;
    int32_t len;
    int32_t * red;
    int32_t * gre;
    int32_t * blu;
    int32_t align_buff_e[16];
} thread_arg_t;


thread_arg_t * arg;

pthread_mutex_t ** lock_tab_r;
pthread_mutex_t ** lock_tab_v;
pthread_mutex_t ** lock_tab_b;

int64_t clocktime() {
   struct timeval full_time;

   gettimeofday(&full_time, NULL);
   return (int64_t) ((full_time.tv_usec + full_time.tv_sec * 1000000) / 1000);
}


/**
 * test_endianess
 */
void test_endianess() {
    uint32_t num = 0x12345678;
    uint8_t * low = (uint8_t *) (&num);
    if (*low ==  0x78) {
        swap = false;
    }
    else if (*low == 0x12) {
        swap = true;
    }
    else {
        fprintf(stderr, "*** Error: Invalid value found in memory\n");
        exit(1);
    }
}


/**
 * swap_bytes
 */
static void swap_bytes(char * bytes, int32_t num_bytes) {
    for (int32_t i = 0; i < num_bytes / 2; i++) {
        printf("Swapping %d and %d\n", bytes[i], bytes[num_bytes - i - 1]);
        uint8_t tmp = bytes[i];
        bytes[i] = bytes[num_bytes - i - 1];
        bytes[num_bytes - i - 1] = tmp;
    }
}


static void init_histogram(int32_t red[256], int32_t gre[256], int32_t blu[256]) {
    memset(&red[0], 0, sizeof(int32_t) * 256);
    memset(&gre[0], 0, sizeof(int32_t) * 256);
    memset(&blu[0], 0, sizeof(int32_t) * 256);
}


/* *
 * calc_seq
 * Sequential computation of the histogram
 * Serves as a reference result
 */
void * calc_seq(thread_arg_t * arg) {    
    int32_t * red_ref = arg->red;
    int32_t * gre_ref = arg->gre;
    int32_t * blu_ref = arg->blu;
    uint8_t * data = arg->data;

    init_histogram(red_ref, gre_ref, blu_ref);
    for (int32_t i = arg->start; i < arg->start + arg->len; i += 3) {

        blu_ref[data[i]] += 1;
        gre_ref[data[i + 1]] += 1;
        red_ref[data[i + 2]] += 1;

    }

    return NULL;
}


void * calc_par_0(void * id) {
    int iid = *((int*)id);
    int32_t * red_ref = arg[iid].red;
    int32_t * gre_ref = arg[iid].gre;
    int32_t * blu_ref = arg[iid].blu;
    uint8_t * data = arg[iid].data;

    init_histogram(red_ref, gre_ref, blu_ref);
    for (int32_t i = arg[iid].start; i < arg[iid].start + arg[iid].len; i += 3) {
        pthread_mutex_lock(&lock);
        blu_ref[data[i]] += 1;
        gre_ref[data[i + 1]] += 1;
        red_ref[data[i + 2]] += 1;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}


void * calc_par_1(void * id) {
   int iid = *((int*)id);
    int32_t * red_ref = arg[iid].red;
    int32_t * gre_ref = arg[iid].gre;
    int32_t * blu_ref = arg[iid].blu;
    uint8_t * data = arg[iid].data;

    init_histogram(red_ref, gre_ref, blu_ref);
    for (int32_t i = arg[iid].start; i < arg[iid].start + arg[iid].len; i += 3) {
        pthread_mutex_lock(&lockb);
        blu_ref[data[i]] += 1;
        pthread_mutex_unlock(&lockb);
        pthread_mutex_lock(&lockv);
        gre_ref[data[i + 1]] += 1;
        pthread_mutex_unlock(&lockv);
        pthread_mutex_lock(&lockr);
        red_ref[data[i + 2]] += 1;
        pthread_mutex_unlock(&lockr);
    }


    
    return NULL;
}


void * calc_par_2(void * id) {
    int iid = *((int*)id);
    int32_t * red_ref = arg[iid].red;
    int32_t * gre_ref = arg[iid].gre;
    int32_t * blu_ref = arg[iid].blu;
    uint8_t * data = arg[iid].data;
    
    lock_tab_r[iid]= (pthread_mutex_t*)malloc(arg[iid].len * sizeof(pthread_mutex_t));
    lock_tab_v[iid]= (pthread_mutex_t*)malloc(arg[iid].len * sizeof(pthread_mutex_t));
    lock_tab_b[iid]= (pthread_mutex_t*)malloc(arg[iid].len * sizeof(pthread_mutex_t));

    init_histogram(red_ref, gre_ref, blu_ref);
    for (int32_t i = arg[iid].start; i < arg[iid].start + arg[iid].len; i += 3) {
        pthread_mutex_lock(& (lock_tab_r[iid][i-arg[iid].start]) );
        blu_ref[data[i]] += 1;
        pthread_mutex_unlock(& (lock_tab_r[iid][i-arg[iid].start]) );
        pthread_mutex_lock(& (lock_tab_v[iid][i-arg[iid].start]) );
        gre_ref[data[i + 1]] += 1;
        pthread_mutex_unlock(& (lock_tab_v[iid][i-arg[iid].start]) );
        pthread_mutex_lock(& (lock_tab_b[iid][i-arg[iid].start]) );
        red_ref[data[i + 2]] += 1;
        pthread_mutex_unlock(& (lock_tab_b[iid][i-arg[iid].start]) );
    }

    return NULL;
}


void * calc_par_3(void * id) {
    return NULL;
}


void * calc_par_4(void * id) {
    return NULL;
}


void * calc_par_5(void * id) {
    return NULL;
}


static bool check_results(int32_t red[256], int32_t gre[256], int32_t blu[256], int32_t red_ref[256], int32_t gre_ref[256], int32_t blu_ref[256]) {
    for (int32_t i = 0; i < 256; i += 1) {
        if (red[i] != red_ref[i] || blu[i] != blu_ref[i] || gre[i] != gre_ref[i]) {
            return false;
        }
    }
    return true;
}


static void usage(char * name) {
    printf("Usage: %s <options> <bitmap filename>\n", name);
    printf("options:\n");
    printf("   -n <num_threads> : Number of threads (default = %d).\n", DEFAULT_NTHREADS);
    printf("   -p               : Print result (default = false)\n");
    printf("   -d               : Dump results for plotting (default = false)\n");
}


int32_t main(int32_t argc, char * argv[]) {

    int32_t fd;
    char * fdata;
    struct stat finfo;
    char * fname;



    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    if (pthread_mutex_init(&lockb, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    if (pthread_mutex_init(&lockv, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    if (pthread_mutex_init(&lockr, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    


    int32_t red[256];
    int32_t gre[256];
    int32_t blu[256];
    
    int32_t red_ref[256];
    int32_t gre_ref[256];
    int32_t blu_ref[256];

    int32_t num_threads = DEFAULT_NTHREADS;
    bool dump_results = false;
    int32_t num_per_thread;
    int32_t excess;

    calc_func_t func_tab[] = {  calc_par_0,   calc_par_1,   calc_par_2,   calc_par_3,   calc_par_4,   calc_par_5, };
    char * func_name[]     = { "calc_par_0", "calc_par_1", "calc_par_2", "calc_par_3", "calc_par_4", "calc_par_5", };

    int32_t ch;

    while ((ch = getopt(argc, argv, "n:hd")) != EOF) {
        switch (ch) {
            case 'n':
                num_threads = atol(optarg);
                if (num_threads > MAX_THREADS) {
                    fprintf(stderr, "*** Error: the number of threads should be less than %d\n", MAX_THREADS);
                    exit(1);
                }
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            case 'd':
                dump_results = true;
                break;
            default:
                usage(argv[0]);
                exit(1);
                break;
        }
    }





    //init mutex en fct du nb de thread
    lock_tab_r = (pthread_mutex_t**)malloc(num_threads * sizeof(pthread_mutex_t*));
    lock_tab_v = (pthread_mutex_t**)malloc(num_threads * sizeof(pthread_mutex_t*));
    lock_tab_b = (pthread_mutex_t**)malloc(num_threads * sizeof(pthread_mutex_t*));
    


    if (argc == 1) {
        usage(argv[0]);
        exit(1);
    }

    fname = argv[argc - 1];

    // Read in the file
    CHECK_ERROR((fd = open(fname, O_RDONLY)) < 0);
    // Get the file info (for file length)
    CHECK_ERROR(fstat(fd, &finfo) < 0);
    // Memory map the file
    CHECK_ERROR((fdata = mmap(0, finfo.st_size - 1, PROT_READ, MAP_PRIVATE, fd, 0)) == NULL);

    if ((fdata[0] != 'B') || (fdata[1] != 'M')) {
        fprintf(stderr, "*** Error: File %s is not a valid bitmap file. Exiting\n", fname);
        exit(1);
    }

    test_endianess(); // will set the variable "swap"

    uint16_t * bitsperpixel = (uint16_t *) &fdata[BITS_PER_PIXEL_POS];
    if (swap) {
        swap_bytes((uint8_t *) (bitsperpixel), sizeof(*bitsperpixel));
    }

    if (*bitsperpixel != 24) {    // ensure its 3 bytes per pixel
        fprintf(stderr, "*** Error: Invalid bitmap format: this application only accepts 24-bit pictures. Exiting\n");
        exit(1);
    }

    uint16_t * data_pos = (uint16_t *) &fdata[IMG_DATA_OFFSET_POS];
    if (swap) {
        swap_bytes((uint8_t *) (data_pos), sizeof(*data_pos));
    }

    int32_t imgdata_bytes = (int32_t) finfo.st_size - (int32_t) (*data_pos);
    int32_t num_pixels = imgdata_bytes / 3;

    num_per_thread = num_pixels / num_threads;
    excess = num_pixels % num_threads;

    printf("*** Starting Histogram ***\n");
    printf(" - Image: %s\n", fname);
    printf(" - Number of threads: %d\n", num_threads);
    printf(" - Number of pixels: %d\n", num_pixels);

    arg = malloc(sizeof(thread_arg_t) * num_threads);
    CHECK_ERROR(arg == NULL);

    /* Sequential computation */
    {
        thread_arg_t arg;
        arg.data = (uint8_t *) fdata;
        arg.start = (int32_t) (*data_pos);
        arg.len = num_pixels * 3;
        arg.red = &red_ref[0];
        arg.gre = &gre_ref[0];
        arg.blu = &blu_ref[0];
        int64_t start = clocktime();
        calc_seq(&arg);
        int64_t end = clocktime();
        printf(" - calc_seq completed   %8lld ms\n", end - start);
    }


    /* Assign portions of the image to each thread */
    int32_t curr_pos = (int32_t) (*data_pos);
    for (int32_t i = 0; i < num_threads; i++) {
        arg[i].data = (uint8_t *) fdata;
        arg[i].start = curr_pos;
        arg[i].len = num_per_thread;
        if (excess > 0) {
            arg[i].len += 1;
            excess -= 1;
        }
        id_table[i] = i;
        arg[i].red = &red[0];
        arg[i].gre = &gre[0];
        arg[i].blu = &blu[0];

        arg[i].len *= 3; // 3 bytes per pixel
        curr_pos += arg[i].len;
    }

    for (int32_t i = 0; i < array_size(func_tab); i += 1) {
        init_histogram(red, gre, blu);
        int64_t start = clocktime();

        CREATE(func_tab[i], num_threads);
        WAIT_FOR_END(num_threads);

        int64_t end = clocktime();
        bool ok = check_results(red, gre, blu, red_ref, gre_ref, blu_ref);

        printf(" - %s completed %8lld ms %s\n", func_name[i], end - start, ok ? "[OK]" : "[KO]");
    }

    CHECK_ERROR(munmap(fdata, finfo.st_size + 1) < 0);
    CHECK_ERROR(close(fd) < 0);
    
    free(arg);

    if (dump_results) {
        const char * data_fname = "data.txt";
        FILE * f = fopen(data_fname, "w");
        if (f == NULL) {
            fprintf(stderr, "*** Error opening file %s\n", data_fname);
            exit(1);
        }
        printf(" - Dumping results...\n");
        for (int32_t j = 0; j < 256; j += 1) {
            fprintf(f, "%6d %6d %6d %6d\n", j, red_ref[j], gre_ref[j], blu_ref[j]);
        }
        fclose(f);
    }


    printf("*** End of Histogram ***\n");

    return 0;
}

// Local Variables:
// tab-width: 4
// c-basic-offset: 4
// c-file-offsets:((innamespace . 0)(inline-open . 0))
// indent-tabs-mode: nil
// End:

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4

