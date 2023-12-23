
#include "main.h"

/*Customer && HUFFTREE*/
class HuffNode 
{
public:
	/* data */
	int bf;
	char data;
	int freq;
	string code;
	HuffNode* left=nullptr;
	HuffNode* right=nullptr;
	bool isLeaf;
	HuffNode(char character, int frequent)//leaf node
	{
	this->data=character;
	this->freq=frequent;
	this->isLeaf=true;
	}
	HuffNode(HuffNode *left, HuffNode *right)//internal node
	{
		this->data='*';
		this->left=left;
		this->right=right;
		this->freq=left->freq+right->freq;
		this->isLeaf=false;
	}
	~HuffNode(){
		left=nullptr;
		right=nullptr;
		delete left;
		delete right;
	}
	void printTree();
	void printNode();
	friend class Hufftree;
};
void HuffNode:: printTree()
{  
	std::cout << this->data << " " << this->freq << endl;

	if (this->isLeaf) {
		return;
	}
	this->left->printTree();
	this->right->printTree();
}
void HuffNode::printNode()
{ 
	std::cout<<this->data<<" "<<this->freq<<endl;
}

class Hufftree
{	
private:
	HuffNode* root;
public:
 	Hufftree(){
		root=nullptr;
	}
	HuffNode*LeftRotate(HuffNode* a){
		HuffNode* b=a->right;
		a->right=b->left;
		b->left=a;
		return b;
	}
	HuffNode*RightRotate(HuffNode* a){
		HuffNode* b=a->left;
		a->left=b->right;
		b->right=a;
		return b;
	}
	int getHeightRec(HuffNode* node){
		if(node==nullptr)return 0;
		return 1+max(getHeightRec(node->left),getHeightRec(node->right));
	}
	int getBalance(HuffNode *temp)
    {
        if (!temp)
            return 0;
        return getHeightRec(temp->left) - getHeightRec(temp->right);
    }
	bool FullNode(HuffNode* root){
		if (root == nullptr) return true;
		if (root->left == nullptr && root->right == nullptr) return true;
		if ((root->left) && (root->right)) return (FullNode(root->left) && FullNode(root->right));
		return false;
	};
	void Rotate(HuffNode*& temp, int& index) {
		if (temp == nullptr || index == 0) return;

		int balance = getHeightRec(temp->right) - getHeightRec(temp->left);

		if (balance >=2 ) {
			while (balance >= 2 && index != 0) {
				int RightB = getHeightRec(temp->right->right) - getHeightRec(temp->right->left);
				if (RightB <= -1) {
					temp->right = RightRotate(temp->right);
					temp = LeftRotate(temp);
					index--;
					Rotate(temp, index);
				}
				else if (RightB >= 1) {
					temp = LeftRotate(temp);
					index--;
					Rotate(temp, index);
				}
				else if (FullNode(temp->right) && RightB == 0) {
					temp = LeftRotate(temp);
					index--;
					Rotate(temp, index);
					break;
				}
				balance = getHeightRec(temp->right) - getHeightRec(temp->left);
			}
		}
		else if (balance <= -2 && index != 0) {
			while (balance <= -2 && index != 0) {
				int LeftB = getHeightRec(temp->left->right) - getHeightRec(temp->left->left);
				if (LeftB >= 1) {
					temp->left = LeftRotate(temp->left);
					temp = RightRotate(temp);
					index--;
					Rotate(temp, index);
				}
				else if (LeftB <= -1) {
					temp = RightRotate(temp);
					index--;
					Rotate(temp, index);
				}
				else if (FullNode(temp->left) && LeftB == 0) {
					temp = RightRotate(temp);
					index--;
					Rotate(temp, index);
					break;
				}
				balance = getHeightRec(temp->right) - getHeightRec(temp->left);
			}
		}
		Rotate(temp->left, index);
		Rotate(temp->right, index);
	};
	class compare1{
			public:
			bool operator()(pair<int,HuffNode*> a,pair<int,HuffNode*> b){
				if(a.second->freq==b.second->freq){
					return a.first>b.first;
				}
				else{
					return a.second->freq>b.second->freq;
				}
			}
	};
	
