#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PROCESSORS 1024

int main (int argc, char **argv) {

    // Process command line arguments
    // int cflag = 0;
    // char *fvalue;
    int pvalue;
    int index;
    int c;

    opterr = 0;


    while ((c = getopt (argc, argv, "f:p:c")) != -1)
        switch (c)
            {
            case 'f':
                // fvalue = optarg;
                break;
            case 'p':
                pvalue = atoi(optarg);
                if (pvalue < 1 || pvalue > MAX_PROCESSORS) {
                    fprintf (stderr, "%d, is an invalid number of processors\n", pvalue);
                    return 1;
                }
                break;
            case 'c':
                // cflag = 1;
                break;
            case '?':
                if (optopt == 'c' || optopt == 'p')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                                     "Unknown option character `\\x%x'.\n",
                                     optopt);
                return 1;
            default:
                abort ();
            }

    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
        
    return 0;
}