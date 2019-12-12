#include<iostream>
using namespace std;

const int n = 5;

struct lqueue {
	struct lnode *f;
	struct lnode *r;
};

struct lnode {
	int data;
	struct lnode *next;
};
typedef struct lnode* LPTR;

//Enque
void enque (lqueue &Q, int T) {
	LPTR I = new(lnode);
	I->data = T;
	if(Q.r == NULL) {
		Q.r = I;
		Q.f = I;
	}
	else
		Q.r->next = I;
	Q.r = I;	
	I->next = NULL;	
}

//Deque
int deque (lqueue &Q) {
	LPTR I = Q.f;
	int T;
	if(Q.f == NULL)
		return -1;
	if(Q.f == Q.r) {
		Q.f = NULL;
		Q.r = NULL;
	}
	else
		Q.f = I->next;
	T = I->data;
	delete(I);
	return T;
}

//Count degree of vertex in Undirected Graph
int degree (int G[n][n], int v) {
	int i, degree = 0;
	for(i = 0; i < n; ++i) {
		if(G[v][i] == 1)
			++degree;
	}
	return degree;
}

//Count indegree of vertex
int indegree (int G[n][n], int v) {
	int i, indegree = 0;
	for(i = 0; i < n; ++i) {
		if(G[v][i] == 1)
			++indegree;
	}
	return indegree;
}

//Count outdegree of vertex
int outdegree (int G[n][n], int v) {
	int i, outdegree = 0;
	for(i = 0; i < n; ++i) {
		if(G[i][v] == 1)
			++outdegree;
	}
	return outdegree;
}

//Function to reset visisted array
void resetVisited (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetVisited(visited, i+1);
}

//Checks if visited array is full and returns 1st non visited vertex
int checkVisited (int visited[n]) {
	int i, u = -1;
	for(i = 0; i < n; ++i) {
		if(visited[i] == 0) {
			u = i;
			break;
		}
	}
	return u;
}

//Copy visited array
void copyVisited (int visited[n], int c[n]) {
	int i;
	for(i = 0; i < n; ++i)
		c[i] = visited[i];
}

//Copy Matrix G to C
void copyMatrix(int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Check if there is a cycle in Graph
int checkCycle (int G[n][n], int u, int visited[n], int prev = -1) {
	int v;
	visited[u] = 1;
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1) {
			if(visited[v] == 1 && v != prev && prev != -1)
				return 1;
			else if (visited[v] != 1)
				return checkCycle(G, v, visited, u);
		}
	}
	return 0;
}


//To check if edge is a bridge
int checkBridge (int G[n][n], int u, int v) {
	int visited[n], i;
	resetVisited(visited);
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	enque(Q, v);
	visited[v] = 1;
	v = deque(Q);
	while(v != -1) {
		if(v == u)
			return -1;
		for(i = 0; i < n; ++i) {
			if(G[i][u] == 1 && visited[i] != 1) {
				enque(Q, i);
				visited[i] = 1;
			}
		}
		v = deque(Q);
	}
	return 1;
}

//To perform Breadth First Traversal
void breadthFirst (int G[n][n], int u, int visited[n]) {
	int v;
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	enque(Q, u);
	visited[u] = 1;
	u = deque(Q);
	while(u != -1) {
		for(v = 0; v < n; ++v) {
			if(G[v][u] == 1 && visited[v] != 1) {
				cout<<"\n" <<u+1 <<"->" <<v+1;
				enque(Q, v);
				visited[v] = 1;
			}
		}
		u = deque(Q);
	}
}

//Performs DFT
void depthFirstHelper (int G[n][n], int u, int visited[n], int prev = -1) {
	int v, count = 0;
	visited[u] = 1;
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1 && visited[v] != 1) {
			++count;
			cout<<"\n" <<u+1 <<"->" <<v+1;
			depthFirstHelper(G, v, visited, u);
		}
	}
	if(count == 0 && prev == -1)
		cout<<"\n"<<u+1;
}

//Controls DFT
void depthFirst (int G[n][n], int u, int visited[n]) {
	while(u != -1) {
		depthFirstHelper(G, u, visited);
		u = checkVisited(visited);
	}
}

int main ()
{
	int G[n][n] = {{0,0,0,1,0},{1,0,0,0,0},{1,1,0,1,0},{0,0,0,0,0},{0,1,0,1,0}}; //Directed Acyclic (1-2,1-3,2-3,2-5,4-1,4-3,4-5
	//int G[n][n] = {{0,0,1,1,0},{1,0,0,0,0},{0,1,0,0,1},{0,0,1,0,0},{0,1,0,1,0}}; //Directed Cyclic (1-2,2-3,2-5,3-4,4-1,4-5,5-3
	//int G[n][n] = {{0,1,1,1,0},{1,0,1,0,1},{1,1,0,1,0},{1,0,1,0,1},{0,1,0,1,0}}; //Undirected(1-2,1-3,1-4,2-3,2-5,3-4,4-5
	int i, visited[n];
	resetVisited(visited);
	for(i = 0; i < n; ++i) 
		cout<<"\nDegrees of " <<i+1 <<" : " <<indegree(G, i) <<", " <<outdegree(G, i);
	lqueue Q;
    cout<<"\nCycle?: " <<checkCycle(G, 0, visited);
    resetVisited(visited);
    breadthFirst(G, 0, visited);
    resetVisited(visited);
	cout<<"\n-----------------------------";
    depthFirst(G, 0, visited);
	return 0;
}

