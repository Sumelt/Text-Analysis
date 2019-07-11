#ifndef __BALANCEBINARYTREE__H_ 
#define __BALANCEBINARYTREE__H_

#include <forward_list> 
using namespace std;

template <typename Key, typename Value>
class AVLTree{
private :
	struct Node{
		Key key;
		Value value;
		Node *left;
		Node *right;
		int height; //高度			
		Node( Key k, Value v ) : 
			left( nullptr ), right( nullptr ), value( v ), key( k ), height( 1 )
			{}
		Node( Node *node ) : key( node->key ), value( node->value ), 
			left( node->left ), right( node->right ), height( 1 )
			{}
};
	Node *root;
	int count; //节点个数 	
	bool contain( Node *root, Key key );
	bool isBlanced( Node *root );	
	void destory( Node *root );
	void inOrder( forward_list<Key>* list, Node* root );
	int getBalanceFactor( Node*root );
	Value *search( Node *root, Key key );
	Node* insert( Node *root, Key key, Value value );
	Node* rightRotate( Node *root );
	Node* leftRotate( Node *root );
	Node* removeNode( Node *node, Key key );
	Node* minNode( Node *node );
	
		
public :
	AVLTree();
	~AVLTree();
	bool isEmpty();
	bool isBSTree();
	bool isBlanced();
	bool contain( Key key );	
	int size();
	int getHeight( Node* root );
	Value *search( Key key );
	void insert( Key key, Value value );
	void removeNode( Key key );

};

template <typename Key, typename Value>
AVLTree<Key, Value>::AVLTree() {
	root = nullptr;	
	count = 0;
}

