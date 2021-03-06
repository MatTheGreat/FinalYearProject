#include "MapDisplay.h"

MapDisplay::MapDisplay()
{
	whiteTile.loadFromFile("Tile.png");
	greenTile.loadFromFile("TileGreen.png");
	orangeTile.loadFromFile("TileOrange.png");
	redTile.loadFromFile("TileRed.png");
	yellowTile.loadFromFile("TileYellow.png");
	greyTile.loadFromFile("TileGrey.png");

	for (int r = 0; r< 50; r++)
	{
		for (int c = 0; c < 50; c++)
		{
			sf::Sprite * sp = new sf::Sprite();
			sp->setTexture(whiteTile);
			sp->setPosition((r * 16), (c * 16));
			tiles.push_back(sp);
		}
	}
	font = new sf::Font();
	font->loadFromFile("arial.ttf");
	algorithimInfoTxt = sf::Text("",*font);
	algorithimInfoTxt.setFillColor(sf::Color::Red);
	algorithimInfoTxt.setStyle(sf::Text::Bold);
	algorithimInfoTxt.setCharacterSize(20);
	algorithimInfoTxt.setPosition(600, 10);
}

MapDisplay::~MapDisplay()
{

}

void MapDisplay::Render(sf::RenderWindow * window)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		window->draw(*tiles.at(i));
	}
	window->draw(algorithimInfoTxt);
}

void MapDisplay::ChangeTile(int index ,std::string tileType)
{
	if (tileType == "Green")
	{
		tiles.at(index)->setTexture(greenTile);
	}
	else if (tileType == "Red")
	{
		tiles.at(index)->setTexture(redTile);
	}
	else if (tileType == "Orange")
	{
		tiles.at(index)->setTexture(orangeTile);
	}
	else if (tileType == "Yellow")
	{
		tiles.at(index)->setTexture(yellowTile);
	}
	else if (tileType == "Grey")
	{
		tiles.at(index)->setTexture(greyTile);
	}
	else if (tileType == "White")
	{
		tiles.at(index)->setTexture(whiteTile);
	}
}

void MapDisplay::UpdateText(std::string algorithim, std::string heuristic)
{
	algorithimInfoTxt.setString(algorithim + " " + heuristic);
}

std::vector<sf::Sprite*>* MapDisplay::getTiles()
{
	return & tiles;
}



