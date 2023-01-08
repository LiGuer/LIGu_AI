#ifndef GO_DEMO_H
#define GO_DEMO_H

#include <QtWidgets/QMainWindow>  
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include "BoardClass.h"
#include "StoneClass.h"

class GoDemo : public QMainWindow
{
public:
    short windowSize;

    GoDemo(QWidget* parent = 0);
    ~GoDemo() { ; };

    // ͼ��  
    BoardClass* boardClass;
    StoneClass* stoneClass;
    QWidget* BoardWidget = new QWidget(this);     //����
    QWidget* StoneWidget = new QWidget(this);     //����

    // ���� 
    void setGridWidget (QWidget* widget);        //�����������
    void setStoneWidget(QWidget* widget);       //�������ӽ���
};

#endif