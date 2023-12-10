
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
	void printTree();
	void printNode();
	friend class Hufftree;
};
void HuffNode:: printTree()
{  

	cout << this->data << " " << this->freq << endl;

	if (this->isLeaf) {
		return;
	}
	this->left->printTree();
	this->right->printTree();
}
void HuffNode::printNode()
{ 
	cout<<this->data<<" "<<this->freq<<endl;

}

class Hufftree
{	
private:
	HuffNode* root;
public:
 	Hufftree(){
		root=nullptr;
	}
	
	HuffNode*rotateLeft(HuffNode* a){
		HuffNode* b=a->right;
		a->right=b->left;
		b->left=a;
		if(a->left!=nullptr&&a->right!=nullptr){
			a->freq=a->left->freq+a->right->freq;
		}
		else if(a->left==nullptr&&a->right!=nullptr){
			a->freq=a->right->freq;
		}
		else if(a->left!=nullptr&&a->right==nullptr){
			a->freq=a->left->freq;
		}
		b->freq=b->left->freq+b->right->freq;
		return b;
	}

	HuffNode*rotateRight(HuffNode* a){
		HuffNode* b=a->left;
		a->left=b->right;
		b->right=a;
		if(a->left!=nullptr&&a->right!=nullptr){
			a->freq=a->left->freq+a->right->freq;
		}
		else if(a->left==nullptr&&a->right!=nullptr){
			a->freq=a->right->freq;
		}
		else if(a->left!=nullptr&&a->right==nullptr){
			a->freq=a->left->freq;
		}
		b->freq=b->left->freq+b->right->freq;
		return b;
	}
	
	int getHeightRec(HuffNode* node){
		if(node==nullptr)return 0;
		return 1+max(getHeightRec(node->left),getHeightRec(node->right));
	}

	int getBalance(HuffNode *subroot)
    {
        if (!subroot)
            return 0;
        return getHeightRec(subroot->left) - getHeightRec(subroot->right);
    }
    
	HuffNode* Rotate(HuffNode*temp, int index){
		//cout<<"Rotate\n";
		if(index>0){
			int l=getHeightRec(temp->left);
			int r=getHeightRec(temp->right);
			if(l-r>=2){
				if(temp->left->bf==1)temp->left=rotateLeft(temp->left);
				temp=rotateRight(temp);
				temp->bf=0;
				index--;
				if(index==0)return temp;
				temp->left=Rotate(temp->left,index);
				temp->right=Rotate(temp->right,index);
				l=getHeightRec(temp->left);
				r=getHeightRec(temp->right);
				if(l-r==1)temp->bf=-1;
				else if(l-r==-1)temp->bf=1;
				else temp->bf=0;
				return temp;
			}
			else if(l-r<=-2){
				if(temp->right->bf==-1)temp->right=rotateRight(temp->right);
				temp=rotateLeft(temp);
				temp->bf=0;
				index--;
				if(index==0)return temp;
				temp->left=Rotate(temp->left,index);
				temp->right=Rotate(temp->right,index);
				l=getHeightRec(temp->left);
				r=getHeightRec(temp->right);
				if(l-r==1)temp->bf=-1;
				else if(l-r==-1)temp->bf=1;
				else temp->bf=0;
				return temp;
			}
			else if(l-r==1)temp->bf=-1;
			else if(l-r==-1)temp->bf=1;
			else temp->bf=0;
		}
		return temp;
	}
	
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
		priority_queue<pair<int,HuffNode*>,vector<pair<int,HuffNode*>>,compare1> minHeap;

		//cout<<"Build Tree\n";
		for(auto x: list){
			//cout<<x.first<<" "<<x.second.first<<" "<<x.second.second<<endl;
			HuffNode* temp=new HuffNode(x.first,x.second.first);
			minHeap.push({x.second.second,temp});
		}	

