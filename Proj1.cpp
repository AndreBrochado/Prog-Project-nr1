#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <locale>

using namespace std;

struct boat{
	int number;
	string name;
	unsigned int size;
	char symbol;
	string color;
	unsigned char xPos, yPos, orientation;
};

// Set text color & background
void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == 0)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
				BACKGROUND_RED);
}

// Set text color
void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//returns the integral value for a string with the name of the color
int colorValue(string color){
	if (color == "Preto") return 0;
	if (color == "Azul") return 1;
	if (color == "Verde") return 2;
	if (color == "Ciano") return 3;
	if (color == "Vermelho") return 4;
	if (color == "Magenta") return 5;
	if (color == "Castanho") return 6;
	if (color == "Cinzento_claro") return 7;
	if (color == "Cinzento_escuro") return 8;
	if (color == "Azul_claro") return 9;
	if (color == "Verde_claro") return 10;
	if (color == "Ciano_claro") return 11;
	if (color == "Vermelho_claro") return 12;
	if (color == "Magenta_claro") return 13;
	if (color == "Amarelo") return 14;
	if (color == "Branco") return 15;
	return -1;
}

//checks if the position given for the given ship is valid (doesn't cross other boats or goes out of the board)
bool checkValidPosition(const vector< vector<char> >& table, boat ship){
	bool valPos=true;
	if(ship.xPos<'a' || ship.xPos>'a'+table[0].size() || ship.yPos<'A' || ship.yPos>'A'+table.size())
		valPos=false;
	else if(! (ship.orientation == 'V' || ship.orientation == 'H'))
		valPos = false;
	else if(ship.orientation == 'V'){
		if(ship.yPos + ship.size > 'A' + table.size())
			valPos=false;
		else{
			for(size_t i=0; i<ship.size; i++){
				if(table[ship.xPos-'a'+i][ship.yPos-'A']!='.')
					valPos = false;
			}
		}
	}
	else if(ship.orientation == 'H'){
		if(ship.yPos + ship.size > 'A' + table.size())
			valPos=false;
		else{
			for(size_t i=0; i<ship.size; i++){
				if(table[ship.xPos-'a'][ship.yPos-'A'+i]!='.')
					valPos = false;
			}
		}
	};
	return valPos;
}

//Returns the color of the boat with given symbol
int boatColor(const vector<boat> &boatList, char symbol){
	for(size_t i=0; i<boatList.size(); i++){
		if(boatList[i].symbol == symbol){
			return colorValue(boatList[i].color);
		}
	}
	return -1;
}

void printTable(const vector< vector<char> >& table, const vector<boat> &boatList){
	cout << " ";
	setcolor(15);
	for(size_t k=0; k<table.size();k++){
		cout << setw(2) << char ('a'+k);
	}
	cout<< endl;

	for(size_t i=0; i<table.size(); i++){
		for(size_t j=0; j<table[0].size(); j++){
			if(j==0){
				setcolor(0,15);
				cout << char ('A'+i);}
			else if(table[i][j]!='.')
				setcolor(9,7);
			else
				setcolor(boatColor(boatList, table[i][j]),7);
			cout<< setw(2) << table[i][j];
		}
		cout<<endl;
	}
}

void addBoat(vector< vector<char> >& table, boat ship){
	if(ship.orientation == 'V')
		for(int i = ship.yPos; i<= ship.yPos + ship.size; i++)
			table[i][ship.xPos]=ship.symbol;

}

// void addBoat(vector< vector<char> > &table, boat)
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
	printTable(table, boatList);
	return 0;
}
