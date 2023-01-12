#ifndef CHESS_DEMO_H
#define CHESS_DEMO_H

#include <QtWidgets/QMainWindow>  
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include "BoardClass.h"
#include "ChessClass.h"

class ChessDemo : public QMainWindow
{
public:
    short windowSize;

    ChessDemo(QWidget* parent = 0);
    ~ChessDemo() { ; };

    // ͼ��  
    BoardClass* boardClass;
    ChessClass* chessClass;
    QWidget* BoardWidget = new QWidget(this);     //����
    QWidget* ChessWidget = new QWidget(this);     //����

    // ���� 
    void setGridWidget (QWidget* widget);        //�����������
    void setChessWidget(QWidget* widget);       //�������ӽ���
};

#endif