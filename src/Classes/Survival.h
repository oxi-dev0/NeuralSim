#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Utils.h"
#include "Vector2D.h"

#include "../Tools/Log.h"

namespace Survival {
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

	extern std::unordered_map<std::string, KeywordType> keywordToType;

	extern std::unordered_map<std::string, void*> parserVariableMap;

	class ParsedCondition { // These are static conditions such as `IF {STEP} > {STEPSPERGEN}/2`. They are not cell specific
	public:
		std::string lSide;
		std::string rSide;
		ConditionOp op;

		bool Evaluate();

		ParsedCondition(std::string conditionString);
		ParsedCondition() { lSide = ""; rSide = ""; op = ConditionOp::EQUALS; }
	};

	class Survival {
	public:
		struct SurvivalCircle {
		public:
			ParsedCondition condition;
			Vector2D pos;
			float radius;
			SurvivalCircle(Vector2D p, float r) { pos = p; radius = r; }
			SurvivalCircle() { pos = Vector2D(0, 0); radius = 0; }
		};

		struct SurvivalRect {
		public:
			ParsedCondition condition;
			float left, right, top, bottom;
			SurvivalRect(float l, float r, float t, float b) { left = l; right = r; top = t; bottom = b; }
			SurvivalRect() { left = 0; right = 0; top = 0; bottom = 0; }
		};

		std::vector<SurvivalCircle> survivalCircs;
		std::vector<SurvivalRect> survivalRects;

		std::vector<bool> evaluatedCircs;
		std::vector<bool> evaluatedRects;

		Survival() { survivalCircs = std::vector<SurvivalCircle>(); survivalRects = std::vector<SurvivalRect>(); }
		void EvaluateShapes();
	};

	extern Survival SurvivalConfig;
	extern void ParseSurvivalFile(std::string survivalConfigFile);
}