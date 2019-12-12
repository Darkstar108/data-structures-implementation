#include<iostream>
using namespace std;

const int n = 7;

struct lqueue {
	struct lnode *f;
	struct lnode *r;
};


struct lstack {
	struct lnode *top;
};

struct lnode {
	int data;
	struct lnode *next;
};
typedef struct lnode* LPTR;

//Push
void push (lstack &S, int x) {
	LPTR T;
	T = new(lnode);
	T->data = x;
	T->next = S.top;
	S.top = T;
}

//Pop
int pop (lstack &S) {
	LPTR T = S.top;
	int x;
	if(S.top == NULL)
		return -1;
	S.top = T->next;
	x = T->data;
	delete(T);
	return x;
}

//Top
int Top (lstack S) {
	if(S.top == NULL)
		return -1;
	return S.top->data;
}

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

//Helper function to reset arrays
void resetArray (int visited[n], int dist[n], int prev[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	dist[i] = 62000;
	prev[i] = -1;
	resetArray(visited, dist, prev, i+1);
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

//Returns vertex with least distance
int leastDistance (int dist[n], int visited[n], int leastVertex = -1, int leastDist = 62000, int u = 0) {
	if(u == n)
		return leastVertex;
	if(dist[u] < leastDist && visited[u] == 0) {
		leastDist = dist[u];
		leastVertex = u;
	}
	leastDistance(dist, visited, leastVertex, leastDist, u+1);
}

//Print Dijkstra's shortest path
void dijkstra (int G[n][n], int visited[n], int dist[n], int prev[n], int s, int e) {
	//When we reach destination
	if(s == e) {
		lstack S;
		G[prev[s]][s] = 0;
		G[s][prev[s]] = 0;
		S.top = NULL;
		push(S, s);
		int i = prev[s];
		cout<<"\n";
		while(i != -1) {
			push(S, i);
			i = prev[i];
		}
		i = pop(S);
		while(i != -1) {
			cout<<i+1 <<" ";
			i = pop(S);
		}
		return;
	}
	if(dist[s] == 62000)
		dist[s] = 0;
	int v;
	for(v = 0; v < n; ++v) {
		if(G[v][s] > 0 && visited[v] == 0 && (dist[v] > dist[s] + G[v][s])) {
			dist[v] = dist[s]+G[v][s];
			prev[v] = s;
		}
	}
	visited[s] = 1;
	s = leastDistance(dist, visited);
	dijkstra(G, visited, dist, prev, s, e);
}

//Find nth shortest path using Dijkstra
void nthDijkstra (int G[n][n], int visited[n], int dist[n], int prev[n], int s, int e, int n) {
	int i = 0, j, k;
	while(i < n) {
		resetArray(visited, dist, prev);
		dijkstra(G, visited, dist, prev, s, e);
		++i;
	}
}

int main ()
{
	int G[n][n] = {{0,0,4,0,0,0,0},{2,0,0,0,0,0,0},{0,0,0,2,0,0,0},{1,3,0,0,0,0,0},
					{0,10,0,2,0,0,0},{0,0,5,8,0,0,1},{0,0,0,4,6,0,0}}; 
	int i, visited[n], dist[n], prev[n];
	int C[n][n];
	copyMatrix(G, C);
	resetArray(visited, dist, prev);
	nthDijkstra(C, visited, dist, prev, 0, 5, 2);
	return 0;
}

