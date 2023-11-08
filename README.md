# Pathfinding-Algorithm-Visualizer
Welcome to the Pathfinding Algorithm Visualizer! I built this application because while learning about pathfinding algorithms as a beginner to programming, I was amazed that the algorithms I was learning were actually the foundations of navigation software. This amazement is what caused me to create this project as I wanted to visually observe how they would interact given certain conditions. You can download this code from the repository and set it up in your own IDE to run the program (make sure to link the appropriate SDL files before running the program), I hope you enjoy this visualization program as much as I enjoyed creating it.

## Algorithms 
**Breadth First Search (BFS)** - This is an unweighted search algorithm that ensures the shortest path however, it is not very efficient.  
**Djikstra's Algorithm** - This is a weighted search algorithm that also ensures the shortest path in a non efficient way simillar to Breadth First Search. This is also the foundational algorithm that all other search algorithms are created from.  
**A star Algorithm** - This is a weighted search algorithm that uses heuristics to gurantee a shortest path in a more efficient way than Djikstra's algorithm. This is arguably the best search algorithm however requires you to know the position of your end node prior to the search.

## Heuristics
**Manhattan Distance** - A simplistic heuristic that is great for 4 way movement on grids. Calculated using the formula: h(n) = |n.x - goal.x| + |n.y - goal.y|.  
**Diagonal Distance** - This heuristic is designed for 8 way movement on grids and requires that the cost of moving diagonally is the same as the cost of moving laterally. Calculated using the formula: h(n) = cost * max(|n.x - goal.x|, |n.y - goal.y|).  
**Euclidean Distance** - This heuristic is designed for grids that allow for movement at any angle. It is quite an inefficient heuristic if applied to 8 way or 4 way movement on a grid. Calculated using the formula: h(n) = sqrt((n.x - goal.x)^2 + (n.y - goal.y)^2)

## Using the Program
![image](https://user-images.githubusercontent.com/65980644/97165622-5f66cc00-175a-11eb-9300-97ac0ffc4cab.png)
I designed this program with user friendly controls to allow for easy use of this program. On the right hand side of the screen there are buttons that allow you to select different algorithms as well as heuristics for the Astar algorithm. The rest of the controls are detailed below:   
**Setting Start Node** - Hold the SHIFT key while left-clicking on any square in the grid.  
**Setting End Node** - Hold the Left-CTRL key while left-clicking on any square in the grid.  
**Setting Obstacles** - Hold W while left-clicking on any square in the grid.  
**Setting Weighted Node** - Hold Q while left-clicking on any square in the grid (this feature is only available to the weighted algorithms Astar and Djikstra's). Weighted nodes are nodes that the algorithm will try to avoid as they "cost" more to traverse through, the darker the weight the more it will be avoided. This can be thought of simillar to how navigation software would avoid heavy traffic areas when recommending the fastest route.
