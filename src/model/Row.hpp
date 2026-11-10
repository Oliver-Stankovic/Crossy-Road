#pragma once
#include <cstdint>
#include <cstdlib>
#include <vector>
#include "Obstacle.hpp"

constexpr int32_t MAP_WIDTH = 15;
constexpr float OBSTACLE_DISTANCE = 4;
constexpr float LOG_SIZE_MIN = 2;
constexpr float LOG_SIZE_MAX = 4;

using namespace std;
enum class RowType
{
    Grass,
    Road,
    River,
    Count
};

class Row
{
    RowType type = RowType::Grass;
    float speed = 0;

    vector<Obstacle> obstacles;

    void AddObstacle()
    {
        float randomSize = 0;
        switch (type)
        {
        case RowType::Grass:
            randomSize = 1;
            break;
        case RowType::Road:
            randomSize = static_cast<float>(rand()) / RAND_MAX;
            if (randomSize < 0.6f)
                randomSize = 1;
            else if (randomSize < 0.8f)
                randomSize = 2;
            else
                randomSize = 3;
            break;
        case RowType::River:
            randomSize = static_cast<float>(rand()) / RAND_MAX * (LOG_SIZE_MAX - LOG_SIZE_MIN) + LOG_SIZE_MIN;
            break;
        default:
            break;
        }
        obstacles.emplace_back();
        if (speed > 0)
            obstacles.back().positionX = -randomSize;
        else if (speed < 0)
            obstacles.back().positionX = MAP_WIDTH;
        obstacles.back().sizeX = randomSize;
    }

public:
    RowType getType() const
    {
        return type;
    }
    float getSpeed() const
    {
        return speed;
    }
    int32_t getObstacleCount() const
    {
        return obstacles.size();
    }
    const Obstacle& getObstacle(const int32_t index) const
    {
        return obstacles.at(index);
    }

    void Setup(const RowType type, const float speed)
    {
        this->type = type;
        this->speed = speed;

        if (type == RowType::Grass)
        {
            obstacles.emplace_back(); // Left edge
            obstacles.back().positionX = 0;
            obstacles.back().sizeX = 1;

            obstacles.emplace_back(); // Right edge
            obstacles.back().positionX = MAP_WIDTH - 1;
            obstacles.back().sizeX = 1;

            obstacles.emplace_back(); // One random
            obstacles.back().positionX = rand() % (MAP_WIDTH - 2) + 1;
            obstacles.back().sizeX = 1;
        }
    }

    void Update(const float deltaTime)
    {
        if (obstacles.empty() ||
            speed > 0 && obstacles.back().positionX > OBSTACLE_DISTANCE ||
            speed < 0 && obstacles.back().positionX + obstacles.back().sizeX < MAP_WIDTH - OBSTACLE_DISTANCE)
        {
            AddObstacle();
        }
        for (int32_t i = 0; i < obstacles.size(); i++)
        {
            obstacles.at(i).positionX += speed * deltaTime;
            if (speed > 0 && obstacles.at(i).positionX > MAP_WIDTH ||
                speed < 0 && obstacles.at(i).positionX + obstacles.at(i).sizeX < 0)
            {
                obstacles.erase(obstacles.begin() + i);
                i--;
            }
        }
    }
};

