
#include <iostream>
//filestream
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

class AStar
{
    vector <char*> grid;



    string input;

    struct Coordinate
    {
        int x_coordinate;
        int y_coordinate;
    };

    struct Node
    {
        int x_coordinate;
        int y_coordinate;
        bool obstacle;
        bool visited;
        vector <Node*> vecNeighbours;
        Node* Parent;
    };

    Node *nodes = nullptr; //general node
    Node *nodeStart = nullptr; //starting node, where the player is located in the file but found later
    Node *nodeEnd = nullptr; //goal node

public:
    int mapWidth;
    int mapHeight;
   



    //characters in the file
    char player = '^';
    char block = '#';
    char freeSpace = ' ';
    char exit = 'E';
    //characters to be added on the grid
    char possibleRoute = 'M';
    char optimalRoute = 'B';

    void ReadFile()
    {
        ifstream file1("../maze1.txt");
       // ifstream file1("../maze2.txt");

        //if there is an error opening the file
        if (!file1.is_open())
        {
            cout << "error" << endl;
        }

        //read the file line by line
        while (getline(file1, input))
        {
            //instantiate a chararray
            char* rowArray = new char[input.length() + 1];
            //use legth of input because there is no length of chararray yet
            for (int i = 0; i < input.length() + 1; i++)
            {
                //transfer the characters from input string to the char array
                rowArray[i] = input[i];
            }
            //add the chararray to 
          
            grid.push_back(rowArray);
        }
        mapWidth = input.length();
        mapHeight = grid.size();
    }
    void Print()
    {
        // for (char* row : grid)
         //{
        // cout << row <<endl;
         //}


        for (int j = 0; j < grid.size(); j++) {
            for (int i = 0; i < input.length(); i++) {
                cout << grid[j][i];
            }
            cout << endl;
        }
        FindPlayer();

    }
    void FindPlayer()
    {

        for (int j = 0; j < grid.size(); j++)
        {
            for (int i = 0; i < input.length(); i++)
            {
                char a = grid[j][i];


                if (a == player)
                {
                    cout << "Player is located in (row, column): " << endl;
                    Coordinate player;
                    player.x_coordinate = i;
                    player.y_coordinate = j;
                    cout << player.y_coordinate << ", " << player.x_coordinate << endl;
                    
                }
            }
        }
        FindExits();
    }
    void FindExits()
    {
        cout << "Exit(s) located in (row, column): " << endl;
        int* exitCoords = new int[4];
        char k = 0;

        for (int j = 0; j < grid.size(); j++)
        {
            for (int i = 0; i < input.length(); i++)
            {
                char a = grid[j][i];


                if (a == exit)
                {
                    switch (k) {
                    case 0:
                        Coordinate firstExit;
                        firstExit.x_coordinate = i;
                        firstExit.y_coordinate = j;
                        cout << firstExit.y_coordinate << ", " << firstExit.x_coordinate << endl;
                        k++;
                        break;

                    case 1:
                        Coordinate secondExit;
                        secondExit.x_coordinate = i;
                        secondExit.y_coordinate = j;
                        cout << secondExit.y_coordinate << ", " << secondExit.x_coordinate << endl;
                        k++;
                        break;
                    case 2:
                        Coordinate thirdExit;
                        thirdExit.x_coordinate = i;
                        thirdExit.y_coordinate = j;
                        cout << thirdExit.y_coordinate << ", " << thirdExit.x_coordinate << endl;
                        k++;
                        break;
                    }
                }
            }
        }

    }

    
};

int main()
{

    AStar executive;
    executive.ReadFile();
    executive.Print();
   // executive.FindPlayer();
    //executive.FindExits();
   
  
    return 0;
}

