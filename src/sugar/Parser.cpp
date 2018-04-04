#include <sugar/Parser.h>

#include <sugar/Block.h>
#include <sugar/Include.h>
#include <sugar/Instruction.h>

#include <iostream>

using namespace std;
using namespace sugar;

Block* parseBlock(string content, size_t offset, size_t* end) {
	Block* block = new Block();

	bool inString = false;
	bool inChar = false;
	bool inLineComment = false;
	bool inBlockComment = false;
	bool inMacro = false;
	size_t macroStart = true;
	bool inIncludeMacro = false;
	bool includeStarted = false;
	size_t includeStart;
	bool inInstruction = false;
	size_t instructionStart;

	char lastChar = '\0';
	for (size_t i = offset; i < content.length(); i++) {
		char c = content.at(i);
		if (inMacro) {
			if (inIncludeMacro) {
				if (includeStarted) {
					if (c == '"' || c == '>') {
						block->contents.push_back(
							new Include(
								content.substr(includeStart,
									i - includeStart)));
					}
				} else {
					if (c == '"' || c == '<') {
						includeStarted = true;
						includeStart = i + 1;
					}
				}
			} else {
				if (c == ' ') {
					if (macroStart < i) {
						string command = content.substr(macroStart, i - macroStart);
						if (command == "include") {
							inIncludeMacro = true;
							includeStarted = false;
						}
					} else {
						macroStart = i + 1;
					}
				}
			}
		} else if (inBlockComment) {
			if (lastChar == '*' && c == '/') {
				inBlockComment = false;
			}
		} else if (inLineComment) {
			if (c == '\n') {
				inLineComment = false;
			}
		} else if (inString) {
			if (c == '"' && lastChar != '\\') {
				inString = false;
			}
		} else if (inChar) {
			if (c == '\'' && lastChar != '\\') {
				inChar = false;
			}
		} else {
			if (c == ' ' || c == '\n' || c == '\r') {
				continue;
			} else if (c == '#') {
				inMacro = true;
				macroStart = i + 1;
			} else if (lastChar == '/' && c == '*') {
				inBlockComment = true;
				inInstruction = false;
			} else if (c == '/' && lastChar == '/') {
				inLineComment = true;
				inInstruction = false;
			} else if (c == '"') {
				inString = true;
			} else if (c == '\'') {
				inChar = true;
			} else if (c == '{') {
				block->contents.push_back(
					parseBlock(content, i+1, &i));
			} else if (c == '}') {
				if (end != NULL) {
					*end = i;
				}
				return block;
			} else {
				if (!inInstruction) {
					inInstruction = true;
					instructionStart = i;
				} else if (c == ';') {
					block->contents.push_back(
						new Instruction(
							content.substr(instructionStart, 
								i - instructionStart)));
					inInstruction = false;
				}
			}
		}
		lastChar = c;
	}

	return block;
}

Block* Parser::parse(string content) {
	return parseBlock(content, 0, NULL);
}