		//cout<<"insert tree\n";
		while(minHeap.size()!=1){
			//cout<<"OK\n";
			/*Lôi 2 thằng ở đầu ra merge*/
			pair<int,HuffNode*> left=minHeap.top(); 
			minHeap.pop();
			pair<int,HuffNode*> right=minHeap.top();
			minHeap.pop();
			//cout<<"left"<<left.second->data<<" "<<left.second->freq<<" \n right"<<right.second->data<<" "<<right.second->freq<<endl;
			/*Tạo 1 thằng mới có freq là tổng 2 thằng trên và chứa 2 thằng trên*/	
			HuffNode* temp=new HuffNode(left.second,right.second);
			temp=Rotate(temp,3);
			/*push vào lại HuffTree*/
			minHeap.push({i,temp});
			//printMinheap(minHeap);
			//displayTreeVertical(temp,0);
			i++;
			//cout<<"-------------------\n";
		}	
		/*set lại root*/
		root=minHeap.top().second;
    }
	
	void printMinheap(priority_queue<pair<int,HuffNode*>,vector<pair<int,HuffNode*>>,compare1> minHeap){
		//cout<<"print minheap\n";
		while(!minHeap.empty()){
			pair<int,HuffNode*> temp=minHeap.top();
			minHeap.pop();
			cout<<temp.second->data<<" "<<temp.second->freq<<endl;
		}
	}
	
	HuffNode* getRoot(){
		return root;
	}

	string getNews(){
		string result="";
		if(root==nullptr){
			return result;
		}
		queue<HuffNode*> q;
		q.push(root);
		while(!q.empty()){
			HuffNode* temp=q.front();
			q.pop();
			if(temp!=nullptr&&temp->isLeaf){
				char a=temp->data;
				string b= to_string(temp->freq);
				result+=temp->data;
				result+="-";
				result+=to_string(temp->freq)+"\n";
			}
			else{
				q.push(temp->left);
				q.push(temp->right);
			}
		}
		return result;
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
			cout << " ";
		cout << node->data<< "-";
		cout << node->freq << "-" <<node->code<< endl;

		displayTreeVertical(node->left, indent + 4);
	}
	
	string printTree(){
		string res="";
		if(root==nullptr){
			return res;
		}
		queue<HuffNode*> q;
		q.push(root);
		while(!q.empty()){
			HuffNode* temp=q.front();
			q.pop();
			if(temp->isLeaf){
				res+=temp->data+"-"+to_string(temp->freq)+'\n';
			}
			else{
				q.push(temp->left);
				q.push(temp->right);
			}
		}
		return res;
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
	vector<pair<char,int>> saving;
	unordered_map<char,string> code;
	Customer(){
	}
	void printCustomer(){
		cout<<result<<endl;
	}
	string convertName(Customer*A){
		string res="";
		string name=A->name;
		for(int i=0;i<name.length();i++){
			char c=name[i];
			//cout<<c<<" "<<A->code[c]<<endl;
			res+=A->code[c];
		}
		return res;
	}
	int convertBinarytoDecimal(string binary){
		//cout<<binary<<endl;
		reverse(binary.begin(),binary.end());
		//cout<<binary<<endl;
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
		//cout<<decimal<<endl;
		return decimal;
	}
};
/*DONE SORT*/
bool compare(pair<char, int>& a, pair<char, int>& b) {
    if (a.second != b.second) {
        return a.second < b.second;
    }
	else{
   		return a.first < b.first;
	}
}

string sortFrequent(Customer *A, unordered_map<char,int>& map){
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
	A->saving=temp;
	if(map.size()<3)return "false";
	return temp2;
}

/*---------------------------------------------*/

/*BINARY TREE*/
void calculateFact(int fact[], int Num)
{
    fact[0] = 1;
    for (long long int i = 1; i < Num; i++) {
        fact[i] = fact[i - 1] * i;
    }
}
 
int nCr(int fact[], int Num, int R)
{
    if (R > Num)
        return 0;
 
    // nCr= fact(n)/(fact(r)*fact(n-r))
    int res = fact[Num] / fact[R];
    res /= fact[Num - R];
 
    return res;
}
 
