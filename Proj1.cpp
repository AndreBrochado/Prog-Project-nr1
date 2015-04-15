#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

using namespace std;

struct boat{
	int number;
	string name;
	int size;
	char symbol;
	string color;
	unsigned char xPos, yPos, orientation;
};

// Set text color & background
void setcolor(unsigned int color, unsigned int background_color)
{
HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
if (background_color == BLACK)
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
	cout << " ";
	setcolor(15);
	for(size_t k=0; k<table.size();k++){
		cout << setw(2) << char ('a'+k);
	}
	cout<< endl;

	for(size_t i=0; i<table.size(); i++){
		for(size_t j=0; j<table[0].size(); j++){
			if(j==0){
				setcolor(15,0);
				cout << char ('A'+i);}
			setcolor(9,7);
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
