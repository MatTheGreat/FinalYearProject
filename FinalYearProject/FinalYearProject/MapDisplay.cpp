#include "MapDisplay.h"

MapDisplay::MapDisplay()
{
	whiteTile.loadFromFile("Tile.png");
	greenTile.loadFromFile("TileGreen.png");
	orangeTile.loadFromFile("TileOrange.png");
	redTile.loadFromFile("TileRed.png");
	yellowTile.loadFromFile("TileYellow.png");

	for (int c = 0; c < 16; c++)
	{
		for (int r = 0; r < 16; r++)
		{
			sf::Sprite * sp = new sf::Sprite();
			sp->setTexture(whiteTile);
			sp->setPosition((r * 48), (c * 48));
			tiles.push_back(sp);
		}
	}
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
	else if (tileType == "White")
	{
		tiles.at(index)->setTexture(whiteTile);
	}
}



