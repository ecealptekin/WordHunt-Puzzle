#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<iomanip>
#include<cctype>    

using namespace std;

//Ece Alptekin 
//24156
//Homework 1 – Word Hunt Puzzle

struct cell
{
	int a,b;
};

void DeletePath(vector <char> & path)
{
	for(int i=0; i<path.size(); i++)
	{
		path.pop_back();
	}
}

bool IsPuzzleFull(vector<vector<char>> & puzzle)
{
	int count=0;
	for(int j=0; j<puzzle.size(); j++)
	{
		for(int k=0; k<puzzle[0].size(); k++)
		{
			if(puzzle[j][k] == '_')
			{
				count++;
			}
		}
	}

	if(count == puzzle.size() * puzzle[0].size())
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool IfWordExists(const vector <char> & myword,const vector <string> & dictionary,string & word)
{
	for(int i=0; i<myword.size(); i++)
	{
		word = word + myword[i];
	}

	for(int i=0; i<dictionary.size(); i++)
	{
		if(word == dictionary[i])
		{
			return true;
		}
	}
	return false;

}

bool IfCellVisited(const vector <cell> visited)
{
	for(int i=0; i<visited.size(); i++)
	{
		for(int j=i+1; j<visited.size(); j++)
		{
			if(visited[i].a == visited[j].a && visited[i].b == visited[j].b)
			{
				return true;
			}
		}
	}
	return false;
}

bool IfCellUsed(const vector <char> & myword)
{
	for(int i=0; i<myword.size(); i++)
	{
		if(myword[i] == '_')
		{
			return true;
		}
	}
	return false;
}

void FindWord(vector<vector<char>> & puzzle,const vector <char> & path,int x,int y,const vector <string> & dictionary,int & score)
{
	vector<vector<char>> temp = puzzle;
	vector <cell> visited;
	vector <char> myword;
	string word;

	cell mycell = {x,y}; //first char
	visited.push_back(mycell);
	myword.push_back(puzzle[x][y]);
	puzzle[x][y] = '_'; 

	for(int i=0; i<path.size(); i++)
	{

		if(path[i] == 'r')
		{
			y++;
		}
		else if(path[i] == 'l')
		{
			y--;
		}
		else if(path[i] == 'd')
		{
			x++;
		}
		else if(path[i] == 'u')
		{
			x--;
		}

		cell mycell = {x,y};
		visited.push_back(mycell);
		myword.push_back(puzzle[x][y]);
		puzzle[x][y] = '_'; 
	}


	if(IfCellVisited(visited))
	{
		cout << "Invalid path - cell visited twice" << endl;
		puzzle = temp;
	}

	else if(IfCellUsed(myword))
	{
		cout << "Invalid path - cell visited previously" << endl;
		puzzle = temp;
	}

	else if(!IfWordExists(myword,dictionary,word))
	{
		cout << "Invalid word - non-existent word " << word << endl;
		puzzle = temp;
	}

	else 
	{
		cout << "Word found: " << word << endl;
		cout << "Word length: " << word.length() << endl;
		cout << "Points gained: " << word.length() * word.length() << endl;
		cout << "Current score: " << score + (word.length() * word.length()) << endl;
		score = score + (word.length() * word.length());
	}

}

bool IsValidPuzzle(const vector<vector<char>> & puzzle)
{
	int size = puzzle[0].size();
	for(int i=0; i<puzzle.size(); i++)
	{
		if(puzzle[i].size() != size)
		{
			cout << "Invalid matrix - inconsistent dimensions" << endl;
			return false;
		}
	}
	return true;
}

bool IsLetterPuzzle(const vector<vector<char>> & puzzle)
{
	for (int j=0; j<puzzle.size(); j++)
	{
		for (int k=0; k<puzzle[j].size(); k++)
		{
			if(!islower(puzzle[j][k]))
			{
				cout << "Invalid matrix - char not lowercase letter" << endl;
				return false;
			}
		}
	}
	return true;
}

bool IfPathLong(vector <char> & path)
{
	if(path.size() < 2)
	{
		cout << "Invalid path - path too short" << endl;
		return false;
	}
	return true;
}

bool IfPathInRange(vector <char> & path,int x,int y,int rows,int cols)
{
	int hor=0;
	int ver=0;

	for(int i=0; i<path.size(); i++)
	{
		if(path[i] == 'd')
		{
			ver++;
		}
		else if(path[i] == 'u')
		{
			ver--;
		}
		else if(path[i] == 'r')
		{
			hor++;
		}
		else if(path[i] == 'l')
		{
			hor--;
		}
	}

	if(x + ver < 0 || x + ver > rows -1) 
	{
		cout << "Invalid path - out of range" << endl;
		return false;
	}

	else if(y + hor < 0 || y + hor > cols -1) 
	{
		cout << "Invalid path - out of range" << endl;
		return false;
	}

	return true;
}

bool IfPathExists(vector <char> & path)
{
	for(int i=0; i<path.size(); i++)
	{
		if(path[i] != 'r' && path[i] != 'l' && path[i] != 'u' && path[i] != 'd')
		{
			cout << "Invalid path - non-RLUD input" << endl;
			return false;
		}
	}
	return true;
}

bool IsValidCoordinate(int x,int y,int rows,int cols,bool & flag)
{

	if(x == -1 || y == -1)
	{
		flag = true;
		return false;
	}

	else if(x > rows-1 || y > cols-1 || x < -1 || y < -1)
	{
		cout << "Invalid coordinate - out of range" << endl;
		return false;
	}

	return true;
}

void Print(const vector<vector<char>> & puzzle)
{
	cout << "\n\n";
	for(int j=0; j<puzzle.size(); j++)
	{   
		for (int k=0; k<puzzle[0].size(); k++)
		{   
			cout << setw(2) << puzzle[j][k];
		}
		cout << endl;
	}
	cout << "\n\n";
}

bool PuzzleFile(vector<vector<char>> & puzzle)
{
	ifstream inp;
	string pfile;

	cout << "Please enter the name of the puzzle file: ";
	cin >> pfile;

	while(pfile != "puzzle1.txt" && pfile != "puzzle2.txt" && pfile != "puzzle3.txt" && pfile != "puzzle4.txt" && pfile != "puzzle5.txt")
	{
		cout << "File could not be opened." << endl;
		cout << "Please enter the name of the puzzle file: ";
		cin >> pfile;
	}

	inp.open(pfile);

	string line;
	while(!inp.eof())
	{
		getline(inp,line);
		stringstream ss(line);

		for(int i=0; i<line.length()-1; i++)
		{
			if(isalpha(line[i]) && isalpha(line[i+1]))
			{
				return false;
			}
		}

		char c;
		vector <char> temp;

		while(ss >> c)
		{
			temp.push_back(c);
		}

		puzzle.push_back(temp);
	}

	return true;
}

void DictionaryFile(vector <string> & dictionary)
{
	ifstream ind;
	string dfile;

	cout << "Please enter the name of the dictionary file: ";
	cin >> dfile;

	while(dfile != "words.dat")
	{
		cout << "File could not be opened." << endl;		cout << "Please enter the name of the dictionary file: ";		cin >> dfile;
	}

	ind.open(dfile);

	string word;
	while(!ind.eof())
	{
		getline(ind,word);
		stringstream s(word);

		string tmp;
		while(s >> tmp)
		{
			dictionary.push_back(tmp);
		}
	}
}

bool Coordinate(int & x,int & y,int rows,int cols,vector<vector<char>> & puzzle,bool & flag)
{
	Print(puzzle);

	cout << "Enter the starting location for word (x, y): ";
	cin >> x >> y;

	flag = false;
	while(!IsValidCoordinate(x,y,rows,cols,flag) && flag == false)
	{
		Print(puzzle);
		cout << "Enter the starting location for word (x, y): ";
		cin >> x >> y;	
	}

	if(flag == true)
	{
		return false;
	}

	return true;
}

void MyPath(vector <char> & path)
{
	string pth;
	cout << "Enter the path: ";
	cin >> pth;

	stringstream ss3(pth);
	char c;

	while(ss3 >> c)
	{
		path.push_back(c);
	}
}

bool Path(vector <char> & path,int x,int y,int rows,int cols,vector<vector<char>> & puzzle)
{

	MyPath(path);

	if(!IfPathInRange(path,x,y,rows,cols))
	{
		return false;
	}

	else if(!IfPathExists(path))
	{
		return false;
	}

	else if(!IfPathLong(path))
	{
		return false;
	}
	return true;
}

int main()
{
	vector <vector<char>> puzzle;
	vector <string> dictionary;

	if(PuzzleFile(puzzle)) //check if every cell contains a single char
	{
		if(IsLetterPuzzle(puzzle)) //check if all the characters are lowercase
		{
			if(IsValidPuzzle(puzzle)) //check the dimension consistency
			{
			int rows = puzzle.size();
			int cols = puzzle[0].size();

			DictionaryFile(dictionary); //store the dictionary

			int x,y;
			bool flag;
			int score = 0;

			while(!IsPuzzleFull(puzzle) && Coordinate(x,y,rows,cols,puzzle,flag) && flag == false)
			{
				vector <char> path;
				if(Path(path,x,y,rows,cols,puzzle)) //entering a path to follow
				{
					FindWord(puzzle,path,x,y,dictionary,score); //check if it is a valid word
				}
				DeletePath(path);
			}

			if(IsPuzzleFull(puzzle)) //if the user completed the puzzle
			{
				cout << "\n\n";
				cout << "Congratulations, you have solved the puzzle perfectly!" << endl;
				cout << "Your final score is " << score << "." << endl;
			}
			else if(flag == true) //if the user enter -1 as one of the coordinates
			{
				cout << "\n\n";
				cout << "You didn't completely solve the puzzle." << endl;
				cout << "Your final score is " << score << "." << endl;
			}

		}
	  }
	}

	else
	{
		cout << "Invalid matrix - cell entry is not a char" << endl;
	}


	cin.get();
	cin.ignore();
	return 0;
}
