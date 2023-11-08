#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include <list>



class Graph {
public:
	struct Node {
		bool wall; //Testing for if node is a wall
		bool visited;
		int local; //Distance from current node to the start node
		double cost; //Sum of and local and heuristic distance from current node to end node
		int col; //x coordinate
		int row; //y coordinate
		std::vector<Node*> neighbours;
		Node* parent;
		Node();
		int weight;
	};
	Graph(); //Function creates array of Node objects and creates connections to its neighbouring nodes
	int graph_Width = 16;
	int graph_Height = 16;
	void set_Start(int x, int y);
	void set_End(int x, int y);
	void switch_Wall(int x, int y);
	void switch_Weight(int x, int y);
	inline Node* get_Nodes() { return nodes; }
	inline Node* get_Start() { return s_Node; }
	inline Node* get_End() { return e_Node; }
	bool Astar(); //Astar pathfinding algorithm
	bool BFS();
	bool Djikstra();
	int h; //Tracks the herustic to use
private:
	Node* nodes;
	Node* s_Node; //Start Node
	Node* e_Node; //End Node
	double heuristic(Node* s_Node, Node* e_Node); //Determines which heuristic to use
	double manhattan(Node* a, Node* b); //Manhattan distance heuristic	
	double euclidean(Node* a, Node* b); //Euclidian distance heuristic	
	double dia_Distance(Node* a, Node* b); //Diagonal distance heuristic	
	bool contained(Node* neighbour, std::list<Node*> q);
	
	
};

