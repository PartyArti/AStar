
#include <iostream>
//filestream
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using std::cout;
using std::cin;
using std::endl;
using std::string;

using std::ifstream;
using std::vector;

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
    int map;



    //characters in the file
    char player = '^';
    char block = '#';
    char freeSpace = ' ';
    char exit = 'E';
    //characters to be added on the grid
    char possibleRoute = 'M';
    char optimalRoute = 'B';

    //select maze with user input
    void SelectFile()
    {
        cout << "Please select file by inputting 1 or 2:" << endl;
        cin >> map;
        //if input is not an integer clear input and ask the user for new input until an integer is given
        while (std::cin.fail()) {
            std::cout << "Invalid input" << std::endl;
            cout << "Please select file by inputting 1 or 2:" << endl;
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cin >> map;
        }
        ReadFile();
    }
    void ReadFile()
    {
        if (map == 1){
            cout << "maze1 selected" << endl;
            ifstream file("../maze1.txt");
          
            if (!file.is_open())             //if there is an error opening the file give error message
            {
                cout << "error" << endl;
            }

            while (getline(file, input)) //read lines from file
            {
               
                char* rowArray = new char[input.length() + 1];  //create a chararray
               
                for (int i = 0; i < input.length() + 1; i++)  //use length of input because there is no length of chararray yet
                {
                   
                    rowArray[i] = input[i];  //transfer the characters from input string to the char array
                }
               
                grid.push_back(rowArray);  //add the chararray to grid
            }
        }
        else if (map == 2){
            cout << "maze2 selected" << endl;
            ifstream file("../maze2.txt");

            if (!file.is_open())             //if there is an error opening the file give error message
            {
                cout << "error" << endl;
            }
            while (getline(file, input)) //read lines from file
            {
             
                char* rowArray = new char[input.length() + 1];    //create a chararray
                //use legth of input because there is no length of chararray yet
                for (int i = 0; i < input.length() + 1; i++)
                {
                    //transfer the characters from input string to the char array
                    rowArray[i] = input[i];
                }
                //add the chararray to 

                grid.push_back(rowArray);
            }
        }
        else //if input was another integer
        {
            cout << "no file selected" << endl;
            SelectFile();

        }
       

      
        mapWidth = input.length();
        mapHeight = grid.size();
    }
    void PrintGrid()
    {
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
    executive.SelectFile();
   
    executive.PrintGrid();


   // executive.FindPlayer();
    //executive.FindExits();
   
  
    return 0;
}

