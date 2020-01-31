#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

#define X 7 //szerokosc planszy
#define Y 7 //wysokosc planszy
#define INFINITY 999999999
#define INPUT_FILE "graph.txt"

class Graph{
private:
	vector<pair<int, int>> nodeEdges[X * Y]; //tablica przechowująca dane krawędzi (indeks, waga) dla każdego wierzchołka (pola)

public:
	Graph(int array[X][Y]) {
		for(int i = 0; i < Y; ++i){
			for(int j = 0; j < X; ++j) { //przypisanie wag oraz indeksów każdej krawędzi danego wierzchołka
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

    int heuristicValue(int adjustment, int vertices, int start, int end) { 
        int temp = adjustment - vertices;
        if(end - start > 0)
			switch (temp) { //faworyzowanie ruchow w lewo i w dol
				case 1:  return 1; //w prawo
				case -1: return -1; //w lewo
				case X:  return -1; //w dol
				case -X: return 1; //w gore
				default: return 0;
			}
		else
			switch (temp) { //faworyzowanie ruchow w gore i w prawo
				case 1:  return -1;
				case -1: return 1;
				case X:  return 1;
				case -X: return -1;
				default: return 0;
			}
    }

	vector<int> findPath(int startIndex, int endIndex) {
		vector<int> distance(X * Y, INFINITY); //wektor przechowujacy wartosc f
		set<pair<int, int>> openSet; //kontener przechowujacy oczekujace na sprawdzenie wierzcholki (distance, index)
		vector<int> theBestPath(X * Y, -1); //wektor przechowujacy numer indeksu poprzedniego pola w sciezce 

		distance[startIndex] = 0; //punkt poczatkowy ma wartosc f=0
		theBestPath[startIndex] = startIndex;
		openSet.insert(make_pair(0, startIndex));

		while(!openSet.empty()) { //sprawdzamy kolejne wierzcholki poki jest co sprawdzac
			int current = openSet.begin()->second;
			openSet.erase(openSet.begin());
			if(current == endIndex) //przerywamy jesli dotarlismy do mety
				break;

			for(auto it : nodeEdges[current]) { //sprawdzamy wszystkie krawedzie obecnego wierzcholka
				int weight = it.second + heuristicValue(it.first, current, startIndex, endIndex); //waga wierzcholka (waga pola + heurystyka)
				if(distance[it.first] > distance[current] + weight) { //sprawdzamy czy mamy mniejsza wartosc (jesli wierzcholek wczesniej nie sprawdzany to ma wartosc INFINITY czyli na pewno go sprawdzimy)
					if(distance[it.first] != INFINITY)
						openSet.erase(openSet.find(make_pair(distance[it.first], it.first)));
					distance[it.first] = distance[current] + weight; //zmieniamy wartosc f
					theBestPath[it.first] = current; //dodajemy go do sciezki
					openSet.insert(make_pair(distance[it.first], it.first)); //aktualizujemy jego wartosc w wierzcholkach do sprawdzenia
				}
			}
		}

        vector<int> path;
        if (theBestPath[endIndex] != -1) {
            int index = endIndex;
            path.push_back(endIndex);

            while (index != startIndex) { //przechodzimy przez sciezke
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
