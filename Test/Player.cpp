#include "Player.hpp"

Player::Player(std::string name) : _score(0), _name(name) { }

Player::~Player(void) { }

std::string	Player::getName(void) const {
	return this->_name;
}

int Player::getScore(void) const {
	return this->_score;
}

void Player::markPoint(void) {
	this->_score++;
}

void Player::announces(void) const {
	std::cout << "My name is " << this->_name << " and I have destroyed ";
	std::cout << this->_score << " enemies so far" << std::endl;
}

Player	*createPlayer(const std::string &name) {
	return new Player(name);
}

void deletePlayer(Player *player) {
	delete player;
}
