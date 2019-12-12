#include<iostream>
using namespace std;

const int n = 7;

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

//Helper function to reset visisted array
void resetVisited (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetVisited(visited, i+1);
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

//To perform Breadth First Traversal
void breadthFirst (int G[n][n], int u, int visited[n]) {
	int v, viscount = 0;
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	enque(Q, u);
	visited[u] = 1;
	++viscount;
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

void courses (int G[n][n], int visited[n]) {
	int u, v, count = 0;
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	while(count != n) {
		for(u = 0; u < n; ++u) {
			if(indegree(G, u) == 0 && visited[u] == 0) {
				++count;
				visited[u] = 1;
				enque(Q, u);
			}
		}
		u = deque(Q);
		while(u != -1) {
			cout<<u+1 <<" ";
			for(v = 0; v < n; ++v) 
				G[v][u] = 0;
			u = deque(Q);
		}
	}
}

int main ()
{
	int G[n][n] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{1,0,1,0,0,0,0},{1,1,0,0,0,0,0},{0,0,0,1,0,0,0},{0,0,0,0,1,1,0}}; 
	int i, visited[n];
	resetVisited(visited);
	for(i = 0; i < n; ++i) 
		cout<<"\nDegrees of " <<i+1 <<" : " <<indegree(G, i) <<", " <<outdegree(G, i);
	lqueue Q;
    breadthFirst(G, 0, visited);
    resetVisited(visited);
    cout<<"\n";
    courses(G, visited);
	return 0;
}

