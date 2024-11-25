#pragma once

// const float g_xwcMin = -450.0, g_xwcMax = 450.0;  // window size
// const float g_ywcMin = -300.0, g_ywcMax = 300.0;
const int STEP_LENGTH = 10;
enum Direction { N, E, S, W };
struct Position {
    int x, y;
    int dir;
    Position();
    Position(int x, int y, int dir);
    bool operator==(const Position& other) const;
};

class Object
{
public:
    Object();
    Position pos;
};

class Executor : public Object
{
public:
    Executor();
    Executor(int x, int y, int dir);
    Position* GetPositon();
    void GetInstructions(char key);
};
