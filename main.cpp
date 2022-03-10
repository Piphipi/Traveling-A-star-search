#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class hNode{
	public :int nodeID;
	public :int edgeCost;
    public :hNode *next;
    public :hNode(int ID, int cost){
		if (ID <= 0){
			cout << "id must be at least 1.\n";
			exit(1);
		}
		nodeID = ID;
		edgeCost = cost;
        next = nullptr;
    }
    
    void printHNode (hNode* node, ofstream& outFile){
        outFile << (" --> (") << (node->nodeID) << (", ") << (node->edgeCost) << ( ")");
    }
};
class AstarNode{
	public :int ID;
	public :int numNodesRemain;
	public :int gStar;
	public :int hStar;
	public :int fStar;
    public :AstarNode *next;
    public :AstarNode *parent;
    public :AstarNode(int nodeID, int nodesRemain, int gstar, int medEdge, AstarNode* Parent){
		ID = nodeID;
        numNodesRemain = nodesRemain;
        gStar = gstar;
        hStar = (nodesRemain * medEdge);
        fStar = (gStar + hStar);
        parent = Parent;
        next = nullptr;
    }
   
    void printAStarNode (AstarNode* node, ofstream& outFile){
    	if (node->hStar == 0 && node->gStar == 0){
    		outFile << ("(") << (node->ID) << (", ") << (node->numNodesRemain) << (", ") << (node->gStar) << (", ") << (node->hStar) << (", ") << (node->fStar) << (", "); 
			if (node->parent == nullptr){
				outFile << "NULL)";
			}
			else{
				outFile << ((node->parent)->ID) << ( ")");
			}	
		}
		else{
			outFile << (" --> (") << (node->ID) << (", ") << (node->numNodesRemain) << (", ") << (node->gStar) << (", ") << (node->hStar) << (", ") << (node->fStar) << (", "); 
			if (node->parent == nullptr){
				outFile << "NULL)";
			}
			else{
				outFile << ((node->parent)->ID) << ( ")");
			}
		}
        
    }
};
class AStarSearch{
	public :int numNodes;
	public :int startID;
	public :hNode** hLList;
	public :AstarNode* Open;
	public :AstarNode* Close;
	public :int medianEdgeCost;
	public :int numEdge = 0;
	public :AStarSearch(ifstream& inFile){
		if (inFile.is_open()){
			inFile >> numNodes;
		}
		hLList = new hNode*[numNodes+1];
		for (int i = 0; i < (numNodes+1); i++){
			hLList[i] = nullptr;
		}
		loadGraph(inFile);
		Open = new AstarNode (9999, 0, 0, 0, nullptr);
		Close = new AstarNode (9999, 0, 0, 0, nullptr);
	}
	
	void loadGraph(ifstream& inFile){
		int i;
		int j;
		int cost;
		if (inFile.is_open()){
			while(inFile >> i) {
				inFile >> j;
				inFile >> cost;
				hashInsert(i,j,cost);
				hashInsert(j,i,cost);
				numEdge++;
			}
		}
	}
	
	void hashInsert(int ID1, int ID2, int cost){
		hNode* newNode = new hNode(ID2, cost);
		hNode* last = hLList[ID1];
		if (last == nullptr){
			hLList[ID1] = newNode;
		}
		else{
			while (last->next != nullptr){
				last = last->next;
	 		}
	 		last->next = newNode;
		}
	}
	
	void printHashTable(ofstream& outFile){
		for (int i = 1; i < (numNodes + 1); i++){
			hNode* last;
			outFile << "hLList[" << i << "]";
			last = hLList[i];
			if (last == nullptr){
				outFile << " -> null\n";
			}
			else{
				while (last != nullptr){
					last->printHNode(last, outFile);
					last = last->next;
	 			}	
	 			outFile << endl;
			}
		}
	}
	
	int getMedian(ifstream& inFile){
		int i;
		int point = 0;
		int cost;
		int* costAry = new int[numEdge];
		if (inFile.is_open()){
			inFile >> i;
			while(inFile >> i) {
				inFile >> i;
				inFile >> cost;
				costAry[point] = cost;
				point++;
			}
		}
		for(int k = 0; k < numEdge; k++){		
			for(int l = k+1; l < numEdge; l++){
				if(costAry[k] > costAry[l]){
					int temp = costAry[k];
					costAry[k] = costAry[l];
					costAry[l] = temp;
				}
			}
		}
		if (numEdge % 2 != 0){
			return costAry[(numEdge/2)];
		}
		else{
			return ((costAry[(numEdge/2)] + costAry[((numEdge/2) - 1)])/2);
		}
	}
    
    void expansion (AstarNode* currentNode, bool foundSolution){
        int nodeID = currentNode->ID;
        hNode* nextNode = hLList[nodeID];
        while (nextNode != nullptr && foundSolution == false){
            if (nextNode != nullptr){
				if (checkVisited(currentNode, nextNode) == false){
					int ID = nextNode->nodeID;
                	int nodesRemain = currentNode->numNodesRemain - 1;
                	int gStar = currentNode->gStar + nextNode->edgeCost;
                	AstarNode* childNode = new AstarNode(ID, nodesRemain, gStar, medianEdgeCost, currentNode);
    				OpenInsert(childNode);
				}
				else if (currentNode->numNodesRemain == 1 && nextNode->nodeID == startID){
					int ID = nextNode->nodeID;
                	int nodesRemain = currentNode->numNodesRemain - 1;
                	int gStar = currentNode->gStar + nextNode->edgeCost;
                	AstarNode* childNode = new AstarNode(ID, nodesRemain, gStar, medianEdgeCost, currentNode);
    				OpenInsert(childNode);
    				
				}
            }
            nextNode = nextNode->next;
        }
    }
	
