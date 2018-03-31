#pragma once

#include <string>
#include <vector>

#include <sugar/Block.h>

namespace sugar {
	class Parser {
	public:
		Parser() {};
		sugar::Block* parse(std::string content);
	};
}