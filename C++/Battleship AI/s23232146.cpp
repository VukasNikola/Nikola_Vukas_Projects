#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <iostream>
#include <utility>
#include <random>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include "Orientation.h"
#include "../../include/players/s23232146.h"

s23232146::PlayerStudent::PlayerStudent() : Player("32146")
{
    gen_.seed(std::random_device()());

    
    dist_ = std::uniform_int_distribution<int>(0, 99);
    createPositions();
    std::vector<int> closeHits {};
    check = 0;
    
}

void s23232146::PlayerStudent::createPositions()
{
    for (int i = 0; i < 100; i++)
    {
        availablePositions.push_back(i); // Added missing semicolon here
    }
    // shuffle the availablePositions vector
    std::shuffle(availablePositions.begin(), availablePositions.end(), gen_);
    // print the availablePositions vector
    //  for (auto &position : availablePositions)
    //  {
    //      std::cout << position.first.first << " " << position.first.second << " " << position.second << std::endl;
    //  }
}

std::pair<int, int> s23232146::PlayerStudent::getMove()
{
    // if closehits is not empty
    if (!closeHits.empty())
    {
        // get the first element of closehits
        int k = closeHits.front();
        // remove the first element of closehits
        closeHits.erase(closeHits.begin());
        auto it = std::find(availablePositions.begin(), availablePositions.end(), k);

        if (it != availablePositions.end())
        {
            int index = std::distance(availablePositions.begin(), it);
            availablePositions.erase(availablePositions.begin() + index);
            //std::cout << "Removed " << k << " from availablePositions" << std::endl;
        }
        // Create coordinates from z
        int x = k / 10;
        int y = k % 10;
        return std::make_pair(x, y);
    }
    else
    {
        dist_ = std::uniform_int_distribution<int>(0, availablePositions.size() - 1);
        int index = dist_(gen_);
        int z = availablePositions[index];
    
        availablePositions.erase(availablePositions.begin() + index);
        
        int x = z / 10;
        int y = z % 10;
        //if x%y != 0 then recursively call getMove()
        // if (x % y != 0)
        // {
        //     return s23232146::PlayerStudent::getMove();
        // }

        return std::make_pair(x, y);
    }
    // Generate random index from 0 to availablePositions.size() - 1
}



Board s23232146::PlayerStudent::setupBoard()
{
    board_.clearBoard();
    availablePositions.clear();
    hitPositions.clear();
    check = 0;

    createPositions();

    std::vector<int> shipSizes = {2, 3, 3, 4, 5};
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 9); // Assuming a 10x10 board

    for (int shipSize : shipSizes) {
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int x = distribution(generator);
            int y = distribution(generator);
            // Explicitly specify the global namespace for the Orientation enum
            ::Orientation orientation = distribution(generator) % 2 == 0 ? ::Orientation::Horizontal : ::Orientation::Vertical;

            if (board_.isValidPosition(x, y, shipSize, orientation)) {
                placed = board_.placeShip(x, y, shipSize, orientation);
            }
            attempts++;
        }

        if (!placed) {
            std::cerr << "Unable to place all ships, retrying board setup." << std::endl;
            return setupBoard(); // Consider adding a limit to recursive calls
        }
    }
    return board_;
}



