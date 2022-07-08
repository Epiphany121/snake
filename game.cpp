#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

#include "game.h"

Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "This is a mock version.");
    mvwprintw(this->mWindows[0], 3, 1, "Please fill in the blanks to make it work properly!!");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

int Game::renderPauseMenu() const
{
        WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Continue","Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "PAUSE");
   // std::string pointString = std::to_string(this->mPoints);
   // mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, 1, menuItems[2].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return 1;
    }
    else if (index == 1)
    {
        return 2;
    }
    else
        return 3;
}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::initializeGame()
{
    // allocate memory for a new snake
		this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
		this->mPtrMap.reset(new Map(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialObstacleNum, this->mInitialPowerPathLength));

    /* TODO
     * initialize the game pionts as zero
     * create a food at randome place
     * make the snake aware of the food
     * other initializations
     */
     this->mPoints = 0;
     this->createRandomFood();
     this->renderFood();
   //  int x = rand()%(this->mGameBoardWidth-1) + 1;
    // int y = rand()%(this->mGameBoardHeight-1) + 1;
    // SnakeBody food(x, y);
    // this->mFood = food;
   //  mvwaddch(this->mWindows[1], y, x, this->mFoodSymbol);
    // wrefresh(this->mWindows[1]);
}

void Game::createRandomFood()
{
/* TODO
 * create a food at random places
 * make sure that the food doesn't overlap with the snake.
 */
    int x, y, condition = 1;
    while (condition) {
        x = rand()%(this->mGameBoardWidth-1) + 1;
        y = rand()%(this->mGameBoardHeight-2) + 1;
        std::vector<SnakeBody>& s = this->mPtrSnake->getSnake();
        std::vector<SnakeBody>& o = this->mPtrMap->getObstacle();
        for (int i = 0; i < s.size(); i++){
            int sx = (s[i]).getX();
            int sy = (s[i]).getY();
            if (x == sx && y == sy) {
                condition = 2;
                break;
            }
        }
        for (int i = 0; i < o.size(); i++) {
            int ox = (o[i]).getX();
            int oy = (o[i]).getY();
            if (x == ox && y == oy) {
                condition = 2;
                break;
            }
        }
        if (condition == 2) {
            condition = 1;
            continue;
        }
        else {
            break;
        }
    }
    SnakeBody food(x, y);
    mPtrSnake->senseFood(food);
    this->mFood = food;
   // mvwaddch(this->mWindows[1], y, x, this->mFoodSymbol);
   // wrefresh(this->mWindows[1]);
}

void Game::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
}

void Game::createRandomPowerPath()
{
    return;
}

void Game::renderObstacle() const
{
    int length = this->mPtrMap->getObstacleNum();
    std::vector<SnakeBody>& obstacle = this->mPtrMap->getObstacle();
    for (int i = 0; i < length; i ++)
    {
        mvwaddch(this->mWindows[1], obstacle[i].getY(), obstacle[i].getX(), this->mObstacleSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Game::controlSnake(int key) const
{
    //int key;
   // key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
				    // TODO change the direction of the snake.

            this->mPtrSnake->changeDirection(Direction::Up);


            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
				    // TODO change the direction of the snake.

            this->mPtrSnake->changeDirection(Direction::Down);


            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
				    // TODO change the direction of the snake.

            this->mPtrSnake->changeDirection(Direction::Left);


            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
				    // TODO change the direction of the snake.

            this->mPtrSnake->changeDirection(Direction::Right);


            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

int Game::runGame()
{
    int moveCondition;
    int keyOne, keyTwo;
    int condition;
    while (true)
    {
				/* TODO
				 * this is the main control loop of the game.
				 * it keeps running a while loop, and does the following things:
				 * 	1. process your keyboard input
				 * 	2. clear the window
				 * 	3. move the current snake forward
				 * 	4. check if the snake has eaten the food after movement
				 * 	5. check if the snake dies after the movement
				 * 	6. make corresponding steps for the ``if conditions'' in 3 and 4.
				 *   7. render the position of the food and snake in the new frame of window.
				 *   8. update other game states and refresh the window
				 */
        this->adjustDelay();


        //clear();

        keyOne = getch();
       // keyTwo = getch();
        if (keyOne == 'p' || keyOne == 'P') {
            condition = renderPauseMenu();
            if (condition == 1) {
                continue;
            }
            else if (condition == 2) {
                return 2;
           }
           else
                return 3;
        }
        this->controlSnake(keyOne);
        for (int i = 0; i < this->mPtrMap->getObstacle().size(); i++){
                this->mPtrSnake->senseObstacle(this->mPtrMap->getObstacle()[i]);
        }

       // clear();
       // this->renderBoards();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);
        moveCondition = this->mPtrSnake->moveFoward(keyOne);
        if (moveCondition == 0){
            this->createRandomFood();
            this->mPoints += 1;
        }
        this->renderSnake();
        this->renderFood();
        this->renderObstacle();


        if (this->mPtrSnake->checkDeath(keyOne))
            break;





        this->renderPoints();
        this->renderDifficulty();


        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
    this->renderBoards();
}

void Game::startGame()
{
    refresh();
    bool choice;
    int condition;
    while (true)
    {
        this->readLeaderBoard();
        this->renderBoards();
        this->initializeGame();
        condition = this->runGame();
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        if (condition == 2){
            continue;
        }
        else if (condition == 3){
            break;
        }
        choice = this->renderRestartMenu();
        if (choice == false)
        {
            break;
        }
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}