	Hufftree (vector<pair<char, pair<int, int>>> &list,int i){
		priority_queue<pair<int,HuffNode*>,vector<pair<int,HuffNode*>>,compare1> dequeCompare;
		//std::cout<<"Build Tree\n";
		for(auto x: list){
			HuffNode* temp=new HuffNode(x.first,x.second.first);
			dequeCompare.push({x.second.second,temp});
		}	
		while(dequeCompare.size()!=1){
			pair<int,HuffNode*> left=dequeCompare.top(); 
			dequeCompare.pop();
			pair<int,HuffNode*> right=dequeCompare.top();
			dequeCompare.pop();
			HuffNode* temp=new HuffNode(left.second,right.second);
			int abc=3;
			Rotate(temp,abc);
			dequeCompare.push({i,temp});
			i++;
		}	
		/*set root*/
		root=dequeCompare.top().second;
		dequeCompare.pop();
    }
	void printMinheap(priority_queue<pair<int,HuffNode*>,vector<pair<int,HuffNode*>>,compare1> minHeap){
		while(!minHeap.empty()){
			pair<int,HuffNode*> temp=minHeap.top();
			minHeap.pop();
			std::cout<<temp.second->data<<" "<<temp.second->freq<<endl;
		}
	}
	HuffNode* getRoot(){
		return root;
	}
	void getNewsInorder(HuffNode*root,string &news){
		if(root==nullptr)return;
		getNewsInorder(root->left,news);
		if(root->isLeaf){
			news+=root->data;
			news+="\n";
		}
		else{
			news+=to_string(root->freq);
			news+="\n";
		}
		getNewsInorder(root->right,news);
	}
	void setCode(HuffNode* node,string codesave,unordered_map<char,string>& namecode){
		if(node==nullptr)return;
		node->code=codesave;
		if(node->isLeaf){
			namecode[node->data]=codesave;
			return;
		}
		setCode(node->left,codesave+"0",namecode);
		setCode(node->right,codesave+"1",namecode);
	}
	void displayTreeVertical(HuffNode *node, int indent) const
	{
		if (node == nullptr)
		return;

		displayTreeVertical(node->right, indent + 4);

		for (int i = 0; i < indent; ++i)
			std::cout << " ";
		std::cout << node->data<< "-";
		std::cout << node->freq << "-" <<node->code<< endl;

		displayTreeVertical(node->left, indent + 4);
	}	
	void detructor(HuffNode* node){
		if(node==nullptr)return;
		detructor(node->left);
		detructor(node->right);
		delete node;
	}
	~Hufftree(){
		detructor(root);
	}
};

struct Customer
{
	string name;
	int result;
	Customer(){
	}
	void printCustomer(){
		std::cout<<result<<endl;
	}
	string convertName(Customer*A,unordered_map<char,string> code){
		string res="";
		string name=A->name;
		for(size_t i=0;i<name.length();i++){
			char c=name[i];
			res+=code[c];
		}
		return res;
	}
	int convertBinarytoDecimal(string binary){
		reverse(binary.begin(),binary.end());
		int decimal=0;
		int n=binary.length();
		if(n>10){
			n=10;
		}
		for(int i=0;i<n;i++){
			if(binary[i]=='1'){
				decimal=decimal*2+1;
			}
			else{
				decimal=decimal*2;
			}
		}
		return decimal;
	}
};
/*DONE SORT*/
bool compare(pair<char, int>& a, pair<char, int>& b) {
    if (a.second != b.second) {
        return a.second < b.second;
    }
	else{
		if(a.first<97&&b.first<97){
			return a.first < b.first;
		}
		else if(a.first>=97&&b.first>=97){
			return a.first < b.first;
		}
   		return a.first > b.first;
	}
}

string sortFrequent2(Customer*A,unordered_map<char,int>& map, vector<pair<char,int>>& saving){
	string E=A->name;
	for(int i=E.length()-1;i>=0;i--){
		char c=E[i];
		map[c]++;
	}

	vector<pair<char,int>> temp;

	for( auto x: map){
		temp.push_back({x.first,x.second});
	}
	
	sort(temp.begin(),temp.end(),compare);
	
	string temp2="";
    
	for (auto x : temp) {
        for (int i = 0; i < x.second; i++) {
            temp2 += x.first;
        }
    }
	saving=temp;
	temp.clear();
	if(map.size()<3)return "false";
	return temp2;
}

