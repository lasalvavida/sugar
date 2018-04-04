#include <sugar/ParserTest.h>

#include <sugar/Include.h>

using namespace std;
using namespace sugar;
using namespace lithic::sentinel;

ParserTest::ParserTest() : UnitTest() {
	name = __func__;

	test("parseSimpleInstruction", [=]() {
		Parser* parser = new Parser();
		Block* block = parser->parse("i++;");
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 1);

		Generic* item = block->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);

		Instruction* line = (Instruction*)item;
		expect->toEqual(line->body, string("i++"));
	});

	test("parseSimpleInstructions", [=]() {
		string code = 
			"int i = 0;\n"
			"int j = 0;\n"
			"i++;";

		Parser* parser = new Parser();
		Block* block = parser->parse(code);
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 3);

		Generic* itemOne = block->contents[0];
		expect->toEqual(itemOne->type,
			Generic::Type::INSTRUCTION);
		Instruction* lineOne = (Instruction*)itemOne;
		expect->toEqual(lineOne->body, string("int i = 0"));

		Generic* itemTwo = block->contents[1];
		expect->toEqual(itemTwo->type,
			Generic::Type::INSTRUCTION);
		Instruction* lineTwo = (Instruction*)itemTwo;
		expect->toEqual(lineTwo->body, string("int j = 0"));

		Generic* itemThree = block->contents[2];
		expect->toEqual(itemThree->type,
			Generic::Type::INSTRUCTION);
		Instruction* lineThree = (Instruction*)itemThree;
		expect->toEqual(lineThree->body, string("i++"));
	});

	test("parseSimpleBlock", [=]() {
		string code = 
			"{\n"
			"    i++;\n"
			"    j++;\n"
			"}";

		Parser* parser = new Parser();
		Block* block = parser->parse(code);
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 1);

		Generic* item = block->contents[0];
		expect->toEqual(item->type, Generic::Type::BLOCK);

		Block* subBlock = (Block*)item;
		expect->notToBeNull(subBlock);

		contentsSize = subBlock->contents.size();
		expect->toEqual(contentsSize, 2);

		item = subBlock->contents[0];
		expect->toEqual(item->type,
			Generic::Type::INSTRUCTION);
		Instruction* instruction = (Instruction*)item;;
		expect->toEqual(instruction->body, string("i++"));

		item = subBlock->contents[1];
		expect->toEqual(item->type,
			Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("j++"));
	});

	test("parseNestedBlocks", [=]() {
		string code = 
		    "int k = 0;"
		    "{\n"
			"    int i = 0;\n"
			"    {\n"
			"        i++;\n"
			"    }\n"
			"    int j = 0;\n"
			"    {\n"
			"        j++;\n"
			"    }"
			"}";

		Parser* parser = new Parser();
		Block* block = parser->parse(code);
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 2);

		Generic* item = block->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		Instruction* instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int k = 0"));

		item = block->contents[1];
		expect->toEqual(item->type, Generic::Type::BLOCK);
		Block* subBlock = (Block*)item;

		contentsSize = subBlock->contents.size();
		expect->toEqual(contentsSize, 4);

		item = subBlock->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int i = 0"));

		item = subBlock->contents[1];
		expect->toEqual(item->type, Generic::Type::BLOCK);
		Block* subSubBlock = (Block*)item;

		contentsSize = subSubBlock->contents.size();
		expect->toEqual(contentsSize, 1);

		item = subSubBlock->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("i++"));

		item = subBlock->contents[2];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int j = 0"));

		item = subBlock->contents[3];
		expect->toEqual(item->type, Generic::Type::BLOCK);
		subSubBlock = (Block*)item;

		contentsSize = subSubBlock->contents.size();
		expect->toEqual(contentsSize, 1);

		item = subSubBlock->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("j++"));
	});

	test("ignoresBlockComments", [=]() {
		string code = 
			"int i = 0; /*\n"
			"none of this;\n"
			"should get \"\n"
			"parsed at all **\n"
			"{'//*/\n"
			"int j = 0;";

		Parser* parser = new Parser();
		Block* block = parser->parse(code);
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 2);

		Generic* item = block->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		Instruction* instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int i = 0"));

		item = block->contents[1];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int j = 0"));
	});

	test("ignoresLineComments", [=]() {
		string code =
			"// A comment for me; /*\n"
			"int i = 0;\n"
			"// A comment for you; {\"\n"
			"int j = 0;\n"
			"// Let's have some comments in a big brown shoe";

		Parser* parser = new Parser();
		Block* block = parser->parse(code);
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 2);

		Generic* item = block->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		Instruction* instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int i = 0"));

		item = block->contents[1];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		instruction = (Instruction*)item;
		expect->toEqual(instruction->body, string("int j = 0"));
	});

	test("ignoresString", [=]() {
		string code = "string s = \"{int i = 0;\\\";}\"";

		Parser* parser = new Parser();
		Block* block = parser->parse(code + ";");
		expect->notToBeNull(block);

		int contentsSize = block->contents.size();
		expect->toEqual(contentsSize, 1);

		Generic* item = block->contents[0];
		expect->toEqual(item->type, Generic::Type::INSTRUCTION);
		Instruction* instruction = (Instruction*)item;
		expect->toEqual(instruction->body, code);
	});

	test("parseIncludeMacro", [=]() {
		string code = "#include <stdio>\n";

		Parser* parser = new Parser();
		Block* block = parser->parse(code);
		expect->notToBeNull(block);

		vector<string> includes = block->getIncludes();
		int includesSize = includes.size();
		expect->toEqual(includesSize, 1);

		expect->toEqual(includes[0], string("stdio"));
	});
}