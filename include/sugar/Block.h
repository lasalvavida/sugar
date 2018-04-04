#pragma once

#include <string>
#include <vector>

#include <sugar/Generic.h>
#include <sugar/Instruction.h>

namespace sugar {
	class Block : public sugar::Generic {
	public:
		std::vector<Generic*> contents;

		Block() : sugar::Generic(sugar::Generic::Type::BLOCK) {};
		std::vector<std::string> getIncludes();
	};
}