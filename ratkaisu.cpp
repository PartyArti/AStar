
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <list>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::list;



class AStar
{

public:

    vector <char*> grid;

    string input;


    struct Node
    {
        //nodes will be listed with an index: y * mapWidth + x
        int x;
        int y;
        char mapSymbol;
        float fLocalGoal; // g
        float fGlobalGoal; // h
        bool obstacle; //if node is an obstacle
        bool visited; //if node is already visited
        bool hasParent;// = false;

        vector <Node*> vecNeighbours; //possible next nodes, vector of pointers
        Node* parent = nullptr; //previous node, pointer
    };



    list <float> bestNodes = {0};
    Node* newMap = nullptr;
    Node* nodes = nullptr;
    Node* nodeStart = nullptr; //starting node, where the player is located in the file but found later
    Node* nodeEnd = nullptr; //goal node

    Node* firstExit;
    Node* secondExit;
    Node* thirdExit;


    int mapWidth;
    int mapHeight;
    int map;
    int numberOfExits = 0;
    int goodNode;



    //characters in the file
    char player = '^';
    char block = '#';
    char freeSpace = ' ';
    char exit = 'E';
    
    char route = 'R';
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
        //define the width and height based on the read file
        mapWidth = input.length();
        mapHeight = grid.size();
        CreateMap();



    }


    void CreateMap() {

        nodes = new Node[mapWidth * mapHeight];

        for (int y = 0; y < mapHeight; y++)
        {

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
                nodes[y * mapWidth + x].x = x;
                nodes[y * mapWidth + x].y = y;
                if (a == block) nodes[y * mapWidth + x].obstacle = true;
                else nodes[y * mapWidth + x].obstacle = false;
                nodes[y * mapWidth + x].parent = nullptr;
                nodes[y * mapWidth + x].visited = false;
                nodes[y * mapWidth + x].mapSymbol = a;
                if (a == player)
                {
                    nodes[y * mapWidth + x].mapSymbol = '^';
                    Node player = nodes[y * mapWidth + x];
                    nodeStart = &player;


                }
                else if (a == exit)
                {
                    switch (numberOfExits) {
                    case 0:
                        nodes[y * mapWidth + x].mapSymbol = exit;
                        firstExit = &nodes[y * mapWidth + x];

                        // cout << firstExit.x << firstExit.y << endl;
                        numberOfExits++;
                        break;

                    case 1:
                        nodes[y * mapWidth + x].mapSymbol = exit;

                        secondExit = &nodes[y * mapWidth + x];


                        numberOfExits++;
                        break;
                    case 2:

                        nodes[y * mapWidth + x].mapSymbol = block;
                        thirdExit = &nodes[y * mapWidth + x];

                        numberOfExits++;
                        break;
                    }
                }
                else if (a == block) { nodes[y * mapWidth + x].mapSymbol = '#'; }
            }

        }

        FindNeighbours();
        if (map == 1) {
            cout << "Please Select an exit (1, 2 or 3): ";
            cin >> numberOfExits;
        }
        //while (numberOfExits >= 0)
        //{

            switch (numberOfExits)

            {
            case 3:
                nodeEnd = thirdExit;
                SolveAStar();

                break;
            case 2:
                nodeEnd = secondExit;
                SolveAStar();

                break;
            case 1:
                nodeEnd = firstExit;
                SolveAStar();

                break;
            default:
             //   cout << "error/no more exits" << endl;
                break;
            }
         //   numberOfExits--;
          //  cout << numberOfExits << endl;
        //}

    }


    void DrawMap()
    {

        for (int y = 0; y < mapHeight; y++)
        {

            for (int x = 0; x < mapWidth; x++) {
                cout << nodes[y * mapWidth + x].mapSymbol;
            }
            cout << endl;
        }
        cout << "if an 'E' has been replaced by an 'R' Pentti found his way out of the maze" << endl;
    }

    void FindNeighbours() {
        for (int x = 0; x < mapWidth; x++)
        {
            for (int y = 0; y < mapHeight; y++)
            {
                if (y > 0)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * mapWidth + (x + 0)]);
                if (y < mapHeight - 1)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * mapWidth + (x + 0)]);
                if (x > 0)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * mapWidth + (x - 1)]);
                if (x < mapWidth - 1)
                    nodes[y * mapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * mapWidth + (x + 1)]);
                };
            }
        }

    
   
  
    virtual bool SolvedMap()
    {
        //If the node has a parent, it is connected to other nodes, no node has a parent by default, so the first ones have to be connected to the starting node
        for (int x = 0; x < mapWidth; x++)
        {
            for (int y = 0; y < mapHeight; y++)

            {
                if (nodes[y * mapWidth + x].hasParent)// && nodes[y * mapWidth + x].mapSymbol != 'E') 
                  
                    nodes[y * mapWidth + x].mapSymbol = route;

            }
        }
        DrawMap();
        return 1;

    }

    void SolveAStar()
    {
        int z;
        cout << "Insert number of moves (20, 150 or 200 or any number..):";
        cin >> z;

      
            for (int y = 0; y < mapHeight; y++)
            {
                for (int x = 0; x < mapWidth; x++)
                {
                    nodes[y * mapWidth + x].visited = false; //no node is visited at first
                    nodes[y * mapWidth + x].fGlobalGoal = INFINITY; //any big number works
                    nodes[y * mapWidth + x].fLocalGoal = z; //user input
                    nodes[y * mapWidth + x].parent = nullptr;	// No parents
                    nodes[y * mapWidth + x].hasParent = false;
                }

            }
            
            auto distance = [](Node* a, Node* b) // Use Pythagoras theorem to get distance
            {
                return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
            };
            auto heuristic = [distance](Node* a, Node* b) //calculate distance between nodes
            {
                return distance(a, b);
            };


            //rest of this function is mostly copied from here: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PathFinding_AStar.cpp
            Node* nodeCurrent = nodeStart;
            nodeStart->fLocalGoal = 0.0f;
            nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

            list <Node*> listNotTestedNodes;

            listNotTestedNodes.push_back(nodeStart);

            //check that there are neighbours
            if (nodes[nodeCurrent->y * mapWidth + nodeCurrent->x].vecNeighbours.empty())
            {
                cout << "error, no existing neighbours" << endl;
            }
            else
            {
                //cout << "number of neighbours = " << nodes[nodeCurrent->y * mapWidth + nodeCurrent->x].vecNeighbours.size() << endl;
                //cout << nodeCurrent->y << nodeCurrent->x << endl;
            }
            while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
            {
                // Sort Untested nodes by global goal, so lowest is first
                listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

                // Front of listNotTestedNodes is potentially the lowest distance node. Our
                // list may also contain nodes that have been visited, so ditch these...
                while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
                    listNotTestedNodes.pop_front();

                // ...or abort because there are no valid nodes left to test
                if (listNotTestedNodes.empty())
                    break;

                nodeCurrent = listNotTestedNodes.front();
                nodeCurrent->visited = true; // We only explore a node once

                Node neighbour = nodes[nodeCurrent->y * mapWidth + nodeCurrent->x];



                for (auto nodeNeighbour : neighbour.vecNeighbours)
                {
                    // ... and only if the neighbour is not visited and is 
                    // not an obstacle, add it to NotTested List
                    if (!nodeNeighbour->visited && nodeNeighbour->mapSymbol != '#')
                        listNotTestedNodes.push_back(nodeNeighbour);

                    // Calculate the neighbours potential lowest parent distance
                    float fLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

                    // If choosing to path through this node is a lower distance than what 
                    // the neighbour currently has set, update the neighbour to use this node
                    // as the path source, and set its distance scores as necessary

                    if (fLowerGoal < nodeNeighbour->fLocalGoal)
                    {


                        nodeNeighbour->parent = nodeCurrent;
                        nodeCurrent->hasParent = true;


                        nodeNeighbour->fLocalGoal = fLowerGoal;


                        // The best path length to the neighbour being tested has changed, so
                        // update the neighbour's score. The heuristic is used to globally bias
                        // the path algorithm, so it knows if its getting better or worse. At some
                        // point the algo will realise this path is worse and abandon it, and then go
                        // and search along the next best path.
                        nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
                        


                    }
                }

              
            

          
        }  
    }
};



int main()
{
    AStar executive;
    executive.SelectFile();
    executive.SolvedMap();

}


