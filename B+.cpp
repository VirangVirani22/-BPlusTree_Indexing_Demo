#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <filesystem>
#include <queue>
#include <stdlib.h>
using namespace std;
class Node {
    public:
    bool isLeaf;
    vector<int> keys;
   
    Node* ptr2next;              
    union ptr {                 
        vector<Node*> ptr2Tree;  
        vector<FILE*> dataPtr;   

        ptr(){
        }
        ~ptr(){
        }
    } ptr2TreeOrData;

    friend class BPTree; 
   
    Node()
    {
         this->isLeaf = false;
         this->ptr2next = NULL;
    }
};

Node* parent = NULL;

class BPTree {
    private:
        int maxIntChildLimit;                                   
        int maxLeafNodeLimit;                                   
        Node* root;                                            
    void insertInternal(int x, Node** cursor, Node** child)
    {
          if ((*cursor)->keys.size() < maxIntChildLimit - 1) 
          {
       
                int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
                (*cursor)->keys.push_back(x);
                (*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

                if (i != (*cursor)->keys.size() - 1) 
                {  
                    for (int j = (*cursor)->keys.size() - 1; j > i; j--) 
                    { 
                        (*cursor)->keys[j] = (*cursor)->keys[j - 1];
                    }

                    for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) 
                    {
                        (*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
                    }

                    (*cursor)->keys[i] = x;
                    (*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
                }
                cout << "Inserted key in the internal node" << endl;
          }
          else 
          {  
            cout << "Inserted Node in internal node successful" << endl;
            cout << "Overflow in internal: splitting internal nodes" << endl;

            vector<int> virtualKeyNode((*cursor)->keys);
            vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);

            int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();  
            virtualKeyNode.push_back(x);                                                                  
            virtualTreePtrNode.push_back(*child);                                                          

            if (i != virtualKeyNode.size() - 1) 
            {
                for (int j = virtualKeyNode.size() - 1; j > i; j--) {  
                    virtualKeyNode[j] = virtualKeyNode[j - 1];
                }

                for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                    virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
                }

                virtualKeyNode[i] = x;
                virtualTreePtrNode[i + 1] = *child;
            }

            int partitionKey;                                            
            partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];  
            int partitionIdx = (virtualKeyNode.size() / 2);


            (*cursor)->keys.resize(partitionIdx);
            (*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
            (*cursor)->ptr2TreeOrData.ptr2Tree.reserve(partitionIdx + 1);
            for (int i = 0; i < partitionIdx; i++) {
                (*cursor)->keys[i] = virtualKeyNode[i];
            }

            for (int i = 0; i < partitionIdx + 1; i++) {
                (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
            }

            Node* newInternalNode = new Node;
            new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
           

            for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
                newInternalNode->keys.push_back(virtualKeyNode[i]);
            }

            for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {  
                newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
            }

            if ((*cursor) == root) 
            {
                
                Node* newRoot = new Node;
                newRoot->keys.push_back(partitionKey);
                new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);
                root = newRoot;
                
            } 
            else 
            {
               insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
            }
        }
    }
    Node** findParent(Node* cursor, Node* child)
    {
        
        if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
            return NULL;

        for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
            if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
                parent = cursor;
            } else {
                Node* tmpCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
                findParent(tmpCursor, child);
            }
        }

        return &parent;
    }
    Node* firstLeftNode(Node* cursor)
    {
         if (cursor->isLeaf)
        return cursor;
        for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
            if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
                return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);

        return NULL;
    }

   public:
    BPTree()
    {
        this->maxIntChildLimit = 4;
        this->maxLeafNodeLimit = 3;
        this->root = NULL;
    }
    BPTree(int degreeInternal, int degreeLeaf)
    {
         this->maxIntChildLimit = degreeInternal;
        this->maxLeafNodeLimit = degreeLeaf;
        this->root = NULL;
    }
    Node* getRoot()
    {
         return this->root;
    }
    int getMaxIntChildLimit()
    {
        return maxIntChildLimit;
    }
    int getMaxLeafNodeLimit()
    {
        return maxLeafNodeLimit;
    }
    void setRoot(Node *ptr)
    {
        this->root = ptr;
    }
    void display(Node* cursor)
    {
        
        if (cursor == NULL) return;
        cout<<"Tree "<<endl;
        queue<Node*> q;
        q.push(cursor);

        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                Node* u = q.front(); q.pop();
                for (int val : u->keys)
                    cout << val << " ";

                cout << "||  ";
                
                if (u->isLeaf != true) {
                    for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
                        q.push(v);
                    }
                }
            }
            cout << endl<<endl;
        }
    }
    void seqDisplay(Node* cursor)
    {
        Node* firstLeft = firstLeftNode(cursor);

        if (firstLeft == NULL) {
            cout << "No Data in the Database yet!" << endl;
            return;
        }
        cout<<"sequential :"<<endl;
        while (firstLeft != NULL) {
            for (int i = 0; i < firstLeft->keys.size(); i++) {
                cout << firstLeft->keys[i] << " -> ";
            }

            firstLeft = firstLeft->ptr2next;
        }
        cout << endl<<endl;;
    }
    void search(int key)
    {
         if (root == NULL) 
         {
            cout << "NO Tuples Inserted yet" << endl;
            return;
         } 
         else 
         {
            Node* cursor = root;
            while (cursor->isLeaf == false) {
                int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
            }

            int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();  

            if (idx == cursor->keys.size() || cursor->keys[idx] != key)
            {
                cout << "Key NOT FOUND" << endl;
                return;
            }

            

            string fileName = "DATA/";
            string data;
            fileName += to_string(key) + ".txt";
            FILE* filePtr = fopen(fileName.c_str(), "r");
            cout << "Key FOUND" << endl;
            cout << "Corresponding Tuple Data is: ";
            cout<<key<<" ";
            char ch = fgetc(filePtr);
            while (ch != EOF) {
                printf("%c", ch);
                ch = fgetc(filePtr);
            }
            fclose(filePtr);
            cout << endl;
        }
    }
    void insert(int key, FILE* filePtr)
    {
        if (root == NULL) 
        {
            root = new Node;
            root->isLeaf = true;
            root->keys.push_back(key);
            new (&root->ptr2TreeOrData.dataPtr) std::vector<FILE*>;

            root->ptr2TreeOrData.dataPtr.push_back(filePtr);

            cout << key << ":  ROOT " << endl;
            return;
       } 
       else 
       {
            Node* cursor = root;
            Node* parent = NULL;
          
            while (cursor->isLeaf == false) {
                parent = cursor;
                int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
            }

            
            if (cursor->keys.size() < maxLeafNodeLimit) {
               
                int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor->keys.push_back(key);
                cursor->ptr2TreeOrData.dataPtr.push_back(filePtr);

                if (i != cursor->keys.size() - 1) {
                    for (int j = cursor->keys.size() - 1; j > i; j--) {  
                        cursor->keys[j] = cursor->keys[j - 1];
                        cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
                    }

                    
                    cursor->keys[i] = key;
                    cursor->ptr2TreeOrData.dataPtr[i] = filePtr;
                }
                cout << "Inserted successfully: " << key << endl;
            } 
            else 
            {

                vector<int> virtualNode(cursor->keys);
                vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

                
                int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

                virtualNode.push_back(key);
                virtualDataNode.push_back(filePtr);  

                if (i != virtualNode.size() - 1) {
                    for (int j = virtualNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                        virtualNode[j] = virtualNode[j - 1];
                        virtualDataNode[j] = virtualDataNode[j - 1];
                    }

                    
                    virtualNode[i] = key;
                    virtualDataNode[i] = filePtr;
                }
             

                Node* newLeaf = new Node;
                newLeaf->isLeaf = true;
                new (&newLeaf->ptr2TreeOrData.dataPtr) std::vector<FILE*>;

                Node* temp = cursor->ptr2next;
                cursor->ptr2next = newLeaf;
                newLeaf->ptr2next = temp;

                
                cursor->keys.resize((maxLeafNodeLimit) / 2 +1);//check +1 or not while partitioning
                cursor->ptr2TreeOrData.dataPtr.reserve((maxLeafNodeLimit) / 2 +1);
                for (int i = 0; i <= (maxLeafNodeLimit) / 2; i++) {
                    cursor->keys[i] = virtualNode[i];
                    cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
                }

               
                for (int i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++) {
                    newLeaf->keys.push_back(virtualNode[i]);
                    newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
                }

                if (cursor == root) {
                  

                    Node* newRoot = new Node;
                    newRoot->keys.push_back(newLeaf->keys[0]);
                    new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                    newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
                    newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
                    root = newRoot;
                   
                } else {
                    
                    insertInternal(newLeaf->keys[0], &parent, &newLeaf);
                }
            }
        }
    }
    void removeKey(int x)
    {
        Node* root = getRoot();

	
        if (root == NULL) {
            cout << "B+ Tree is Empty" << endl;
            return;
        }

        Node* cursor = root;
        Node* parent;
        int leftSibling, rightSibling;

       
        while (cursor->isLeaf != true) {
            for (int i = 0; i < cursor->keys.size(); i++) {
                parent = cursor;
                leftSibling = i - 1;
                rightSibling = i + 1;

                if (x < cursor->keys[i]) {
                    cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    leftSibling = i;
                    rightSibling = i + 2;
                    cursor = cursor->ptr2TreeOrData.ptr2Tree[i+1];
                    break;
                }
            }
        }

       
        int pos = 0;
        bool found = false;
        for (pos = 0; pos < cursor->keys.size(); pos++) {
            if (cursor->keys[pos] == x) {
                found = true;
                break;
            }
        }

        auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

        if (itr == cursor->keys.end()) {
            cout << "Key Not Found in the Tree" << endl;
            return;
        }
        
       
        string fileName = "DATA/" + to_string(x) + ".txt";
        char filePtr[256];
        strcpy(filePtr, fileName.c_str());

        
        if (remove(filePtr) == 0)
            cout << "SuccessFully Deleted file: " << fileName << endl<<endl;
        else
            cout << "Unable to delete the file: " << fileName << endl<<endl;

        
        for (int i = pos; i < cursor->keys.size()-1; i++) {
            cursor->keys[i] = cursor->keys[i+1];
            cursor->ptr2TreeOrData.dataPtr[i] = cursor->ptr2TreeOrData.dataPtr[i + 1];
        }
        int prev_size = cursor->keys.size();
        cursor->keys.resize(prev_size - 1);
        cursor->ptr2TreeOrData.dataPtr.resize(prev_size - 1);

       
        if (cursor == root) {
            if (cursor->keys.size() == 0) {
                setRoot(NULL);
                cout << "Tree is Empty Now :(" << endl;
            }
        }
        
        cout << "Deleted " << x << " From Leaf Node successfully" << endl;
        if (cursor->keys.size() >= (getMaxLeafNodeLimit()+1) / 2) {
           
            return;
        }

        cout << "UnderFlow in the leaf Node Happended" << endl;
        

        
        if (leftSibling >= 0) {
            Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

           
            if (leftNode->keys.size() >= (getMaxLeafNodeLimit()+1) / 2 +1) {

                
                int maxIdx = leftNode->keys.size()-1;
                cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
                cursor->ptr2TreeOrData.dataPtr
                    .insert(cursor->ptr2TreeOrData.dataPtr.begin(), leftNode->ptr2TreeOrData.dataPtr[maxIdx]);

               
                leftNode->keys.resize(maxIdx);
                leftNode->ptr2TreeOrData.dataPtr.resize(maxIdx);

                
                parent->keys[leftSibling] = cursor->keys[0];
                printf("Transferred from left sibling of leaf node\n");
                return;
            }
        }

        
        if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
            Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];


            if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

                
                int minIdx = 0;
                cursor->keys.push_back(rightNode->keys[minIdx]);
                cursor->ptr2TreeOrData.dataPtr
                    .push_back(rightNode->ptr2TreeOrData.dataPtr[minIdx]);

              
                rightNode->keys.erase(rightNode->keys.begin());
                rightNode->ptr2TreeOrData.dataPtr.erase(rightNode->ptr2TreeOrData.dataPtr.begin());

                
                parent->keys[rightSibling-1] = rightNode->keys[0];
                printf("Transferred from right sibling of leaf node\n");
                return;
            }
        }

       
        if (leftSibling >= 0) {
            Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
            
            for (int i = 0; i < cursor->keys.size(); i++) {
                leftNode->keys.push_back(cursor->keys[i]);
                leftNode->ptr2TreeOrData.dataPtr
                    .push_back(cursor->ptr2TreeOrData.dataPtr[i]);
            }
            leftNode->ptr2next = cursor->ptr2next;
            cout << "Merging two leaf Nodes" << endl;
            removeInternal(parent->keys[leftSibling], parent, cursor);
        }
        else if (rightSibling <= parent->keys.size()) {
            Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
           
            for (int i = 0; i < rightNode->keys.size(); i++) {
                cursor->keys.push_back(rightNode->keys[i]);
                cursor->ptr2TreeOrData.dataPtr
                    .push_back(rightNode->ptr2TreeOrData.dataPtr[i]);
            }
            cursor->ptr2next = rightNode->ptr2next;
            cout << "Merging two leaf Nodes" << endl;
            removeInternal(parent->keys[rightSibling-1], parent, rightNode);
            //delete rightNode;
        }
    }
    void removeInternal(int x, Node* cursor, Node* child)
    {
        Node* root = getRoot();

	
        if (cursor == root) {
            if (cursor->keys.size() == 1) {
                
                if (cursor->ptr2TreeOrData.ptr2Tree[1] == child) {
                    setRoot(cursor->ptr2TreeOrData.ptr2Tree[0]);
                   
                    cout << "Wow! New Changed Root" <<endl;
                    return;
                }
                else if (cursor->ptr2TreeOrData.ptr2Tree[0] == child) {
                    setRoot(cursor->ptr2TreeOrData.ptr2Tree[1]);
                   
                    cout << "Wow! New Changed Root" << endl;
                    return;
                }
            }
        }

       
        int pos;
        for (pos = 0; pos < cursor->keys.size(); pos++) {
            if (cursor->keys[pos] == x) {
                break;
            }
        }
        for (int i = pos; i < cursor->keys.size()-1; i++) {
            cursor->keys[i] = cursor->keys[i + 1];
        }
        cursor->keys.resize(cursor->keys.size() - 1);

        
        for (pos = 0; pos < cursor->ptr2TreeOrData.ptr2Tree.size(); pos++) {
            if (cursor->ptr2TreeOrData.ptr2Tree[pos] == child) {
                break;
            }
        }

        for (int i = pos; i < cursor->ptr2TreeOrData.ptr2Tree.size() - 1; i++) {
            cursor->ptr2TreeOrData.ptr2Tree[i] = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
        }
        cursor->ptr2TreeOrData.ptr2Tree
            .resize(cursor->ptr2TreeOrData.ptr2Tree.size()-1);

        
        if (cursor->keys.size() >= (getMaxIntChildLimit() + 1) / 2 - 1) {
            cout << "Deleted " << x << " from internal node successfully\n";
            return;
        }

        cout << "UnderFlow in internal Node! What did you do :/" << endl;

        if (cursor == root) {
            return;
        }

        Node** p1 = findParent(root, cursor);
        Node* parent = *p1;

        int leftSibling, rightSibling;

       
        for (pos = 0; pos < parent->ptr2TreeOrData.ptr2Tree.size(); pos++) {
            if (parent->ptr2TreeOrData.ptr2Tree[pos] == cursor) {
                leftSibling = pos - 1;
                rightSibling = pos + 1;
                break;
            }
        }

        
        if (leftSibling >= 0) {
            Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

           
            if (leftNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2 ) {

                
                int maxIdxKey = leftNode->keys.size() - 1;
                cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
                parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

                int maxIdxPtr = leftNode->ptr2TreeOrData.ptr2Tree.size()-1;
                cursor->ptr2TreeOrData.ptr2Tree.insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree[maxIdxPtr]);

               
                leftNode->keys.resize(maxIdxKey);
                leftNode->ptr2TreeOrData.dataPtr.resize(maxIdxPtr);

                return;
            }
        }

        
        if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
            Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

           
            if (rightNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {

                
                int maxIdxKey = rightNode->keys.size() - 1;
                cursor->keys.push_back(parent->keys[pos]);
                parent->keys[pos] = rightNode->keys[0];
                rightNode->keys.erase(rightNode->keys.begin());

                
                cursor->ptr2TreeOrData.ptr2Tree
                    .push_back(rightNode->ptr2TreeOrData.ptr2Tree[0]);
                cursor->ptr2TreeOrData.ptr2Tree
                    .erase(cursor->ptr2TreeOrData.ptr2Tree.begin());
                
                return;
            }
        }

        
        if (leftSibling >= 0) {
            
            Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
            leftNode->keys.push_back(parent->keys[leftSibling]);

            for (int val : cursor->keys) {
                leftNode->keys.push_back(val);
            }

            for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
                leftNode->ptr2TreeOrData.ptr2Tree
                    .push_back(cursor->ptr2TreeOrData.ptr2Tree[i]);
                cursor->ptr2TreeOrData.ptr2Tree[i] = NULL;
            }

            cursor->ptr2TreeOrData.ptr2Tree.resize(0);
            cursor->keys.resize(0);

            removeInternal(parent->keys[leftSibling], parent, cursor);
            cout << "Merged with left sibling"<<endl;
        }
        else if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
           
            Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
            cursor->keys.push_back(parent->keys[rightSibling - 1]);

            for (int val : rightNode->keys) {
                cursor->keys.push_back(val);
            }

            for (int i = 0; i < rightNode->ptr2TreeOrData.ptr2Tree.size(); i++) {
                cursor->ptr2TreeOrData.ptr2Tree
                    .push_back(rightNode->ptr2TreeOrData.ptr2Tree[i]);
                rightNode->ptr2TreeOrData.ptr2Tree[i] = NULL;
            }

            rightNode->ptr2TreeOrData.ptr2Tree.resize(0);
            rightNode->keys.resize(0);

            removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
            cout << "Merged with right sibling\n";
        }
        }
};