/*BINARY TREE*/
class BinarySearchTree
{
public:
    class Node;
private:
    Node* root;
	int count=0;
public:
	vector<vector<long long>> pascal;
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree()
    {	clearTree(root);
		pascal.clear();
    }
	void clearTree(Node*root){
		if(root==nullptr)return;
		clearTree(root->pLeft);
		clearTree(root->pRight);
		Customer* temp=root->value;
		delete temp;
		delete root;
	}
    /*Insert*/
    void addhelp(Node*&root, Customer* value, Node*insert){
        if(root == nullptr){
            root = insert;
        }
        else if(value->result >= root->value->result){
            addhelp(root->pRight, value, insert);
        }
        else{
            addhelp(root->pLeft, value, insert);
        }
    }

    void add(Customer* value){
        Node*temp=new Node(value);
        addhelp(root,value,temp);
		count++;
    }
   
    Node*findMinimumRight(Node*root){
    if(root->pLeft!=nullptr){
        return findMinimumRight(root->pLeft);
    }
    else{return root;}  
    }
    Node* deleteMinimum(Node* root) {
		if (root->pLeft == nullptr) {
			Node* temp = root->pRight;
			delete root;
			return temp;
		}
		root->pLeft = deleteMinimum(root->pLeft);
		return root;
	}
    /*delete*/
    Node* deleteHelp (Node*root,Customer* value){
    if(root==nullptr)return nullptr;
    if(value->result<root->value->result){
        root->pLeft= deleteHelp(root->pLeft,value);
    }
    else if(value->result>=root->value->result&&value!=root->value){
        root->pRight= deleteHelp(root->pRight,value);
    }
    else{
        //case no child
        if(root->pRight==nullptr&&root->pLeft==nullptr){
			//std::cout<<"case 0 child\n";
            delete root;
            root=nullptr;
        }
        //case 1 child
        else if(root->pRight==nullptr){
			//std::cout<<"case 1 child\n";
            Node*temp=root;
            root=root->pLeft;
            delete temp;
        }
        else if(root->pLeft==nullptr){
			//std::cout<<"case 1 child\n";
            Node*temp=root;
            root=root->pRight;
            delete temp;
        }
        //case 2 child
        else{
			// //std::cout<<"case 2 child\n";
            Node*temp=findMinimumRight(root->pRight);
            root->value=temp->value;
            root->pRight=deleteMinimum(root->pRight);
			
        }
    }
    return root;
    }
    
    void deleteNode(Customer* value){
        if(root!=nullptr){
            root=deleteHelp(root,value);
			count--;
        }
    }
    
    /*printInorder*/
    void inOrder(Node* root) {
        if (root != nullptr) {
            inOrder(root->pLeft);
            root->value->printCustomer();
            inOrder(root->pRight);
        }
    }
    
    Node* getRoot(){
        return root;
    }
    
    /*postOrder&Hoanvi-KOSUKEN*/
    void postOrderhelp(Node* root,vector<int>&A) {
        if (root != nullptr) {
            postOrderhelp(root->pLeft,A);
            postOrderhelp(root->pRight,A);
            A.push_back(root->value->result);
        }
    }
    void convertPostorder(vector<int>& A){
        postOrderhelp(root,A);
    }
	long long dfs(vector<int> &A)
	{
		int lenght = A.size();
		if (lenght <= 2)
		return 1;
		vector<int> left, right;
		for (int i = 1; i < lenght; i++)
		{
			if (A[i] < A[0]){left.push_back(A[i]);}
			else{right.push_back(A[i]);}
		}
		long long l = dfs(left);
		long long r = dfs(right);
		int llength = left.size();
		return (((pascal[lenght - 1][llength] * l)) * r);
	}

	long long numPer(vector<int> &A)
	{
		int n = A.size();
		pascal.resize(n + 1);
		for (int i = 0; i < n + 1; i++){
		pascal[i] = vector<long long>(i + 1, 1);
			for (int j = 1; j < i; j++){
				pascal[i][j] = (pascal[i - 1][j - 1] + pascal[i - 1][j]);
			}
		}
		long long ans = dfs(A);
		for (auto it: pascal){it.clear();}
		pascal.clear();
		return ans;
	}
    long long  caculatePermutation(){
        vector<int> A;
        convertPostorder(A);
		vector<int> A1;
		int len = A.size();
		for (int i = len - 1; i >= 0; i--){
			A1.push_back(A[i]);
			A.pop_back();
		}
		long long result= numPer(A1);
		pascal.clear();
		A.clear();
		return result;
    }
	

    class Node  
    {
    private:
        Customer* value;
        Node* pLeft, * pRight;
        friend class BinarySearchTree;
    public:
        Node(Customer* value) : value(value), pLeft(NULL), pRight(NULL) {}	
    };
};

