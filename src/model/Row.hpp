#pragma once
#include <cstdint>
#include <cstdlib>
#include <vector>
#include "Obstacle.hpp"

constexpr int32_t MAP_WIDTH = 15;
constexpr float OBSTACLE_DISTANCE = 4;
constexpr float OBSTACLE_SIZE_MIN = 2;
constexpr float OBSTACLE_SIZE_MAX = 4;

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
        const float randomSize = static_cast<float>(rand()) / RAND_MAX * (OBSTACLE_SIZE_MAX - OBSTACLE_SIZE_MIN) + OBSTACLE_SIZE_MIN;
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

