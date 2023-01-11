#ifndef GO_DEMO_STONE_H
#define GO_DEMO_STONE_H

#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QThread> 
#include <stdio.h>
#include "C:/Users/29753/Desktop/Projects/Games/src/Go/Go.h"
#include "BoardClass.h"
#include "ThreadClass.h"

class StoneClass : public QWidget
{
public:
    StoneClass(QWidget* parent);

protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* e);//������¼�

private:
    QWidget* w;

    const int stoneSize = BoardClass::gridSize * 0.8;
    int ai_is_open = 0;

    QLabel* Stone[361];
    QLabel* StoneWarn = new QLabel(this);
    QLabel* WinLable  = new QLabel(this);

    Go::State* state = new Go::State();

    void openAI();
    void aiEvaluate();
    void aiEvaluate_visit();

    void printStone(Mat<Go::Stone>& Board);
    void printWin(int win);
    void printQi(Go::State& s);
    void printMark(Go::State& s);
    void printNumber(Go::State& s);
};
#endif