/*SUKUNA*/
/*Node Sukuna*/
struct nodeSukuna{
	int ID;
	nodeSukuna* left=nullptr;
	nodeSukuna* right=nullptr;
	deque<Customer*> rootDeque;
	nodeSukuna(int ID){this->ID=ID;}
	nodeSukuna* insert(Customer* customerAdd);
	void printLifo(int NUM,int ID);
	void remove();
	~nodeSukuna(){
		while(!rootDeque.empty()){
			Customer* temp=rootDeque.front();
			rootDeque.pop_front();
			delete temp;
		}
	}
};
/*In theo Lifo- Lenh CLEAVE*/
void nodeSukuna::printLifo(int NUM,int ID){
	if(int(rootDeque.size())<NUM){
		NUM=rootDeque.size();
	}
	for(int i=0;i<NUM;i++){
		std::cout<<ID<<"-"<<rootDeque[rootDeque.size()-i-1]->result<<endl;
	}
}
/*Insert Node*/
nodeSukuna *nodeSukuna::insert(Customer *customerAdd)
{	
    
	rootDeque.push_back(customerAdd);
	return this;
}
/*Xoa theo FIFO va print*/
void nodeSukuna::remove()
{	Customer* temp=rootDeque.front();
	std::cout<<rootDeque.front()->result<<"-"<<ID<<endl;
	rootDeque.pop_front();
	delete temp;
}

class sukunaRes
{
	private:
		/* Min-Heap */
		vector<nodeSukuna*> vectorSukuna;
		vector<int> LRU;
		int MAXSIZE;
	public:
		sukunaRes(int MAXSIZE){
			this->MAXSIZE=MAXSIZE;
		}
		void insert(Customer * customerAdd);
		nodeSukuna* sreach(int ID);
		int sreachIndex(nodeSukuna* node);
		void sukunaDelete(int NUM);
		void printPreorder(int& NUM,int index);
		void reHeapUp(int& index);
		void reHeapDown(int& index);
		void heapify();
		bool compare(nodeSukuna* node1,nodeSukuna* node2);
		void selectSort(vector<nodeSukuna*> &IDdelete,int NUM,int left, int right);

		~sukunaRes();
};

void sukunaRes::insert(Customer *customerAdd)
{

			int ID= customerAdd->result%MAXSIZE+1;

			nodeSukuna*temp= sreach(ID);

			if(temp==nullptr){
				nodeSukuna* newNode= new nodeSukuna(ID);
				newNode->insert(customerAdd);
				vectorSukuna.push_back(newNode);
				int index=vectorSukuna.size()-1;
				LRU.push_back(ID);
				reHeapUp(index);
			}
			else{
				temp->insert(customerAdd);
				for(int i=0;i<int(LRU.size());i++){
					if(LRU[i]==ID){
						LRU.erase(LRU.begin()+i);
						break;
					}
				}
				int tr=sreachIndex(temp);
				LRU.push_back(ID);
				reHeapDown(tr);
				reHeapUp(tr);
			}
}

nodeSukuna *sukunaRes::sreach(int ID)
{
	for(size_t i=0;i<vectorSukuna.size();i++){
		if(vectorSukuna[i]->ID==ID){
			return vectorSukuna[i];
		}
	}
    return nullptr;
}

int sukunaRes::sreachIndex(nodeSukuna *node)
{
	for(size_t i=0;i<vectorSukuna.size();i++){
		if(vectorSukuna[i]==node){
			return i;
		}
	}
	return -1;
}
void sukunaRes::sukunaDelete(int NUM)
{	
	list<nodeSukuna*> IDdelete;
	for(size_t i=0;i<vectorSukuna.size();i++){
		IDdelete.push_back(vectorSukuna[i]);
	}
	IDdelete.sort([this](nodeSukuna*a,nodeSukuna*b){
		return compare(a,b);}
	);
	int Num2=NUM;
	if(NUM>int(IDdelete.size())){
		NUM=IDdelete.size();
	}
	for(int i=0;i<NUM;i++){

		for(size_t k=0;k<LRU.size();k++){
			if(LRU[k]==IDdelete.front()->ID){
				LRU.erase(LRU.begin()+k);
				break;
			}
		}
		LRU.push_back(IDdelete.front()->ID);
		int index=sreachIndex(IDdelete.front());
		for(int j=0;j<Num2;j++){
			IDdelete.front()->remove();
			if(IDdelete.front()->rootDeque.size()==0)break;
		}
		reHeapUp(index);
		reHeapDown(index);
		if(IDdelete.front()->rootDeque.size()==0){
			LRU.pop_back();
			swap(vectorSukuna[index],vectorSukuna[vectorSukuna.size()-1]);
			vectorSukuna.pop_back();
			reHeapDown(index);
			nodeSukuna* temp=IDdelete.front();
			delete temp;
		}
		IDdelete.pop_front();
	}
	IDdelete.clear();
}

