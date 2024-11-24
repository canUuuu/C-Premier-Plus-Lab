#include <iostream>
float xwcMin = -450.0, xwcMax = 450.0;
float ywcMin = -300.0, ywcMax = 300.0;

const int derta = 10;

struct GLTransform;
struct Pos {
    int x, y;
    char dir;
    Pos(int x, int y, char dir)
    {
        this->x = x;
        this->y = y;
        this->dir = dir;
    }

    bool operator==(const Pos &other) const
    {
        return x == other.x && y == other.y && dir == other.dir;
    }
};

class Object;

// GLObject 派生类
class Executor;  // 点

struct GLTransform {
    // 世界坐标，轴旋转，轴缩放
    int x = 0, y = 0, z = 0;
};

class Object
{
public:
    char dir;
    Object()
    {
        dir = 'N';
    }

    GLTransform transform;
};

class Executor : public Object
{
public:
    Executor()
    {
        dir = 'N';
        transform.x = 0;
        transform.y = 0;
    }
    Executor(int x, int y, char d)
    {
        dir = d;
        transform.x = x;
        transform.y = y;
    }

    // GLPoint* ctrlPoints[MAX_BEZIER_CONTROL_POINTS_NUM];
    void KeyboardMove(int right, int up)
    {
        if (transform.x + right > xwcMax || transform.x + right < xwcMin) {
            if (transform.x + right > xwcMax)
                transform.x = right - xwcMax;
            else
                transform.x = xwcMax + right;
            return;
        }
        transform.x += right;
        if (transform.y + up > ywcMax || transform.y + up < ywcMin) {
            if (transform.y + up > ywcMax)
                transform.y = up - ywcMax;
            else
                transform.y = up + ywcMax;
            return;
        }
        transform.y += up;
    }
    Pos *GetPos()
    {
        return new Pos(transform.x, transform.y, dir);
    }
    void GetInst(char key)
    {
        switch (key) {
        case 'm':
            switch (dir) {
            case 'N':
                KeyboardMove(0, derta);
                break;
            case 'E':
                KeyboardMove(derta, 0);
                break;
            case 'S':
                KeyboardMove(0, -derta);
                break;
            case 'W':
                KeyboardMove(-derta, 0);
                break;
            default:
                break;
            }
            break;
        case 'r':
            switch (dir) {
            case 'N':
                dir = 'E';
                break;
            case 'E':
                dir = 'S';
                break;
            case 'S':
                dir = 'W';
                break;
            case 'W':
                dir = 'N';
                break;
            default:
                break;
            }
            break;
        case 'l':
            switch (dir) {
            case 'N':
                dir = 'W';
                break;
            case 'E':
                dir = 'N';
                break;
            case 'S':
                dir = 'E';
                break;
            case 'W':
                dir = 'S';
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
};
