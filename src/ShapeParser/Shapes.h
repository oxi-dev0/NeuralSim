#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "../Classes/Utils.h"
#include "../Classes/Vector2D.h"

#include "../Tools/Log.h"

namespace ShapeParser {

	enum KeywordType {
		CONDITION,
		SHAPE
	};

	enum ConditionOp {
		EQUALS,
		NOTEQUALS,
		GREATERTHAN,
		LESSERTHAN,
		GREATERTHANOREQUALS,
		LESSERTHANOREQUALS
	};

	class ParsedCondition { // These are static conditions such as `IF {GEN} > 75`. They are not cell specific
	public:
		std::string lSide;
		std::string rSide;
		ConditionOp op;

		bool Evaluate();

		ParsedCondition(std::string conditionString);
		ParsedCondition() { lSide = ""; rSide = ""; op = ConditionOp::EQUALS; }
	};

	struct Circle {
	public:
		ParsedCondition condition;
		Vector2D pos;
		unsigned int radius;
		Circle(Vector2D p, unsigned int r) { pos = p; radius = r; }
		Circle() { pos = Vector2D(0, 0); radius = 0; }
	};

	struct Rect {
	public:
		ParsedCondition condition;
		int left, right, top, bottom;
		Rect(int l, int r, int t, int b) { left = l; right = r; top = t; bottom = b; }
		Rect() { left = 0; right = 0; top = 0; bottom = 0; }
	};

	extern std::unordered_map<std::string, KeywordType> keywordToType;

	extern std::unordered_map<std::string, void*> parserVariableMap;

	class ShapeFile {
	public:
		std::vector<Circle> circles;
		std::vector<Rect> rects;

		std::vector<bool> evaluatedCircles; // These are kept as caches instead of calling circle.condition.evaluate for each shape to save cycles
		std::vector<bool> evaluatedRects;

		ShapeFile() { circles = std::vector<Circle>(); rects = std::vector<Rect>(); }
		void EvaluateShapes();
	};

	extern ShapeFile FromFile(std::string file);

	extern ShapeFile SurvivalConfig; // Stored in here as only generation.h and main.h need to reference it
}