#include <string>
#include <cstdlib>
#include <vector>

#include <iostream>
#include "map.h"

using namespace std;


Map::Map(int gameBoardWidth, int gameBoardHeight, int initialObstacleNum, int initialPowerPathLength)
        : mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialObstacleNum(initialObstacleNum), mInitialPowerPathLength(initialPowerPathLength)
{
    this->initializeMap();
}


void Map::initializeMap()
{
    int centerX = this->mGameBoardWidth/2 - this->mInitialObstacleNum/2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialObstacleNum; i ++)
    {
        this->obstacle.push_back(SnakeBody(centerX + i, centerY));
    }
}

vector<SnakeBody>& Map::getObstacle()
{
    return this->obstacle;
}


int Map::getObstacleNum()
{
    return this->obstacle.size();
}


vector<SnakeBody>& Map::getPowerPath()
{
    return this->powerPath;
}

int Map::getPowerPathLength()
{
    return this->powerPath.size();
}

void Map::setPowerPath(vector<SnakeBody> pp)
{
    this->powerPath = pp;
}
