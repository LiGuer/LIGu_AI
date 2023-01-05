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
#ifndef MINIMAX_H
#define MINIMAX_H
#include <vector>
template<class State>
class MiniMax {
public:
	int MaxLevel = 5;
	int  (*Evaluate)		(State&);					//���ۺ���
	bool (*newStateFunc)	(State&, State&);			//������״̬
	char (*judgeWin)		(State&);					//�ж���Ӯ
	void (*endingState)		(State&);					//״̬��β����
	std::vector<int> ansScoreSet;
	State maxScoreState;
	/*----------------[ ����/�������� ]----------------*/
	MiniMax(
		int (*_Evaluate)	(State&), 
		bool(*_newStateFunc)(State&, State&), 
		char(*_judgeWin)	(State&),
		void(*_endingState) (State&),
		int _MaxLevel = 5
	) {
		Evaluate     = _Evaluate;
		newStateFunc = _newStateFunc;
		judgeWin	 = _judgeWin;
		endingState  = _endingState;
		MaxLevel	 = _MaxLevel;
	}
	/******************************************************************************
	*                    ���ߺ���  ������
	*	[����]:
			[0] ���������ֵ, ���ص�ǰ״̬������(���ۺ���)
			[1] ���ڵ�ǰ״̬, ������״̬, ����ÿһ�����ܵ���״̬
				[2] �ݹ���ߺ���, ���������������&����������
				[3] ��β
					[3.1] �������Ĳ�(���ֲ�) ȡ��С
					[3.2] ˫�����Ĳ�(��  ��) ȡ���
					[3.3] alpha-beta��֦
	******************************************************************************/
	int Policy(char level, State& state, int alpha, int beta) {
		//[0]
		if (level > MaxLevel || judgeWin(state) != 0)
			return Evaluate(state);
		int max = -0x7fffffff, 
			min =  0x7fffffff;
		//[1]
		State newState;
		while (newStateFunc(state, newState)) {
			//[2]
			int score = Policy(level + 1, newState, alpha, beta);
			//[3]
			if (level == 0) {
				ansScoreSet.push_back(score);
				if (score > max) maxScoreState = newState;
			}
			if (level % 2 == 0)
				 max = max >= score ? max : score, alpha = alpha >= score ? alpha : score;
			else min = min <= score ? min : score,  beta =  beta <= score ?  beta : score;
			if (alpha >= beta) break;
		}
		endingState(newState);
		return level % 2 == 0 ? max : min;
	}
};
#endif