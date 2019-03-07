#pragma once

#include <iostream>

class IAudio {
 public:
  virtual void playAudio(void) = 0;

  virtual ~IAudio(){};
};
