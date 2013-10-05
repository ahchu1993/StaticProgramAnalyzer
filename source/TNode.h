#include <vector>
#include <string>
using namespace std;
class TNode
{
private:
	friend class AST;

	string type;
	int content;
	int lineNo;
	TNode* leftChild;
	TNode* rightChild;

public:
	// Link parent and left child node
	void makeLeftChild(TNode* leftChild);
	int hasRightChild;
	int hasLeftChild;
	// Link parent and right child node
	void makeRightChild(TNode* rightChild);

	// Get left child of the given node
	TNode* getLeftChild();

	// Get right child of the given node
	TNode* getRightChild();

	// Get content of the node
	int getContent();

	// Get line number of the node
	int getLineNo();

	// Get type of the node
	string getType();


	// Constructor: type can be "ROOT", "OPT", "VAR" or "CONST"
	// conttent is -1 for "ROOT" and "OPT", varIndex for "VAR" and constValue for "CONST"
	TNode(string type, int content, int lineNo);
	~TNode();
};
