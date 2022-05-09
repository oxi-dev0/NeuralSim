#pragma once

#include <sstream>
#include <locale>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../Tools/Random.h"
#include "../Tools/Log.h"

#include <SFML/Graphics.hpp>
#include <nodesoup.hpp>

#include "../Classes/Utils.h"

namespace Visualisation {

	typedef std::tuple<int, int> Connection;

	struct VisualNode {
	public:
		int id;
		std::string type;
		std::string name;
		std::vector<Connection> connections;
		std::vector<Connection> forwardCons;

		std::vector<float> randomAngles;

		VisualNode() { type = ""; name = ""; connections = std::vector<Connection>(); }
		VisualNode(std::string type_, std::string name_) { type = type_; name = name_; connections = std::vector<Connection>(); }
	};
	
	typedef std::tuple<nodesoup::adj_list_t, std::vector<VisualNode>> Edges;

	extern void VisualiseNeuralMap(sf::RenderTexture& texture, std::string file);

}

