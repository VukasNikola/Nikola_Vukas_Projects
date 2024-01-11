#ifndef S23232146_H
#define S23232146_H

#include "Player.h"
#include <random>
#include <utility>
#include <vector>

namespace s23232146
{

    class PlayerStudent : public Player
    {
    public:
        PlayerStudent();

        std::pair<int, int> getMove() override;
        Board setupBoard() override;
        void logMoveResult(int x, int y, bool result) override;
        void logOpponentMove(int x, int y) override;

    private:
        std::mt19937 gen_;
        std::uniform_int_distribution<int> dist_;
        std::vector<int> availablePositions;
        std::vector<int> hitPositions;
        void createPositions();
        std::vector<int> closeHits;
        void addCloseHit(int pos, int boundary);
        enum Orientation
        {
            Horizontal,
            Vertical
        };
        int check;
        std::vector<int> hitShip;
        int k;
        int pravi;  
    };

}

#endif // S23232146_H
