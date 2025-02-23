#include "hotload.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static void *libplug = NULL;
static const char *libplug_name = "output/libplug.so";

plug_print_t *plug_print = NULL;

bool reload_plug() {
  if (libplug != NULL) {
    dlclose(libplug);
  }
  libplug = dlopen(libplug_name, RTLD_NOW);
  if (libplug == NULL) {
    printf("hotload: could not load %s: %s", libplug_name, dlerror());
    return false;
  }
  plug_print = dlsym(libplug, "plug_print");
  if (plug_print == NULL) {
    printf("hotload: could not find %s: %s", "plug_print", dlerror());
  }
  return true;
}
