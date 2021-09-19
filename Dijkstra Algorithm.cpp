#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

const int INT_INF = 2147483647;
int extractMin(int array[], int nodeCount);
long long myhash(string c);
typedef struct Edge;
typedef struct Vertex;
Edge* addFlight(Vertex* hashVertex, Edge* currentFlight, Edge* newFlight);
Vertex* flightAndChainListCompare(Vertex* currentVertex, Edge* flight);
void addToEndOfADJList(Edge* newEdge, Edge* nextEdge);
void restoreUp(int index, int k);
Vertex* extractMin();
void swap(Vertex* child, Vertex* parent);
void Dijkstra(Vertex G[], Vertex* s);
Vertex* addToChain(Vertex* currentVertex, Vertex* newVertex);
Vertex* getVertex(string code);



// Vertex Object:
typedef struct Vertex {
	string code = "";
	int heap_pos = 0;
	int hash_pos = 0;
	Edge* adj_list = NULL;
	Vertex* chain_list = NULL;
	int dvalue = INT_INF;
	Vertex* parent = NULL;
	bool visited = false;
};

// Edge Object:
typedef struct Edge {
	string origin;
	string dest;
	string airlines;
	int fltno;
	int deptime;
	int arrtime;
	int distance;
	int totaltime;
	Edge* next = NULL;		// to be used in adjacency list
};


Vertex hashArray[1000] = { };
Vertex* heapArray[1000] = { };
int heapSize = 0;
Vertex* cloudArray[1000] = { };
Vertex* firstVertex = new Vertex;
Vertex* lastVertex = new Vertex;
string firstVertexCode;
string lastVertexCode;



int main(int argc, char** argv)
{
	int hashSize;
	int i, j;
	int positionCounter = 0;
	int numOfFlights;
	ifstream inFile;
	string airportCode;
	string line;
	int flightsLineIndex = 3;


	inFile.open("airports.txt");
	inFile >> hashSize;
	heapSize = hashSize;
	
	// goes through each index and inserts a blank vertex
	for (i = 0; i < hashSize; i++)		
	{
		Vertex filler;
		hashArray[i] = filler;
	}

	// receives the airports from the file and adds them to the hash array/table; adding to chain if needed
	for (i = 0; i < hashSize; i++)		
	{
		inFile >> airportCode;
		Vertex* temp = new Vertex;
		temp->code = airportCode;
		temp->hash_pos = temp->heap_pos = positionCounter;
		heapArray[temp->heap_pos] = temp;
		positionCounter++;
		hashArray[myhash(temp->code)].chain_list = addToChain(hashArray[myhash(temp->code)].chain_list, temp);
	}

	inFile.close();
	inFile.open("flights.txt");
	inFile >> numOfFlights;
	inFile >> line >> line >> line >> line >> line >> line >> line;
	Vertex* flightVertex = new Vertex;
	
	for (i = 0; i < numOfFlights; i++)
	{
		Edge* tempEdge = new Edge;		// temporary edge
		tempEdge->next = NULL;
		inFile >> tempEdge->airlines >> tempEdge->fltno >> tempEdge->origin >> tempEdge->dest >> tempEdge->deptime >> tempEdge->arrtime >> tempEdge->distance;		// receives information for flight
		flightVertex = flightAndChainListCompare(hashArray[myhash(tempEdge->origin)].chain_list, tempEdge);
		flightVertex->adj_list = addFlight(flightVertex, flightVertex->adj_list, tempEdge);
	}


	firstVertexCode = argv[1];
	lastVertexCode = argv[2];


	Dijkstra(hashArray, getVertex(firstVertexCode));
}

Edge* addFlight(Vertex* hashVertex, Edge* currentFlight, Edge* newFlight)
{
	if (currentFlight == nullptr)	// if the adjacency list has no flights currently in it, adds newFlight as the first flight
	{
		currentFlight = newFlight;	
		return currentFlight;
	}
	currentFlight->next = addFlight(hashVertex, currentFlight->next, newFlight);	// recalls the function with the next edge in the adjacency list
	return currentFlight; 
}

Vertex* flightAndChainListCompare(Vertex* currentVertex, Edge* flight)		// funciton that uses the origin of the flight to find its correct vertex
{
	if (currentVertex->code == flight->origin)
	{
		return currentVertex;
	}
	else
	{
		flightAndChainListCompare(currentVertex->chain_list, flight);
	}
}

Vertex* addToChain(Vertex* currentVertex, Vertex* newVertex)
{
	if (currentVertex == nullptr)
	{
		currentVertex = newVertex;
		return currentVertex;
	}
	currentVertex->chain_list = addToChain(currentVertex->chain_list, newVertex);
	return currentVertex;
}

