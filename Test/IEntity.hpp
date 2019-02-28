#pragma once

#include <iostream>

class IEntity {
public:
	virtual std::string getName(void) const = 0;
	virtual int getScore(void) const = 0;
	virtual void markPoint(void) = 0;
	virtual void announces(void) const = 0;
};
