#pragma once

namespace sugar {
	class Generic {
	public:
		enum Type {
			BLOCK,
			INSTRUCTION,
			INCLUDE
		};

		Type type;

		Generic(Type genericType) : type(genericType) {};
	};
}