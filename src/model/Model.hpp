#pragma once
#include <algorithm>
#include <ostream>
#include <vector>
#include "Row.hpp"
#include "Input.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Err.hpp>

using namespace sf;
using namespace std;

class CrossyRoadModel
{
    bool playerAlive = true;
    Vector2f playerPosition;
    vector<Row> rows;
    int32_t lastRowIndex = 0;
    uint32_t playerScore = 0;

    void AddRow(const RowType type)
    {
        const float speed = type != RowType::Grass ? lastRowIndex / 20.f + 0.2f + (rand() % 15 / 10.f) : 0.f;
        rows.emplace_back();
        rows.back().Setup(type, (rand() % 2 ? 1.f : -1.f) * speed);
        lastRowIndex++;
    }
    void RemoveRow()
    {
        rows.erase(rows.begin());
    }
public:
    bool isPlayerAlive() const
    {
        return playerAlive;
    }
    uint32_t getPlayerScore() const
    {
        return playerScore;
    }
    Vector2f getPlayerPosition() const
    {
        return playerPosition;
    }
    const Row& getRow(const int32_t index) const
    {
        return rows.at(index);
    }
    int32_t getRowCount() const
    {
        return rows.size();
    }
    int32_t getLastRowIndex() const
    {
        return lastRowIndex;
    }
    int32_t getPlayerRowIndex() const
    {
        return static_cast<int32_t>(rows.size()) - (playerPosition.y + lastRowIndex);
    }
    void Start()
    {
        playerPosition.x = MAP_WIDTH / 2;
        AddRow(RowType::Grass);
        AddRow(RowType::Grass);
        for (int32_t i = 0; i < 10; i++)
        {
            const RowType type = static_cast<RowType>(rand() % static_cast<int32_t>(RowType::Count));
            AddRow(type);
        }

        //Make some time pass for obstacles to spawn
        for (int32_t i = 0; i < 100; i++)
            Update(0.1f, Input::None);
    }
    bool CheckCollision(const bool testWhole)
    {
        const int32_t rowID = static_cast<int32_t>(rows.size()) - (playerPosition.y + lastRowIndex);
        const Row& playerRow = rows.at(rowID);
        for (int32_t i = 0; i < playerRow.getObstacleCount(); i++)
        {
            const Obstacle& obstacle = playerRow.getObstacle(i);
            if (!testWhole && playerPosition.x + 0.8f > obstacle.positionX && playerPosition.x + 0.2f <= obstacle.positionX + obstacle.sizeX)
                return true;
            if (testWhole && playerPosition.x + 0.4f > obstacle.positionX && playerPosition.x + 0.6f <= obstacle.positionX + obstacle.sizeX)
                return true;
        }
        return false;
    }
    void Update(const float deltaTime, const Input action)
    {
        Vector2f playerMove;
        switch (action)
        {
        case Input::Left:
            if (playerPosition.x > 0)
                playerMove = Vector2f(-0.5f, 0);
            break;
        case Input::Right:
            if (playerPosition.x < MAP_WIDTH - 1)
                playerMove = Vector2f(0.5f, 0);
            break;
        case Input::Up:
            playerMove = Vector2f(0, -1);
            break;
        case Input::Down:
            if (playerPosition.y < 0 && playerPosition.y + lastRowIndex < rows.size())
                playerMove = Vector2f(0, 1);
            break;
        default:
            break;
        }
        playerPosition += playerMove;
        if (std::abs(playerPosition.y) > playerScore)
            playerScore = std::abs(playerPosition.y);

        switch (rows.at(getPlayerRowIndex()).getType())
        {
        case RowType::Grass:
            if (CheckCollision(false))
                playerPosition -= playerMove;
            break;
        case RowType::River:
            if (!CheckCollision(true))
                playerAlive = false;
            else
                playerPosition.x += rows.at(getPlayerRowIndex()).getSpeed() * deltaTime;
            break;
        case RowType::Road:
            if (CheckCollision(false))
                playerAlive = false;
        default:
            break;
        }
        if (playerPosition.x + 0.2f < 0 || playerPosition.x + 0.8f > MAP_WIDTH)
            playerAlive = false;

        const int32_t rowID = static_cast<int32_t>(rows.size()) - (playerPosition.y + lastRowIndex);
        if (rowID > 5)
            RemoveRow();
        if (rowID > static_cast<int32_t>(rows.size()) - 15)
            AddRow(static_cast<RowType>(rand() % static_cast<int32_t>(RowType::Count)));

        for (Row& n : rows)
            n.Update(deltaTime);
    }

};