template <typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree() {
	destory( root );
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::destory( Node *root ) {
	if( root != nullptr ) {
		destory( root->left );
		destory( root->right );
		delete root;
		--count;
	}		
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::isEmpty() {
	return count == 0;
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::size() {
	return count;
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::contain( Key key ) {
	return contain( root, key );
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::contain( Node *root, Key key ) {
	if( root == nullptr )
		return false;
	if( root->key == key )
		return true;
	else if( key > root->key )
		return contain( root->right, key );
	else return contain( root->left, key );	
}

template <typename Key, typename Value>
Value* AVLTree<Key, Value>::search( Key key ) {
	return search( root, key );
}

template <typename Key, typename Value>
Value* AVLTree<Key, Value>::search( Node *root, Key key ) {
	if( root == nullptr )
		return nullptr;
	if( root->key == key )
		return &( root->value );
	else if( key > root->key )
		return search( root->right, key );
	else 
		return search( root->left, key );
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::insert( Key key, Value value ) {
	root = insert( root, key, value );//递归插入 
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* 
AVLTree<Key, Value>::insert( Node *root, Key key, Value value ) {
	if( root == nullptr ) {
		count += 1;
		return new Node( key, value );
	}
	if( root->key == key )//已经插入，则更新 
		root->value = value;
	else if( key > root->key )
		root->right = insert( root->right, key, value );
	else 
		root->left = insert( root->left, key, value );
	
	//更新根节点的高度值 
	root->height = 1 + max( getHeight( root->left ), getHeight( root->right ) );
	
	int balanceFactor = getBalanceFactor( root );//求得平衡因子
	
	//右旋：以第一个不平衡节点的左节点为旋转中心
	//第一个不平衡的节点的原因发生在左侧的左侧
	//必然出现第一个不平衡的节点的平衡因子大于1并且第一个不平衡节点的左子树的平衡因子必然大于0
	if( balanceFactor > 1 && getBalanceFactor( root->left ) >= 0 )
	    return rightRotate( root );
	
	//左旋：以第一个不平衡节点的右节点为旋转中心
	//第一个不平衡节点的原因发生在右侧的右侧
	//必然出现第一个不平衡的节点的平衡因子大于1并且第一个不平衡节点的右子树的平衡因子必然小于0
	if( balanceFactor < -1 && getBalanceFactor( root->right ) <= 0 )
	    return leftRotate( root );		
	
	//LR
	//原因发生在第一个不平衡节点的左节点的右子树
	if( balanceFactor > 1 && getBalanceFactor( root->left ) < 0 ) {
	    root->left = leftRotate( root->left );//对第一个不平衡节点的左节点进行左旋调整成LL
	    return rightRotate( root );//再对第一个不平衡节点(即是根节点进行右旋)
	}					
	
	//RL
	//原因发生在第一个不平衡节点的右节点的左子树
	if( balanceFactor < -1 && getBalanceFactor( root->right ) > 0 ) {
	    root->right = rightRotate( root->right );//对第一个不平衡节点的右节点进行右旋
	    return leftRotate( root );	//再对第一个不平衡节点(即是根节点进行左旋)
	}
	
	return root;
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::getBalanceFactor( Node* root )  {
	if( root == nullptr ) 
		return 0;
	else 
		return getHeight( root->left ) - getHeight( root->right );
} 

template <typename Key, typename Value>
int AVLTree<Key, Value>::getHeight( Node* root ) {
	if( root == nullptr )
		return 0;
	else return root->height;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::inOrder( forward_list<Key>* list, Node* root ) {
	if( root == nullptr )
		return;
	inOrder( list, root->left );
	list->push_front( root->key );
	inOrder( list, root->right );
} 

template <typename Key, typename Value>
bool AVLTree<Key, Value>::isBSTree() {
	forward_list<Key>list;
	
	inOrder( &list, root ); //中序变量检验 
	if( list.empty() ) return true;
	
	auto a = list.begin();
	auto b = a;
	for( ++b; b != list.end(); ++b ) {
		if( *a < *b ) return false;
		a = b;
	}
	return true;
} 

template <typename Key, typename Value>
bool AVLTree<Key, Value>::isBlanced() {
	return isBlanced( root );
} 

template <typename Key, typename Value>
bool AVLTree<Key, Value>::isBlanced( Node *root ) {
	if( root == nullptr ) 
		return true;
	
	int balanceFactor = getBalanceFactor( root );
	if( balanceFactor > 1 ) {
		cout << "balanceFactor: " << balanceFactor << endl;
		return false;
	}
		
	return isBlanced( root->left ) && isBlanced( root->right );
} 

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* 
AVLTree<Key, Value>::rightRotate( Node* root ) {
	
	Node* leftNode = root->left;
	Node* node = leftNode->right;
	
	leftNode->right = root;//成为新的根节点 
	root->left = node;
	
	//先更新原先的root高度
	root->height = 1 + 
		max( getHeight( root->left ), getHeight( root->right ) ); 
	//后更新新的rootleft
	leftNode->height = 1 + 
		max( getHeight( leftNode->left ), getHeight( leftNode->right ) );

	return leftNode;//返回新的根节点 
} 

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* 
AVLTree<Key, Value>::leftRotate( Node* root ) {
	Node* rightNode = root->right;
	Node* node = rightNode->left;
	
	rightNode->left = root;
	root->right = node;
	
	root->height = 1 + 
		max( getHeight( root->left ), getHeight( root->right ) );
	
	rightNode->height = 1 + 
		max( getHeight( rightNode->left ), getHeight( rightNode->right ) );
	
	return rightNode;	
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* 
AVLTree<Key, Value>::minNode( Node *node ) {
	if( node == nullptr )
		return nullptr;
	if( node->left == nullptr )//找到最小的节点 
		return node;
	return minNode( node->left );
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::removeNode( Key key ) {
	if( contain( key ) ) //存在才删除 
		root = removeNode( root, key );//删除从根节点开始的找到的key
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* 
AVLTree<Key, Value>::removeNode( Node *root, Key key ) {
	if( root == nullptr )
		return nullptr;
	 
	// 三种情况的处理
	Node *retNode = nullptr;//上层返回的节点，需要进行平衡维护
	
	if( key > root->key ) {
		root->right = removeNode( root->right, key );
		retNode = root; //该节点可能不平衡，要进行平衡维护 
	}
	else if( key < root->key ) {
		root->left = removeNode( root->left, key );
		retNode = root; 		
	}
	//找到目标节点 
	else {
	//要删除的节点只有左子树 
		if( root->right == nullptr ) {
			Node *node = root->left;
			delete root;
			--count;
			retNode = node;
		} 
	//要删除的节点只有右子树
		else if( root->left == nullptr ) {
			Node *node = root->right;
			delete root;
			--count;
			retNode = node;		
		}
	//要删除的节点左右子树都存在
		else if( root->left && root->right ) {
			Node *successor = new Node( minNode( root->right ) );
			Node *curNode = root;
			successor->left = root->left;
			successor->right = removeNode( root->right, successor->key );//这里删除最小节点同时维护平衡 
			delete curNode;
			retNode = successor;
			
		}
	}
	
	if( retNode == nullptr )
		return nullptr;
	
	retNode->height = 1 + max( getHeight( retNode->left ), getHeight( retNode->right ) );
	//自平衡机制 
	 int balanceFactor = getBalanceFactor( retNode );
	 
 	//LL
 	if( balanceFactor > 1 && getBalanceFactor( retNode->left ) >= 0 )
 		return rightRotate( retNode );
 		
	//RR
	if( balanceFactor < -1 && getBalanceFactor( retNode->right ) <= 0 )
		return leftRotate( retNode );
		
	//LR
	if( balanceFactor > 1 && getBalanceFactor( retNode->left ) < 0 ) {
		retNode->left = leftRotate( retNode->left );
		return rightRotate( retNode );
	}
	
	//RL
	if( balanceFactor < -1 && getBalanceFactor( retNode->right ) > 0 ) {
		retNode->right = rightRotate( retNode->right );
		return leftRotate( retNode );
	}

	return retNode;
}

#endif