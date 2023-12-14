
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
		// if(a->left!=nullptr&&a->right!=nullptr){
		// 	a->freq=a->left->freq+a->right->freq;
		// }
		// else if(a->left==nullptr&&a->right!=nullptr){
		// 	a->freq=a->right->freq;
		// }
		// else if(a->left!=nullptr&&a->right==nullptr){
		// 	a->freq=a->left->freq;
		// }
		// b->freq=b->left->freq+b->right->freq;
		return b;
	}

	HuffNode*rotateRight(HuffNode* a){
		HuffNode* b=a->left;
		a->left=b->right;
		b->right=a;
		// if(a->left!=nullptr&&a->right!=nullptr){
		// 	a->freq=a->left->freq+a->right->freq;
		// }
		// else if(a->left==nullptr&&a->right!=nullptr){
		// 	a->freq=a->right->freq;
		// }
		// else if(a->left!=nullptr&&a->right==nullptr){
		// 	a->freq=a->left->freq;
		// }
		// b->freq=b->left->freq+b->right->freq;
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
				//if(index==0)return temp;
				temp->right=Rotate(temp->right,index);
				/*Preorder*/

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
		priority_queue<pair<int,HuffNode*>,vector<pair<int,HuffNode*>>,compare1> dequeCompare;

		//cout<<"Build Tree\n";
		for(auto x: list){
			//cout<<x.first<<" "<<x.second.first<<" "<<x.second.second<<endl;
			HuffNode* temp=new HuffNode(x.first,x.second.first);
			dequeCompare.push({x.second.second,temp});
		}	

		//cout<<"insert tree\n";
		while(dequeCompare.size()!=1){
			//cout<<"OK\n";
			/*Lôi 2 thằng ở đầu ra merge*/
			pair<int,HuffNode*> left=dequeCompare.top(); 
			dequeCompare.pop();
			pair<int,HuffNode*> right=dequeCompare.top();
			dequeCompare.pop();
			//cout<<"left"<<left.second->data<<" "<<left.second->freq<<" \n right"<<right.second->data<<" "<<right.second->freq<<endl;
			/*Tạo 1 thằng mới có freq là tổng 2 thằng trên và chứa 2 thằng trên*/	
			HuffNode* temp=new HuffNode(left.second,right.second);
			temp=Rotate(temp,3);
			/*push vào lại Queue*/
			dequeCompare.push({i,temp});
			//printMinheap(dequeCompare);
			//displayTreeVertical(temp,0);
			i++;
			//cout<<"-------------------\n";
		}	
		/*set lại root*/
		root=dequeCompare.top().second;
		dequeCompare.pop();
		//delete &dequeCompare;
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
	// vector<pair<char,int>> saving;
	// unordered_map<char,string> code;
	Customer(){
	}
	void printCustomer(){
		cout<<result<<endl;
	}
	string convertName(Customer*A,unordered_map<char,string> code){
		string res="";
		string name=A->name;
		for(size_t i=0;i<name.length();i++){
			char c=name[i];
			//cout<<c<<" "<<code[c]<<endl;
			res+=code[c];
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
	// ~Customer(){
	// 	saving.clear();
	// 	code.clear();
	// }	
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

// string sortFrequent(Customer *A, unordered_map<char,int>& map){
// 	string E=A->name;
// 	for(int i=E.length()-1;i>=0;i--){
// 		char c=E[i];
// 		map[c]++;
// 	}

// 	vector<pair<char,int>> temp;

// 	for( auto x: map){
// 		temp.push_back({x.first,x.second});
// 	}
	
// 	sort(temp.begin(),temp.end(),compare);
	
// 	string temp2="";
    
// 	for (auto x : temp) {
//         for (int i = 0; i < x.second; i++) {
//             temp2 += x.first;
//         }
//     }
// 	A->saving.clear();
// 	A->saving=temp;
// 	temp.clear();
// 	if(map.size()<3)return "false";
// 	return temp2;
// }

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
 
    size_t N1 = left.size();
 
    //size_t N2 = right.size();
 
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
	int count=0;
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree()
    {	
		while (root != nullptr) {
			deleteNode(root->value);
		}
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
		//cout<<"deleteHelp\n";
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
			//cout<<"case 0 child\n";
            delete root;
            root=nullptr;
        }
        //case 1 child
        else if(root->pRight==nullptr){
			//cout<<"case 1 child\n";
            Node*temp=root;
            root=root->pLeft;
            delete temp;
        }
        else if(root->pLeft==nullptr){
			//cout<<"case 1 child\n";
            Node*temp=root;
            root=root->pRight;
            delete temp;
        }
        //case 2 child
        else{
			//cout<<"case 2 child\n";
            Node*temp=findMinimumRight(root->pRight);
            root->value=temp->value;
            root->pRight=deleteMinimum(root->pRight);
        }
    }
    return root;
    }
    
    void deleteNode(Customer* value){
        if(root!=nullptr){
			//cout<<value->result<<endl;
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
    int caculatePermutation(){
        vector<int> A;
		//cout<<"A\n";
        convertPostorder(A);
		//cout<<"A\n";
        int fact[A.size()];
        calculateFact(fact,A.size());
		//cout<<"fact\n";
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
    };
};

/*---------------------------------------------*/

/*SUKUNA*/
/*Node Sukuna*/
struct nodeSukuna{
    //int count=0;
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
		cout<<ID<<"-"<<rootDeque[rootDeque.size()-i-1]->result<<endl;
	}
}
/*Insert Node*/
nodeSukuna *nodeSukuna::insert(Customer *customerAdd)
{	
    
	//this->count++;
	rootDeque.push_back(customerAdd);
	return this;
}
/*Xoa theo FIFO va print*/
void nodeSukuna::remove()
{	Customer* temp=rootDeque.front();
	cout<<rootDeque.front()->result<<"-"<<ID<<endl;
	//count--;
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
		void printTreeMinHeap(int index, int indent = 0) {
			if (index < int(vectorSukuna.size())) {
				printTreeMinHeap(2 * index + 2, indent + 4);  // right child

				if (indent) {
					std::cout << std::setw(indent) << ' ';
				}

				if (2 * index + 2 < int(vectorSukuna.size())) {
					std::cout << "/\n" << std::setw(indent) << ' ';
				}

				std::cout << vectorSukuna[index]->ID << "-" << vectorSukuna[index]->rootDeque.size() << "\n ";

				if (2 * index + 1 <int( vectorSukuna.size())) {
					std::cout << std::setw(indent) << ' ' << "\\\n";
					printTreeMinHeap(2 * index + 1, indent + 4);  // left child
				}
			}
		}
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
	//cout<<"IDdelete\n";
	for(int i=0;i<NUM;i++){
		//cout<<"STEP"<<i<<endl;
		//printTreeMinHeap(0);

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
		//cout<<"reHeap\n";
		reHeapUp(index);
		reHeapDown(index);
		//cout<<"index"<<index<<endl;
		if(IDdelete.front()->rootDeque.size()==0){
			//cout<<"delete\n";
			LRU.pop_back();
			swap(vectorSukuna[index],vectorSukuna[vectorSukuna.size()-1]);
			
			//cout<<"delete\n";
			vectorSukuna.pop_back();
			//cout<<"index"<<index<<endl;
			//cout<<"delete\n";
			reHeapDown(index);
			nodeSukuna* temp=IDdelete.front();
			delete temp;
			//cout<<"delete\n";
		}
		IDdelete.pop_front();
	}
	//cout<<"OK\n";	
	IDdelete.clear();
}

// void sukunaRes::selectSort(vector<nodeSukuna*> &IDdelete,int NUM,int left, int right)
// {
// 	if(NUM==0||left>=vectorSukuna.size())return ;
// 	else{
// 		if(right>=vectorSukuna.size()){
// 			NUM--;
// 			IDdelete.push_back(vectorSukuna[left]);
// 			selectSort(IDdelete,NUM,2*left+1,2*left+2);
// 		}
// 		else if(compare(vectorSukuna[left],vectorSukuna[right])){
// 			NUM--;
// 			IDdelete.push_back(vectorSukuna[left]);
// 			if(compare(vectorSukuna[2*left+1],vectorSukuna[2*left+2])){
// 				selectSort(IDdelete,NUM,2*left+1,right);
// 			}
// 			else{
// 				selectSort(IDdelete,NUM,2*left+2,right);
// 			}
// 		}
// 		else{
// 			NUM--;
// 			IDdelete.push_back(vectorSukuna[right]);
// 			if(compare(vectorSukuna[2*right+1],vectorSukuna[2*right+2])){
// 				selectSort(IDdelete,NUM,left,2*right+2);
// 			}
// 			else{
// 				selectSort(IDdelete,NUM,left,2*right+1);
// 			}
// 		}
// 	}
// }


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
		//  cout<<vectorSukuna[index]->ID<<" "<<vectorSukuna[index]->rootDeque.size()<<endl;
		//  cout<<vectorSukuna[(index-1)/2]->ID<<" "<<vectorSukuna[(index-1)/2]->rootDeque.size()<<endl;
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
{	//cout<<"reHeapDown\n";
	while(index<int(vectorSukuna.size())){
		//cout<<"index"<<index<<endl;
		int left=2*index+1;
		int right=2*index+2;
		//cout<<"left"<<left<<endl;
		//cout<<"right"<<right<<endl;
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
			//cout<<ID<<endl;	
			temp->rootBST->add(demo);
			//cout<<"BST\n";
			temp->listCustomer.push(demo);
			//cout<<"Queue\n";
			temp->count++;
		}
		void printInorder(int NUM){
			nodeGojo*temp=hashMap.find(NUM)->second;
			if(temp->count==0)return;
			temp->rootBST->inOrder(temp->rootBST->getRoot());
		}

		void deletePostorder(){
			//cout<<"delete\n";
			for(int i=1;i<=MAXSIZE;i++){
				//cout<<i<<endl;
				nodeGojo* temp=hashMap.find(i)->second;
				if(temp->count==0){
					//cout<<"nodelete\n";
					continue;
				}

				//cout<<"candelete\n";
				int Y=temp->rootBST->caculatePermutation()%MAXSIZE;
				//cout<<Y<<endl;
				//cout<<"caculate\n";	
				/*Tien hanh xoa CustomerNode-FIFO*/
				if(Y>temp->count){
					Y=temp->count;
				}
				for(int i=0;i<Y;i++){
					if(temp->listCustomer.empty())break;
					Customer*deletcus=temp->listCustomer.front();
					temp->listCustomer.pop();
					//cout<<deletcus->result<<endl;
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

/*SUKUNA*/
void restaurant::LAPSE(string NAME){
	
	this->news="";
	Customer* A=new Customer();
	A->name=NAME;
	//A->name=NAME;
	string NAME2=NAME;
	vector<pair<char,int>> saving;
	unordered_map<char,int> map;
	//string temp= sortFrequent(A, map);
	string temp= sortFrequent2(A, map, saving);
		// cout<<temp<<endl;
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
		// cout<<NAME<<endl;
	A->name=NAME;
	map.clear();
	//temp= sortFrequent(A,map);
	temp=sortFrequent2(A,map,saving);
		// cout<<temp<<endl;
	/*BUILD TREE*/
     vector<pair<char, pair<int, int>>> list;
	int i=0;
	for(auto x: saving){
		list.push_back({x.first,{x.second,i}});
		i++;
	}

		// for(auto x: list){
		// 	cout<<x.first<<" "<<x.second.first<<" "<<x.second.second<<endl;
		// }
	Hufftree* treetemp= new Hufftree(list,i);
	
	unordered_map<char,string> code;
	treetemp->setCode(treetemp->getRoot(),"",code);

	A->result=A->convertBinarytoDecimal(A->convertName(A,code));
	//A->tree->displayTreeVertical(A->tree->getRoot(),0);
	//cout<<A->result<<endl;

	//int ID=A->result%MAXSIZE+1;
	if(A->result%2!=0){
		//cout<<"GOJO\n";
		GOJO->insert(A);
	}
	else{
		//cout<<"SUKUNA\n";
		SUKUNA->insert(A);
	}
	map.clear();
	code.clear();
	saving.clear();
	// A->code.clear();
	// A->saving.clear();
	list.clear();

	//cout<<A->result<<endl;
	//treetemp->displayTreeVertical(treetemp->getRoot(),0);
	//set news here
	treetemp->getNewsInorder(treetemp->getRoot(),news);
	delete treetemp;
}

/*DONE*/
void restaurant::HAND(){
	cout<<news;
}

/*SUKUNA*/

void restaurant::KEITEIKEN(int NUM){
	if(NUM<=0)return;
	SUKUNA->sukunaDelete(NUM);
}

/*DONE*/
void restaurant::CLEAVE(int NUM){
	//SUKUNA->printTreeMinHeap(0);
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
	{   //cout<<"Buoc thu "<<i<<endl;
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
	delete run;

	return;
}