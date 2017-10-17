#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <stack>

using namespace std;

const int GRIDSIZE = 6;

class Node
{
public:
	bool isWalkable;
	bool isFinished;
	int xPos, yPos;
	double GCost, HCost, FCost;
	//GCost = dist from start 
	//HCost = dist from end
	//FCost = HCost + GCost
	Node * parent;

	Node(int x, int y)
	{
		xPos = x;
		yPos = y;
		isWalkable = true;
		isFinished = false;
	}
	void reset()
	{
		GCost = HCost = FCost = 0;
		isFinished = false;
		parent = nullptr;
	}
};

class NodeComparer
{
public:
	/*bool operator()(Node a, Node b)
	{
		if (a.FCost == b.FCost)
			return a.HCost > b.HCost;
		return a.FCost > b.FCost;
	}
	bool operator()(Node & a, Node & b)
	{
		if (a.FCost == b.FCost)
			return a.HCost > b.HCost;
		return a.FCost > b.FCost;
	}*/
	bool operator()(const Node * a, const Node * b)
	{
		if (a->FCost == b->FCost)
			return a->HCost > b->HCost;
		return a->FCost > b->FCost;
	}
};
class Grid
{
private:
	vector<vector<shared_ptr<Node>>> grid;
public:
	Grid()
	{
		for (int x = 0; x < GRIDSIZE; x++)
		{
			vector<shared_ptr<Node>> vtemp;
			grid.push_back(vtemp);
			for (int y = 0; y < GRIDSIZE; y++)
			{
				shared_ptr<Node> ntemp(new Node(x, y));
				grid[x].push_back(ntemp);
			}
		}
		
		grid[1][4]->isWalkable = false;
		grid[2][1]->isWalkable = false;
		grid[2][2]->isWalkable = false;
		grid[2][3]->isWalkable = false;
		grid[2][4]->isWalkable = false;
		grid[4][3]->isWalkable = false;
		

	}

	void printGrid()
	{
		for (unsigned int x = 0; x < grid.size(); x++)
		{
			for (unsigned int y = 0; y < grid[x].size(); y++)
			{
				if (grid[x][y]->isWalkable)
					cout << "[ ]";
				else
					cout << "[-]";
			}
			cout << endl;
		}
	}

	void resetGrid()
	{
		for (unsigned int x = 0; x < grid.size(); x++)
			for (unsigned int y = 0; y < grid[x].size(); y++)
				grid[x][y]->reset();
	}

	stack<Node *> redrawPath(Node * end)
	{
		stack<Node *> temp;
		Node * curr = end;
		while (curr != nullptr)
		{
			temp.push(curr);
			curr = curr->parent;
		}
		return temp;
	}

	Node * findShortestPath(int startX, int startY, int endX, int endY)
	{
		resetGrid();
		priority_queue <Node *, vector<Node *>, NodeComparer> openSet;
		if (startX > GRIDSIZE || startY > GRIDSIZE || endX > GRIDSIZE || endY > GRIDSIZE)
			return nullptr;
		Node * start = (grid[startX][startY].get());
		calculateDists(start, startX, startY, endX, endY);
		openSet.push(start);

		while (!openSet.empty())
		{
			Node * curr = openSet.top();
			openSet.pop();
			curr->isFinished = true;
			//cout << "curr (" << curr->xPos << "," << curr->yPos << ")" << endl << "G:" << curr->GCost << endl << "H:" << curr->HCost << endl << "F:" << curr->FCost << endl;
			if (curr->HCost == 0)
				return curr;
			else
			{
				vector<Node *> temp = getAdjacentNodes(curr->xPos, curr->yPos);
				for (int x = 0; x < temp.size(); x++)
				{
					//cout << "Adding (" << temp[x]->xPos << "," << temp[x]->yPos << ")" << endl;
					calculateDists(temp[x], startX, startY, endX, endY);
					temp[x]->parent = curr;
					temp[x]->isFinished = true;
					openSet.push(temp[x]);
				}
			}
		}

	}
	


	void calculateDists(Node * temp, int startX, int startY, int endX, int endY)
	{
		temp->HCost = sqrt(pow(endX - temp->xPos, 2) + pow(endY - temp->yPos,2));
		temp->GCost = sqrt(pow(startX - temp->xPos, 2) + pow(startY - temp->yPos, 2));
		temp->FCost = temp->HCost + temp->GCost;
	}

	vector<Node *> getAdjacentNodes(int xpos, int ypos)
	{
		vector<Node *> temp;
		if (xpos != 0)
			temp.push_back(grid[xpos - 1][ypos].get());
		if (xpos != GRIDSIZE-1)
			temp.push_back(grid[xpos + 1][ypos].get());
		if (ypos != 0)
			temp.push_back(grid[xpos][ypos-1].get());
		if (ypos != GRIDSIZE-1)
				temp.push_back(grid[xpos][ypos+1].get());
		for (int x = 0; x < temp.size(); x++)
		{
			if (!temp[x]->isWalkable || temp[x]->isFinished)
			{
				temp.erase(temp.begin() + x);
				x--;
			}
		}
		return temp;
	}

};
 
int main()
{
	Grid temp;
	temp.printGrid();

	Node * temp2 = temp.findShortestPath(5, 0, 0, 5);
	stack<Node *> path = temp.redrawPath(temp2);
	cout << "The Path" << endl;
	int counter = 0;
	while (!path.empty())
	{
		counter++;
		cout << "Step " << counter << ": (" << path.top()->xPos << ", " << path.top()->yPos << ")" << endl;
		path.pop();
	}

	system("pause");
	return 0;
}