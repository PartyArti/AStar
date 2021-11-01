
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
        vector <Node*> vecNeighbours; //possible next nodes, vector of pointers
        Node* parent = nullptr; //previous node, pointer
    };

    list <Node*> bestNodes;


    Node *nodes = nullptr; //general node
    Node *nodeStart = nullptr; //starting node, where the player is located in the file but found later
    Node *nodeEnd = nullptr; //goal node

    Node firstExit;
    Node secondExit;
    Node thirdExit;

public:
    int mapWidth;
    int mapHeight;
    int map;
    int numberOfExits = 0;



    //characters in the file
    char player = '^';
    char block = '#';
    char freeSpace = ' ';
    char exit = 'E';

    char visitedNode = 'V';
    char optimalNode = 'O';

    void ReadFile()
    {

        ifstream file("../maze2.txt");

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




        mapWidth = input.length();
        mapHeight = grid.size();
        CreateMap();
        DrawMap();


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
                if (a == exit)
                {
                    switch (numberOfExits) {
                    case 0:
                        nodes[y * mapWidth + x].mapSymbol = 'E';
                        firstExit = nodes[y * mapWidth + x];

                        // cout << firstExit.x << firstExit.y << endl;
                        numberOfExits++;
                        break;

                    case 1:
                        nodes[y * mapWidth + x].mapSymbol = 'E';
                        secondExit = nodes[y * mapWidth + x];

                        numberOfExits++;
                        break;
                    case 2:
                        nodes[y * mapWidth + x].mapSymbol = 'E';
                        thirdExit = nodes[y * mapWidth + x];
                        numberOfExits++;
                        break;
                    }
                }
                if (a == block) { nodes[y * mapWidth + x].mapSymbol = '#'; }

            }
            for (int y = 0; y < mapHeight; y++)
            {
                for (int x = 0; x < mapWidth; x++)
                {
                    nodes[y * mapWidth + x].visited = false;
                    nodes[y * mapWidth + x].fGlobalGoal = INFINITY;
                    nodes[y * mapWidth + x].fLocalGoal = INFINITY;
                    nodes[y * mapWidth + x].parent = nullptr;	// No parents
                }
               
            }
        }
      
        FindNeighbours();
       
        while (numberOfExits >= 0)
        {
         
            switch (numberOfExits)

            {
            case 3:
                nodeEnd = &thirdExit;
                SolveAStar();
               
                break;
            case 2:
                nodeEnd = &secondExit;
                SolveAStar();
              
                break;
            case 1:
                nodeEnd = &firstExit;
                
                SolveAStar();
               
                break;
            default:
                cout << "error/no more exits" << endl;
                break;
            }
            numberOfExits--;
            cout << "here" << endl;
            cout << numberOfExits << endl;
        }

    }

   
   void DrawMap()
    {
       
        for (int y = 0; y < mapHeight; y++)
        {

            for (int x = 0; x < mapWidth; x++) {
                cout << nodes[y * mapWidth + x].mapSymbol;
            }
            cout<< endl;
            }
    }
   public :
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
               cout << nodes[y * mapWidth + x].vecNeighbours.size() << endl;
           }
       }

   }
  

   

   void SolveAStar()
   {
       cout << nodeStart->vecNeighbours.size() << endl;

       cout << "blaa" << endl;
       auto distance = [](Node* a, Node* b) // Use Pythagoras theorem to get distance
       {
           return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
       };
       auto heuristic = [distance](Node* a, Node* b) //calculate distance between nodes
       {
           return distance(a, b);
       };

      

       Node *nodeCurrent = nodeStart;
    

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
       cout << "number of neighbours = " << nodes[nodeCurrent->y * mapWidth + nodeCurrent->x].vecNeighbours.size() << endl;
       cout << nodeCurrent->y << nodeCurrent->x << endl;
       }
       for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
       {
           cout << nodeCurrent->fLocalGoal << endl;
           // ... and only if the neighbour is not visited and is 
           // not an obstacle, add it to NotTested List
           if (!nodeNeighbour->visited && nodeNeighbour->mapSymbol == '#')
               listNotTestedNodes.push_back(nodeNeighbour);

           // Calculate the neighbours potential lowest parent distance
           float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);


           // If choosing to path through this node is a lower distance than what 
           // the neighbour currently has set, update the neighbour to use this node
           // as the path source, and set its distance scores as necessary
           if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)


               nodeNeighbour->parent = nodeCurrent;
           nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
           bestNodes.push_back(nodeCurrent);

           // The best path length to the neighbour being tested has changed, so
           // update the neighbour's score. The heuristic is used to globally bias
           // the path algorithm, so it knows if its getting better or worse. At some
           // point the algo will realise this path is worse and abandon it, and then go
           // and search along the next best path.
           nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);

       }

   
   
   
   
   
   
   
   
   
   
   }


};

int main()
{

    AStar executive;
    executive.DrawMap();
    executive.ReadFile();
    

    return 0;
}

