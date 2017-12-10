#include "header.h"
int writeMmf(char buf[]) {
  int len;
  len = len_file;
  len_file += strlen(buf);

  /*if ((addr = mremap(addr, len, len_file, MREMAP_MAYMOVE)) == MAP_FAILED) {
      perror("Error extending mapping");
      return EXIT_FAILURE;
  }*/
  if (ftruncate(logfd, len_file) != 0){
      perror("Error extending file");
      return EXIT_FAILURE;
  }
  memcpy(addr+len, buf, len_file - len);
  printf( "Val:%s\n",addr ) ; //Checking purpose
  return 1;
}