void s23232146::PlayerStudent::logMoveResult(int x, int y, bool result)
{
    if (result == true && check==0)
    {
        int check = 1;
        //std::cout << "Hit at (" << x << ", " << y << ")" << std::endl;
        int k = x * 10 + y;
        hitShip.push_back(k);

        
        // Check and add positions intelligently around the hit
        addCloseHit(k + 1, 10); // Right
        addCloseHit(k - 1, -1); // Left
        addCloseHit(k + 10, 100); // Down
        addCloseHit(k - 10, -10); // Up
    }
    else if(result == true && check==1){
        //std::cout << "Hit at (" << x << ", " << y << ")" << std::endl;
        int k = x * 10 + y;
        hitShip.push_back(k);
        //NE ZABORAVI DA POGLEDAS POSLE DA LI IMA VEC U LISTI
        if (hitShip[0]-hitShip[1]==1 || hitShip[0]-hitShip[1]==-1)
        {
            //from the closeHits list remove the Up and Down positions using hitShip[0] as k
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[0] + 10), closeHits.end());
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[0] - 10), closeHits.end());
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[1] + 10), closeHits.end());
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[1] - 10), closeHits.end());

            //same as before, just erase from availablePositions
            auto it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[0] + 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[0] - 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[1] + 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[1] - 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }

            if (hitShip[0]-hitShip[1] == -1)
            {
                addCloseHit(k + 1, 10); // Right
                addCloseHit(k - 2, -1); // Left
            }
            else
            {
                addCloseHit(k + 2, 10); // Right
                addCloseHit(k - 1, -1); // Left
            }
            check=2;
            if(hitShip.empty()==true)
            {
                check = 0;
            }
        }
        else if( hitShip[0]-hitShip[1]==10 || hitShip[0]-hitShip[1]==-10)
        {
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[0] + 1), closeHits.end());
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[0] - 1), closeHits.end());
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[1] + 1), closeHits.end());
            closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[1] - 1), closeHits.end());
            
            auto it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[0] + 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[0] - 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[1] + 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[1] - 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            if(hitShip[0]-hitShip[1] == 10)//probaj da obrnes posle
            {
                addCloseHit(k + 10, 100); // Down
                addCloseHit(k - 20, -10); // Up
            }
            else
            {
                addCloseHit(k + 20, 100); // Down
                addCloseHit(k - 10, -10); // Up
            }
            
            check = 3;
            if(hitShip.empty()==true)
            {
                check = 0;
            }
            
        }
    else if(result == true && check==2){
        int k = x * 10 + y;
        hitShip.push_back(k);
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[2] + 10), closeHits.end());
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[2] - 10), closeHits.end());
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[3] + 10), closeHits.end());
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[3] - 10), closeHits.end());
        auto it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[2] + 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[2] - 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[3] + 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[3] - 10);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
        if (hitShip[2]-hitShip[3] < 0)
        {
            addCloseHit(k + 1, 10); // Right
            addCloseHit(k - 3, -1); // Left
        }
        else
        {
            addCloseHit(k + 3, 10); // Right
            addCloseHit(k - 1, -1); // Left
        }}
        

        check = 0;
        if(hitShip.empty()==true)
            {
                check = 0;
            }
        hitShip.clear();
        }
    // else if(result==true && check == 4){
    //     hitShip.push_back(k);
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[4] + 10), closeHits.end());
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[4] - 10), closeHits.end());
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[5] + 10), closeHits.end());
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[5] - 10), closeHits.end());
    //     auto it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[4] + 10);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
    //         it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[4] - 10);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
    //         it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[5] + 10);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
    //         it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[5] - 10);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
        
    else if(result==true && check == 3){
        hitShip.push_back(k);
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[2] + 1), closeHits.end());
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[2] - 1), closeHits.end());
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[3] + 1), closeHits.end());
        closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[3] - 1), closeHits.end());
        auto it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[2] + 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[2] - 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[3] + 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
            it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[3] - 1);
            if (it != availablePositions.end())
            {
                int index = std::distance(availablePositions.begin(), it);
                availablePositions.erase(availablePositions.begin() + index);
            }
        if(hitShip[2]-hitShip[3] < 0)
        {
            addCloseHit(k + 10, 10); // Right
            addCloseHit(k - 30, -1); // Left
        }
        else
        {
            addCloseHit(k + 30, 10); // Right
            addCloseHit(k - 10, -1); // Left
        }
        check = 0;
        if(hitShip.empty()==true)
            {
                check = 0;
            }
        hitShip.clear();

    }
    // else if(result==true && check == 5){
    //     hitShip.push_back(k);
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[4] + 1), closeHits.end());
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[4] - 1), closeHits.end());
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[5] + 1), closeHits.end());
    //     closeHits.erase(std::remove(closeHits.begin(), closeHits.end(), hitShip[5] - 1), closeHits.end());
    //     auto it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[4] + 1);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
    //         it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[4] - 1);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
    //         it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[5] + 1);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
    //         it = std::find(availablePositions.begin(), availablePositions.end(), hitShip[5] - 1);
    //         if (it != availablePositions.end())
    //         {
    //             int index = std::distance(availablePositions.begin(), it);
    //             availablePositions.erase(availablePositions.begin() + index);
    //         }
        
    // }
}

void s23232146::PlayerStudent::addCloseHit(int pos, int boundary)
{
    if (pos >= 0 && pos < 100 && std::find(closeHits.begin(), closeHits.end(), pos) == closeHits.end())
    {
        if (boundary > 0 && pos % 10 != boundary % 10) closeHits.push_back(pos);
        else if (boundary < 0 && pos % 10 != (boundary + 10) % 10) closeHits.push_back(pos);
    }
}
void s23232146::PlayerStudent::logOpponentMove(int x, int y) {}
