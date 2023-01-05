/*
Copyright 2020,2021 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef CHESS_H
#define CHESS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <algorithm> 
#include "../../LiGu_AlgorithmLib/Mat.h"
#include "MontecarloTreeSearch.h"
/******************************************************************************
*                    ��������AI  Chess-AI
------------------------------------------------------------------------------
*Example:
	#include "../LiGu_Codes/Daiyu-Go/src/chess.h"
	int main() {
		Chess_AI::State board;
		board.player = -1;
		Chess_AI::boardInit(board.board);
		while (true) {
			board.clear();
			Chess_AI::run(&board);
			printf("AI:%d %d\n", board.st, board.ed);
			for (char x = 0; x < BOARDSIZE; x++) {
				for (char y = 0; y < BOARDSIZE; y++) {
					printf("%2d ", board(x, y));
				}printf("\n");
			}printf("\n");
		}
	}
******************************************************************************/
namespace Chess_AI {
#define BOARDSIZE 8
#define BLACK  1
#define WHITE -1
typedef char Piece;										//����
enum { King = 1, Queen, Bishop, Knight, Rook, Pawn };	//��,��,��,��,��,��
/******************************************************************************
*                    ״̬
*	[����]: [1]����λ��		[2]����		[3]����
******************************************************************************/
struct State {
	int   st, ed, kidNum = -1;
	char  player = 0;
	Piece eat    = 0;
	std::vector<int> kidIndex, kidList;
	Mat<Piece> board{ BOARDSIZE ,BOARDSIZE };
	Piece& operator[](int i)		{ return board[i]; }
	Piece& operator()(int x, int y) { return board(x, y); }
	void clear() {
		kidIndex.clear();
		kidList .clear();
		kidNum = -1;
	}
};
/******************************************************************************
*                    ��������
******************************************************************************/
//��������
bool	newStateRand	(State& state, State& newState, bool isSimulation);
bool	newStateRand	(State& state);
char	judgeWin		(State& state);
int		getNextStep		(Mat<Piece>& board, int pos, int index = -1);
/*--------------------------------[ ���� ]--------------------------------*/
void run(State* board) {
	typedef MontecarloTreeSearch<State> AI;
	AI ai(
		newStateRand,
		judgeWin
	);
	*board = *ai.run(board);
}
/*--------------------------------------------------------------------------------
						�����״̬���� New State Rand
*	[�㷨]: ��������[����]���ڿ��ܵĶ����£����ѡ��һ������
*	[����]:
		[1] ȷ��״̬�ռ�����
			[1.1] ������Ϊ��, ����ʧ��
		[2] ���ѡ��һ��״̬, ������
--------------------------------------------------------------------------------*/
// ����Montecarlo[2. ��չ]
bool newStateRand(State& state, State& newState, bool isSimulation) {
	if (isSimulation) return newStateRand(state);
	//[1]
	if (state.kidNum == -1) {
		state.kidNum =  0;
		for (int i = 0; i < state.board.size(); i++)
			if (state.board[i] * -state.player > 0) {
				int t = getNextStep(state.board, i);
				state.kidNum += t;
				state.kidList.push_back(t);
			}
	}
	if (state.kidNum - state.kidIndex.size() == 0) return false;
	//[2]
	int index = rand() % (state.kidNum - state.kidIndex.size()) + 1, 
		cur   = 0;
	std::sort(state.kidIndex.begin(), state.kidIndex.end());
	for (int i = 0; i < state.kidIndex.size(); i++)
		if (state.kidIndex[i] == index)index++;
	state.kidIndex.push_back(index);

	for (int i = 0; i < state.board.size(); i++)
		if (state.board[i] * -state.player > 0) {
			if  (index <= state.kidList[cur]) {
				newState.st = i;
				newState.ed = getNextStep(state.board, i, index);
				break;
			}
			else index -= state.kidList[cur++];
		}
	newState.board			= state.board;
	newState.player			=-state.player;
	newState.eat			= state[newState.ed];
	newState[newState.ed]	= state[newState.st];
	newState[newState.st]	= 0;
	return true;
}
// ����Montecarlo[3. ģ��]
bool newStateRand(State& state) {
	//[1]
	int num = 0;
	std::vector<int> numList;
	for (int i = 0; i < state.board.size(); i++)
		if (state.board[i] * -state.player > 0) {
			int t = getNextStep(state.board, i);
			numList.push_back(t);
			num += t;
		}
	if (num == 0) return false;
	//[2]
	int index = rand() % num + 1, cur = 0;
	for (int i = 0; i < state.board.size(); i++)
		if (state.board[i] * -state.player > 0) {
			if (index <= numList[cur]) {
				state.st = i;
				state.ed = getNextStep(state.board, i, index);
				break;
			}
			else index -= numList[cur++];
		}
	state.player    =-state.player;
	state.eat		= state[state.ed];
	state[state.ed] = state[state.st];
	state[state.st] = 0;
	return true;
}
/*--------------------------------[ �ж��Ƿ���� ]--------------------------------*/
bool judgeOut(int x, int y) {
	return (x >= 0 && x < BOARDSIZE
		 && y >= 0 && y < BOARDSIZE) ? false : true;
}
/******************************************************************************
*                   �������������
******************************************************************************/
/*--------------------------------[ �ж���Ӯ ]--------------------------------*/
void boardInit(Mat<Piece>& board) {
	Piece t[] = {
		 Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook,
		 Pawn, Pawn, Pawn, Pawn, Pawn, Pawn, Pawn, Pawn,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		-Pawn,-Pawn,-Pawn,-Pawn,-Pawn,-Pawn,-Pawn,-Pawn,
		-Rook,-Knight,-Bishop,-Queen,-King,-Bishop,-Knight,-Rook,
	};
	board.get(t);
}
/*--------------------------------[ �ж���Ӯ ]--------------------------------*/
char judgeWin(State& state) {
	bool flagB = 1, flagW = 1;
	for (int i = 0; i < state.board.size(); i++) {
		if (state[i] ==  King) flagW = 0;
		if (state[i] == -King) flagB = 0;
	}
	return flagB ? BLACK : (flagW ? WHITE : 0);
}
/*--------------------------------[ ��һ��״̬���� ]--------------------------------*/
int getNextStep(Mat<Piece>& board, int pos, int index) {
	Piece piece = board[pos];
	int x = board.i2x(pos),
		y = board.i2y(pos);
	static const int 
		x_step[] = { 1,-1, 0, 0, 1,-1, 1,-1 },
		y_step[] = { 0, 0, 1,-1, 1, 1,-1,-1 };
	int NextStepNum = 0;
	switch (abs(piece)) {
	case King: {													// ��
		for (int i = 0; i < 8; i++) {
			int xt = x + x_step[i], 
				yt = y + y_step[i];
			if (judgeOut(xt, yt) || board(xt, yt) * piece > 0) continue;
			NextStepNum++;
			if (index-- == 1) return board.xy2i(xt, yt);
		}
	}break;
	case Queen: {												// Ů��
		for (int i = 0; i < 8; i++) {
			int xt = x + x_step[i], 
				yt = y + y_step[i];
			while (!judgeOut(xt, yt) && board(xt, yt) * piece <= 0) {
				NextStepNum++;
				if (index-- == 1) return board.xy2i(xt, yt);
				if (board(xt, yt) * piece < 0) break;
				xt += x_step[i];
				yt += y_step[i];
			}
		}
	}break;
	case Bishop: {												// ��
		for (int i = 4; i < 8; i++) {
			int xt = x + x_step[i], 
				yt = y + y_step[i];
			while (!judgeOut(xt, yt) && board(xt, yt) * piece <= 0) {
				NextStepNum++;
				if (index-- == 1) return board.xy2i(xt, yt);
				if (board(xt, yt) * piece < 0) break;
				xt += x_step[i];
				yt += y_step[i];
			}
		}
	}break;
	case Knight: {												// ��
		for (int i = 0; i < 8; i++) {
			int xt = x + x_step[i % 4 + 4] * (i <= 4 ? 2 : 1),
				yt = y + y_step[i % 4 + 4] * (i >  4 ? 2 : 1);
			int xfoot = x + x_step[i % 4 + 4] * (i <= 4 ? 1 : 0),
				yfoot = y + y_step[i % 4 + 4] * (i >  4 ? 1 : 0);
			if (judgeOut(xt, yt) 
				|| board(xt, yt) * piece > 0
				|| board(xfoot, yfoot) != 0						//������
			) continue;
			NextStepNum++;
			if (index-- == 1) return board.xy2i(xt, yt);
		}
	}break;
	case Rook: {													// ��
		for (int i = 0; i < 4; i++) {
			int xt = x + x_step[i], 
				yt = y + y_step[i];
			while (!judgeOut(xt, yt) && board(xt, yt) * piece <= 0) {
				NextStepNum++;
				if (index-- == 1) return board.xy2i(xt, yt);
				if (board(xt, yt) * piece < 0) break;
				xt += x_step[i];
				yt += y_step[i];
			}
		}
	}break;
	case Pawn: {													// ��
		int t = piece > 0 ? 1 : -1;
		if (!judgeOut(x + t, y) && board(x + t, y) * piece <= 0) {
			NextStepNum++;
			if (index-- == 1) return board.xy2i(x + t, y);
		}
		if (((piece > 0 && x == 1)
		||   (piece < 0 && x == 6))
		&& !judgeOut(x + 2 * t, y) && board(x + 2 * t, y)* piece <= 0) {
			NextStepNum++;
			if (index-- == 1) return board.xy2i(x + 2 * t, y);
		}
	}break;
	}
	return NextStepNum;
}
};
#endif