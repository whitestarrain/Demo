#include "hotload.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  for (int i = 20; i > 0; i--) {
    // TODO: tiggered by siagal
    reload_plug();
    plug_print();
    sleep(1);
  }
  return EXIT_SUCCESS;
}
