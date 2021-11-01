# AStar
My solution to a task where you have to find your way out of a 2D maze using an algorithm. I'm using A*
For the calculation part I used this repo: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PathFinding_AStar.cpp

First the program prompts the user to select a file, after which it reads it and saves the lines into a vector of charArrays (grid). Then it draws a map from the grid and simultaniously checks if the characters are something else than freespace and saves them to an array of structs. Then the user is asked about the exits if they chose file 1 and then about the number of moves. For some reason the third exit in map one turns into a wall after being printed the first time.

 The routes start from the player's node and spread out in every possible direction. If you wanted to make an optimal route you would have to compare the local and global goal values of the nodes more than was done here.
 
 The maze-files should be in the same folder as the other files.