int countWays(vector<int>& arr, int fact[])
{
    int Num = arr.size();
 
    if (Num <= 2) {
        return 1;
    }
 
    vector<int> left;
    vector<int> right;
 
    int root = arr[0];
 
    for (int i = 1; i < Num; i++) {
 
        if (arr[i] < root) {
            left.push_back(
                arr[i]);
        }
        else {
            right.push_back(
                arr[i]);
        }
    }
 
    int N1 = left.size();
 
    int N2 = right.size();
 
    int countLeft
        = countWays(left,
                    fact);
    int countRight
        = countWays(right,
                    fact);
 
    return nCr(fact, Num - 1, N1)* countLeft * countRight;
}

class BinarySearchTree
{
public:
    class Node;
private:
    Node* root;
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree()
    {
        
    }
    /*Insert*/
    void addhelp(Node*&root, Customer* value, Node*insert){
        if(root == nullptr){
            root = insert;
        }
        else if(value->result > root->value->result){
            addhelp(root->pRight, value, insert);
        }
        else{
            addhelp(root->pLeft, value, insert);
        }
    }

    void add(Customer* value){
        Node*temp=new Node(value);
        addhelp(root,value,temp);
    }
   
    Node*findMinimumRight(Node*root){
    if(root->pLeft!=nullptr){
        return findMinimumRight(root->pLeft);
    }
    else{return root;}  
    }
    
    /*delete*/
    Node* deleteHelp (Node*root,Customer* value){
    if(root==nullptr)return nullptr;
    if(value->result<root->value->result){
        root->pLeft= deleteHelp(root->pLeft,value);
    }
    else if(value->result>root->value->result){
        root->pRight= deleteHelp(root->pRight,value);
    }
    else{
        //case no child
        if(root->pRight==nullptr&&root->pLeft==nullptr){
            delete root;
            root=nullptr;
        }
        //case 1 child
        else if(root->pRight==nullptr){
            Node*temp=root;
            root=root->pLeft;
            delete temp;
        }
        else if(root->pLeft==nullptr){
            Node*temp=root;
            root=root->pRight;
            delete temp;
        }
        //case 2 child
        else{
            Node*temp=findMinimumRight(root->pRight);
            root->value=temp->value;
            root->pRight=deleteHelp(root->pRight,temp->value);
        }
    }
    return root;
    }
    
    void deleteNode(Customer* value){
        if(root!=nullptr){
            root=deleteHelp(root,value);
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
    int caculatePermutation(){
        vector<int> A;
        convertPostorder(A);
        int fact[A.size()];
        calculateFact(fact,A.size());
        return countWays(A,fact);
    }

    class Node  
    {
    private:
        Customer* value;
        Node* pLeft, * pRight;
        friend class BinarySearchTree;
    public:
        Node(Customer* value) : value(value), pLeft(NULL), pRight(NULL) {}
        ~Node() {}
    };
};

/*---------------------------------------------*/

/*SUKUNA*/
struct nodeSukuna{
    int count=0;
	int ID;
	nodeSukuna* left=nullptr;
	nodeSukuna* right=nullptr;
	deque<Customer*> rootDeque;
	nodeSukuna(int ID){this->ID=ID;}
	nodeSukuna* insert(Customer* customerAdd);
	void printLifo(int NUM,int ID);
	void remove();
};
/*In theo Lifo- Lenh CLEAVE*/
void nodeSukuna::printLifo(int NUM,int ID){
	if(rootDeque.size()<NUM){
		NUM=rootDeque.size();
	}
	for(int i=0;i<NUM;i++){
		cout<<ID<<"-"<<rootDeque[i]->result<<endl;
	}
	cout<<endl;
}
/*Insert Node*/
nodeSukuna *nodeSukuna::insert(Customer *customerAdd)
{	
    
	this->count++;
	rootDeque.push_back(customerAdd);
	return this;
}
/*Xoa theo FIFO va print*/
void nodeSukuna::remove()
{
	cout<<rootDeque.front()->result<<ID<<endl;
	count--;
	rootDeque.pop_front();
}

/*Class Sukuna*/
class sukunaRes
{
	private:
		/* Min-Heap */
		vector<nodeSukuna*> vectorSukuna;
		vector<nodeSukuna*> LRU;
		int MAXSIZE;
	public:
		sukunaRes(int MAXSIZE){
			this->MAXSIZE=MAXSIZE;
		}
		void insert(Customer * customerAdd);
		nodeSukuna* sreach(int ID);
		void sukunaDelete(int NUM);
		void printPreorder(int NUM,int index);
		void reHeapUp(int index);
		void reHeapDown(int index);
		void heapify();
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
			}
			else{
				temp->insert(customerAdd);
			}

			
}

nodeSukuna *sukunaRes::sreach(int ID)
{
	for(int i=0;i<vectorSukuna.size();i++){
		if(vectorSukuna[i]->ID==ID){
			return vectorSukuna[i];
		}
	}
    return nullptr;
}

void sukunaRes::sukunaDelete(int NUM)
{
			
}

void sukunaRes::printPreorder(int NUM,int index){}
/*---------------------------------------------*/

struct nodeGojo{
	/*BST tree root*/ 
	int count=0;
	BinarySearchTree* rootBST;
	queue<Customer*> listCustomer;
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
			for(int i=0;i<MAXSIZE;i++){

				nodeGojo* temp=hashMap.find(i)->second;
				if(temp->count==0)continue;

				int Y=temp->rootBST->caculatePermutation()%MAXSIZE;

				/*Tien hanh xoa CustomerNode-FIFO*/
				for(int i=0;i<Y;i++){
					Customer*deletcus=temp->listCustomer.front();
					temp->listCustomer.pop();
					temp->rootBST->deleteNode(deletcus);
				}
			}
		}
		~gojoRes();
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
	~restaurant();
};


