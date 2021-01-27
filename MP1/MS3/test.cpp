#include<iostream>

using namespace std;

int fib(int idx){
	int fib[idx+1];
	int i; 
	
	fib[0]= 1;
	fib[1] = 2;
	
	for(i = 2; i<=idx; ++i){
		fib[i] = fib[i-1]+fib[i-2];
	}
	return fib[idx];
}

int main(){

	
	int x = fib(6);
	
	cout << "The fib is " << x << endl;
}