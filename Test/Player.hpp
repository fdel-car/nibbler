#pragma once

#include "IEntity.hpp"
#include <iostream>

class Player : public IEntity {

private:
	std::string _name;
	int 		_score;
	Player(void);

public:
	Player(std::string name);
	~Player(void);

	std::string getName(void) const;
	int getScore(void) const;
	void markPoint(void);
	void announces(void) const;

};

extern "C" {
	Player	*createPlayer(const std::string &name);
	void	deletePlayer(Player *player);
}
