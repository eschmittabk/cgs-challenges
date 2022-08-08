#pragma once

struct Node {
	char x;
	char y;
	int m_cost;
	//int m_fScore;
	//int m_gScore;
	//int m_hScore;

	Node(char _x, char _y, int _cost);
	~Node();

	void DisplayNode();
	bool CompareNode(Node secondNode);
};
/*
struct Edge {
	Node m_pointA;
	Node m_pointB;
	int m_cost;

	Edge(Node pointA, Node pointB, int _cost);
	~Edge();

	void DisplayEdge();
};
*/