#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class MapDisplay
{
public:
	MapDisplay();
	~MapDisplay();

	void Render(sf::RenderWindow* window);
	void ChangeTile(int index, std::string tileType);
	void UpdateText(std::string algorithim, std::string heuristic);
	std::vector<sf::Sprite*>* getTiles();

private:
	std::vector<sf::Sprite*> tiles;
	sf::Texture whiteTile;
	sf::Texture redTile;
	sf::Texture orangeTile;
	sf::Texture greenTile;
	sf::Texture yellowTile;
	sf::Texture greyTile;
	sf::Text algorithimInfoTxt;
	sf::Font* font;
	
};