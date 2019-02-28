#pragma once

#include <iostream>

class IDisplay {
 public:
  virtual bool windowIsOpen(void) const = 0;
  virtual void renderScene(void) = 0;
};