	void OpenInsert(AstarNode* node){
		AstarNode* spot = Open;
        while(spot->next != nullptr){
        	if (((spot->next)->fStar) < node->fStar){
        		spot = spot->next;
			}
			else{
				break;
			}
        }
        node->next = spot->next;
        spot->next = node;
	}
	
	void CloseInsert(AstarNode* node){
		AstarNode* spot = Close;
        while(spot->next != nullptr){
        	if (((spot->next)->ID) < node->ID){
        		spot = spot->next;
			}
			else{
				break;
			}
        }
        node->next = spot->next;
        spot->next = node;
	}
	
	void printList(AstarNode* list, ofstream& outFile){
		AstarNode* print = list; 
		print->printAStarNode(print, outFile);
		for (int i = 0; i < 8; i++){
			if (print->next != nullptr){
				print->printAStarNode(print->next, outFile);
				print = print->next;
			}
		}
		outFile << endl;
	}
	
	AstarNode* removeOpen(){
		AstarNode* spot = Open->next;
		Open->next = spot->next;
		spot->next = nullptr;
		return spot;
	}
	
	bool checkGoal(AstarNode* node, ofstream& outFile){
		node->printAStarNode(node, outFile);
		outFile << endl;
		if (node->ID != startID){
			return false;
		}
		if (visitedAll(node,outFile) == true){
			return true;
		}
		else{
			return false;
		}
		cout << "Something is wrong in checkGoal method\n";
		return false;
	}
	
	bool visitedAll(AstarNode* node, ofstream& outFile){
		int* visitedAry = new int[numNodes+1];
		for (int i = 0; i < numNodes +1; i++){
			visitedAry[i] = 0;
		}
		AstarNode* checker = node->parent;
		while (checker != nullptr){
			int ID = checker->ID;
			visitedAry[ID]++;
			if ((ID == startID && visitedAry[ID] > 2) || (ID != startID && visitedAry[ID] >= 2)){
				cout << "Error! visited a node more than once!\n";
				exit(1);
			}
			checker = checker->parent;
		}
		outFile << "Printing Visited Ary\n";
		for (int i = 0; i < numNodes +1; i++){
			outFile << "visitedAry[" << i << "] = " << visitedAry[i] << endl;
		}
		int i = 1;
		while (i <= numNodes){
			if (visitedAry[i] <= 0){
				return false;
			}
			i++;
		}
		return true;
	}
	
	bool checkVisited(AstarNode* currentNode, hNode* nextNode){
		AstarNode* checker = currentNode->parent;
		while (checker != nullptr){
			if (checker != nullptr && checker->ID == nextNode->nodeID){
				return true;
			}
			checker = checker->parent;
		}
		return false;
	}
	
	void printRoute(AstarNode* node, ofstream& outFile){
		if (startID == 1){
			outFile << "==============================\n";
			outFile << "There are " << numNodes << " nodes in the input graph. Below are the solution route for each node in the graph:\n";
			outFile << "==============================\n";
		}
		AstarNode* print = node->parent;
		outFile << "Start node = " << startID << endl;
		outFile << "The solution found: " << startID << "-> ";
		while (print->parent != nullptr){
			outFile << (print->ID) << "-> ";
			print = print->parent;
		}
		outFile << (print->ID) << endl;
		outFile << "==============================\n";
	}
};
int main (int argc, char **argv) {
	ifstream inFile;
	ofstream outFile1;
	ofstream outFile2;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	AStarSearch A(inFile);
	inFile.close();
	inFile.open(argv[1]);
	A.medianEdgeCost = A.getMedian(inFile);
	A.startID = 0;
	A.printHashTable(outFile2);
	while (A.startID < A.numNodes){
		A.startID++;
		AstarNode* startNode = new AstarNode(A.startID, A.numNodes, 0, A.medianEdgeCost, nullptr);
		A.Open->next = nullptr;
		A.Close->next = nullptr;
		A.OpenInsert(startNode);
		outFile2 << "Open list after insert startNode\n";
		A.printList(A.Open, outFile2);
		bool foundSolution = false;
		AstarNode* currentNode = startNode;
		while (foundSolution == false && A.Open->next != nullptr){
			currentNode = A.removeOpen();
			outFile2 << "Open List after remove its front node\n";
			A.printList(A.Open, outFile2);
			if (currentNode != nullptr){
				if (A.checkGoal(currentNode, outFile2) == true){
					outFile1 << "A solution is found!!\n";
					A.printRoute(currentNode, outFile1);
					foundSolution = true;
				}
				else{
					A.expansion(currentNode, foundSolution);
				}
			}
			A.CloseInsert(currentNode);
			outFile2 << "Close list after insert currentNode\n";
			A.printList(A.Close, outFile2);
		}
		if (foundSolution == false && A.Open->next != nullptr){
			outFile1 << "no solution found from startID = " << A.startID << endl;
		}
	}
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}
