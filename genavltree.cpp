#include<iostream>
#include<cstdio>
#include<sstream>
#include<algorithm>
#include <stack>
#include <array>
#include <vector>

#define pow2(n) (1 << (n))
using namespace std;
 
/*
 * Node Declaration
 */
template<typename T>
class avl_node
{
    public:
    T data;
    struct avl_node<T> *left;
    struct avl_node<T> *right;
};
 
/*
 * Class Declaration
 */


struct lesser
{
	bool operator()(int x, int y) { return x < y; }
    bool operator()(double x, double y) { return x < y; }
	bool operator()(string x, string y) { return x.length() < y.length(); }
}myless;

template<typename T,typename oper>
class avlTree
{

    public:
            oper op;
            T arr[5] = { }; 
            vector<avl_node<T>*> inord;
            avl_node<T> *root;
        int height(avl_node<T> *);
        int diff(avl_node<T> *);
        avl_node<T> *rr_rotation(avl_node<T> *);
        avl_node<T> *ll_rotation(avl_node<T> *);
        avl_node<T> *lr_rotation(avl_node<T> *);
        avl_node<T> *rl_rotation(avl_node<T> *);
        avl_node<T>* balance(avl_node<T> *);
        avl_node<T>* findLeftMost(avl_node<T> *);
        avl_node<T>* findRightMost(avl_node<T> *);
        avl_node<T>* inOrderSuccessor(avl_node<T> *,oper op);
        avl_node<T>* inOrderPredecessor(avl_node<T> *,oper op);
        avl_node<T>* insert(avl_node<T> *, T value ,oper op);

        //void display(ptr_t first, int);
        void inorder(avl_node<T> *);
        void preorder(avl_node<T> *);
        void postorder(avl_node<T> *);
        avlTree<T,oper>()
        {
            root = NULL;
            op=myless;//change here
        }

    
    class Iterator
	        {
		        public:
                avl_node<T>* p_it_;
                oper op_;
                avlTree &obj_;
		        explicit Iterator(avl_node<T> *p_it,oper op,avlTree &obj) : p_it_(p_it),op_(op),obj_(obj)
		        {	

		        }
		        
                T operator*() const//change here
		        {   
                                       // cout<<"inside * ,"<<p_it_->data<<"\t";
			        return p_it_->data;
		        }
		        Iterator& operator++() // pre
		        {
			        p_it_=obj_.inOrderSuccessor(p_it_,op_);
                    //cout<<"\t in ++ \t"<<*p_it_;
			        return *this;
		        
                }

                 Iterator& operator--() // pre
		        {
                    //cout<<"inside --\n";
			        p_it_=obj_.inOrderPredecessor(p_it_,op_);
                    //cout<<"\t in ++ \t"<<*p_it_;

			        return *this;
		        
                }
                

            

                Iterator  operator--(int){ // post

			        Iterator temp(*this);
                    --(*this);
                    return temp; 
                }
            
                Iterator  operator++(int) // post
		        {
			        Iterator temp(*this);
			        ++*this;
			        return temp;
		        }
                    bool operator==(const Iterator& rhs) const
		        {
			        return p_it_ == rhs.p_it_;
		        }
                bool operator!=(const Iterator& rhs) const
		        {
		        //	return p_it_ != rhs.p_it_;
			        return !(*this	== rhs);
		        }

	        };
	        Iterator begin(avlTree<T,oper> &obj)
	        {
                                   
                                        inorder(root);                  
		        return Iterator(inord.front(),op,obj);
	        }
	        Iterator end(avlTree<T,oper> &obj)
	        {
                inorder(root);     
                //inord.push_back({0});
                //cout<<"back .."<<inord.back()->data;   
		        return Iterator(inord.back(),op,obj);//change here when changing size
	        }

};

 
/*
 * Main Contains Menu
 */
 
/*
 * Height of AVL Tree
 */
