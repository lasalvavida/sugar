#pragma once

#include <string>

#include <sugar/Generic.h>

namespace sugar {
	class Instruction : public sugar::Generic {
	public:
		std::string body;

		Instruction(std::string instructionBody) : 
			sugar::Generic(sugar::Generic::Type::INSTRUCTION), 
			body(instructionBody) {};
	};
}