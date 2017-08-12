#include <bits/stdc++.h>

using namespace std;

typedef struct STATE 
{
	char board[3][3];
	struct STATE * parent;

	int f_index;
	int h_index;
	int g_index;
}state;

state GOAL;
state INITIAL;

void set_goal()
{
	int k = 1,i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			GOAL.board[i][j] = k + '0';
			k++;
		}
	}

	GOAL.board[2][2] = '-';
}

state set_h_index(state s)
{
	s.h_index = 0;
	return s;
}

void set_initial(char in[][3])
{
	int i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			INITIAL.board[i][j] = in[i][j];
			
		}
	}

	INITIAL.g_index = 0;
	INITIAL = set_h_index(INITIAL);
	INITIAL.f_index = INITIAL.g_index + INITIAL.h_index; 
	INITIAL.parent = NULL;
}


bool goal_check(state s)
{
	int i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(s.board[i][j] != GOAL.board[i][j]) return false;
		}
		
	}
	return true;
}

bool verify_input(char in[][3])
{
	int track[9] = {0};
	int i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(in[i][j] == '-') track[0]++;
			else
			{
				track[in[i][j] - '0']++;
			}
			
		}
	}

	for(i= 0; i < 9; i++)
	{
		if(track[i] != 1) return false;
	} 
	return true;
}



int main()
{
	set_goal();
	char in[3][3];
	int i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			cin >> in[i][j];
		}
	}

	if(!(verify_input(in))) 
	{
		cout << "invalid input" <<endl;
		return 0;
	}

	set_initial(in);


	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			cout << INITIAL.board[i][j] << " ";
		}
		cout << endl;
	}
	

	return 0;
}