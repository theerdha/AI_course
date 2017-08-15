#include <bits/stdc++.h>

using namespace std;

typedef struct STATE 
{
	char board[3][3];
	struct STATE * parent;
	
	int f_index;
	int h_index;
	int g_index;
	
}
state;

typedef struct COMPARE
{
    bool operator()(const state left, const state right) const
    {
        return (left.f_index ) > (right.f_index);
    }
}compare;

bool verify_input(char in[][3]);
void set_goal();
bool goal_check(state* s);
int set_h_index(int arr[][3]);
bool possibility_checker(int i, int j);
state * set_initial(char in[][3]);
pair<int,int> find_dash(int arr[][3]);
state * new_state(int x_,int y_,state * immparent);
int check_frontier(priority_queue<state, vector<state >,compare> frontier , char a[][3]);
priority_queue<state, vector<state >,compare> modify_frontier(priority_queue<state, vector<state >,compare> frontier,int value, char a[][3]);
int Astar(char initial[][3]);
int getInvCount(char arr[]);
bool isSolvable(char puzzle[3][3]);
bool verify_input(char in[][3]);
bool array_test(char a[][3], char b[][3]);




state GOAL;
int number_of_iterations = 0;
// priority_queue<int> PQ;

void set_goal()
{
	int k = 0,i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			GOAL.board[i][j] = k + '0';
			k++;
		}
	}

	GOAL.board[0][0] = '-';
	return;

}

bool goal_check(state* s)
{
	int i,j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(s -> board[i][j] != GOAL.board[i][j]) return false;
		}
		
	}
	return true;
}


//manhattan distance
// int set_h_index(char arr[][3])
// {
// 	//return 0;
// 	int dist = 0;
// 	for(int i=0;i<3;i++)
// 	{
// 		for(int j=0;j<3;j++)
// 		{
// 			if(arr[i][j]!= '-')
// 			{
// 				for(int k=0;k<3;k++)
// 				{
// 					for(int l=0;l<3;l++)
// 					{
// 						if(arr[i][j] == GOAL.board[k][l])
// 							dist+=abs(i-k)+abs(j-l);
// 					}
// 				}
// 			}
// 		}
// 	}
// 	return dist;
// }

//naive h = 0

int set_h_index(char arr[][3])
{
	return 0;
}

//misplaced tiles
/*int set_h_index(char arr[][3])
{
	int k = 1;
	int l = 0;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(!(i == 2 && j == 2))
			{
				if(arr[i][j] != k - '0') l++;
			}
			k++;
		}
	}
	return l;
}*/




bool possibility_checker(int i, int j)
{
	if(i >= 0 && i <= 2 && j >= 0 && j <= 2) return true;
	else return false;
}



state * set_initial(char in[][3])
{
	int i,j;
	state * INITIAL = new state();
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			INITIAL -> board[i][j] = in[i][j];
			
		}
	}

	INITIAL -> g_index = 0;
	INITIAL -> h_index = set_h_index(in);
	INITIAL -> f_index = INITIAL -> g_index + INITIAL -> h_index; 
	INITIAL -> parent = NULL;
	return INITIAL;
}



pair<int,int> find_dash(char arr[][3])
{
	pair<int,int> dash;
	for (int i = 0; i < 3; i++)
    {
    	for (int j = 0; j < 3; j++)
    	{
    		if(arr[i][j] == '-') 
    		{
    			dash.first = i;
    			dash.second = j;
    			break;
    		}
    	}
    }	
    return dash;
}

state * new_state(int x_,int y_,state * immparent)
{
	pair<int,int> dash;
	dash = find_dash(immparent -> board);
	state * s = new state();
	// memcpy(s -> board,immparent -> board,sizeof(s -> board));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			s -> board[i][j] = immparent -> board[i][j];
		}
	}
	swap(s -> board[dash.first][dash.second], s -> board[x_][y_]);
	s -> g_index = (immparent -> g_index)+1;
	s -> h_index = set_h_index(s -> board);
	s -> f_index = s -> g_index + s -> h_index;
	s -> parent = immparent;

	return s;
} 

int check_frontier(priority_queue<state, vector<state >,compare> frontier , char a[][3])
{
	state * temp = new state() ;
	while(!frontier.empty())
	{
		*temp = frontier.top();
		frontier.pop();
		int k = -1;
		for (int i = 0; i < 3; ++i)
		 {
		 	for (int j = 0; j < 3; ++j)
		 	{
		 		if(temp -> board[i][j] != a[i][j]) k++;
		 	}
		 } 
		 if(k == -1) return temp -> f_index;
    }
	 return -1;
}

