#include "StoneClass.h"


StoneClass::StoneClass(QWidget* parent) : QWidget(parent)
{
    w = parent;

    resize(1000, 1000);
    setAttribute(Qt::WA_TranslucentBackground, true);

    StoneWarn->setStyleSheet("QLabel{background:#FF0000;}");
    StoneWarn->setGeometry(0, 0, 0, 0);

    for (int i = 0; i < 361; i++)
        Stone[i] = new QLabel(this);

    setFocusPolicy(Qt::StrongFocus);

    GoAI::evaluate_result.resize(BOARDSIZE * BOARDSIZE, 0);
    GoAI::evaluate_visit.resize(BOARDSIZE * BOARDSIZE, 0);
}

/*
 *  �����Ӧ
 */
void StoneClass::mousePressEvent(QMouseEvent* e) {
    signed char x, y;
    
    if (e->button() == Qt::LeftButton) {    //�������
        x = (e->x() - BoardClass::boardMargin + BoardClass::gridSize / 2) / BoardClass::gridSize;
        y = (e->y() - BoardClass::boardMargin + BoardClass::gridSize / 2) / BoardClass::gridSize;

        Go::State* s_ = Go::nextState(*state, x, y);
        if (s_ == NULL)
            return;

        state = s_;

        printStone(state->board);

        if (ai_is_open) {
            GoAI::move_pos = x * BOARDSIZE + y;
        }
    }
    else if (e->button() == Qt::RightButton) {

    }
    else if (e->button() == Qt::MiddleButton) {
        if (state->parent != NULL) {  
            Go::State* s = state;
            state = state->parent;
            s->parent = NULL;
            delete s;
        }

        printStone(state->board);
    }
    
    // if win
    if (Go::judgeWin(*state) == BLACK) {
        printWin(BLACK);
        return;
    }

    if (Go::judgeWin(*state) == WHITE) {
        printWin(WHITE);
        return;
    }

    StoneWarn->setGeometry(
        BoardClass::boardMargin + x * BoardClass::gridSize - stoneSize / 2,
        BoardClass::boardMargin + y * BoardClass::gridSize - stoneSize / 2,
        4, 4
    );
}

/*
 *  ������Ӧ
 */
void StoneClass::keyPressEvent(QKeyEvent* event) {

    switch (event->key())    {
    case Qt::Key_A: openAI(); break;
    case Qt::Key_S: aiEvaluate(); break;
    case Qt::Key_D: aiEvaluate_visit(); break;
    case Qt::Key_Q: printQi(*state); break;
    case Qt::Key_M: printMark(*state); break;
    case Qt::Key_N: printNumber(*state); break;

    }
}

void StoneClass::openAI() {  
    static ThreadClass ai;
    static QThread thread;

    if (ai_is_open == 0) {
        ai_is_open = 1;
        ai.s = state;

        QObject::connect(&thread, &QThread::started, &ai, &ThreadClass::openAI);
        ai.moveToThread(&thread);
        thread.start();
    }

    return;
}

/*
 *  ��ʾAI�������
 */
void StoneClass::aiEvaluate() {
    static int fg = 0;
    static QLabel** labels = new QLabel * [BOARDSIZE * BOARDSIZE];
    static QFont font("Times New Roman", 12, 50);

    if (fg == 0) {
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            labels[i] = new QLabel(w);
            labels[i]->setFont(font);
            labels[i]->setAlignment(Qt::AlignCenter);
        }
        fg = 1;
    }

    float maxn = -0x7FFFFFFF;
    int maxi = 0;

    if (fg == 1) {
        GoAI::evaluate_fg = 1;
        QThread::msleep(50);

        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++)
            if (state->mark[i] == -1 && maxn < GoAI::evaluate_result[i]) {
                maxn = GoAI::evaluate_result[i];
                maxi = i;
            } 
    } 

    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if ((state->mark[i] == -1) && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE,
                v = GoAI::evaluate_result[i] * 100;   

            if(i == maxi)
                labels[i]->setStyleSheet("color:Red");
            else  
                labels[i]->setStyleSheet("color:Blue");

            labels[i]->setText(QString::fromStdString(to_string(v)));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.35), 30, 30);
            labels[i]->show();
        }
        else {
            labels[i]->hide();
        }
    }
    fg = -fg;
}

/*
 *  ��ʾAI�������
 */
void StoneClass::aiEvaluate_visit() {
    static int fg = 0;
    static QLabel** labels = new QLabel * [BOARDSIZE * BOARDSIZE];
    static QFont font("Times New Roman", 8, 50);

    if (fg == 0) {
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            labels[i] = new QLabel(w);
            labels[i]->setFont(font);
            labels[i]->setAlignment(Qt::AlignCenter);
            labels[i]->setStyleSheet("color:Blue");
        }
        fg = 1;
    }

    if (fg == 1) {
        GoAI::evaluate_fg = 1;
        QThread::msleep(50);
    }

    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if ((state->mark[i] == -1) && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE,
                v = GoAI::evaluate_visit[i];

            labels[i]->setText(QString::fromStdString(to_string(v)));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.35), 30, 30);
            labels[i]->show();
        }
        else {
            labels[i]->hide();
        }
    }
    fg = -fg;
}