template<typename T,typename oper>
int avlTree<T,oper>::height(avl_node<T> *temp)
{
    int h = 0;
    if (temp != NULL)
    {
        int l_height = height (temp->left);
        int r_height = height (temp->right);
        int max_height = max (l_height, r_height);
        h = max_height + 1;
    }
    return h;
}
 
/*
 * Height Difference 
 */
template<typename T,typename oper>
int avlTree<T,oper>::diff(avl_node<T> *temp)
{
    int l_height = height (temp->left);
    int r_height = height (temp->right);
    int b_factor= l_height - r_height;
    return b_factor;
}
 
/*
 * Right- Right Rotation
 */
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::rr_rotation(avl_node<T> *parent)
{
    avl_node<T> *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}
/*
 * Left- Left Rotation
 */
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::ll_rotation(avl_node<T> *parent)
{
    avl_node<T> *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}
 
/*
 * Left - Right Rotation
 */
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::lr_rotation(avl_node<T> *parent)
{
    avl_node<T> *temp;
    temp = parent->left;
    parent->left = rr_rotation (temp);
    return ll_rotation (parent);
}
 
/*
 * Right- Left Rotation
 */
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::rl_rotation(avl_node<T> *parent)
{
    avl_node<T> *temp;
    temp = parent->right;
    parent->right = ll_rotation (temp);
    return rr_rotation (parent);
}
 
/*
 * Balancing AVL Tree
 */
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::balance(avl_node<T> *temp)
{
    int bal_factor = diff (temp);
    if (bal_factor > 1)
    {
        if (diff (temp->left) > 0)
            temp = ll_rotation (temp);
        else
            temp = lr_rotation (temp);
    }
    else if (bal_factor < -1)
    {
        if (diff (temp->right) > 0)
            temp = rl_rotation (temp);
        else
            temp = rr_rotation (temp);
    }
    return temp;
}
 
/*
 * Insert Element into the tree
 */
