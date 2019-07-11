/*----------------------------------------------------------------

* @Author: Su

* @Description: 

* @Creath Date:

----------------------------------------------------------------*/

#include <iostream>
#include <vector>
#include <cassert>
#include "./BalancedBinaryTree.h"
#include "../openFile.h"
using namespace std;


void openTXTFile(){
	AVLTree<string, int> map;
	string filePath = "../Set_Map/pride-and-prejudice.txt";
	vector<string>words;
	
	opfile::openfile( filePath, words );
	cout << "all words: " << words.size() << endl;
	
	for( auto iters = words.begin(); iters != words.end(); ++iters ) {
		map.insert( *iters, 1 );
	}

	cout << "after handle words: " << map.size() << endl;
//	cout << "pride sum : " << *map.search( "pride" ) << endl;
	cout << "is BSTree: " << map.isBSTree() << endl; 
	cout << "is Blanced: " << map.isBlanced() << endl;
//	
	for( auto value : words ) {
		map.removeNode( value );
		if( !map.isBSTree() ) {
			throw ( runtime_error( "isBSTree error" ) );
		}
		
		if( !map.isBlanced() ) {
			throw ( runtime_error( "isBlanced error" ) );
		}		
	}
	cout <<"now size: " << map.size() << endl;
	 
}

int main(int argc, char *argv[])
{
	openTXTFile();
	return 0;
}