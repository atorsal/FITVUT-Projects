// Import kniznic s funkciami
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

/*
Program: Praca s textom (1. projekt v IZP)
Autor: David Bolvansky (xbolva00)
*/


// Prototypy funkcii
int getLength(char input[]);
bool isPrimeNumber(int number);
bool isPalindrome(char input[]);
bool isNumber(char input[]);
bool isDate(char input[], char **weekDay);
void processInput(char input[]);


// Funkcia na zistenie dlzky retazca
int getLength(char input[]) {
    int length = 0;
    while(input[length] != '\0') {
        length++;
    }
    return length;
}

// Funkcia, ktora zistuje, ci cislo je prvocislo
bool isPrimeNumber(int number) {
    // Overenie prvocisla priebeha na cislach od 1 po INT_MAX
    // Cislo 1 nie je prvocislo, overenie prvocisla zacina od cisla 2
    if (number < 2 || number > INT_MAX) {
        return false;
    }

    // Zistovanie delitelov, ak najdeny delitel nie je nase cislo
    // cislo nie je prvocislo
    for (int i = 2; i < number/2; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }
    return true;
}

// Funkcia, ktora zistuje, ci retazec je palidrom
bool isPalindrome(char input[]) {
    // Dlzka retazca
    int len = getLength(input);
    // Porovnanie - prvy znak s poslednym, druhy s predposlednym, atd.
    for (int i = 0; i <= (len/2)-1; i++) {
        if (input[i] != input[len-i-1]) {
            return false;
        }
    }
    return true;
}

// Funkcia, ktora zistuje, ci retazec je cislo
bool isNumber(char input[]) {
    // Chybova cast v retazci pri prevode na cislo
    char *wrongNumberPart;
    // Ziadna chyba
    errno = 0;

    // Prevod retazca na číslo
    long int number = strtol(input, &wrongNumberPart, 10);

    // Kontrola pretecenia long integer
    if (errno == ERANGE) {
        return false;
    }

    // Ak je cislo zaporne, tak je retazec
    if (number < 0) {
        return false;
    }

    // Ak je chybova cast prazdna, prevod na cislo bol uspesny
    if (!*wrongNumberPart) {
        return true;
    }
    else {
        return false;
    }
}

// Funkcia na zistenie, ci vstup je datum
bool isDate(char input[], char **weekDay) {
    // Datumova struktura
    struct tm date;
    // Inicializacia premennych pre den, mesiac, rok
    int year, month, day = 0;
    // Nazvy dni
    char *days[] = { "Sun" , "Mon", "Tue", "Wen", "Thu", "Fri", "Sat"};

    // Nacitanie datumu, ak vstup suhlasi so vzorom yyyy-mm-dd a dlzka retazca je 10 znakov
    if (sscanf(input, "%4d-%2d-%2d", &year, &month, &day) == 3 && getLength(input) == 10) {

        // Kontrola ziskanych udajov o datume
        // Zaporny rok => neplatny datum
        if (year <= 0) {
            return false;
        }

        // Mesiac musi byt z rozsahu 1-12
        if (month < 1 || month > 12) {
            return false;
        }

        // Den musi byt z rozsahu 1 az 31
        if (day < 1 || day > 31) {
            return false;
        }

        // inicializacia datumovej struktury na overenie datumu
        date.tm_sec = 0;
        date.tm_min = 0;
        date.tm_hour = 12;
        date.tm_mday = day;
        date.tm_mon = month - 1;
        date.tm_year = year - 1900;

        // Vysledok prevodu datumu
        int retVal = mktime(&date);

        // Kontrola chyby pri prevode datumu
        if (retVal == -1) {
            return false;
        }

        // Priradenie dna v tyzdni pre datum na vstupe
        *weekDay = days[date.tm_wday];
        return true;
    }
    return false;
}

// Funkcia na spracovanie vstupu
void processInput(char input[]) {
    // Den v tyzdni
    char *weekDay;
    
    // Overenie ci vstup je cislo
    if (isNumber(input)) {
        long int number = strtol(input, NULL, 10);
        // Overenie, ci je cislo prvocislo
        if (isPrimeNumber(number)) {
            printf("number: %ld (prime)\n", number);
        }
            else {
            printf("number: %ld\n", number);
        }
    }

    // Overenie, ci vstup je datum
    else if (isDate(input, &weekDay)) {
        printf("date: %s %s\n", weekDay, input);
    }

    // Vstup je retazec
    else {
        // Overenie, ci retazec je palindrom
        if (isPalindrome(input)) {
            printf("word: %s (palindrome)\n", input);
        }
        else {
                printf("word: %s\n", input);
        }
    }
}

// Zakladna funkcia programu
int main(int argc, char *argv[]) {
    (void) argv;
    // Vstup
    char input[101];

    // Ak je pocet argumentov 2 alebo viac, vypiseme informacie o programe
    if (argc>=2) {
        printf("Program: Praca s textom, autor: David Bolvansky (xbolva00)\n\n");
        printf("Program pracuje zo vstupom z klavesnice (./proj1) alebo ako vstup\nje mozne pouzit presmerovany subor (./proj1 < data.txt)\n");
        printf("Vstup sa spracovava, zistuje sa, ci vstup je cislo a datum,\nak to nie je cislo ani datum, berieme vstup ako retazec.\n\nPri cisle sa zistuje, ci je prvocislo.\n");
        printf("Pri datume sa zistuje jeho platnost a vypisuje sa nazov dna pre dany datum\nPri retazci sa zistuje, ci je palindrom.\n");

        return EXIT_SUCCESS;
    }

    // Nacitanie vstupu az do jeho konca, limit 100 znakov
    while(scanf("%100s", input) != EOF) {
        // Spracovanie vstupu
        processInput(input);
    }
    return EXIT_SUCCESS;
}
