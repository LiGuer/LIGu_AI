#ifndef MINIMAX_H
#define MINIMAX_H
template<class State>
class MiniMax {
public:
	int MaxLevel;
	int		(*Evaluate)		(State*);					//评价函数
	State*	(*newStateFunc)	(State*);					//生成新状态
	/*----------------[ 构造/析构函数 ]----------------*/
	MiniMax(int(*_Evaluate)(State*), State* (*_newStateFunc)(State*),int _MaxLevel = 5) {
		Evaluate     = _Evaluate;
		newStateFunc = _newStateFunc;
		MaxLevel	 = _MaxLevel;
	}
	/******************************************************************************
	*                    决策函数  博弈树
	*	[流程]:
			[0] 到达层数阈值, 返回当前状态的评价(评价函数)
			[1] 基于当前状态, 生成新状态, 对于每一个可能的新状态
				[2] 递归决策函数, 深度优先搜索生成&遍历博弈树
				[3] 收尾
					[3,1] 单数博弈层(对手层) 取最小
					[3.2] 双数博弈层(我  层) 取最大
					[3.3] alpha-beta剪枝
	******************************************************************************/
	int Policy(char level, State* state, int alpha, int beta) {
		//[0]
		if (level > MaxLevel)
			return Evaluate(state);
		int max = -0x7fffffff, 
			min =  0x7fffffff;
		//[1]
		State* newState;
		for ((newState = newStateFunc(state)) != NULL) {
			//[2]
			int score = Policy(level + 1, newState, alpha, beta);
			//[3]
			level % 2 == 1 ?
				min = min < score ? min : score, beta  = beta  < score ? beta  : score:
				max = max > score ? max : score, alpha = alpha > score ? alpha : score;
			if (alpha >= beta) 
				return level % 2 == 0 ? max : min;
		}
		return level % 2 == 0 ? max : min;
	}
};
#endif