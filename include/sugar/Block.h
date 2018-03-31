#pragma once

#include <string>
#include <vector>

#include <sugar/Generic.h>
#include <sugar/Instruction.h>

namespace sugar {
	class Block : public sugar::Generic {
	public:
		Block() : sugar::Generic(sugar::Generic::Type::BLOCK) {};
		std::vector<Generic*> contents;
	};
}