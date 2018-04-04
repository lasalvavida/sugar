#include <sugar/Block.h>

#include <sugar/Include.h>

using namespace std;
using namespace sugar;

void getIncludesRecursive(Block* block, vector<string>* includes) {
	for (Generic* generic : block->contents) {
		if (generic->type == Generic::Type::INCLUDE) {
			Include* include = (Include*)generic;
			includes->push_back(include->target);
		} else if (generic->type == Generic::Type::BLOCK) {
			getIncludesRecursive((Block*)generic, includes);
		}
	}
}

vector<string> Block::getIncludes() {
	vector<string> includes;
	getIncludesRecursive(this, &includes);
	return includes;
}
