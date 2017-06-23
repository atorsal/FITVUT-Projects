// tail2.c
// Riesenie IJC-DU2, priklad a), 28.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: g++ 4.9
// Popis: Vypis poslednych N riadkov zo vstupu alebo suboru

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stdlib.h>
#include <string.h>

// Default N value for tail
#define DEFAULT_LINE_COUNT 10

using namespace std;

int to_number(char *str);
int getLinesCount(int argc, char *argv[]);
istream *getFileStream(fstream * fileStream, int argc, char *argv[]);
void printLikeTail(istream * fileStream, unsigned int lines);

int to_number(char *str)
{
	char *error = NULL;
	int number = strtol(str, &error, 10);
	if (!*error) {
		return number;
	} else {
		fprintf(stderr, "Wrong line count number.\n");
		exit(1);
	}

}

int getLinesCount(int argc, char *argv[])
{
	if (argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0)) {
		return DEFAULT_LINE_COUNT;
	}

	if (argc == 2 && strcmp(argv[1], "-n")) {
		return DEFAULT_LINE_COUNT;
	}

	if (argc == 3 && strcmp(argv[1], "-n") == 0) {
		return to_number(argv[2]);
	}

	if (argc == 4 && strcmp(argv[1], "-n") == 0) {
		return to_number(argv[2]);
	}

	if (argc == 4 && strcmp(argv[2], "-n") == 0) {
		return to_number(argv[3]);
	}

	return -1;
}

istream *getFileStream(fstream * fileStream, int argc, char *argv[])
{
	if (argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0)
	    || (argc == 3 && strcmp(argv[1], "-n") == 0)) {
		return &cin;
	}

	if ((argc == 2 && strcmp(argv[1], "-n") != 0)
	    || (argc == 3 && strcmp(argv[1], "-n") != 0)) {
		(*fileStream).open(argv[1], ios::in);
		if (fileStream == NULL || (*fileStream).fail()) {
			cerr << "Unable to open file " << argv[1] << endl;
		} else {
			return fileStream;
		}
	}


	if (argc == 4 && strcmp(argv[1], "-n") == 0) {
		(*fileStream).open(argv[3], ios::in);
		if (fileStream == NULL || (*fileStream).fail()) {
			cerr << "Unable to open file " << argv[3] << endl;
		} else {
			return fileStream;
		}
	}

	if (argc == 4 && strcmp(argv[2], "-n") == 0) {
		(*fileStream).open(argv[1], ios::in);
		if (fileStream == NULL || (*fileStream).fail()) {
			cerr << "Unable to open file " << argv[1] << endl;
		} else {
			return fileStream;
		}
	}

	cerr << "Invalid arguments." << endl;

	return NULL;
}


void printLikeTail(istream * fileStream, unsigned int lines)
{
	queue < string > buffer;
	string data;
	unsigned int lines_counter = 0;
	while (getline(*fileStream, data) && lines_counter < lines) {
		buffer.push(data);

		if (buffer.size() > lines) {
			buffer.pop();
		}

		lines_counter++;
	}

	while (!buffer.empty()) {
		cout << buffer.front() << endl;
		buffer.pop();
	}

}

int main(int argc, char *argv[])
{
	ios::sync_with_stdio(false);

	fstream file;
	istream *fileStream = getFileStream(&file, argc, argv);
	if (fileStream == NULL) {
		return 1;
	}

	int lines = getLinesCount(argc, argv);
	if (lines < 0) {
		lines = -lines;
	}
	if (lines != 0) {
		printLikeTail(fileStream, lines);
	}
	file.close();

	return EXIT_SUCCESS;
}
