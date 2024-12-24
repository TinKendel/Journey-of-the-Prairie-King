#include <sstream>

#include "MapLoader.hpp"

std::vector<int> MapLoader::parseCSV(const std::string& csvData)
{
	std::vector<int> tileIDs;
	std::stringstream ss(csvData);
	std::string token;

	while (std::getline(ss, token, ',')) // Split by comma
	{
		tileIDs.push_back(std::stoi(token)); // Convert to integer and store
	}

	return tileIDs;
}

void MapLoader::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;

	// Draw the map layers
	for (size_t i = 0; i < m_layers.size(); ++i)
	{
		if (m_layerVisibility[i]) // Check if the layer is visible
		{
			target.draw(m_layers[i], states);
		}
	}

	// Draw the red overlays for collision areas (debugging)
	/*for (const auto& shape : m_collisionShapes)
	{
		target.draw(shape, states);
	}*/
}

bool MapLoader::load(const std::string& tmxFile, const std::string& tilesetImage, sf::Vector2u tileSize, unsigned int width, unsigned int height)
{
	if (!m_tileset.loadFromFile(tilesetImage))
	{
		return false;
	}

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(tmxFile.c_str()) != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Failed to load map file!" << std::endl;
		return false;
	}

	tinyxml2::XMLElement* map = doc.FirstChildElement("map");

	tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
	
	while (layer)
	{
		std::string layerName = layer->Attribute("name");

		std::string csvData = layer->FirstChildElement("data")->GetText();
		std::vector<int> tileIDs = parseCSV(csvData);

		// Create vertex array for this layer
		sf::VertexArray layerVertices(sf::Quads, width * height * 4);

		for (unsigned int i = 0; i < width; ++i)
		{
			for (unsigned int j = 0; j < height; ++j)
			{
				int tileNumber = tileIDs[i + j * width] - 1; // TMX starts at 1, so adjust to 0-index

				if (tileNumber < 0)
				{
					continue; // Skip empty tiles
				}

				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				sf::Vertex* quad = &layerVertices[(i + j * width) * 4];

				// Set vertex positions
				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				// Set texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}
		}

		m_layers.push_back(layerVertices);
		m_layerVisibility.push_back(true);

		layer = layer->NextSiblingElement("layer");
	}

	return true;
}

void MapLoader::toggleLayerVisibility()
{
	if (!m_layers.empty())
	{
		size_t lastLayerIndex = m_layers.size() - 1;
		m_layerVisibility[lastLayerIndex] = !m_layerVisibility[lastLayerIndex];
	}
}

void MapLoader::wallAnimation()
{
	if (wallClock.getElapsedTime().asMilliseconds() >= 1000)
	{
		toggleLayerVisibility();
		wallClock.restart();
	}
}

bool MapLoader::loadCollision(const std::string& tmxFile)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(tmxFile.c_str()) != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Failed to load collision data from file: " << tmxFile << std::endl;
		return false; // Signal failure
	}

	tinyxml2::XMLElement* objectGroup = doc.FirstChildElement("map")->FirstChildElement("objectgroup");

	while (objectGroup)
	{
		tinyxml2::XMLElement* object = objectGroup->FirstChildElement("object");

		while (object)
		{
			CollisionObject collisionObject;

			collisionObject.rect = sf::FloatRect(
				object->FloatAttribute("x") * GameConfig::MAP_SCALE,
				object->FloatAttribute("y") * GameConfig::MAP_SCALE,
				object->FloatAttribute("width") * GameConfig::MAP_SCALE,
				object->FloatAttribute("height") * GameConfig::MAP_SCALE
			);

			const char* type = object->Attribute("type");
			if (type)
			{
				collisionObject.type = type;
			}

			tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
			if (properties)
			{
				tinyxml2::XMLElement* property = properties->FirstChildElement("property");
				while (property)
				{
					if (std::string(property->Attribute("name")) == "nextArea")
					{
						collisionObject.nextArea = property->BoolAttribute("value");
					}
					property = property->NextSiblingElement("property");
				}
			}

			// Create and configure the red overlay for this collision object
			sf::RectangleShape shape;
			shape.setPosition(collisionObject.rect.left / GameConfig::MAP_SCALE, collisionObject.rect.top / GameConfig::MAP_SCALE);
			shape.setSize({ collisionObject.rect.width / GameConfig::MAP_SCALE, collisionObject.rect.height / GameConfig::MAP_SCALE });
			shape.setFillColor(sf::Color(255, 0, 0, 100)); // Red with transparency
			m_collisionShapes.push_back(shape);

			m_collisionObjects.push_back(collisionObject);
			object = object->NextSiblingElement("object");
		}

		objectGroup = objectGroup->NextSiblingElement("objectgroup");
	}

	std::cout << "Collision data loaded successfully!" << std::endl;
	return true; // Signal success
}


bool MapLoader::checkCollision(const sf::FloatRect& playerBounds) const 
{
	for (const auto& obj : m_collisionObjects) 
	{
		if (playerBounds.intersects(obj.rect)) 
		{
			return true; // Collision detected
		}
	}
	return false; // No collision
}
