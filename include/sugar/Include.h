#pragma once

#include <string>

#include <sugar/Generic.h>

namespace sugar {
	class Include : public sugar::Generic {
	public:
		std::string target;

		Include(std::string includeTarget) : 
			sugar::Generic(sugar::Generic::Type::INCLUDE),
			target(includeTarget) {};
	};
}