/*
 *  ��ʾ��Ӯ���
 */
void StoneClass::printWin(int win)
{
    QFont font("Times New Roman", 50, 50, true);//���壬��С����ϸ��50���������Ƿ�б��
    WinLable->setGeometry(0, 0, 0, 0);
    WinLable->setFont(font);
    WinLable->setGeometry(0, 0, 100, 100);

    if (win == BLACK)
        WinLable->setText("BLACK Win");
    else if(win == WHITE)
        WinLable->setText("WHITE Win");
}

/*
 *  ��ʾ����
 */
void StoneClass::printStone(Mat<Go::Stone>& board) {
    int StoneCur = 0;

    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if (board[i] != 0) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE;

            x = BoardClass::boardMargin + x * BoardClass::gridSize - stoneSize / 2;
            y = BoardClass::boardMargin + y * BoardClass::gridSize - stoneSize / 2;

            Stone[StoneCur]->setGeometry(x, y, stoneSize, stoneSize);

            if (board[i] == BLACK)
                Stone[StoneCur]->setStyleSheet(
                    "QLabel{background:#000000; border-radius: 16px; border:0px solid black;}"
                );
            else
                Stone[StoneCur]->setStyleSheet(
                    "QLabel{background:#FF0000; border-radius: 16px; border:0px solid #FF0000;}"
                );

            Stone[StoneCur]->show();
            StoneCur++;
        }
    }
    for (int i = StoneCur; i < BOARDSIZE * BOARDSIZE; i++)
        Stone[i]->hide();
}

/*
 *  ��ʾ����
 */
void StoneClass::printQi(Go::State& s) {
    static int fg = 0;
    static QLabel** labels = new QLabel * [BOARDSIZE * BOARDSIZE];
    static QFont font("Times New Roman", 15, 50);

    if (fg == 0) {
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            labels[i] = new QLabel(w);
            labels[i]->setFont(font);
            labels[i]->setStyleSheet("color:White");
            labels[i]->setAlignment(Qt::AlignCenter);
        }
        fg = 1;
    }

    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if (s.board[i] != 0 && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE; 

            labels[i]->setText(QString::fromStdString(to_string(s.qi[s.mark[i]])));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.35), 30, 30);
            labels[i]->show();
        }
        else {
            labels[i]->hide();
        }
    }
    fg = -fg;
}

/*
 *  ��ʾ�����
 */
void StoneClass::printMark(Go::State& s) {
    static int fg = 0;
    static QLabel** labels = new QLabel * [BOARDSIZE * BOARDSIZE];
    static QFont font("Times New Roman", 15, 50);

    if (fg == 0) {
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            labels[i] = new QLabel(w);
            labels[i]->setFont(font);  
            labels[i]->setAlignment(Qt::AlignCenter);
        }
        fg = 1;
    }

    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if (abs(s.mark[i]) == BANPOINT && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE; 

            labels[i]->setText(QString('x'));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.36), 30, 30);
            labels[i]->setStyleSheet("color:Blue");
            labels[i]->show();
        }
        else if (abs(s.mark[i]) == EYEPOINT && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE;

            labels[i]->setText(QString('o'));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.36), 30, 30);
            labels[i]->setStyleSheet("color:Blue");
            labels[i]->show();
        }
        else if (s.mark[i] >= 0 && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE;

            labels[i]->setText(QString::fromStdString(to_string(s.mark[i])));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.35), 30, 30);
            labels[i]->setStyleSheet("color:White");
            labels[i]->show();
        }
        else {
            labels[i]->hide();
        }
    }
    fg = -fg;
}
/*
 *  ��ʾ����
 */
void StoneClass::printNumber(Go::State& s) {
    static int fg = 0, number[BOARDSIZE * BOARDSIZE];
    static QLabel** labels = new QLabel * [BOARDSIZE * BOARDSIZE];
    static QFont font("Times New Roman", 15, 50);

    if (fg == 0) {
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            labels[i] = new QLabel(w);
            labels[i]->setFont(font);
            labels[i]->setStyleSheet("color:White");
            labels[i]->setAlignment(Qt::AlignCenter);
        }
        fg = 1;
    }

    memset(number, 0, sizeof(int) * BOARDSIZE * BOARDSIZE);
    Go::State* s_ = &s;
    int num = 0, sum = 0;

    while (s_ != NULL) {
        num--;
        sum++;

        if(s.board[s_->action] != 0 && number[s_->action] == 0)
            number[s_->action] = num;

        s_ = s_->parent;
    }

    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if (number[i] != 0)
            number[i] += sum;  

        if (s.board[i] != 0 && fg == 1) {
            int x = i / BOARDSIZE,
                y = i % BOARDSIZE;

            labels[i]->setText(QString::fromStdString(to_string(number[i])));
            labels[i]->setGeometry(
                BoardClass::boardMargin + BoardClass::gridSize * (x - 0.36),
                BoardClass::boardMargin + BoardClass::gridSize * (y - 0.35), 30, 30);
            labels[i]->show();
        }
        else {
            labels[i]->hide();
        }
    }
    fg = -fg;
}