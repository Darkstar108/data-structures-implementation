#include<iostream>
using namespace std;

const int n = 9;

struct edge {
	int dist;
	int cost;
	int u;
	int v;
};

struct dqueue {
	struct dnode *f;
	struct dnode *r;
};

struct dnode {
	struct dnode *l;
	edge E;
	struct dnode *r;
};
typedef struct dnode *DPTR;

//Enque front
void enqueFront (dqueue &DQ, edge E) {
	DPTR T = new(dnode);
	T->E = E;
	if(DQ.f == NULL) {
		DQ.r = T;
		T->r = NULL;
	}
	else {
		T->r = DQ.f;
		DQ.f->l = T;
	}
	DQ.f = T;
	T->l = NULL;
}

//Enque rear
void enqueRear (dqueue &DQ, edge E) {
	DPTR T = new(dnode);
	T->E = E;
	if(DQ.f == NULL) {
		DQ.f = T;
		T->l = NULL;
	}
	else {
		DQ.r->r = T;
		T->l = DQ.r;
	}
	DQ.r = T;
	T->r = NULL;
}

//Deque front 
int dequeFront (dqueue &DQ) {
	if(DQ.f == NULL)
		return -1;
	DPTR T = DQ.f;
	int x = DQ.f->E.v;
	if(DQ.f == DQ.r) 
		DQ.f = DQ.r = NULL;
	else {
		DQ.f = T->r;
		DQ.f->l = NULL;
	}
	delete(T);
	return x;
}

//Helper function to reset edge info
void initialiseEdges (edge E[n], int i = 0) {
	if(i == n)
		return;
	E[i].dist = 62000;
	E[i].v = i;
	E[i].u = -1;
	E[i].cost = -1;
	initialiseEdges(E, i+1);
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

//Copy Matrix G to C
void copyMatrix(int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}


//Find shortest path using 0-1 BFS
void zeroOneBFS (int G[n][n], int u, edge E[n], dqueue Q) {
	if(u == -1)
		return;
	int v;
	if(E[u].dist == 62000) {
		E[u].u = u;
		E[u].dist = 0;
	}
	for(v = 0; v < n; ++v) {
		if(G[v][u] > 0) {
			if(E[u].dist + G[v][u] < E[v].dist) {
				E[v].dist = E[u].dist + G[v][u];
				E[v].u = u;
				E[v].cost = G[v][u];
				if(E[v].cost == 1)
					enqueFront(Q, E[v]);
				else
					enqueRear(Q, E[v]);
			}
		}
	}
	u = dequeFront(Q);
	zeroOneBFS(G, u, E, Q);
}

int main ()
{
	int G[n][n] = {{0,1,0,0,0,0,0,2,0},{1,0,2,0,0,0,0,2,0},{0,2,0,1,0,1,0,0,2},{0,0,1,0,2,2,0,0,0},
		{0,0,0,2,0,2,0,0,0},{0,0,1,2,2,0,2,0,0},{0,0,0,0,0,2,0,2,2},{2,2,0,0,0,0,2,0,2},{0,0,2,0,0,0,2,2,0}}; 
	int C[n][n], i;
	edge E[n];
	initialiseEdges(E);
	dqueue Q;
	Q.f = Q.r = NULL;
	copyMatrix(G, C);
	zeroOneBFS(C, 1, E, Q);
	for(i = 0; i < n; ++i) 
		cout<<E[i].u <<" - " <<E[i].v <<" : " <<E[i].dist <<"\n";
	return 0;
}