priority_queue<state, vector<state >,compare> modify_frontier(priority_queue<state, vector<state >,compare> frontier,int value, char a[][3])
{
	priority_queue <state, vector<state >,compare> newfrontier;
	state * temp = new state() ;


	while(!frontier.empty())
	{
		
		*temp = frontier.top();
		frontier.pop();
		int k = -1;
		for (int i = 0; i < 3; ++i)
		 {
		 	for (int j = 0; j < 3; ++j)
		 	{
		 		if(temp -> board[i][j] != a[i][j]) k++;
		 	}
		 }
		 if(k != -1)
		 {
		 	newfrontier.push(*temp);
		 }
		 else
		 {
		 	temp -> f_index = value;
		 	newfrontier.push(*temp);

		 } 
	}
	return newfrontier;
}

bool array_test(vector<vector<char> > v, char b[][3])
{
	int k = -1;
	
	for(int i =0; i < 3 ;i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(b[i][j] != v[i][j]) k++;
		}
	}
	if(k == -1) return true;
	else return false;
}

vector<vector<char> > converter(char a[][3])
{
	vector<vector<char> > v(3,vector<char>(3));
	for(int i =0; i < 3 ;i++)
	{
		for(int j = 0; j < 3; j++)
		{
			v[i][j] = a[i][j];
		}
	}
	return v;

}

void printMatrix(char mat[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            cout << mat[i][j] << " ";
        printf("\n");
    }
    number_of_iterations ++;
    return;
}

void printPath(state* root)
{
    if (root == NULL)
        return;
    printPath(root->parent);
    printMatrix(root->board);
 
    printf("\n");
    return;
}


int Astar(char initial[][3])
{
	priority_queue <state, vector<state >,compare> frontier;
	//pair<int,int>dash;
	//dash = find_dash(initial);
	state * root = set_initial(initial);
	

	vector<vector<vector<char> > > explored;
	int expanded = 0;
	
	frontier.push(*root);
	
	int row[] = { 1, 0, -1, 0 };
	int col[] = { 0, -1, 0, 1 };
	
	while(true)
	{
		pair<int,int> dash;
		
		
		state * child;
		//cout << frontier.size() <<endl;
		if(frontier.empty()) return -1;
		state * node = new state();
		*node = frontier.top();
		frontier.pop();

		expanded ++;
		//cout << "here" <<endl;
		if(goal_check(node))
		{
			printPath(node); 
			return expanded;
		}
		explored.push_back(converter(node -> board));
		
		dash = find_dash(node -> board);

		for(int i = 0; i <= 4; i++)
		{
			
				//cout << dash.first  << " a " << dash.second << endl;
				if(possibility_checker(dash.first + row[i],dash.second + col[i]))
				{
					//cout << dash.first + row[i] << " " << dash.second + col[i] << endl;
					child = new state();
					child = new_state(dash.first + row[i],dash.second + col[i],node);
					
					unsigned int temp2;
					int l = -1;
					
					for(temp2 = 0; temp2 < explored.size(); temp2++)
					{

						if ( array_test(explored[temp2],child -> board) == true) 
						{
							l++;
							break;
						}
					}
					

					//cout << "above " << frontier.top().board[0][0] << endl;
					int front_check = check_frontier(frontier,child -> board);
					//cout << "below " <<frontier.top().board[0][0] << endl;

					if (l== -1 && front_check == -1)
					{
						frontier.push(*child);
						//explored.push_back(converter(child -> board));
					}
					else if(front_check != -1 && front_check > child -> f_index)
					{
						priority_queue <state, vector<state >,compare> newfrontier = modify_frontier(frontier, child -> f_index, child -> board);
						swap(newfrontier,frontier);
						//frontier.push(child);
						//explored.push_back(converter(child -> board));
					}
					
				}
			
		}
	}

	return -1;
}


//solvability
int getInvCount(char arr[])
{
    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++)
    {
    	for (int j = i+1; j < 9; j++)
    	{
        	if (arr[j] != '-' && arr[i] != '-' &&  arr[i] - '0' > arr[j] - '0') inv_count++;
        }
    }
    return inv_count;
}
 
bool isSolvable(char puzzle[3][3])
{
    
    int invCount = getInvCount((char *)puzzle);
    return (invCount%2 == 0);
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

	cout <<endl;

	if(!(verify_input(in))) 
	{
		cout << "invalid input" <<endl;
		return 0;
	}

	if(!(isSolvable(in)))
	{
		cout << "not solvable" <<endl;
		return 0;
	}
	// pair<int,int> dash;
	// dash = find_dash(in);


	int z = Astar(in);
    cout << z << endl;

    cout << number_of_iterations - 1<< endl;
	

	return 0;
}