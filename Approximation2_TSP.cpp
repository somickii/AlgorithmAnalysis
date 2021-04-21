#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

//constant for the number of cities
const int NOC = 5;

//--------------------------------------------------------------------graph functions--------------------------------------------------------------------

void makeMatrix(string name[], int mat[][NOC]){
	cout<<"Enter Below 5 cities \n\n";
	
	for (int x = 0; x < NOC; x++){
		//prompts for city name entries
		cout<<"Enter city "<<x+1<<" (do NOT use spaces):\n";
		cin>>name[x];
	}
	
	cout<<"\n\nBelow are the cities: \n";
	
	
	for (int a = 0; a < NOC; a++){
		
		for (int b = a; b < NOC; b++){
			int assign;
			
			if(b == a){
				assign = 0;
				mat[a][b] = assign;
				continue;
			}
			
			//pseudorandom generation of weights for each edge
			else{
				assign = 1 + rand() % NOC*2;
				
				//creating a metric graph
				if(a>0){
					for(int c = 0; c < b-2; c++){
						if(c==a || mat[a][c] + assign >= mat[c][b]){
							continue;
						}
						else{
							assign += mat[c][b];
						}
					}
				}
			}
				
			
			//this matrix represents an undirected graph
			mat[a][b] = assign;
			mat[b][a] = assign;
			
		}
		
		//ouputs the name of the cities
		cout<<" -"<<name[a]<<endl;
	}
}

void printGraph(int mat[][NOC]){
	cout<<"\nBelow is the adjaceny matrix for the cities graph: \n";
	for (int y = 0; y < NOC; y++){
		//outputs the matrix
		for (int z = 0; z < NOC; z++){
			cout<<mat[y][z]<<"  ";
		}
		cout<<endl;
	}
}

//-------------------------------------------------------------------MST functions---------------------------------------------------------------------

int find(int x,int arr[]){
	if(arr[x]==x)
		return x;
	return find(arr[x],arr);
}

void unify(int x, int y, int arr[]){
	int headX = find(x,arr);
	int headY = find(y,arr);
	arr[headX] = headY;
}

int kMST(int mdarray[][NOC],int arr[],int first[],int second[],int weight[]){
	int cost = 0;
	for (int i = 0;  i < NOC; i++)
		arr[i]=i;
	
	int edges = 0;
	while (edges < NOC-1){
		int min = INT_MAX, a = -1, b = -1;
		for (int x = 0; x<NOC;x++){
			for(int y=0;y<NOC;y++){
				if(find(x,arr)!=find(y,arr) && mdarray[x][y] < min){
					min = mdarray[x][y];
					a = x;
					b = y;
				}
			}
		}
		unify(a,b,arr);
		cout<<"Edge"<< edges <<": "<<a<<" , "<<b<<" -> Cost: "<<min<<endl;
		
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
	for(int i = 0; i < (NOC-1)*2; i+=2){
		df[i] = f[i/2];
		df[i+1] = s[i/2];
		
		ds[i] = s[i/2];
		ds[i+1] = f[i/2];
		
		pw[i] = pw[i+1] = w[i/2];
	}
	pw[(NOC-1)*2] = w[0];
}

void eulerTour(int out[], int df[], int path[]){
	for(int i = 0; i < (NOC-1)*2; i++){
		out[df[i]]++;
	}
	
	for(int i = 0; i < (NOC-1)*2; i++){
		path[i] = df[i];
	}
	path[(NOC-1)*2] = df[0];
}

void printTSP(int c, int fw, int fpath[], int mat[][NOC], string name[]){
	cout<<"Final path: "<<fpath[0]<<" = "<<name[0]<<endl<<endl;
	for(int final = 1; final < c; final++){
		fw += mat[fpath[final-1]][fpath[final]];
		cout<<"Final path: "<<fpath[final]<<" = "<<name[fpath[final]]<<".\nEdge "<<final-1<<" weight = "<<mat[fpath[final-1]][fpath[final]]<<endl<<endl;
	}
	cout<<"\nThe final traversal cost = "<<fw;
}

//-------------------------------------------------------------------------main function----------------------------------------------------------

int main(){
	//create adjacency matrix for metric graph
	string city[NOC];
	int cities[NOC][NOC];
	
	makeMatrix(city,cities);
	printGraph(cities);
	
	//create kruskal's algorithm-based MST for the graph
	int head[NOC];
	int first[NOC-1];
	int second[NOC-1];
	int weight[NOC-1];
	
	cout<<endl;
	
	int tcost = kMST(cities,head,first,second,weight);
	
	cout<<"\n\nMST weight = "<<tcost<<endl<<endl;
	
	//TSP Approximation
	int dfirst[(NOC-1)*2];
	int dsecond[(NOC-1)*2];
	
	int out[NOC]={0,0,0,0,0};
	bool visited[NOC]={false,false,false,false,false};
	
	int path[(NOC*2)-1];
	int pweight[(NOC*2)-1];
	
	int fpath[NOC*2];
	int fweight = 0;
	
	doubleTree(first,second,dfirst,dsecond,weight,pweight);
	eulerTour(out,dfirst,path);
		
	int check = 0;
	
	for(int i = 0; i < (NOC*2)-1; i++){
		if(!visited[path[i]]){
			fpath[check] = path[i];
			check++;
			visited[path[i]] = true;
		}
		if(i == (NOC*2)-2){
			fpath[check] = path[i];
			check++;
			break;
		}
	}
	
	printTSP(check,fweight,fpath,cities,city);
	
	return 0;
}




