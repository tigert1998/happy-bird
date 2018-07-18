#include <iostream>
using namespace std;

#include "random.h"

int main(void){
	for(int i = 0; i < 20; i++){
		cout << "> " << Random::QueryFloatRandom(-5,5) << endl;
	}
}