#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>



#define inf 1000000

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

void set_goal();
bool goal_check(state* s);
int set_h_index(char arr[][3]);
bool possibility_checker(int i, int j);
pair<int,int> find_dash(char arr[][3]);
state * new_state(int x_,int y_,state * immparent);
int check_path(stack <state, vector<state > >path, char a[][3]);
int ida_star(char arr[][3]);
int search(stack <state, vector<state > > *path, int g, int *bound);
int getInvCount(char arr[]);
bool isSolvable(char puzzle[3][3]);
bool verify_input(char in[][3]);
pair<int,int> find_char(char arr[][3],char c);
int present_in_row(char aa[][3],char c);

state GOAL;
int number_of_iterations = 0;
int path_length = 0;
// priority_queue<int> PQ;

int present_in_row(char aa[][3],char c)
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(aa[i][j] == c) return i;
		}
	}
	return -1;
}

pair<int,int> find_char(char arr[][3],char c)
{
	pair<int,int> dash;
	for (int i = 0; i < 3; i++)
    {
    	for (int j = 0; j < 3; j++)
    	{
    		if(arr[i][j] == c) 
    		{
    			dash.first = i;
    			dash.second = j;
    			break;
    		}
    	}
    }	
    return dash;
}

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
int set_h_index(char arr[][3])
{
	//return 0;
	int dist = 0;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(arr[i][j]!= '-')
			{
				for(int k=0;k<3;k++)
				{
					for(int l=0;l<3;l++)
					{
						if(arr[i][j] == GOAL.board[k][l])
							dist+=abs(i-k)+abs(j-l);
					}
				}
			}
		}
	}
	return dist;
}




//linear conflict
// int set_h_index(char arr[][3])
// {
// 	int dist = 0;
// 	for(int i = 0; i < 3; i++)
// 	{
// 		for(int j = 0; j < 3; j++)
// 		{
// 			for(int k = j +1; k < 3; k++)
// 			{

// 				if(arr[i][j] != '-' && arr[i][k] != '-')
// 				{
// 					if(present_in_row(GOAL.board,arr[i][j]) ==  present_in_row(GOAL.board,arr[i][k]) && find_char(GOAL.board,arr[i][j]).second > find_char(GOAL.board,arr[i][k]).second)
// 					dist ++;
// 			    }
// 			}
// 		}
		
// 	}
// 	return dist;
// }

//number of tiles out of row and column
// int set_h_index(char arr[][3])
// {
// 	int dist = 0;
// 	for(int i = 0; i < 3; i++)
// 	{
// 		for(int j = 0; j < 3; j++)
// 		{
// 			if(arr[i][j] != '-')
// 			{
// 				if(i != find_char(GOAL.board,arr[i][j]).first) dist++;
// 				if(j != find_char(GOAL.board,arr[i][j]).second) dist++;

// 			}
// 		}
// 	}
// 	return dist;
// }






//naive h = 0

// int set_h_index(char arr[][3])
// {
// 	return 0;
// }

//misplaced tiles
// int set_h_index(char arr[][3])
// {
// 	int k = 1;
// 	int l = 0;
// 	for(int i=0;i<3;i++)
// 	{
// 		for(int j=0;j<3;j++)
// 		{
// 			if(!(i == 2 && j == 2))
// 			{
// 				if(arr[i][j] != k - '0') l++;
// 			}
// 			k++;
// 		}
// 	}
// 	return l;
// }

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

int check_path(stack <state, vector<state > >path, char a[][3])
{
	state * temp = new state() ;
	while(!path.empty())
	{
		*temp = path.top();
		path.pop();
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

void printMatrix(char mat[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            cout << mat[i][j] << " ";
        printf("\n");
    }
     printf("\n");
     path_length++;
    return;
}

void printStack(stack <state, vector<state > > path)
{
	stack <state, vector<state > > path1;
	while(!path.empty())
	{
		path1.push(path.top());
		path.pop();
	}

	while(!path1.empty())
	{
		printMatrix(path1.top().board);
		path1.pop();
	}
	return;
}

int ida_star(char arr[][3])
{
	int bound = set_h_index(arr);
	stack <state, vector<state > > path;
	state * root = set_initial(arr);
	path.push(*root);

	while(true)
	{
		int t = search(&path,0,&bound);
		if(t == -1)
		{
			printStack(path);
			return bound;
		}
		if(t == inf)return -2;
		bound = t;
	}
}

int search(stack <state, vector<state > > *path, int g, int *bound)
{
	state* node = new state();
	*node = (*path).top();
	int f = g + set_h_index(node -> board);
	if(f > *bound) return f;
	if(goal_check(node)) return -1;

	int min = inf;
	pair<int,int> dash;
	dash = find_dash(node -> board);
	int row[] = { 1, 0, -1, 0 };
	int col[] = { 0, -1, 0, 1 };

	for(int i = 0; i <= 4; i++)
	{
		
			//cout << dash.first  << " a " << dash.second << endl;
		if(possibility_checker(dash.first + row[i],dash.second + col[i]))
		{
			//cout << dash.first + row[i] << " " << dash.second + col[i] << endl;
			state * child = new state();
			child = new_state(dash.first + row[i],dash.second + col[i],node);
			int pathcheck = check_path(*path,child -> board);
			if(pathcheck == -1)
			{
				(*path).push(*child);
				int t = search(path,g+1,bound);
				if(t == -1)return -1;
				if(t < min) min = t;
				(*path).pop();
				number_of_iterations ++;
			}
		}
	}
	return min;
			
}


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
	char arr[9];
	ofstream myfile;
	myfile.open ("manhattanIDAstar.txt");
	int noofinput = 2;
	char a;

	ifstream fin("test.txt");
	if(fin.is_open())
	{
		while(noofinput --){
		int position = 0;
		while(!fin.eof() && position < 9)
		{

			fin.get(in[position/3][position%3]);
			position ++;
		}

		for (int i = 0; i < 3; i++)
	    {
	        for (int j = 0; j < 3; j++)
	            cout << in[i][j] << " ";
	        printf("\n");
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

	 	clock_t begin = clock();
		int z = ida_star(in);
		clock_t end = clock();

		
	  	
	  	myfile << "=============================" <<endl;
	  	for(int row = 0; row < 3; row++)
	  	{
	  		for(int col = 0; col < 3; col++)
	  		{
	  			myfile << in[row][col] << " ";
	  		}
	  		myfile<<endl;
	  	}
	  	myfile<<endl;
	  	myfile<< "expanded " <<  number_of_iterations << endl;
	    myfile << "path length " <<  path_length - 1 << endl;

	    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    myfile << "time elapsed " << elapsed_secs << " seconds" << endl;
	    myfile << "=============================" <<endl;

	    fin.get(a);
		number_of_iterations = 0;
		path_length = 0;
		}
		
	}

		
	myfile.close();

    
	

	return 0;
}
