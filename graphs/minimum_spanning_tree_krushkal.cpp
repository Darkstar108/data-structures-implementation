#include<iostream>
using namespace std;

const int n = 7;

struct edge {
	int u;
	int v;
	int cost;
};

//Find root of disjoint set
int find (int S[], int a) {
	if(S[a] == a)
		return a;
	return find(S, S[a]);
}

//Unify two disjoint sets
int unify (int S[], int a, int b) {
	a = find(S, a);
	b = find(S, b);
	if(a != b) {
		S[b] = a;
		return 1;
	}
	return -1;
} 

//Helper function to reset arrays
void resetArray (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetArray(visited, i+1);
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
void copyMatrix (int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Counts Edges of Graph
int countEdges (int G[n][n]) {
	int i, j, edges = 0;
	for(i = 0; i < n; ++i) {
		for(j = i+1; j < n; ++j) {
			if(G[j][i] > 0)
				++edges;
		}
	}
	return edges;
}

//Initialise edge structure
void intialiseEdges (int G[n][n], edge E[]) {
	int u, v, i = 0;
	for(u = 0; u < n; ++u) {
		for(v = u+1; v < n; ++v) {
			if(G[u][v] == 0)
				continue;
			E[i].u = u;
			E[i].v = v;
			E[i++].cost = G[u][v];
		}
	}
}

//Sort edges by cost
void sortEdgesCost (edge E[], int edges) {
	int i, j, min, tmp;
	for(i = 0; i < edges; ++i) {
		min = E[i].cost;
		tmp = i;
		for(j = i+1; j < edges; ++j) {
			if(E[j].cost < min) {
				min = E[j].cost;
				tmp = j;
			}
		}
		if(min != E[i].cost) {
			int tmpu = E[tmp].u, tmpv = E[tmp].v;
			E[tmp].cost = E[i].cost;
			E[tmp].u = E[i].u;
			E[tmp].v = E[i].v;
			E[i].cost = min;
			E[i].u = tmpu;
			E[i].v = tmpv;
		}
	}
}

//Print contents of edges
void printEdges (edge E[], int edges) {
	int i;
	for(i = 0; i < edges; ++i) 
		cout<<"\n" <<E[i].u+1 <<"-" <<E[i].v+1 <<" (" <<E[i].cost <<")";
}

//Minimum Spanning Tree (Krushkals Algorithm)
void minSpanTree (edge E[], int S[n]) {
	int u, v, count, i = 0;
	while(count < n-1) {
		u = E[i].u;
		v = E[i++].v;
		if(unify(S, u, v) == 1) {
			cout<<"\n" <<u+1 <<"-" <<v+1;
			++count;
		}
	}
}

int main ()
{
	int G[n][n] = {{0,2,4,1,0,0,0},{2,0,0,3,10,0,0},{4,0,0,2,0,5,0},{1,3,2,0,7,8,4},
					{0,10,0,7,0,0,6},{0,0,5,8,0,0,1},{0,0,0,4,6,1,0}}; 
	int i, C[n][n], S[n];
	for(i = 0; i < n; ++i)
		S[i] = i;
	const int edges = countEdges(G);
	edge E[edges];
	copyMatrix(G, C);
	intialiseEdges(C, E);
	sortEdgesCost(E, edges);
	printEdges(E, edges);
	minSpanTree(E, S);
	cout<<"\n";
	for(i = 0; i < n; ++i)
		cout<<find(S, i) <<" ";
	return 0;
}

