#include "Visualise.h"


namespace Visualisation {

	const float  PI_F = 3.14159265358979f;

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

	Edges FormMapEdges(std::string mapfile) {
		std::ifstream f(mapfile);

		nodesoup::adj_list_t edges;

		std::string line;
		std::vector<VisualNode> nodes;
		int i = 0;
		while (std::getline(f, line)) {
			auto data = split(line, ' ');
			VisualNode newNode = VisualNode();
			newNode.type = data[0];
			newNode.name = data[1];
			newNode.id = i;

			if (data.size() > 2) {
				std::string rawCons = data[2];
				auto connections = split(rawCons, '{');
				for (std::string con : connections) {
					if (con == "") { continue; }
					con = con.substr(0, con.length() - 1);
					auto conData = split(con, '@');
					int ref = std::stoi(conData[0]);
					int weight = std::stoi(conData[1]);
					newNode.connections.push_back(Connection(ref, weight));
				}
			}
			nodes.push_back(newNode);
			edges.resize(edges.size() + 1);
			i++;
		}

		for (VisualNode& node : nodes) {
			for (Connection con : node.connections) {
				VisualNode& refNode = nodes[std::get<0>(con)];
				refNode.forwardCons.push_back(Connection(node.id, std::get<1>(con))); // Give that node a forward reference to this
				edges[node.id].push_back(refNode.id);
				edges[refNode.id].push_back(node.id);
			}
		}

		return Edges(edges, nodes);
	}

	sf::Vector2f operator*(const float& a, const sf::Vector2f& b) {
		return sf::Vector2f(a * b.x, a * b.y);
	}

	sf::Vector2f operator/(const sf::Vector2f& a, const float& b) {
		return sf::Vector2f(a.x/b, a.y/b);
	}

	std::vector<sf::Vector2f> CurvedLine(sf::RenderTexture& texture, float width, sf::Color col, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
		// P = ((1−t)^2 * P1) + (2 * (1−t) * t * P2) + (t^2 * P3)
		std::vector<sf::Vector2f> points;
		for (float t = 0; t < 1.0f; t += 0.01) {
			sf::Vector2f newPoint;
			newPoint = ((pow(1 - t, 2)) * a) + (2 * (1 - t) * t * b) + (pow(t, 2) * c);
			points.push_back(newPoint);
		}

		for (int p = 1; p < points.size(); p++) {
			sf::VertexArray line(sf::TriangleStrip, 4);

			sf::Vector2f dir = (points[p] - points[p-1]).normalized();
			sf::Vector2f widthDir = sf::Vector2f(dir.y, -dir.x); // Clockwise perpendicular

			line[0].position = points[p - 1] + (widthDir * std::abs(width / 2));
			line[1].position = points[p - 1] - (widthDir * std::abs(width / 2));
			line[2].position = points[p] + (widthDir * std::abs(width / 2));
			line[3].position = points[p] - (widthDir * std::abs(width / 2));

			line[0].color = col;
			line[1].color = col;
			line[2].color = col;
			line[3].color = col;

			texture.draw(line);
		}

		return points;
	}

	void ArrowHead(sf::RenderTexture& texture, float width, float height, sf::Color col, sf::Vector2f headPoint, sf::Vector2f backTarget) {
		auto arrowBack = headPoint + ((backTarget - headPoint).normalized() * height);
		auto dir = (headPoint - arrowBack).normalized();
		auto perp = sf::Vector2f(dir.y, -dir.x);

		sf::VertexArray arrow(sf::Triangles, 6);

		auto leftP = arrowBack + (perp * width);
		auto rightP = arrowBack - (perp * width);

		arrow[0].position = headPoint;
		arrow[1].position = arrowBack;
		arrow[2].position = leftP;
		arrow[3].position = headPoint;
		arrow[4].position = arrowBack;
		arrow[5].position = rightP;

		for (int v = 0; v < arrow.getVertexCount(); v++) {
			arrow[v].color = col;
		}

		texture.draw(arrow);
	}

	sf::Vector2f UnitCircleVec(float angleD) {
		//x = cx + r * cos(a)
		//y = cy + r * sin(a)
		float x = std::cos(angleD * (PI_F / 180));
		float y = std::sin(angleD * (PI_F / 180));
		return sf::Vector2f(x, y);
	}

