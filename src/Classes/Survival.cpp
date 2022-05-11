#include "Survival.h"

namespace Survival {

	std::unordered_map<std::string, KeywordType> keywordToType = {
		{"IF", KeywordType::CONDITION},
		{"ELSE", KeywordType::CONDITION},
		{"ENDIF", KeywordType::CONDITION},
		{"BOX", KeywordType::SHAPE},
		{"CIRCLE", KeywordType::SHAPE}
	};

	std::unordered_map<std::string, void*> parserVariableMap = {
		{"STEP", &(Utils::GlobalSimData.currentStep)},
		{"STEPSPERGEN", &(Utils::GlobalConfig.stepsPerGen)},
		{"GEN", &(Utils::GlobalSimData.currentGen)}
	};

	Survival SurvivalConfig;

	std::vector<std::string> split(const std::string& s, char seperator)
	{
		std::vector<std::string> output;

		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = s.find(seperator, pos)) != std::string::npos)
		{
			std::string substring(s.substr(prev_pos, pos - prev_pos));

			output.push_back(substring);

			prev_pos = ++pos;
		}

		output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

		return output;
	}

	bool ParsedCondition::Evaluate() {
		bool lVariable = false;
		if (lSide[0] == '{') {
			lVariable = true;
		}

		bool rVariable = false;
		if (rSide[0] == '{') {
			rVariable = true;
		}

		std::string lVar;
		std::string rVar;
		
		if (lVariable) {
			lVar = lSide.substr(1, lSide.size() - 2);
			if (parserVariableMap.find(lVar) == parserVariableMap.end()) { LOG_CRITICAL("Unknown condition statement {0}", lVar); std::terminate(); }
		}

		if (rVariable) {
			rVar = rSide.substr(1, rSide.size() - 2);
			if (parserVariableMap.find(rVar) == parserVariableMap.end()) { LOG_CRITICAL("Unknown condition statement {0}", rVar); std::terminate(); }
		}

		int l = 0;
		int r = 0;

		if (lVariable) {
			l = *static_cast<int*>(parserVariableMap[lVar]);
		}
		else {
			l = std::stoi(lSide);
		}

		if (rVariable) {
			r = *static_cast<int*>(parserVariableMap[rVar]);
		}
		else {
			r = std::stoi(rSide);
		}

		switch (op) {
		case ConditionOp::EQUALS:				return l == r;	break;
		case ConditionOp::GREATERTHAN:			return l > r;	break;
		case ConditionOp::GREATERTHANOREQUALS:	return l >= r;	break;
		case ConditionOp::LESSERTHAN:			return l < r;	break;
		case ConditionOp::LESSERTHANOREQUALS:	return l <= r;	break;
		case ConditionOp::NOTEQUALS:			return l != r;	break;
		}
	}

	ParsedCondition::ParsedCondition(std::string conditionString) {
		auto data = split(conditionString, ' ');
		lSide = data[0];
		rSide = data[2];

		std::string opString = data[1];
		if (opString == "==") { op = ConditionOp::EQUALS; return; }
		if (opString == "!=") { op = ConditionOp::NOTEQUALS; return; }
		if (opString == ">") { op = ConditionOp::GREATERTHAN; return; }
		if (opString == ">=") { op = ConditionOp::GREATERTHANOREQUALS; return; }
		if (opString == "<") { op = ConditionOp::LESSERTHAN; return; }
		if (opString == "<=") { op = ConditionOp::LESSERTHANOREQUALS; return; }

		LOG_CRITICAL("Invalid IF statement operator {0}", opString);
	}

	ConditionOp InvertOp(ConditionOp op) {
		switch (op) {
		case ConditionOp::EQUALS:
			return ConditionOp::NOTEQUALS;
			break;
		case ConditionOp::NOTEQUALS:
			return ConditionOp::EQUALS;
			break;
		case ConditionOp::GREATERTHAN:
			return ConditionOp::LESSERTHANOREQUALS;
			break;
		case ConditionOp::GREATERTHANOREQUALS:
			return ConditionOp::LESSERTHAN;
			break;
		case ConditionOp::LESSERTHAN:
			return ConditionOp::GREATERTHANOREQUALS;
			break;
		case ConditionOp::LESSERTHANOREQUALS:
			return ConditionOp::GREATERTHAN;
			break;
		}
	}

	std::string join(std::vector<std::string> lst, std::string delim, int startIndex)
	{
		std::string ret;
		int i = 0;
		for (const auto& s : lst) {
			if (i < startIndex) { i++; continue; }
			if (i > startIndex) { ret += delim; }
			ret += s;
			i++;
		}
		return ret;
	}

	void ParseSurvivalFile(std::string survivalConfigFile) {
		if (!std::filesystem::exists(survivalConfigFile)) { LOG_CRITICAL("Could not load survival condition file ({0})", survivalConfigFile); std::terminate(); }
		std::ifstream f(survivalConfigFile);
		Survival newSurvival;

		ParsedCondition currentCondition; // When an if statement is parsed, everything after it has this condition applied to it, other than if its another condition keyword

		std::string line;
		while (std::getline(f, line)) {
			auto data = split(line, ' ');
			if (data.size() == 0) { continue; }

			std::string keyword = data[0];
			if (keywordToType.find(keyword) == keywordToType.end()) { LOG_CRITICAL("Invalid keyword '{0}' in survival condition config"); std::terminate(); }
			KeywordType type = keywordToType[keyword];			
			
			switch (type) {
			case KeywordType::CONDITION:
			{
				if (keyword == "IF") {
					if (data.size() != 4) { LOG_CRITICAL("Invalid amount of args to IF keyword in survival condition config ({0} instead of {1})", data.size(), 4); std::terminate(); }
					std::string conditionStr = join(data, " ", 1);
					currentCondition = ParsedCondition(conditionStr);
					break;
				}
				if (keyword == "ELSE") {
					if (currentCondition.lSide == "") { LOG_CRITICAL("ELSE keyword before IF keyword in survival condition config"); std::terminate(); }
					ConditionOp newOp = InvertOp(currentCondition.op);
					currentCondition.op = newOp;
					break;
				}
				if (keyword == "ENDIF") {
					if (currentCondition.lSide == "") { LOG_CRITICAL("ENDIF keyword before IF keyword in survival condition config"); std::terminate(); }
					currentCondition = ParsedCondition();
					break;
				}
			} break;
			case KeywordType::SHAPE:
			{
				if (keyword == "BOX") {
					if (data.size() != 5) { LOG_CRITICAL("Invalid amount of args to BOX keyword in survival condition config ({0} instead of {1})", data.size(), 5); std::terminate(); }
					int l = std::stoi(data[1]);
					int r = std::stoi(data[2]);
					int t = std::stoi(data[3]);
					int b = std::stoi(data[4]);
					Survival::SurvivalRect newRect(l, r, t, b);
					newRect.condition = currentCondition;
					newSurvival.survivalRects.push_back(newRect);
					break;
				}
				if (keyword == "CIRCLE") {
					if (data.size() != 4) { LOG_CRITICAL("Invalid amount of args to CIRCLE keyword in survival condition config ({0} instead of {1})", data.size(), 4); std::terminate(); }
					int x = std::stoi(data[1]);
					int y = std::stoi(data[2]);
					int r = std::stoi(data[3]);
					Survival::SurvivalCircle newCircle(Vector2D(x, y), r);
					newCircle.condition = currentCondition;
					newSurvival.survivalCircs.push_back(newCircle);
					break;
				}
			} break;
			}
		}

		SurvivalConfig = newSurvival;
	}

	void Survival::EvaluateShapes() {
		evaluatedCircs.clear();
		evaluatedRects.clear();

		for (SurvivalCircle& circle : survivalCircs) {
			if (circle.condition.lSide == "") {
				// This shape was not in an if block, always runs
				evaluatedCircs.push_back(true);
				continue;
			}
			evaluatedCircs.push_back(circle.condition.Evaluate());
		}
		for (SurvivalRect& rect : survivalRects) {
			if (rect.condition.lSide == "") {
				// This shape was not in an if block, always runs
				evaluatedRects.push_back(true);
				continue;
			}
			evaluatedRects.push_back(rect.condition.Evaluate());
		}
	}

}