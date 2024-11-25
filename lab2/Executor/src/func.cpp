#include "func.h"

#include <iostream>
#include <vector>

Position::Position()
{
    this->x = 0;
    this->y = 0;
    this->dir = N;
}
Position::Position(int x, int y, int dir)
{
    this->x = x;
    this->y = y;
    this->dir = dir % 4;
}

bool Position::operator==(const Position& other) const
{
    return x == other.x && y == other.y && dir == other.dir;
}

Object::Object() {};

Executor::Executor() {};
Executor::Executor(int x, int y, int dir)
{
    pos.x = x;
    pos.y = y;
    pos.dir = dir;
}
Position* Executor::GetPositon()
{
    return new Position(pos.x, pos.y, pos.dir);
}

void Executor::GetInstructions(char key)
{
    switch (key) {
    case 'm':
        switch (pos.dir) {
        case N:
            pos.y += STEP_LENGTH;
            break;
        case S:
            pos.y -= STEP_LENGTH;
            break;
        case W:
            pos.x -= STEP_LENGTH;
            break;
        case E:
            pos.x += STEP_LENGTH;
            break;
        default:
            break;
        }

        break;
    case 'r':
        pos.dir = (pos.dir + 1) % 4;
        break;
    case 'l':
        pos.dir = (pos.dir + 3) % 4;
    default:
        break;
    }
}
