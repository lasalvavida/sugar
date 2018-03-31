#pragma once

namespace sugar {
	class Generic {
	public:
		enum Type {
			BLOCK,
			INSTRUCTION
		};

		Type type;

		Generic(Type genericType) : type(genericType) {};
	};
}