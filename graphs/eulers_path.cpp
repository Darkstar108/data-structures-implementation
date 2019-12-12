#include<iostream>
using namespace std;

const int n = 12;

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

//Counts Edges of Graph
int countEdges (int G[n][n]) {
	int i, j, edges = 0;
	for(i = 0; i < n; ++i) {
		for(j = i+1; j < n; ++j) {
			if(G[j][i] == 1)
				++edges;
		}
	}
	return edges;
}

//Copy Matrix G to C
void copyMatrix(int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Function to reset visisted array
void resetVisited (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetVisited(visited, i+1);
}

//Copy visited array
void copyVisited (int visited[n], int c[n]) {
	int i;
	for(i = 0; i < n; ++i)
		c[i] = visited[i];
}

//To check if edge is a bridge
int checkBridge (int G[n][n], int u, int v) {
	int visited[n], i, j;
	resetVisited(visited);
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	enque(Q, v);
	visited[v] = 1;
	j = deque(Q);
	while(j != -1) {
		if(j == u)
			return -1;
		for(i = 0; i < n; ++i) {
			if(j == v && i == u)
				continue;
			if(G[i][j] == 1 && visited[i] != 1) {
				enque(Q, i);
				visited[i] = 1;
			}
		}
		j = deque(Q);
	}
	return 1;
}

//To print Eulers Path
int printEulersPath (int G[n][n], int u, int edges, lqueue &Q, lqueue &T, int remEdges = 0, int flag = 0) {
	int v;
	enque(Q, u);
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1) {
			G[v][u] = 0;
			G[u][v] = 0;
			flag = printEulersPath(G, v, edges, Q, T, remEdges+1);
			break;
		}
	}
	//If all edges are not yet traversed
	if(remEdges != edges && flag == 0) {
		v = deque(T);
		while(v != -1) {
			enque(Q, v);
			v = deque(T);
		}
		v = deque(Q);
		while(degree(G, v) == 0) {
			enque(T, v);
			v = deque(Q);
		}
		//Swapping Queues
		lqueue Tmp;
		Tmp.f = T.f;
		Tmp.r = T.r;
		T.f = Q.f;
		T.r = Q.r;
		Q.f = Tmp.f;
		Q.r = Tmp.r;
		flag = printEulersPath(G, v, edges, Q, T, remEdges);
	}
	else if(remEdges == edges && flag == 0) {
		int i = deque(Q);
		//Printing Eulers Path
		while(i != -1) {
			cout<<i+1 <<" ";
			i = deque(Q);
		}
		i = deque(T);
		while(i != -1) {
			cout<<i+1 <<" ";
			i = deque(T);
		}
		return 1;
	}
	else
		return 1;
}

//To print all Eulers Paths
void printAllEulersPath (int G[n][n], int u, lqueue Q, int edges, int remEdges = 0) {
	int v;
	enque(Q, u);
	if(edges == remEdges) {
		int i = deque(Q);
		while(i != -1) {
			cout<<i+1 <<" ";
			i = deque(Q);
		}
		cout<<"\n";
		return;
	}
	for(v = 0; v < n; ++v) {
		//If there is an edge
		if(G[v][u] == 1) {
			//If degree is 1
			if(degree(G, u) == 1) {
				G[v][u] = 0;
				G[u][v] = 0;
				printAllEulersPath(G, v, Q, edges, remEdges+1);
				G[v][u] = 1;
				G[u][v] = 1;
			}
			//If degree is greater than 1
			else {
				//If it is not a bridge
				if(checkBridge(G, u, v) == -1) {
					G[v][u] = 0;
					G[u][v] = 0;
					printAllEulersPath(G, v, Q, edges, remEdges+1);
					G[v][u] = 1;
					G[u][v] = 1;
				}
			}
		}
	}
}
int main ()
{
	int G[n][n] = {{0,0,1,1,0,0,0,0,0,0,0,0},{0,0,1,0,0,0,0,1,0,0,0,0},
					{1,1,0,1,0,1,1,0,1,0,0,0},{1,0,1,0,1,0,1,0,0,1,1,0},
					{0,0,0,1,0,0,0,0,0,1,0,0},{0,0,1,0,0,0,0,0,1,0,0,0},
					{0,0,1,1,0,0,0,0,1,1,0,0},{0,1,0,0,0,0,0,0,1,0,0,0},
					{0,0,1,0,0,1,1,1,0,1,0,1},{0,0,0,1,1,0,1,0,1,0,1,1},
					{0,0,0,1,0,0,0,0,0,1,0,0},{0,0,0,0,0,0,0,0,1,1,0,0}};
	/**int G[n][n] = {{0,1,0,0,0,0,0,1},{1,0,1,1,0,0,1,0},
					{0,1,0,1,0,0,0,0},{0,1,1,0,0,0,0,0},
					{0,0,0,0,0,1,1,0},{0,0,0,0,1,0,1,0},
					{0,1,0,0,1,1,0,1},{1,0,0,0,0,0,1,0}};**/
	int C[n][n], visited[n];
    lqueue Q, T;
    Q.f = Q.r = T.f = T.r = NULL;
    int edges = countEdges(G);
	copyMatrix(G, C);
	printAllEulersPath(C, 0, Q, edges);
	return 0;
}