template<typename T, typename oper>
avl_node<T> *avlTree<T,oper>::insert(avl_node<T> *root, T value, oper op)
{
    if (root == NULL)
    {
        root = new avl_node<T>;
        root->data = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    if (op(value , root->data))
    {
        root->left = insert(root->left, value,op);
        root = balance (root);
    }
    else 
    {
        root->right = insert(root->right, value,op);
        root = balance (root);
    }
    return root;
}
 
/*
 * Display AVL Tree
 */
/*
template<typename T,typename oper>
void avlTree<T,oper>::display(avl_node<T> *ptr, int level)
{
    int i;
    if (ptr!=NULL)
    {
        display(ptr->right, level + 1);
        printf("\n");
        if (ptr == root)
        cout<<"Root -> ";
        for (i = 0; i < level && ptr != root; i++)
            cout<<"        ";
        cout<<ptr->data;
        display(ptr->left, level + 1);
    }
}*/

 

template<typename T,typename oper>
void avlTree<T,oper>::inorder(avl_node<T> *tree)
{
    avl_node<T> *temp;
    if (tree == NULL)
        return;
    inorder (tree->left);
        //cout<<tree->data<<"  \t";
    inord.push_back(tree);
    inorder (tree->right);
}
/*
 * Preorder Traversal of AVL Tree
 */
template<typename T,typename oper>
void avlTree<T,oper>::preorder(avl_node<T> *tree)
{
    if (tree == NULL)
        return;
    cout<<tree->data<<"  ";
    preorder (tree->left);
    preorder (tree->right);
 
}
 
/*
 * Postorder Traversal of AVL Tree
 */
template<typename T,typename oper>
void avlTree<T,oper>::postorder(avl_node<T> *tree)
{
    if (tree == NULL)
        return;
    postorder ( tree ->left );
    postorder ( tree ->right );
    cout<<tree->data<<"  ";
}

template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::inOrderPredecessor(avl_node<T> *n,oper op){
    avl_node<T> *temp = root;
    if( n->left != NULL )
        return findRightMost(n->left);
 
    avl_node<T> *pre = NULL;
 
    while (temp != NULL){
        if (op(n->data,temp->data)){
            temp = temp->left;
        }
        else if(n->data!=temp->data && !op(n->data,temp->data)){
             pre = temp;    
             temp = temp->right;
        }
        else
           break;
    }
    return pre;
}


template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::inOrderSuccessor(avl_node<T> *n,oper op){
    avl_node<T> *temp = root;
    if( n->right != NULL )
        return findLeftMost(n->right);
 
    avl_node<T> *succ = NULL;
 
    while (temp != NULL){
        if (op(n->data,temp->data)){
            succ = temp;
            temp = temp->left;
        }
        else if (n->data!=temp->data && !op(n->data,temp->data))
            temp = temp->right;
        else
           break;
    }
    return succ;
}



// Find the left most node
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::findLeftMost(avl_node<T> *node){
    avl_node<T> *current = node;
     while (current->left != NULL)
        current = current->left;
    return current;
}

//Find Right Most Node
template<typename T,typename oper>
avl_node<T> *avlTree<T,oper>::findRightMost(avl_node<T> *node){
    avl_node<T> *current = node;
     while (current->right != NULL)
        current = current->right;
    return current;
}
template<typename ptr_t>
void display(ptr_t first,ptr_t last)
{       cout<<*last;
        while(last!=first){

        cout<<"..\t"<<(*(--last));
        }
      

        
    }
    



template<typename ptr_t,typename T, typename oper>
ptr_t search(ptr_t first,ptr_t last,T val,oper op)
{

       while(first!=last){
            if(op(*(first++),val)){
                return first;
            }
            //    cout<< " ...\t"<<(*(last));
        cout<<(*(first++));
        cout<<"\t";
    }
    
}




 


struct equal1
{
    bool operator()(int x, int y) { return x == y; }
        bool operator()(string x, string y) { return x.compare(y); }
}myequal; 


int main()
{
    string s;//to be changed when searching    
    int choice;
    string item;   // to be changed to make it generic
    avlTree<string,lesser> avl; // to be changed to make it generic

    while (1)
    {
                    
        cout<<endl;
        cout<<"1.Insert Element into the tree"<<endl;
        cout<<"2.Display Balanced AVL Tree"<<endl;
        cout<<"3.InOrder traversal"<<endl;
        cout<<"4.PreOrder traversal"<<endl;
        cout<<"5.PostOrder traversal"<<endl;
        cout<<"6.Exit"<<endl;
        cout<<"Enter your Choice: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            cout<<"Enter value to be inserted: ";
            cin>>item;
            avl.root = avl.insert(avl.root, item,myless);
            break;
        case 2:
            if (avl.root == NULL)
            {
                cout<<"Tree is Empty"<<endl;
                continue;
            }
            cout<<"Balanced AVL Tree:"<<endl;
            display(avl.begin(avl),avl.end(avl));
            break;
        case 3:
            cout<<"Inorder Traversal:"<<endl;
            avl.inorder(avl.root);
            cout<<endl;
            break;
        case 4:
            cout<<"Preorder Traversal:"<<endl;
            avl.preorder(avl.root);
            cout<<endl;
            break;
        case 5:
            cout<<"Postorder Traversal:"<<endl;
            avl.postorder(avl.root);    
            cout<<endl;
            break;
        case 6:
            {
            cout<<"Search :"<<endl;
            cout<<"Enter the value to be searched\n";
            cin>>s;
            auto it = search(avl.begin(avl),avl.end(avl),s,myequal);
            cout<<"Search result : "<<*it;    
            cout<<endl;
            break;
            }        
        case 7:
            exit(1);    
            break;
        default:
            cout<<"Wrong Choice"<<endl;
        }
    }
    return 0;
}

