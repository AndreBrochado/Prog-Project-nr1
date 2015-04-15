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
	unsigned char xPos, yPos, orientation;
};

bool checkValidPosition(const vector< vector<char> >& table, boat ship){
	bool valPos=true;
	if(ship.xPos<'a' || ship.xPos>'a'+table[0].size() || ship.yPos<'A' || ship.yPos>'A'+table.size())
		valPos=false;
	if(! (ship.orientation == 'V' || ship.orientation == 'H'))
		valPos = false;
	else if(ship.orientation == 'V')
		for(int i=0; i<ship.size; i++){
			if(table[ship.xPos-'a'+i][ship.yPos-'A']!='.')
				valPos = false;
		}
	else if(ship.orientation == 'H')
		for(int i=0; i<ship.size; i++){
					if(table[ship.xPos-'a'][ship.yPos-'A'+i]!='.')
						valPos = false;
				};
	return valPos;
}

void printTable(const vector< vector<char> >& table){
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
		if(fileName.find('.') == string::npos) //This will add
			fileName+=".txt";     //the .txt extension if the user didn't include one.

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
