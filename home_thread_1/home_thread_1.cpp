#define _USE_MATH_DEFINES
#include <Windows.h>
#include "Timer.h"
#include<iostream>
#include<thread>



using namespace std;

void func1(int& count, int&  max_client){
	while (count!=max_client){
		this_thread::sleep_for(1s);
		consol_parameter::SetColor(15, 0);
		cout << "client " << ++count << " ";
		cout << endl;
	}
}

void func2(int& count, int& max_client){
	 int val{};
	 while (!(count == 0 && val == 1)){
		this_thread::sleep_for(2s);
		consol_parameter::SetColor(10, 0);
		if (count == max_client) val = 1;
		cout << "operation " << --count << " ";
		cout << endl;
	 }
}


int main(){
	
	int count{};
	int max_client{5};
	thread t1(func1, ref(count), ref(max_client));
	thread t2(func2, ref(count), ref(max_client));
	t1.join();
	t2.join();
	
	return EXIT_SUCCESS;
}