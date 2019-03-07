#pragma once

#include <SFML/Audio.hpp>

#include "IAudio.hpp"

class SFMLAudio : public IAudio {
 public:
  SFMLAudio(void);
  virtual ~SFMLAudio(void);

  void playAudio(void);

 private:
  sf::Music _music;

  SFMLAudio(SFMLAudio const &src);

  SFMLAudio &operator=(SFMLAudio const &rhs);
};

extern "C" {
SFMLAudio *createAudio(void);
void deleteAudio(SFMLAudio *Audio);
}
