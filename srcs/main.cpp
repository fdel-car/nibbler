#include <dlfcn.h>
#include "IDisplay.hpp"

void dlerrorWrapper(void) {
  std::cerr << "Error: " << dlerror() << std::endl;
  exit(EXIT_FAILURE);
}

int main(int ac, char **av) {
  IDisplay *(*displayCreator)(void);
  void (*displayDestructor)(IDisplay *);

  if (ac != 2) return EXIT_FAILURE;
  void *handle = dlopen(av[1], RTLD_LAZY);
  if (!handle) dlerrorWrapper();

  displayCreator = (IDisplay * (*)(void)) dlsym(handle, "createDisplay");
  if (!displayCreator) dlerrorWrapper();

  IDisplay *display = displayCreator();
  while (display->windowIsOpen()) {
    display->renderScene();
  }

  displayDestructor = (void (*)(IDisplay *))dlsym(handle, "deleteDisplay");
  if (!displayDestructor) dlerrorWrapper();
  displayDestructor(display);

  dlclose(handle);
  return EXIT_SUCCESS;
}
