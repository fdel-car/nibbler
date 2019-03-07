#include "AUDIO/SFMLAudio.hpp"

SFMLAudio::SFMLAudio(void) {
	std::cout << "Start Audio" << std::endl;
}

SFMLAudio::~SFMLAudio(void) { }

void SFMLAudio::playAudio(void) {
	if (_music.openFromFile("./audio.ogg")) {
		// _music.play();
		_music.setLoop(true);
	}
}

SFMLAudio *createAudio(void) { return new SFMLAudio(); }

void deleteAudio(SFMLAudio *audio) { delete audio; }
