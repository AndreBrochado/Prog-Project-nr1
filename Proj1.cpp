#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>


using namespace std;

struct boat{
	int number;
	string name;
	int size;
	char symbol;
	string color;
	char xpos, ypos, orientation;
};

void printTable(vector< vector<char> > table){
	for(size_t i=0; i<table.size(); i++){
		for(size_t j=0; j<table[0].size(); j++){
			cout<< setw(2) << table[i][j];
		}
		cout<<endl;
	}
}

int main(){
	string fileName, dimensions, trash; //trash will be used to get full words like the board name
	char aux; //gets out chars like 'x' and '-'
	ifstream configFile;
	int tableWidth, tableHeight;
	vector<boat> boatList;
	boat tempBoat;

	do{
		cout<< "Insira o nome do ficheiro de configuracao:  " << endl;
		cin>> fileName;
		if(fileName.length()<=4 || fileName.substr(fileName.length()-4, 4)!= ".txt") //This will add
			fileName+=".txt";     //the .txt extension if the user didn't.

		configFile.open(fileName.c_str(), ios::in);

		if(!configFile.is_open())
			cerr << "Ficheiro nao existe ou nao pode ser aberto!" << endl
			<< "Introduza novamente o nome do ficheiro" << endl;
	}while(!configFile.is_open());

	configFile >> trash >> tableHeight >> aux >> tableWidth; //gets "boardName: " and "x" to aux, gets number of lines(height) and columns(width)
	vector< vector<char> > table(tableHeight, vector<char>(tableWidth, '.'));

	do{
		configFile>>tempBoat.number>>aux>>tempBoat.name>>aux
		>>tempBoat.size>>aux>>tempBoat.symbol>>aux>>tempBoat.color;
		boatList.push_back(tempBoat);

	}while(!configFile.eof());
	printTable(table);
	return 0;
}
