// tail.c
// Riesenie IJC-DU2, priklad a), 28.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Vypis poslednych N riadkov zo vstupu alebo suboru


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Max line length (510)
#define LINE_LIMIT 510

// Default N value for tail
#define DEFAULT_LINE_COUNT 10

int to_number(char *str);
int getLinesCount(int argc, char *argv[]);
FILE *getFileStream(int argc, char *argv[]);
char **allocLinesBuffer(int lines);
void freeLinesBuffer(char **linesBuffer, int lines);
void printLikeTail(FILE * fileStream, char *linesBuffer[], int lines);

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



FILE *getFileStream(int argc, char *argv[])
{
	if (argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0)
	    || (argc == 3 && strcmp(argv[1], "-n") == 0)) {
		return stdin;
	}

	if ((argc == 2 && strcmp(argv[1], "-n") != 0)
	    || (argc == 3 && strcmp(argv[1], "-n") != 0)) {
		FILE *fileStream = fopen(argv[1], "r");
		if (fileStream == NULL) {
			fprintf(stderr, "Unable to open file %s\n",
				argv[1]);
		} else {
			return fileStream;
		}
	}


	if (argc == 4 && strcmp(argv[1], "-n") == 0) {
		FILE *fileStream = fopen(argv[3], "r");
		if (fileStream == NULL) {
			fprintf(stderr, "Unable to open file %s\n",
				argv[3]);
		} else {
			return fileStream;
		}
	}

	if (argc == 4 && strcmp(argv[2], "-n") == 0) {
		FILE *fileStream = fopen(argv[1], "r");
		if (fileStream == NULL) {
			fprintf(stderr, "Unable to open file %s\n",
				argv[1]);
		} else {
			return fileStream;
		}
	}

	fprintf(stderr, "Invalid arguments.\n");
	return NULL;
}


// Allocate buffer with array of lines
char **allocLinesBuffer(int lines)
{
	// Array of lines
	char **linesBuffer;

	// Allocate memory for array
	linesBuffer = malloc(lines * sizeof(char *));
	if (linesBuffer == NULL)
		return NULL;

	// Allocate lines with certain length
	for (int i = 0; i < lines; i++) {
		// LINE_LIMIT + 2 because string is 510 + \0 and \n
		linesBuffer[i] = malloc((LINE_LIMIT + 2) * sizeof(char *));
		// If malloc failed, we need to free memory
		if (linesBuffer[i] == NULL) {
			// Free everything
			freeLinesBuffer(linesBuffer, i);
			return NULL;
		}
	}

	return linesBuffer;
}

// Free memory with lines
void freeLinesBuffer(char **linesBuffer, int lines)
{
	// Free lines
	for (int i = 0; i < lines; i++) {
		free(linesBuffer[i]);
	}

	// Free linesBuffer
	free(linesBuffer);
}

void printLikeTail(FILE * fileStream, char *linesBuffer[], int lines)
{
	int is_warning = 0;
	char str[LINE_LIMIT + 2];
	char c;
	// Line number - 1
	int i = 0;

	while (fgets(str, LINE_LIMIT + 1, fileStream) != NULL) {
		if (str[strlen(str) - 1] != '\n') {
			if (!feof(fileStream)) {
				strcat(str, "\n");
				while ((c = fgetc(fileStream)) != '\n'
				       && c != EOF);

				if (!is_warning) {
					fprintf(stderr,
						"Max line length (%d) was reached (line n.%d)\n",
						LINE_LIMIT, i + 1);
					is_warning = 1;
				}
			}

		}

		strcpy(linesBuffer[i % lines], str);

		i++;
	}
	int cur_pos = i % lines;


	// When file has less lines that we want to print, cut it lines count to number of lines in file
	if (lines > i) {
		lines = i;
	}
	// Print lines      
	for (int n = cur_pos; n < lines; n++) {
		printf("%s", linesBuffer[n]);
	}

	for (int n = 0; n < cur_pos; n++) {
		printf("%s", linesBuffer[n]);
	}

}

int main(int argc, char *argv[])
{

	FILE *fileStream = getFileStream(argc, argv);
	if (fileStream == NULL) {
		return 1;
	}

	int lines = getLinesCount(argc, argv);
	if (lines < 0) {
		lines = -lines;
	}

	if (lines != 0) {
		// Allocate memory for array of lines
		char **linesBuffer = allocLinesBuffer(lines);

		if (linesBuffer != NULL) {
			printLikeTail(fileStream, linesBuffer, lines);
			freeLinesBuffer(linesBuffer, lines);
		} else {
			fprintf(stderr, "Unable to allocate memory\n");
			return 2;
		}
	}

	fclose(fileStream);

	return EXIT_SUCCESS;
}
