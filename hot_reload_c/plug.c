#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int plug_print() {
  char message[] = "hello 1\n";
  write(1, message, strlen(message));
  return EXIT_SUCCESS;
}
