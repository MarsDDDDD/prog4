#include "Command.h"
#include "InputManager.h"
#include <iostream>

using namespace dae;

void MoveUpCommand::Execute()
{
	std::cout << "up" << std::endl;
}

void MoveDownCommand::Execute()
{
	std::cout << "down" << std::endl;
}

void MoveLeftCommand::Execute()
{
	std::cout << "left" << std::endl;
}

void MoveRightCommand::Execute()
{
	std::cout << "right" << std::endl;
}
