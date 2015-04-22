#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <limits>

using namespace std;

struct boat {
	unsigned int number;
	string name;
	unsigned int size, line, column;
	char symbol;
	string color;
	unsigned char orientation;
};

// Set text color & background
void setcolor(unsigned int color, unsigned int background_color) {
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == 0)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
				BACKGROUND_RED);
}

// Set text color
void setcolor(unsigned int color) {
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

/**
 *returns the integer value for a string with the name of the color -- CHANGE TO VECTOR, ADD DEFINES
 *returns 15 (white) in case of an unrecognized color
 *future implementation would return -1 to find bugs
 */
int colorValue(string color) {
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
	return 15;
}

/**
 * Goes through the list of boats
 * and when it finds the boat with the given symbol
 * returns its color.
 * returns -1 in case of error (should never happen since symbol input comes from the table matrix)
 */
int boatColor(const vector<boat> &boatList, char symbol) {
	for (size_t i = 0; i < boatList.size(); i++) {
		if (boatList[i].symbol == symbol) {
			return colorValue(boatList[i].color);
		}
	}
	return -1;
}

/**
 * Given the output file stream
 * saves the table dimensions
 * (number of lines x number of columns)
 */
void writeDimensionsToFile(const vector < vector<char> >& table, ofstream& outputFile){
	outputFile<<table.size()<<" x "<<table[0].size()<<endl;
}

/**
 * Given a boat with set position
 * writes its info on the given output file stream
 */
void writeBoatToFile(boat ship, ofstream& outputFile){
	outputFile<<ship.symbol<<" "
			<<ship.size<<" "
			<<char(ship.line +'A')<<""
			<<char(ship.column + 'a')<<" "
			<<ship.orientation<<" "
			<<colorValue(ship.color)<<endl;
}

/**
 * returns false if the ship is out of bounds (either partially or fully)
 * returns false if the ship crosses another one
 * returns true if the position is valid
 */
bool isValidPosition(const vector<vector <char> > &table, boat ship) {
	if (ship.column > table[0].size() || ship.line > table.size())
		return false;
	if (!(ship.orientation == 'V' || ship.orientation == 'H'))
		return false;
	if (ship.orientation == 'V') {
		if (ship.line + ship.size > table.size())
			return false;
		else {
			for (size_t i = 0; i < ship.size; i++) {
				if (table[ship.line+i][ship.column] != '.')
					return false;
			}
		}
	}
	if (ship.orientation == 'H') {
		if (ship.column + ship.size > table[0].size())
			return false;
		else {
			for (size_t i = 0; i < ship.size; i++) {
				if (table[ship.line][ship.column+i] != '.')
					return false;
			}
		}
	};
	return true;
}

/**
 * prints to the console the table vector (with light gray background and light blue dots)
 * if it finds a symbol different than '.' it will look up for its color
 * and print the symbol with the according color
 */
void printTable(const vector<vector <char> > &table, const vector<boat> &boatList) {
	cout << " ";
	setcolor(15);
	for (size_t k = 0; k < table.size(); k++) {
		cout << setw(2) << char('a' + k);
	}
	cout << endl;

	for (size_t i = 0; i < table.size(); i++) {
		for (size_t j = 0; j < table[0].size(); j++) {
			if (j == 0) {
				setcolor(15, 0);
				cout << char('A' + i);
			}
			if (table[i][j] == '.')
				setcolor(9, 7);
			else
				setcolor(boatColor(boatList, table[i][j]), 7);
			cout << setw(2) << table[i][j];
		}
		cout << endl;
	}
}

/**
 * adds a boat to its given position (and orientation) in the table matrix
 */
void addBoatManual(vector<vector<char> >& table, boat ship) {
	if (ship.orientation == 'V')
		for (size_t i = ship.line; i < ship.line + ship.size; i++) {
			table[i][ship.column] = ship.symbol;
		}
	else
		for (size_t i = ship.column; i < ship.column + ship.size; i++) {
			table[ship.line][i] = ship.symbol;
		}
}

/**
 * adds a boat to a randomly assigned (and orientation) in the table matrix -- create randomBetween function
 * always checking if its valid and repeating the assignation process if it isn't (until it is)
 */
void addBoatRandom(vector< vector<char> >& table, boat &ship){
	do{
		ship.line = rand()%table.size();
		ship.column = rand()%table[0].size();
		rand()%2==0 ? ship.orientation = 'H' : ship.orientation = 'V';
	}while(!isValidPosition(table, ship));
	if (ship.orientation == 'V')
		for (size_t i = ship.line; i < ship.line + ship.size; i++) {
			table[i][ship.column] = ship.symbol;
		}
	else
		for (size_t i = ship.column; i < ship.column + ship.size; i++) {
			table[ship.line][i] = ship.symbol;
		}
}

/**
 * given the input file stream, it will ask for the name of the file - i/o in english
 * and open it, repeating the process if the name doesn't exist
 */
void loadConfigFile(ifstream& configFile) {
	string fileName="";
	do {
		cout << "Insira o nome do ficheiro de configuracao:  " << endl;
		cin >> fileName;
		if (fileName.find('.') == string::npos)
			//This will add
			fileName += ".txt"; //the .txt extension if the user didn't include one.

		configFile.open(fileName.c_str(), ios::in);
		if (!configFile.is_open())
			cerr << "Ficheiro nao existe ou nao pode ser aberto!" << endl
			<< "Introduza novamente o nome do ficheiro" << endl;
	} while (!configFile.is_open());
}

/**
 * given the output file stream, it will ask for the name of the file
 * and open/create it
 */
void createOutputFile(ofstream& outputFile) {
	string outputFileName="";
	cout << "Insira o nome do ficheiro de tabuleiro (para gravacao): " << endl;
	cin >> outputFileName;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (outputFileName.find('.') == string::npos)
		//This will add
		outputFileName += ".txt"; //the .txt extension if the user didn't include one.

	outputFile.open(outputFileName.c_str(), ios::out);
}

/**
 * given a vector and the input file stream
 * this will extract the info for every boat in the file
 * and push them into the vector, making it a list of boats
 */
void readConfigFile(ifstream& configFile, vector<boat>& boatList) {
	boat tempBoat;
	char charTrash;
	do {
		configFile >> tempBoat.number >> charTrash >> tempBoat.name >> charTrash
		>> tempBoat.size >> charTrash >> tempBoat.symbol >> charTrash
		>> tempBoat.color;
		boatList.push_back(tempBoat);
	} while (!configFile.eof());
}

/**
 * reads the mode of writing the boats on the game board
 * doesn't need to return since it's passed by reference
 */
void readMode(char &mode) {
	do {
		cout
		<< "Escolha o modo de colocacao dos barcos (A para automatico, M para manual):";
		cin >> mode;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (!(mode == 'A' || mode == 'M'))
			cerr << "Por favor escolha entre modo Automatico (A) ou manual (M)"
			<< endl;
	} while (!(mode == 'A' || mode == 'M'));
}

/**
 * Given the mode, the list of boats, the board matrix and the output file stream
 * In automatic mode, call the function to add boats randomly until there are no boats left
 * In manual mode, it will ask for the position and orientation
 * and if the position is valid, add it.
 * then, for both cases, it writes the boat info in the output file
 * -- COULD MAKE readCoordinates
 */
void addBoatsAndSaveToFile(char mode, vector<boat>& boatList,
		vector<vector<char> >& table, ofstream& outputFile) {
	if (mode == 'A') {
		for (size_t i = 0; i < boatList.size(); i++) {
			for (size_t j = 0; j < boatList[i].number; j++) {
				addBoatRandom(table, boatList[i]);
				writeBoatToFile(boatList[i], outputFile);
			}
		}
	} else if (mode == 'M') {
		char line, column;
		for (size_t i = 0; i < boatList.size(); i++) {
			for (size_t j = boatList.at(i).number; j > 0; j--) {
				do {
					printTable(table, boatList);
					setcolor(colorValue(boatList[i].color), 0);
					cout << boatList[i].symbol << " - " << boatList[i].name
							<< ". Tamanho= " << boatList[i].size
							<< ". Falta(m) " << j << "." << endl;
					setcolor(15, 0);
					cout << "LINHA (A.." << char('A' + table.size() - 1)
													<< ")  COLUNA (a.."
													<< char('a' + table[0].size() - 1)
													<< ") ORIENTACAO(H V) ? ";
					cin >> line >> column >> boatList[i].orientation;
					if(cin.fail()){
							cin.clear();
							cin.ignore(1000, '\n');
					}
					boatList[i].line = line - 'A';
					boatList[i].column = column - 'a';
					if (!isValidPosition(table, boatList[i]))
						cerr << "Posicao invalida!" << endl;
				} while (!isValidPosition(table, boatList[i]));
				addBoatManual(table, boatList[i]);
				writeBoatToFile(boatList[i], outputFile);
			}
		}
	}
}

int main() {
	string stringTrash = "";
	char charTrash, mode;
	ifstream configFile;
	ofstream outputFile;
	int tableWidth, tableHeight;
	vector<boat> boatList;

	srand(time(NULL));

	loadConfigFile(configFile);
	createOutputFile(outputFile);

	configFile >> stringTrash >> tableHeight >> charTrash >> tableWidth; //gets "boardName: " and "x" to aux, gets number of lines(height) and columns(width)

	vector<vector <char> > table(tableHeight, vector<char>(tableWidth, '.'));

	writeDimensionsToFile(table, outputFile);
	readConfigFile(configFile, boatList);
	configFile.close();

	readMode(mode);

	addBoatsAndSaveToFile(mode, boatList, table, outputFile);
	printTable(table, boatList);
	setcolor(15,0);
	outputFile.close();
	return 0;
}
