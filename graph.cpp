#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

#define X 7
#define Y 7
#define INFINITY 999999999
#define INPUT_FILE "graph.txt"

class Graph{
private:
	vector<pair<int, int>> nodeEdges[X * Y];

public:
	Graph(int array[X][Y]) {
		for(int i = 0; i < Y; ++i){
			for(int j = 0; j < X; ++j) {
				int index = Y * i + j;
				if(i + 1 < Y)
					nodeEdges[index].push_back(make_pair(index + Y, array[j][i + 1]));
				if(j + 1 < X)
					nodeEdges[index].push_back(make_pair(index + 1, array[j + 1][i]));
				if(j - 1 >= 0)
					nodeEdges[index].push_back(make_pair(index - 1, array[j - 1][i]));
				if(i - 1 >= 0)
					nodeEdges[index].push_back(make_pair(index - Y, array[j][i - 1]));
			}
		}
	}

    int heuristicValue(int adjustment, int vertices) {
        int temp = adjustment - vertices;
        switch (temp) {
            case 1:  return 1;
            case -1: return 0;
            case X:  return 0;
            case -X: return 1;
            default: return 0;
        }
    }

	vector<int> findPath(int startIndex, int endIndex) {
		vector<int> distance(X * Y, INFINITY);
		set<pair<int, int>> openSet;
		vector<int> theBestPath(X * Y, -1);

		distance[startIndex] = 0;
		theBestPath[startIndex] = startIndex;
		openSet.insert(make_pair(0, startIndex));

		while(!openSet.empty()) {
			int current = openSet.begin()->second;
			openSet.erase(openSet.begin());

			for(auto it : nodeEdges[current]) {
				int weight = it.second + heuristicValue(it.first, endIndex);
				if(distance[it.first] > distance[current] + weight) {
					if(distance[it.first] != INFINITY)
						openSet.erase(openSet.find(make_pair(distance[it.first], it.first)));
					distance[it.first] = distance[current] + weight;
					theBestPath[it.first] = current;
					openSet.insert(make_pair(distance[it.first], it.first));
				}
			}
		}

        vector<int> path;
        if (theBestPath[endIndex] != -1) {
            int index = endIndex;
            path.push_back(endIndex);

            while (index != startIndex) {
                index = theBestPath[index];
                path.push_back(index);
            }
        }
        return path;
    }
};

void readArray(int graph[X][Y]) {
	ifstream graphFile;
	graphFile.open(INPUT_FILE);
	for(int i = 0; i< Y; ++i) {
		string temp;
		graphFile >> temp;
		for(int j = 0; j < X; ++j)
			graph[j][i] = temp[j] - '0';
	}
}

void printArray(int graph[X][Y]) {
	for(int i = 0; i < Y; ++i) {
		for(int j = 0; j < X; ++j)
			cout<< graph[j][i];
		cout<<endl;
	}
}

void printPath(vector<int> path) {
    vector<char> charArray(X * Y, '.');
    for(auto a : path)
        charArray[a] = '1';
    charArray[*path.begin()] = '0';
    charArray[*(--path.end())] = '0';

    std::string out;
    for (int y = 0; y < Y; ++y) {
        for (int x = 0; x < X; ++x)
            out.push_back(charArray[y * X + x]);
        out.push_back('\n');
    }
    std::cout << out;
}

int main() {
	int graph[X][Y];
	readArray(graph);
	std::cout << "Start graph:" << endl;
	printArray(graph);
	Graph map(graph);

	vector<int> path = map.findPath(X - 1, (Y - 1) * X);
	std::cout << endl << "Found path:" << endl;
	printPath(path);

	return 0;
}