	sf::Vector2f CircleIntersection(sf::Vector2f pos0, float r0, sf::Vector2f pos1, float r1) {
		float d = (pos1 - pos0).length();

		// Check circles actually intersect
		assert(d <= r0 + r1); // Circles are seperate if false
		assert(d >= std::abs(r0 - r1)); // Circles are inside eachother if false
		assert(!(d == 0 && r0 == r1));

		//http://paulbourke.net/geometry/circlesphere/
		//a = (r02 - r12 + d2 ) / (2 d) 
		//P2 = P0 + a ( P1 - P0 ) / d 
		float a = (pow(r0, 2) - pow(r1, 2) + pow(d, 2)) / (2 * d);
		sf::Vector2f pos2 = pos0 + a * (pos1 - pos0) / d; // Intersection point inbetween the circles

		//h2 = r02 - a2
		float h = std::sqrt(pow(r0, 2) - pow(a, 2));

		//x3 = x2 +- h ( y1 - y0 ) / d 
		//y3 = y2 -+ h ( x1 - x0 ) / d 
		float x = pos2.x + h * (pos1.y - pos0.y) / d;
		float y = pos2.y - h * (pos1.x - pos0.x) / d;
		return sf::Vector2f(x, y);
	}

	sf::Vector2f VectorLerp(sf::Vector2f a, sf::Vector2f b, float t) {
		//return (1 - t) * v0 + t * v1;
		return (1 - t) * a + t * b;
	}

	void RenderNeuralMap(sf::RenderTexture& texture, Edges edges) {
		auto g = std::get<0>(edges);
		auto nodes = std::get<1>(edges);

		nodesoup::iter_callback_t cback = nullptr;
		std::vector<nodesoup::Point2D> positions;

		sf::Font font;
		assert(font.loadFromFile("resources/arial.ttf"));

		double k = 400;
		double iters = 10000;

		std::vector<double> radii = nodesoup::size_radiuses(g, 20.0, k);

		positions = nodesoup::fruchterman_reingold(g, texture.getSize().x, texture.getSize().y, iters, k, cback);
		sf::Vector2f total;
		for (auto& pos : positions) {
			total.x += pos.x;
			total.y += pos.y;
		}
		sf::Vector2f center = total / (float)positions.size();
		sf::Vector2 offset = (sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2) - center);
		for (int p = 0; p < positions.size(); p++) {
			nodesoup::Point2D newPos(positions[p].x + offset.x, positions[p].y + offset.y);
			positions[p] = newPos;
		}

		texture.clear(sf::Color::White);

		std::vector<sf::CircleShape> nodesToRender;
		std::vector<sf::Text> textToRender;

