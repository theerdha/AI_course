#include <iostream>
#include <fstream>
#include <vector>
#include <queue> 
#include <cmath>
#include <ctime>

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
int H1(int arr[][3]);
int H2(int arr[][3]);
int H3(int arr[][3]);
int H4(int arr[][3]);
bool possibility_checker(int i, int j);
state * set_initial(char in[][3],int (*H)(char[3][3]));
pair<int,int> find_dash(int arr[][3]);
state * new_state(int x_,int y_,state * immparent,int (*H)(char[3][3]));
int check_frontier(priority_queue<state, vector<state >,compare> frontier , char a[][3]);
priority_queue<state, vector<state >,compare> modify_frontier(priority_queue<state, vector<state >,compare> frontier,int value, char a[][3]);
int Astar(char initial[][3]);
int getInvCount(char arr[]);
bool isSolvable(char puzzle[3][3]);
bool verify_input(char in[][3]);
bool array_test(char a[][3], char b[][3]);
pair<int,int> find_char(char arr[][3],char c);
int present_in_row(char aa[][3],char c);


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





//naive h = 0
int H1(char arr[][3])
{
	return 0;
}

//number of tiles out of row and column
int H2(char arr[][3])
{
	int dist = 0;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(arr[i][j] != '-')
			{
				if(i != find_char(GOAL.board,arr[i][j]).first) dist++;
				if(j != find_char(GOAL.board,arr[i][j]).second) dist++;

			}
		}
	}
	return dist;
}

//manhattan distance
int H3(char arr[][3])
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

//misplaced tiles
// int H4(char arr[][3])
// {
// 	int cost = 0;
// 	for(int i = 0; i < 3; i++){
// 		for(int j = 0; j < 3; j++){
// 			if(( arr[i][j]!=(3*i+j+1)+'0')  && arr[i][j] != '-')
// 				cost++;
// 		}
// 	}
// 	return cost;
// }


//linear conflict
int H4(char arr[][3])
{
	int dist = 0;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			for(int k = j +1; k < 3; k++)
			{
				if(present_in_row(GOAL.board,arr[i][j]) ==  present_in_row(GOAL.board,arr[i][k]) && find_char(GOAL.board,arr[i][j]).second > find_char(GOAL.board,arr[i][k]).second)
				dist ++;
			}
		}
		
	}
	return dist;
}







bool possibility_checker(int i, int j)
{
	if(i >= 0 && i <= 2 && j >= 0 && j <= 2) return true;
	else return false;
}



state * set_initial(char in[][3],int (*H)(char[3][3]))
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
	INITIAL -> h_index = H(in);
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

state * new_state(int x_,int y_,state * immparent,int (*H)(char[3][3]))
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
	s -> h_index = H(s -> board);
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


int Astar(char initial[][3],int (*H)(char[3][3]))
{
	priority_queue <state, vector<state >,compare> frontier;
	//pair<int,int>dash;
	//dash = find_dash(initial);
	state * root = set_initial(initial,H);
	

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
				child = new_state(dash.first + row[i],dash.second + col[i],node,H);
				
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
	char arr[9];
	ofstream myfile;
	myfile.open ("Astar.txt");
	int noofinput = 1;
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

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 	clock_t begin = clock();
		int z = Astar(in,H1);
		clock_t end = clock();

	  	myfile << "=========using h = 0=============" <<endl;
	  	for(int row = 0; row < 3; row++)
	  	{
	  		for(int col = 0; col < 3; col++)
	  		{
	  			myfile << in[row][col] << " ";
	  		}
	  		myfile<<endl;
	  	}
	  	myfile<<endl;
	  	myfile<< "expanded " <<  z << endl;
	    myfile << "path length " <<  number_of_iterations - 1 << endl;

	    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    myfile << "time elapsed " << elapsed_secs << " seconds" << endl;
	    myfile << "=============================" <<endl;
	    number_of_iterations = 0;
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		begin = clock();
		z = Astar(in,H2);
	    end = clock();

	  	myfile << "==========number of tiles out of row and column=============" <<endl;
	  	for(int row = 0; row < 3; row++)
	  	{
	  		for(int col = 0; col < 3; col++)
	  		{
	  			myfile << in[row][col] << " ";
	  		}
	  		myfile<<endl;
	  	}
	  	myfile<<endl;
	  	myfile<< "expanded " <<  z << endl;
	    myfile << "path length " <<  number_of_iterations - 1 << endl;

	    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    myfile << "time elapsed " << elapsed_secs << " seconds" << endl;
	    myfile << "=============================" <<endl;
	    number_of_iterations = 0;
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		begin = clock();
		z = Astar(in,H3);
		end = clock();

	  	myfile << "=========manhattan=============" <<endl;
	  	for(int row = 0; row < 3; row++)
	  	{
	  		for(int col = 0; col < 3; col++)
	  		{
	  			myfile << in[row][col] << " ";
	  		}
	  		myfile<<endl;
	  	}
	  	myfile<<endl;
	  	myfile<< "expanded " <<  z << endl;
	    myfile << "path length " <<  number_of_iterations - 1 << endl;

	    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    myfile << "time elapsed " << elapsed_secs << " seconds" << endl;
	    myfile << "=============================" <<endl;
	    number_of_iterations = 0;
		

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		begin = clock();
		z = Astar(in,H4);
		end = clock();

	  	myfile << "==========misplaced tiles============" <<endl;
	  	for(int row = 0; row < 3; row++)
	  	{
	  		for(int col = 0; col < 3; col++)
	  		{
	  			myfile << in[row][col] << " ";
	  		}
	  		myfile<<endl;
	  	}
	  	myfile<<endl;
	  	myfile<< "expanded " <<  z << endl;
	    myfile << "path length " << number_of_iterations - 1 << endl;

	    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	    myfile << "time elapsed " << elapsed_secs << " seconds" << endl;
	    myfile << "=============================" <<endl;
	    number_of_iterations = 0;
		

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	    fin.get(a);
		
		}
		
	}

		
	myfile.close();

    
	

	return 0;
}