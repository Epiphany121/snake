#ifndef GAME_H
#define GAME_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>

#include "snake.h"
#include "map.h"


class Game
{
public:
    Game();
    ~Game();

		void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

		void createInstructionBoard();
    void renderInstructionBoard() const;

		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

		void renderBoards() const;

		void initializeGame();
    int runGame();
    void renderPoints() const;
    void renderDifficulty() const;

		void createRandomFood();
    void renderFood() const;

    void renderObstacle() const;
    void renderSnake() const;
    void controlSnake(int key) const;

    void createRandomPowerPath();

		void startGame();
    bool renderRestartMenu() const;
    int renderPauseMenu() const;
    void adjustDelay();


private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;

    // Food information
    SnakeBody mFood;
    const char mFoodSymbol = '#';

    const int mInitialObstacleNum = 10;
    const char mObstacleSymbol = '!';
    const int mInitialPowerPathLength = 10;
    const char mPowerPathSymbol = '*';
    std::unique_ptr<Map> mPtrMap;


    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
   // int mBaseDelay = 200;
    int mDelay;
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
};

#endif

