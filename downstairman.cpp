#include "GameBoard.h"
#include <iostream>
using namespace std;


int main(int argcs, char* argv[])
{
	cout<<"start main"<<endl;
	CGameBoard gb;
	if( !gb.init() )
		return 0;
	cout<<"init ok"<<endl;
	gb.initGameBoard();
	gb.start();
	return 0;
}
