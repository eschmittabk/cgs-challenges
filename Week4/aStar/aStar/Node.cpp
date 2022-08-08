#include <iostream>
#include "Node.h"

Node::Node(char _x, char _y, int _cost) :
	x(_x),
	y(_y),
	m_cost(_cost)
{

}

Node::~Node()
{

}

void Node::DisplayNode() {
	std::cout << "NODE: " << "\n * START: " << x << "\n * GOAL: " << y << "\n * COST: " << m_cost << std::endl;
}

bool Node::CompareNode(Node secondNode) {
	return this->x == secondNode.x &&
		this->y == secondNode.y &&
		this->m_cost == secondNode.m_cost;
}
/*
Edge::Edge(Node pointA, Node pointB, int cost) :
	m_pointA(pointA),
	m_pointB(pointB),
	m_cost(cost)
{

}

Edge::~Edge()
{

}

void Edge::DisplayEdge()
{

}
*/