		for (nodesoup::vertex_id_t nodeId = 0; nodeId < g.size(); nodeId++) {
			if (nodes[nodeId].connections.size() == 0 && nodes[nodeId].forwardCons.size() == 0) { continue; }

			float noderadius = (float)radii[nodeId];
			sf::CircleShape nodeCirc(noderadius);

			sf::Color nodeCol;
			std::string type = nodes[nodeId].type;
			if (type == "r") { nodeCol = sf::Color::Color(52, 235, 204); }
			if (type == "i") { nodeCol = sf::Color::Color(176, 176, 176); }
			if (type == "e") { nodeCol = sf::Color::Color(255, 120, 196); }

			nodeCirc.setFillColor(nodeCol);
			nodeCirc.setOutlineThickness(1);
			nodeCirc.setOutlineColor(sf::Color::Black);
			nodeCirc.setPosition(sf::Vector2f(positions[nodeId].x, positions[nodeId].y) - sf::Vector2f(noderadius, noderadius));

			nodesToRender.push_back(nodeCirc);

			sf::Text text;
			text.setFont(font);
			text.setString(nodes[nodeId].name);
			text.setCharacterSize(noderadius*0.75f); // pixels

			//CENTER
			size_t CharacterSize = text.getCharacterSize();
			size_t MaxHeight = 0;

			for (size_t x = 0; x < nodes[nodeId].name.size(); ++x)
			{
				sf::Uint32 Character = nodes[nodeId].name.at(x);

				const sf::Glyph& CurrentGlyph = font.getGlyph(Character, CharacterSize, false);

				size_t Height = CurrentGlyph.bounds.height;

				if (MaxHeight < Height)
					MaxHeight = Height;
			}

			sf::FloatRect rect = text.getLocalBounds();

			rect.left = (positions[nodeId].x) - (rect.width / 2.0f);
			rect.top = (positions[nodeId].y) - (MaxHeight / 2.0f) - (rect.height - MaxHeight) + ((rect.height - CharacterSize) / 2.0f);

			text.setPosition(sf::Vector2f(rect.left, rect.top));
			text.setFillColor(sf::Color::Black);

			textToRender.push_back(text);

			int selfCons = 0;
			auto& cons = nodes[nodeId].forwardCons;
			for (auto& con : cons) {
				int refId = std::get<0>(con);
				int weightI = std::get<1>(con);

				if (refId == nodeId) {
					// SELF REFERENCE; NEED TO DO CIRCLE
					sf::Vector2f orig = sf::Vector2f(positions[nodeId].x, positions[nodeId].y);
					float radius = (float)radii[nodeId] + 5.0f;

					auto circleDir = UnitCircleVec((selfCons*40)-70);
					sf::Vector2f center = orig + (circleDir * radius);
					sf::CircleShape refCircle(radius);

					float weightF = weightI / 8192.0f;
					float width = 6 + 0.7 * weightF;
					width *= 2;

					refCircle.setFillColor(sf::Color::Transparent);
					refCircle.setOutlineThickness(width / 2);
					sf::Color lineCol = weightF > 0 ? sf::Color::Green : sf::Color::Red;
					refCircle.setOutlineColor(lineCol);
					refCircle.setPosition(center - sf::Vector2f(radius, radius));

					texture.draw(refCircle);

					sf::Vector2f arrowPoint = CircleIntersection(sf::Vector2f(positions[nodeId].x, positions[nodeId].y), noderadius, center, radius);
					sf::Vector2f arrowBaseRef = arrowPoint + VectorLerp((arrowPoint - sf::Vector2f(positions[nodeId].x, positions[nodeId].y)), (center - sf::Vector2f(positions[nodeId].x, positions[nodeId].y)), 0.45f);
					arrowPoint += ((arrowPoint - sf::Vector2f(positions[nodeId].x, positions[nodeId].y)) - (center - sf::Vector2f(positions[nodeId].x, positions[nodeId].y))) * 0.1f;
					ArrowHead(texture, 10.0f, 15.0f, lineCol, arrowPoint, arrowBaseRef);

					selfCons++;
					continue;
				}

				sf::Vector2f pos1 = sf::Vector2f(positions[nodeId].x, positions[nodeId].y);
				sf::Vector2f pos2 = sf::Vector2f(positions[refId].x, positions[refId].y);


				auto dir = (pos2 - pos1).normalized();
				auto mid = (pos1 + pos2) / 2;

				float bendStrength = 60.0f;
				auto bendDir = sf::Vector2f(dir.y, -dir.x); // Clockwise Perpendicular
				sf::Vector2f bendPos = mid + (bendDir * bendStrength);

				float weightF = weightI / 8192.0f;
				float width = 6 + (0.7 * weightF);
				sf::Color lineCol = weightF > 0 ? sf::Color::Green : sf::Color::Red;

				std::vector<sf::Vector2f> curvedPoints = CurvedLine(texture, width, lineCol, pos1, bendPos, pos2);

				float refRadius = (float)radii[refId];
				sf::Vector2f closestP;
				int closestI = -1;
				float closestD = 10000000.0f;
				int i = 0;
				for (sf::Vector2f p : curvedPoints) {
					auto newR = (pos2 - p).length();
					float off = std::abs(newR - refRadius);
					if (off < closestD) {
						closestD = off;
						closestP = p;
						closestI = i;
					}
					i++;
				}

				ArrowHead(texture, 10.0f, 20.0f, lineCol, closestP, curvedPoints[closestI - 10]);
			}
		}

		for (auto& node : nodesToRender) {
			texture.draw(node);
		}

		for (auto& text : textToRender) {
			texture.draw(text);
		}

		texture.display();
	}

	void VisualiseNeuralMap(sf::RenderTexture& texture, std::string file) {
		if (!std::filesystem::exists(file)) { LOG_CRITICAL("Could not open the neural map file"); return; }

		Edges edges = FormMapEdges(file);

		std::stringstream windowTss;
		windowTss << "Neural Map Visualisation: " << file;
		std::string windowT(windowTss.str());

		sf::RenderWindow window(sf::VideoMode(texture.getSize().x, texture.getSize().y), windowT);

		while (window.isOpen())
		{
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			RenderNeuralMap(texture, edges);
			const sf::Texture& dt = texture.getTexture();
			sf::Sprite sprite(dt);
			window.draw(sprite);
			window.display();
		}
	}

}