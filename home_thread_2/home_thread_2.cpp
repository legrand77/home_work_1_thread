#define _USE_MATH_DEFINES
#include<iostream>
#include<thread>
#include <Windows.h>
#include<vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <execution>
#include <cmath>


using namespace std;


void Summa_one(vector<int>& v, vector<int>& v1, vector<int>& v2, int x, int y) {
	for (int i = x; i < y; i++) {
		v[i] = v1[i] + v2[i];
	}
}
auto Summa(vector<int> &v, vector<int> &v1, vector<int>& v2, int x, int y) {
	auto start = chrono::high_resolution_clock::now();
	Summa_one(v, v1, v2, x, y);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> time = end - start;
	return time.count();
}

auto Summa_start(vector<int>& v, vector<int>& v1, vector<int>& v2, int x, int y) {
	auto start = chrono::high_resolution_clock::now();
	Summa_one(v, v1, v2, x, y);
	return start;
}
auto Summa_end(vector<int>& v, vector<int>& v1, vector<int>& v2, int x, int y) {
	Summa_one(v, v1, v2, x, y);
	auto end = chrono::high_resolution_clock::now();
	return end;
}

void generate_1(vector<int>& v) {
	mt19937 gen;
	uniform_int_distribution<int> dis(0, 100);
	auto rand_num([=]() mutable { return dis(gen); });
	generate(execution::par, begin(v), end(v), rand_num);
};


int main(){
	setlocale(LC_ALL, "Russian");
	system("chcp 1251");
	int dec{ 5 };
	vector<int> d;
	for (int j = 0; j < dec; j++) {
		d.push_back(1000 * pow(10.0, static_cast<double>(j)));
	};
	vector<vector<int>> t;
	for (int i = 0; i < dec; i++) {
		for (int j = 0; j < 3; j++) {
			t.push_back(vector<int>(d[i]));
		};
	};
	cout << "Количество аппаратных ядер - " << thread::hardware_concurrency() << endl;
	for (int i = 1; i < 12; i++) {
		if (i != 3 && i != 6 && i != 9)
			generate_1(t[i]);
	};

	chrono::high_resolution_clock::time_point R1{};
	chrono::high_resolution_clock::time_point R2{};
	vector<thread> T;
	
	for (int i = 0; i < dec; i++) {
		cout << "            " << static_cast <long>(d[i]) << "   ";
	}
	cout << endl;

	cout << "1 потоков  ";
	for (int i = 0; i < (dec * 2 + 3); i+=3) {
		cout <<  Summa(t[i], t[i + 1], t[i + 2], 0, d[i / 3]) << " ms           ";
	}
	cout  << endl;

	this_thread::sleep_for(500ms);

	cout << "2 потоков  ";
	for (int i = 0; i < (dec*2+3); i += 3) {
		this_thread::sleep_for(300ms); 
		T.push_back(thread([&]() {R1=(Summa_start(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), 0, ref(d[i / 3]) * 0.5));}));
		T.push_back(thread([&]() {R2=(Summa_end(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), ref(d[i / 3]) * 0.5, ref(d[i / 3]))); }));
		this_thread::sleep_for(300ms);
		chrono::duration<double, milli> time = R2-R1;
		cout << time.count() << " ms           ";
		
	};
	cout << endl;
	this_thread::sleep_for(500ms);

   cout << "4 потоков  ";
	for (int i = 0; i < (dec * 2 + 3); i += 3) {
		this_thread::sleep_for(300ms);
		T.push_back(thread ([&]() {R1 = Summa_start(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), 0, ref(d[i / 3]) * 0.25); }));
		T.push_back(thread (Summa_one,ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), ref(d[i / 3])*0.25, ref(d[i / 3])*0.5));
		T.push_back(thread (Summa_one,ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), ref(d[i / 3])*0.5, ref(d[i / 3])*0.75));
		T.push_back(thread ([&]() {R2 = Summa_end(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), ref(d[i / 3])*0.75, ref(d[i / 3])); }));
		this_thread::sleep_for(300ms);
		chrono::duration<double, milli> time = R2-R1;
		cout << time.count() << " ms           ";
		
	}
	cout << endl;

	this_thread::sleep_for(500ms);

	 cout << "8 потоков  ";
	for (int i = 0; i < (dec * 2 + 3); i += 3) {
		this_thread::sleep_for(300ms);
		T.push_back(thread ([&]() {R1 = (Summa_start(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), 0, ref(d[i / 3]) * 0.125)); }));
		
		for (int j = 1; j < 7; j++) {
			T.push_back(thread(Summa_one,ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), (ref(d[i / 3]) * 0.125 * j), (ref(d[i / 3]) * 0.125 * (j + 1))));
		};
		T.push_back(thread ([&]() {R2=Summa_end(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), ref(d[i / 3]) * 0.125 * 7, ref(d[i / 3])); }));
		this_thread::sleep_for(300ms);
		chrono::duration<double, milli> time = R2-R1;
		cout << time.count() << " ms           ";
		
	}
	
	cout << endl; 

	this_thread::sleep_for(500ms);
	
	cout << "16 потоков ";

	for (int i = 0; i < (dec * 2 + 3); i += 3) {
		this_thread::sleep_for(300ms);
		T.push_back(thread([&]() {R1=(Summa_start(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), 0, ref(d[i / 3]) * 0.062)); }));
		
		for (int j = 1; j < 15; j++) {
			T.push_back(thread([&]() {Summa_one(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), (ref(d[i / 3]) * 0.062 * j), (ref(d[i / 3]) * 0.062 * (j + 1))); }));
		};
		T.push_back(thread([&]() {R2=(Summa_end(ref(t[i]), ref(t[i + 1]), ref(t[i + 2]), ref(d[i / 3]) * 0.062 * 15, ref(d[i / 3]))); }));
		this_thread::sleep_for(300ms);
		chrono::duration<double, milli> time = R2-R1;
		cout << time.count() << " ms           ";
		
	};

	this_thread::sleep_for(500ms);

	for (auto& thr : T)
		{
			thr.join();
		};
	
	return EXIT_SUCCESS;
}