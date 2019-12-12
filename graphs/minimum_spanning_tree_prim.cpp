#include<iostream>
using namespace std;

const int n = 7;

//Helper function to reset arrays
void resetArray (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetArray(visited, i+1);
}

//Copy Matrix G to C
void copyMatrix (int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Returns unvisited adjacent vertex with least cost
int leastCostAdjacent (int G[n][n], int u, int visited[n]) {
	int v, minVertex = -1, minCost = 62000;
	for(v = 0; v < n; ++v) {
		if(G[v][u] > 0 && visited[v] == 0) {
			if(G[v][u] < minCost) {
				minCost = G[v][u];
				minVertex = v;
			}
		}
	}
	return minVertex;
}

//Minimum Spanning Tree (Prim's Algorithm)
void minSpanTree (int G[n][n], int visited[n]) {
	int viscount = 0, u, v, i, j;
	while(viscount != n) {
		if(viscount == 0)
			u = 0;
		else {
			int min = 62000;
			for(i = 0; i < n; ++i) {
				j = leastCostAdjacent(G, i, visited);
				if(visited[i] == 1 && G[j][i] < min && j != -1) {
					min = G[j][i];
					v = i;
					u = j;
				}
			}
			cout<<"\n" <<v+1 <<"->" <<u+1;
		}
		visited[u] = 1;
		++viscount;
	}
}

int main ()
{
	int G[n][n] = {{0,2,4,1,0,0,0},{2,0,0,3,10,0,0},{4,0,0,2,0,5,0},{1,3,2,0,7,8,4},
					{0,10,0,7,0,0,6},{0,0,5,8,0,0,1},{0,0,0,4,6,1,0}}; 
	int i, visited[n], dist[n], prev[n];
	int C[n][n];
	copyMatrix(G, C);
	resetArray(visited);
	minSpanTree(C, visited);
	return 0;
}