fstream f;

void insertionMethod(BPTree** bPTree,int c) {
    int rollNo;
    int sem;
    float CGPA;
    string name,tempstr;
    if(c==1)
    {
        getline(f,tempstr);

        rollNo=stoi(tempstr);
        getline(f,name);
        
        getline(f,tempstr);
        sem = stoi(tempstr);
        getline(f,tempstr);
        CGPA = stof(tempstr);
    }
    else
    {
        cout << "Enter RollNo: ";
        cin >> rollNo;
        cout<< "Enter Students Name : ";
        cin>>name;
        cout<<"Enter Semester :";
        cin>>sem;
        cout<<"Enter CGPA :  ";
        cin>>CGPA;

    }

    

     

    

    

    string fileName = "DATA/";
    fileName += to_string(rollNo) + ".txt";
    FILE* filePtr = fopen(fileName.c_str(), "w");
    string userTuple = name + " " + to_string(sem) + " "+to_string(CGPA) + "\n";
    fprintf(filePtr, userTuple.c_str());
    //fclose(filePtr);

    (*bPTree)->insert(rollNo, filePtr);
    fclose(filePtr);
    //cout << "Insertion of roll No: " << rollNo << " Successful "<<endl<<endl;
}

void searchMethod(BPTree* bPTree,int c) 
{
    int rollNo;
    if(c==1)
    {
         string tempstr;
        getline(f,tempstr);

        rollNo=stoi(tempstr);
    }
    else
    {
        cout << "Enter RollNo to Search? ";
        cin >> rollNo;
    }
   
    

    bPTree->search(rollNo);
}