/*BOTH*/

/*SUKUNA*/
void restaurant::LAPSE(string NAME){

	Customer* A=new Customer();
	A->name=NAME;

	unordered_map<char,int> map;
	string temp= sortFrequent(A, map);
		// cout<<temp<<endl;
	if(temp=="false") return ;
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
		// cout<<NAME<<endl;
	A->name=NAME;
	map.clear();
	temp= sortFrequent(A,map);
		// cout<<temp<<endl;
	/*BUILD TREE*/
     vector<pair<char, pair<int, int>>> list;
	int i=0;
	for(auto x: A->saving){
		list.push_back({x.first,{x.second,i}});
		i++;
	}

		// for(auto x: list){
		// 	cout<<x.first<<" "<<x.second.first<<" "<<x.second.second<<endl;
		// }
	Hufftree* treetemp= new Hufftree(list,i);
	treetemp->setCode(treetemp->getRoot(),"",A->code);
	A->result=A->convertBinarytoDecimal(A->convertName(A));
	//A->tree->displayTreeVertical(A->tree->getRoot(),0);
	cout<<A->result<<endl;

	int ID=A->result%MAXSIZE+1;
	if(A->result%2!=0){
		GOJO->insert(A);
	}
	else{
		SUKUNA->insert(A);
	}
	map.clear();
	A->code.clear();
	A->saving.clear();
	//set news here
	news=treetemp->getNews();
	delete treetemp;
}

/*DONE*/
void restaurant::HAND(){
	cout<<news;
}

/*SUKUNA*/

void restaurant::KEITEIKEN(int NUM){
	
}

/*DONE*/
void restaurant::CLEAVE(int NUM){
	SUKUNA->printPreorder(NUM,0);
}


/*GOJO-DONE*/
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
	int i=1;
	while(ss >> str)
	{   cout<<"Buoc thu "<<i<<endl;
		if(str == "MAXSIZE")
		{
			ss>> maxsize;
			run->setMAXSIZE(stoi(maxsize));
    	}
        else if(str == "LAPSE") // RED <NAME> <ENERGY>
        {
            ss>> name;
			run->LAPSE(name);
    	}
		else if(str == "HAND") // RED <NAME> <ENERGY>
        {
            run->HAND();
    	}
    	else if(str == "KEITEIKEN") // RED <NAME> <ENERGY>
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
		i++;
	}

	return;
}