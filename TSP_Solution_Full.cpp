#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

//Potential new metric graphs (/delta graphs/euclidean-metric graph ... whatever)
//https://cs.stackexchange.com/questions/100618/christofides-algorithm-by-hand-suboptimal-solution-is-it-my-fault
//https://cs.stackexchange.com/questions/77461/msts-in-christofides-algorithm (used)

//constant for the number of cities
//const int NOC = 5;
const int NOC = 6;

//--------------------------------------------------------------------graph function--------------------------------------------------------------------

void printGraph(int mat[][NOC]){
	cout<<"\nBelow is the adjaceny matrix for the cities graph: \n\n";
	for (int y = 0; y < NOC; y++){
		//outputs the matrix
		for (int z = 0; z < NOC; z++){
			cout<<mat[y][z]<<"  ";
		}
		cout<<endl;
	}
}

//-------------------------------------------------------------------MST functions---------------------------------------------------------------------

int find(int x,int arr[]){	//returns index of x
	if(arr[x]==x)
		return x;
	return find(arr[x],arr);
}

void unify(int x, int y, int arr[]){	//performs union of two sets
	int headX = find(x,arr);
	int headY = find(y,arr);
	arr[headX] = headY;
}

int kMST(int mdarray[][NOC],int arr[],int first[],int second[],int weight[]){
	int cost = 0;
	for (int i = 0;  i < NOC; i++)
		arr[i]=i;	//array of all index values
	
	int edges = 0;
	while (edges < NOC-1){
		int min = INT_MAX, a = -1, b = -1;
		for (int x = 0; x<NOC;x++){
			for(int y=0;y<NOC;y++){
				if(find(x,arr)!=find(y,arr) && mdarray[x][y] < min){	
				//searches for the minimal edges left in the array without making any loops
					min = mdarray[x][y];
					a = x;
					b = y;
				}
			}
		}
		unify(a,b,arr);	//union of a in arr[b]
		cout<<"Edge "<< edges <<": "<<a<<" , "<<b<<" -> Cost: "<<min<<endl;
		
		first[edges]=a;
		second[edges]=b;
		weight[edges]=min;
		
		edges++;
		
		cost += min;
	}
	return cost;
}

//---------------------------------------------------------------------TSP functions--------------------------------------------------------------

void doubleTree(int f[], int s[], int df[], int ds[], int w[], int pw[]){
	for(int i = 0; i < (NOC-1)*2; i+=2){	//creating a separate path to and from each node of the MST
		df[i] = f[i/2];
		df[i+1] = s[i/2];
		
		ds[i] = s[i/2];
		ds[i+1] = f[i/2];
		
		pw[i] = pw[i+1] = w[i/2];
	}
	pw[(NOC-1)*2] = w[0];
}

void eulerTour(int out[], int df[], int path[]){
	for(int i = 0; i < (NOC-1)*2; i++){		//traverse the entire double tree
		out[df[i]]++;
		path[i] = df[i];
	}
	
	path[(NOC-1)*2] = df[0];
}

void printTSP(int c, int fw, int fpath[], int mat[][NOC], string name[]){ //find the 
	cout<<"Path: "<<fpath[0]<<" = "<<name[fpath[0]]<<".\n\n";
	for(int final = 1; final < c; final++){
		fw += mat[fpath[final-1]][fpath[final]];
		cout<<"Path: "<<fpath[final]<<" = "<<name[fpath[final]]<<".\nEdge "<<
			final-1<<": weight = "<<mat[fpath[final-1]][fpath[final]]<<endl<<endl;
	}
	cout<<"\nThe traversal cost = "<<fw;
}

//-----------------------------------------------------TSP Optimal Algorithm

int optTSP(int mat[][NOC], int c, int pth[]){
	vector<int> vec;
	
	//fills the vector vec with all indexes of the matrix except the start value c
	for (int i = 0; i < NOC; i++){
		if (i != c)
			vec.push_back(i);   	
	}
	
	int minw = INT_MAX; // store minimum weight of a graph
	pth[0] = c;
	do{
		int cweight = 0;
		int k = c;
		
		for (int i = 0; i < vec.size(); i++){
			cweight += mat[k][vec[i]];
			k = vec[i];
		}
		
		cweight += mat[k][c];
		
		if(cweight<=minw){
			for (int i = 0; i < vec.size(); i++){
				k = vec[i];
				pth[i+1] = k;
			}
			pth[NOC] = c;
		}
		minw = min(minw, cweight); // to update the value of minimum weight
	}
	
	//checks for all permutations of the vector vec, which chosen path has the lowest weight
	while (next_permutation(vec.begin(), vec.end()));
	return minw;
}

