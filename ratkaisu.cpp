
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <list>

using std::cout;
using std::cin;
using std::endl;
using std::string;

using std::ifstream;
using std::vector;

using std::list;



class AStar
{


    vector <char*> grid;



    string input;


    struct Node
    {
        int x_coordinate;
        int y_coordinate;
        float fGlobalGoal;
        float fLocalGoal;
        bool obstacle; //if node is an obstacle
        bool visited; //if node is already visited
        vector <Node*> vecNeighbours; //possible next nodes, vector of pointers
        Node* parent; //previous node, pointer
    };


    list <Node*> listNotTestedNodes;

    Node* nodes = nullptr; //general node
    Node* nodeStart = nullptr; //starting node, where the player is located in the file but found later
    Node* nodeEnd = nullptr; //goal node

    Node firstExit;
    Node secondExit;
    Node thirdExit;

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
        while (cin.fail()) {
            cout << "Invalid input" << endl;
            cout << "Please select file by inputting 1 or 2:" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> map;
        }
        ReadFile();
    }
    void ReadFile()
    {
        if (map == 1) {
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
        else if (map == 2) {
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
        OnUserCreate();

        //SolveAStar();
       // Draw();

    }
    void PrintGrid()
    {
        for (int j = 0; j < grid.size(); j++) {
            for (int i = 0; i < input.length(); i++) {
                cout << grid[j][i];
            }
            cout << endl;
        }
        //    FindPlayer();

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
                    nodeStart->x_coordinate = i;
                    nodeStart->y_coordinate = j;
                    cout << nodeStart->y_coordinate << ", " << nodeStart->x_coordinate << endl;

                }
            }
        }
      
    }

    virtual bool OnUserCreate() {
        char a;
        int k = 0;
        nodes = new Node[mapWidth * mapHeight];
        for (int y = 0; y < mapHeight; y++)
            for (int x = 0; x < mapWidth; x++)
            {
                char a = grid[y][x];
                if (x != mapWidth - 1)
                {

                    cout << a;
                }
                else {
                    cout << a << endl;

                }
                nodes[y * mapWidth + x].x_coordinate = x;
                nodes[y * mapWidth + x].y_coordinate = y;
                if (a == block) nodes[y * mapWidth + x].obstacle = true;
                else nodes[y * mapWidth + x].obstacle = false;
                nodes[y * mapWidth + x].parent = nullptr;
                nodes[y * mapWidth + x].visited = false;
                if (a == player)
                {
                    nodeStart = nodes;

                }
                if (a == exit)
                {
                    switch (k) {
                    case 0:
                        firstExit.x_coordinate = x;
                        firstExit.y_coordinate = y;

                       // cout << firstExit.x_coordinate << firstExit.y_coordinate << endl;
                        k++;
                        break;

                    case 1:

                        secondExit.x_coordinate = x;
                        secondExit.y_coordinate = y;
                      //  cout << secondExit.x_coordinate + ", " << secondExit.y_coordinate << endl;
                        k++;
                        break;
                    case 2:
                        thirdExit.x_coordinate = x;
                        thirdExit.y_coordinate = y;
                     //   cout << thirdExit.x_coordinate + ", " << thirdExit.y_coordinate << endl;
                        k++;
                        break;
                    }
                }
                
            }
        cout << "Exit(s) are located in (row,column):" << endl;
        cout << firstExit.y_coordinate << +", " << firstExit.x_coordinate << endl;
        if (k > 1)
        {
            cout << secondExit.y_coordinate << +", " << secondExit.x_coordinate << endl;
            cout << thirdExit.y_coordinate << +", " << thirdExit.x_coordinate << endl;
        }

     /*   for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++)
            {
                if (y > 0)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * mapWidth + (x + 0)]);
                if (y < mapHeight - 1)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * mapWidth + (x + 0)]);
                if (x > 0)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * mapWidth + (x - 1)]);
                if (x < mapWidth - 1)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * mapWidth + (x + 1)]);
            }
        }*/

        return true;
    }


};

int main()
{

    AStar executive;
    executive.SelectFile();
    //executive.SolveAStar();
    executive.FindPlayer();
 

   // executive.FindPlayer();
    //executive.FindExits();
   
  
    return 0;
}

