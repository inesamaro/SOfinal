#include "header.h"
int mmf() {
    if ((logfd = open("file.log", O_RDWR | O_CREAT, FILEMODE)) < 0){
        perror("Error in file opening");
        return EXIT_FAILURE;
    }

    if ((ret = fstat(logfd, &st)) < 0) {
        perror("Error in fstat");
        return EXIT_FAILURE;
    }

    len_file = st.st_size;

    if ((addr = mmap(NULL, len_file, PROT_READ|PROT_WRITE, MAP_SHARED, logfd, 0)) == MAP_FAILED){
        perror("Error in mmap");
        return EXIT_FAILURE;
    }
    return 1;
}