//-------------------------------------------------------------------------main function----------------------------------------------------------

int main(){
	//create adjacency matrix for metric graph
	
//	string city[NOC]={"Kingston","Montego_Bay","Mandeville","Spanish_Town","Falmouth"};
	string city[NOC]={"Zeroeth","First","Second","Third","Fourth","Fifth"};

	
//	int cities[NOC][NOC]={
//		{0,6,7,3,6},
//		{6,0,7,9,1},
//		{7,7,0,4,7},
//		{3,9,4,0,3},
//		{6,1,7,3,0}
//	};

	int cities[NOC][NOC]={
		{0,11,9,9,16,15},
		{11,0,14,10,15,10},
		{9,14,0,6,11,13},
		{9,10,6,0,10,9},
		{16,15,11,10,0,8},
		{15,10,13,9,8,0}
	};
	
	printGraph(cities);
	
	//create kruskal's algorithm-based MST for the graph
	
	int head[NOC];		
	int first[NOC-1];	//to hold the start nodes for all edges of the MST
	int second[NOC-1];	//to hold the end node for all edges of the MST
	int weight[NOC-1];	//to hold the weight of all edges of the MST
	
	cout<<endl<<"The edges of the MST are:\n\n";
	
	int tcost = kMST(cities,head,first,second,weight);	//finds the MST, returns its weight
	
	cout<<"\nMST weight = "<<tcost<<endl<<endl;
	
	//TSP Approximation
	int dfirst[(NOC-1)*2];	//to hold the start nodes for all edges of the Double Tree
	int dsecond[(NOC-1)*2];	//to hold the end nodes for all edges of the Double Tree
	
	int out[NOC];	//to hold the out degree for all the nodes of the double tree
	bool visited[NOC];	//to hold the visited state of all nodes of the graph
	
	for(int i=0;i<NOC;i++){
		out[i] = 0;
		visited[i] = false;
	}
	
	int path[(NOC*2)-1];	//to hold the euler path of the double tree
	int pweight[(NOC*2)-1];	//to hold the weight of all edges of the euler cycle
	
	int fpath[NOC*2];	//to hold the final approximate path of the 
	int fweight = 0;	//to hold the weight of the approximate path
	
	doubleTree(first,second,dfirst,dsecond,weight,pweight);	//create double tree
	eulerTour(out,dfirst,path);	//find euler path
		
	int check = 0;
	
	for(int i = 0; i < (NOC*2)-1; i++){	//assigns each unvisited node of the euler path to the final path
		if(!visited[path[i]]){
			fpath[check] = path[i];
			check++;
			visited[path[i]] = true;
		}
		if(i == (NOC*2)-2){	//assigns the start node to the end of the path
			fpath[check] = path[i];
			check++;
			break;
		}
	}
	
	cout<<"\n\n--------------------------------------------------------------\n"<<
			"The path derived by the 2-opt approximation algorithm is:\n"<<
			"--------------------------------------------------------------\n\n";
	printTSP(check,fweight,fpath,cities,city);	//print out the final approximate path details
	
	//Optimal TSP Solution
	int optimalpath[NOC+1];
//	int start = 1;
	int start = 2;
	int optweight = optTSP(cities, start, optimalpath);	//checks all possible paths for the lowest cost path
														//returns optimal path weight
	
	cout<<"\n\n\n--------------------------------------------------------------\n"<<
			"The path derived by the optimal algorithm is:\n"<<
			"--------------------------------------------------------------\n\n";
	for(int i = 0;i<NOC+1;i++){
		cout<<"Path: "<<optimalpath[i]<<" = "<<city[optimalpath[i]]<<".\n";
		if(i>0){
			cout<<"Edge "<<i-1<<": weight = "<<cities[optimalpath[i-1]][optimalpath[i]]<<endl<<endl;
		}
		else{
			cout<<endl;
		}
	}
	
	cout<<"\nThe traversal cost = "<<optweight<<endl;
	
	return 0;
}
