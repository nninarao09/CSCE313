#include<iostream>

using namespace std;

int roundup(int len, int round){
	int blocksize = 4;
	
	if(len%round != 0){
		while(len > round){
			round = round + blocksize;
			cout << "round: " << round << endl;
		}
		len = round;
		cout << "len" << len << endl;
	}
	return len;
}

int main(){
	int len = 6;
	int round = 4;
	
	int x = roundup(len,round);
	
	cout << x << endl;
}