sukunaRes::~sukunaRes()
{
	for(size_t i=0;i<vectorSukuna.size();i++){
		delete vectorSukuna[i];
	}
}

void sukunaRes::printPreorder(int& NUM,int index)
{	if(NUM<=0)return;
	if(index>=int(vectorSukuna.size()))return;
	if(vectorSukuna[index]->rootDeque.size()==0)return;
	vectorSukuna[index]->printLifo(NUM,vectorSukuna[index]->ID);
	printPreorder(NUM,index*2+1);
	printPreorder(NUM,index*2+2);
}

void sukunaRes::reHeapUp(int& index)
{
	while(index>0){
		if(compare(vectorSukuna[index],vectorSukuna[(index-1)/2])){
			swap(vectorSukuna[index],vectorSukuna[(index-1)/2]);
			index=(index-1)/2;
		}
		else{
			break;
		}
	}
}

void sukunaRes::reHeapDown(int& index)
{	
	while(index<int(vectorSukuna.size())){
		int left=2*index+1;
		int right=2*index+2;
		if(left<int(vectorSukuna.size())){
			int min=left;
			if(right<int(vectorSukuna.size())&&compare(vectorSukuna[right],vectorSukuna[left])){
				min=right;
			}
			if(compare(vectorSukuna[min],vectorSukuna[index])){
				swap(vectorSukuna[index],vectorSukuna[min]);
				index=min;
			} 
			else{
				break;
			}
		}
		else{
			break;
		}
	}
}

void sukunaRes::heapify()
{
	for(int i=vectorSukuna.size()/2;i>=0;i--){
		reHeapDown(i);
	}
}

bool sukunaRes::compare(nodeSukuna *node1, nodeSukuna *node2){
			if(node1->rootDeque.size()==node2->rootDeque.size()){
				for(size_t i=0;i<this->LRU.size();i++){
					if(this->LRU[i]==node1->ID){
						return true; /*Node1 <Node2*/
					}
					if(this->LRU[i]==node2->ID){
						return false; /*Node1 >Node2*/
					}
				}
			}
			return node1->rootDeque.size()<node2->rootDeque.size();
}

/*---------------------------------------------*/
struct nodeGojo{
	/*BST tree root*/ 
	int count=0;
	BinarySearchTree* rootBST;
	queue<Customer*> listCustomer;
	nodeGojo(){
		rootBST=new BinarySearchTree();
	}
	~nodeGojo(){
		delete rootBST;
	}
};

class gojoRes
{
	private:
		/* data Hash-Map*/
		int MAXSIZE;
		unordered_map<int,nodeGojo*> hashMap;
	public:
		gojoRes(int MAXSIZE){
			this->MAXSIZE=MAXSIZE;
			for(int i=1;i<=MAXSIZE;i++){
				hashMap[i]=new nodeGojo();
			}
		};
		void insert(Customer* demo){
			int ID=demo->result%MAXSIZE+1;
			nodeGojo* temp= hashMap.find(ID)->second;
			temp->rootBST->add(demo);
			temp->listCustomer.push(demo);
			temp->count++;
		}
		void printInorder(int NUM){
			nodeGojo*temp=hashMap.find(NUM)->second;
			if(temp->count==0)return;
			temp->rootBST->inOrder(temp->rootBST->getRoot());
		}

		void deletePostorder(){
			for(int i=1;i<=MAXSIZE;i++){
				nodeGojo* temp=hashMap.find(i)->second;
				if(temp->count==0){
					continue;
				}

				int Y=temp->rootBST->caculatePermutation()%MAXSIZE;
				if(Y>int(temp->listCustomer.size())){
					Y=int(temp->listCustomer.size());
				}
				for(int i=0;i<Y;i++){
					if(temp->listCustomer.empty())break;
					Customer*deletcus=temp->listCustomer.front();
					temp->listCustomer.pop();
					temp->rootBST->deleteNode(deletcus);
					delete deletcus;
					temp->count--;
				}
			}
		}
		~gojoRes(){
			for(int i=1;i<=MAXSIZE;i++){
				nodeGojo* temp=hashMap.find(i)->second;
				delete temp;
			}
		}
};
		
