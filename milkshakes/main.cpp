#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <time.h>
using namespace std;
struct Compare;

typedef vector<pair<int,int>> custVec;
typedef multiset<custVec*, Compare> custsSet; // all customers

void runCase();
void buildVectors();
bool custsSatisfied(custVec*);
void printCusts();
void outputFlavors();

struct Compare {
	bool operator() (const custVec* lhs, const custVec* rhs) const {
		return lhs->size() < rhs->size();
	}
};

int cases = 0, numFlavors = 0, numCusts = 0, custIndex = 0, custNum = 1;
pair<int, int> milkShake, maltShake;

custVec *customer;
custsSet customers2;
custsSet::iterator it;

vector<int> flavors, changedVec;
//string infileName = "smallIn.in", outfileName = "smallOut.out", outfileTest = "testOut.out";
string infileName = "largeIn.in", outfileName = "largeOut.out", outfileTest = "testOut.out", outfileCusts = "custOut.out";


ifstream infile(infileName.c_str());
ofstream outfile(outfileName.c_str());
ofstream outTest(outfileTest.c_str());
ofstream outCusts(outfileCusts.c_str());


int main() {

	clock_t tStart = clock();

	infile >> cases;
	for (int i = 1; i <= cases; i++) {
		outfile << "Case #" << i << ": ";
		outTest << "Case #" << i << endl;
		cout << "Case #" << i << endl;
		runCase();
		printf("%.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	}
	infile.close();
	outfile.close();
	outTest.close();
	outCusts.close();
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	cin.get();
	return 0;
}

void runCase() {
	custNum = 1;
	infile >> numFlavors >> numCusts;
	buildVectors();
	it = customers2.begin();
	if (custsSatisfied(*it)) {
		outputFlavors();
	}
	else outfile << "IMPOSSIBLE" << endl;
	
	it = customers2.begin();
	customers2.clear();
	flavors.clear();
	changedVec.clear();
	//delete customers2; customers2 = NULL;
}

void buildVectors() {
	int types; // how many flavors a customer likes
	bool malt; // whether or not a customer likes a malt
	for (int i = 0; i < numCusts; i++) {
		customer = new custVec;
		malt = false;
		infile >> types;
		for (int j = 0; j < types; j++) {
			infile >> milkShake.first >> milkShake.second;
			if (milkShake.second == 1) { // if malted
				malt = true;
				maltShake = milkShake; // there can be only one malt
			}
			else customer->push_back(milkShake); // insert each unmalted
		}
		if (malt) customer->push_back(maltShake);
		customers2.insert(customer);
		customer = NULL;
	}
	//printCusts();
	for (int i = 0; i < numFlavors; i++) flavors.push_back(-1); // -1 means no malt/unmalted selected yet
	for (int i = 0; i < numCusts; i++) changedVec.push_back(-1);
}


/*
bool custsSatisfied(custVec *cust) {
	int flavor = -1, maltOrUnmalt = -1;

	for (unsigned int i = 0; i < cust->size(); i++) {

		flavor = cust->at(i).first;
		maltOrUnmalt = cust->at(i).second;


		if (flavors[flavor-1] == -1) flavors[flavor-1] = maltOrUnmalt;
		if (flavors[flavor-1] == maltOrUnmalt) {
			it++;
			if (it == customers2.end()) return true;
			if (custsSatisfied(*it)) return true;

		}

		else if (maltOrUnmalt == 1) {
			flavors[flavor-1] = 1;
		}
	}
	it--;
	return false;
}
*/

bool custsSatisfied(custVec *cust) {

	int flavor = -1, maltOrUnmalt = -1;

	for (unsigned int i = 0; i < cust->size(); i++) {

		flavor = cust->at(i).first;
		maltOrUnmalt = cust->at(i).second;
		//outTest << "now on customer: " <<  distance(customers2.begin(),it)+1 << endl;
		//outTest << "flavor " << flavor << "(" << maltOrUnmalt << ")" << endl;

		if (flavors[flavor-1] == maltOrUnmalt) {
			//outTest << flavors[flavor-1] << " == " << maltOrUnmalt << endl;
			//outTest << "flavors[" << flavor-1 << "] == " << flavors[flavor-1] << endl;
			it++;
			if (it == customers2.end()) {
				it--;
				return true;
			}
			if (custsSatisfied(*it)) {
				it--;
				return true;
			}
		}
		else if (flavors[flavor-1] == -1) {
			//outTest << "flavor == -1" << endl;
			if (i == (cust->size()-1)) {
				//outTest << "last customer flavor" << endl;
				flavors[flavor-1] = maltOrUnmalt;
				changedVec[distance(customers2.begin(),it)] = flavor;
				/*
				for (int j=0; j<changedVec.size();j++) {
					if (changedVec[j] != -1) outTest << j+1 << "(" << changedVec[j] << "), ";
					else if (j < 29) outTest << j+1 << "(" << changedVec[j] << "), ";
				}
				outTest << endl;
				*/
			}
			it++;
			if (it == customers2.end()) {
				it--;
				return true;
			}
			if (custsSatisfied(*it)) {
				it--;
				if (flavors[flavor-1] == maltOrUnmalt || flavors[flavor-1] == -1) {
					//it--; // wrong place
					return true;
				}
				
				//outTest << "customers satisfied but now i'm not" << endl;
				//outTest << "customer: " <<  distance(customers2.begin(),it)+1 << endl;
				
				/*
				for (unsigned int j = 0; j < (*it)->size(); j++)
					outTest << "flavor: " << (*it)->at(j).first << " " << (*it)->at(j).second << endl;
					*/
				for (int j = distance(customers2.begin(),it); j < numCusts; j++) {
					int current = changedVec[j];
					if (current != -1) {
						//outTest << "customer " << j+1 << "!= -1 in changedVec" << endl;
						flavors[current-1] = -1;
						//outTest << "flavors[" << current << "] == " << flavors[current] << endl;
						changedVec[j] = -1;
					}
				}
				
				/*
				for (int j=0; j<changedVec.size();j++) {
					if (changedVec[j] != -1) outTest << j+1 << "(" << changedVec[j] << "), ";
					else if (j < 29) outTest << j+1 << "(" << changedVec[j] << "), ";
				}
				outTest << endl;
				*/
				
			}
		}
	}
	it--;
	return false;
}

void printCusts() {
	/*
	cout << "customers vector:" << endl;
	for (unsigned int i = 0; i < customers.size(); i++) {
		cout << "customer: " << i+1 << endl;
		for (unsigned int j = 0; j < customers[i].size(); j++)
			cout << "flavor: " << customers[i][j].first << " " << customers[i][j].second << endl;
	} */

	int setIndex = 1;
	
	outCusts << "customers: " << numCusts << endl;
	for(it = customers2.begin(); it != customers2.end(); it++) {
		outCusts << "customer: " <<  setIndex++ << endl;
		for (unsigned int j = 0; j < (*it)->size(); j++)
			outCusts << "flavor: " << (*it)->at(j).first << " " << (*it)->at(j).second << endl;
	}
	
	
}

void outputFlavors() {
	for (int i = 0; i < numFlavors; i++) {
		if (flavors[i] == -1) flavors[i] = 0;
		outfile << flavors[i] << " ";
	}
	outfile << endl;
}