void sinkDown(Vertex* min, int i)
{
	Vertex* leftChild = heapArray[min->heap_pos * 2 + 1];
	Vertex* rightChild = heapArray[min->heap_pos * 2 + 2];
	Vertex* smallest = heapArray[i];

	if (leftChild->heap_pos < heapSize && heapArray[leftChild->heap_pos]->dvalue < heapArray[smallest->heap_pos]->dvalue)
	{
		smallest = leftChild;
	}
	
	if (rightChild->heap_pos < heapSize && heapArray[rightChild->heap_pos]->dvalue > heapArray[rightChild->heap_pos]->dvalue)
	{
		smallest = rightChild;
	}

	if (smallest->heap_pos != i)
	{
		swap(heapArray[i], heapArray[smallest->heap_pos]);
		sinkDown(smallest, smallest->heap_pos);
	}

}

void restoreUp(int index)
{
	int parent = (index - 1) / 2;
	while (parent >= 0)
	{
		if (heapArray[index]->dvalue < heapArray[parent]->dvalue)
		{
			swap(heapArray[index], heapArray[parent]);
			index = parent;
			parent = (index - 1) / 2;
		}
		else
			break;
	}
}

void swap(Vertex* child, Vertex* parent)
{
	int size = heapSize;
	int i = 0;
	Vertex* temp = new Vertex;
	temp = heapArray[child->heap_pos];
	heapArray[child->heap_pos] = heapArray[parent->heap_pos];
	heapArray[parent->heap_pos] = temp;

	int tempPos;
	tempPos = child->heap_pos;
	child->heap_pos = parent->heap_pos;
	parent->heap_pos = tempPos;
}

// Hash Function:
long long myhash(string c)
{
	int p = 3;
	int m = 10000;
	long long power_of_p = 1;
	long long hash_val = 0;

	for (int i = 0; i < c.length(); i++)
	{
		hash_val = (hash_val + (c[i] - 'A' + 1) * power_of_p) % m;
		power_of_p = (power_of_p * p) % m;
	}
	return hash_val % 1000;
}

void relax(Vertex* u, Vertex* w, int departureTime, int arrivalTime)
{
	if (u->dvalue <= departureTime)		// if the origin airport's dvalue is less than the departure time (meaning it can leave on time)...
	{
		if (arrivalTime <= w->dvalue)		// if the the arrival time is less than the destination ariport's dvalue...
		{
			w->dvalue = arrivalTime;	// the destination airport's dvalue is updated to the new arrival time
			w->parent = u;
			restoreUp(w->heap_pos);
			// need to add parent flight of flight (notes) //
		}
	}
}

Vertex* getVertex(string code)
{
	int p = myhash(code);
	Vertex* chainList = hashArray[myhash(code)].chain_list;

	while (chainList != nullptr)
	{
		if (chainList->code == code)
		{
			return chainList;
		}
		chainList = chainList->chain_list;
	}
}


Vertex* extractMin()
{
	swap(heapArray[0], heapArray[heapSize - 1]);
	sinkDown(heapArray[0], heapArray[0]->heap_pos);
	heapSize--;


	return heapArray[heapSize];
}

void Dijkstra(Vertex G[], Vertex* s)
{
	Vertex* v, * origin, * destination;
	int counter;
	int size = heapSize;

	heapArray[s->heap_pos]->dvalue = 0;


	s->visited = true;
	restoreUp(s->heap_pos);		// sets the source's dvalue to 0 and brings it to the root of the heapArray

	origin = s;
	Vertex* destinationVertex = new Vertex;
	destinationVertex = getVertex(origin->adj_list->dest);		// gets the vertex of the destination of the current flight to update its dvalue
	int currentHashIndex = 0;
	int cloudArrayIndex = 0;

	while (heapSize > 0)
	{
		cloudArray[cloudArrayIndex] = extractMin();		// extracts the minimum value of heapArray and adds it to the cloud

		while (origin->adj_list != nullptr)		// while the origin still has edges to relax...
		{
			destinationVertex = getVertex(origin->adj_list->dest);
			relax(origin, destinationVertex, origin->adj_list->deptime, origin->adj_list->arrtime);		// relax the edges at the origin..
			origin->adj_list = origin->adj_list->next;		// goes to the next edge and recall
		}
		origin->visited = true;
		origin = heapArray[0];
		cloudArrayIndex++;
	}


	string output;
	Vertex* temp = new Vertex;
	temp = getVertex(lastVertexCode);
	output = to_string(getVertex(lastVertexCode)->dvalue);
	output = lastVertexCode + "-" + output;
	while (temp->parent != nullptr)
	{
		output = temp->parent->code + "-" + output;
		temp = temp->parent;
	}
	cout << output << endl;
}