class restaurant
{
	private:
	gojoRes* GOJO;
	sukunaRes* SUKUNA;
	string news;
	int MAXSIZE;
	public:
	restaurant(){
		GOJO=nullptr;
		SUKUNA=nullptr;
		news="";
	}
	void setMAXSIZE(int MAXSIZE){
		this->MAXSIZE=MAXSIZE;
		this->GOJO = new gojoRes(this->MAXSIZE);
		this->SUKUNA = new sukunaRes(this->MAXSIZE);
	}
	void LAPSE(string NAME);
	void KOKUSEN();
	void KEITEIKEN(int NUM);
	void HAND();
	void LIMITLESS(int NUM);
	void CLEAVE(int NUM);
	~restaurant(){
		delete GOJO;
		delete SUKUNA;
	}
};

/*BOTH*/
void restaurant::LAPSE(string NAME){
	
	Customer* A=new Customer();
	A->name=NAME;
	string NAME2=NAME;
	vector<pair<char,int>> saving;
	unordered_map<char,int> map;
	string temp= sortFrequent2(A, map, saving);
	if(temp=="false"){ 
		delete A;
		map.clear();
		return ;
	}
		/*tao danh sach X*/
	for(size_t i=0;i<temp.length();i++){
		int s=map[NAME[i]];
		if(NAME[i]>=65 && NAME[i]<=90){
			char new_char = ((NAME[i] - 65) + s) % 26 + 65;
			NAME[i] =new_char;
		}else if(NAME[i]>=97 && NAME[i]<=122){
			char new_char = ((NAME[i] - 97) + s) % 26 + 97;
			NAME[i] =new_char;
		}else{
			NAME[i]+=s;
		}
	}
	A->name=NAME;
	map.clear();
	temp=sortFrequent2(A,map,saving);
	/*BUILD TREE*/
     vector<pair<char, pair<int, int>>> list;
	int i=0;
	for(auto x: saving){
		list.push_back({x.first,{x.second,i}});
		i++;
	}
	Hufftree* treetemp= new Hufftree(list,i);
	
	unordered_map<char,string> code;
	treetemp->setCode(treetemp->getRoot(),"",code);
	A->result=A->convertBinarytoDecimal(A->convertName(A,code));
	
	if(A->result%2!=0){
		GOJO->insert(A);
	}
	else{
		SUKUNA->insert(A);
	}
	
	map.clear();
	code.clear();
	saving.clear();
	list.clear();
	this->news="";
	treetemp->getNewsInorder(treetemp->getRoot(),news);
	delete treetemp;
}
void restaurant::HAND(){
	std::cout<<news;
}

/*SUKUNA*/
void restaurant::KEITEIKEN(int NUM){
	if(NUM<=0)return;
	SUKUNA->sukunaDelete(NUM);
}
void restaurant::CLEAVE(int NUM){
	SUKUNA->printPreorder(NUM,0);
}

/*GOJO*/
void restaurant::KOKUSEN(){
	GOJO->deletePostorder();
}
void restaurant::LIMITLESS(int NUM){
	if(NUM<=0||NUM>MAXSIZE)return;
	GOJO->printInorder(NUM);
}

void simulate(string filename)
{
	ifstream ss(filename);
	string str, maxsize, name, num;
	restaurant * run=new restaurant();
	while(ss >> str)
	{   
		if(str == "MAXSIZE")
		{
			ss>> maxsize;
			run->setMAXSIZE(stoi(maxsize));
    	}
        else if(str == "LAPSE") 
        {
            ss>> name;
			run->LAPSE(name);
    	}
		else if(str == "HAND")
        {
            run->HAND();
    	}
    	else if(str == "KEITEIKEN") 
        {
            ss>> num;
			run->KEITEIKEN(stoi(num));
    	}
		else if(str=="KOKUSEN")
		{
			run->KOKUSEN();
		}
		else if(str=="CLEAVE"){
			ss>> num;
			run->CLEAVE(stoi(num));
		}
		else if(str=="LIMITLESS"){
			ss>> num;
			run->LIMITLESS(stoi(num));
		}
	}
	delete run;

	return;
}