void printMethod(BPTree* bPTree,int c) {

    int opt;
    if(c==1)
    {
        string tempstr;
        getline(f,tempstr);

        opt=stoi(tempstr);
    }
    else{
        cout << "Press \n\t1.Hierarical-Display \n\t2.Sequential-Display\n";

        cin >> opt;
    }
    

    
    if (opt == 1)
        bPTree->display(bPTree->getRoot());
    else
        bPTree->seqDisplay(bPTree->getRoot());
}

void deleteMethod(BPTree* bPTree,int c) {
    int tmp;
    if(c==1)
    {
        string tempstr;
        getline(f,tempstr);

        tmp=stoi(tempstr);
    }
    else{
        cout << "Showing you the Tree, Choose a key from here: " << endl;
        bPTree->display(bPTree->getRoot());
    
        
        cout << "Enter a key to delete: " << endl;
        cin >> tmp;
    }
    
    bPTree->removeKey(tmp);

    cout<<"Tree After Deleting : "<<tmp<<endl;
    bPTree->display(bPTree->getRoot());
}

int main() {
    
    cout << "\n====================Welcome=====================\n"<< endl;

    bool flag = true;
    int option;
    string tempopt;

    int maxChildInt , maxNodeLeaf,order;
    cout<<"Enter order of B+ Tree : ";
    cin>>order;
    maxChildInt = order;
    maxNodeLeaf = order-1;
    BPTree* bPTree = new BPTree(maxChildInt, maxNodeLeaf);
    f.open("students.txt",ios::in);
    do {
        
        getline(f,tempopt);
        option = stoi(tempopt);

        switch (option) {
            case 1:
                insertionMethod(&bPTree,1);
                break;
            case 2:
                searchMethod(bPTree,1);
                break;
            case 3:
                printMethod(bPTree,1);
                break;
            case 4:
                deleteMethod(bPTree,1);
                break;
            case 5:
                flag=false;
                break;
            default:
                cout<<"Enter Valid Choice "<<endl<<endl;
        }
    }while (flag);
    flag=true;

    do {
        cout << "\nEnter Operation you want to perform : " << endl;
        cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Display\n\tPress 4: Delete \n\tPress 5: Exit" << endl;
        cin >> option;

        switch (option) {
            case 1:
                insertionMethod(&bPTree,2);
                break;
            case 2:
                searchMethod(bPTree,2);
                break;
            case 3:
                printMethod(bPTree,2);
                break;
            case 4:
                deleteMethod(bPTree,2);
                break;
            case 5:
                flag=false;
                break;
            default:
                cout<<"Enter Valid Choice "<<endl<<endl;
                break;
        }
        
    }while (flag);

    return 0;
}
