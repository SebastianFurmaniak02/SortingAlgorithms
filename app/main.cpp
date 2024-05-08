#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <chrono>

#include "../inc/mergesort.hh"
#include "../inc/quicksort.hh"
#include "../inc/introsort.hh"

#define N 1300000
#define M 9800000

//Funkcja pozwalajaca przeksztalcic stringa ID Filmu na intiger
int getID(std::string movieID) {
    std::string copy = movieID;
    std::istringstream string_stream(copy);
    int id;
    char c = string_stream.get();
    string_stream.get(c);
    string_stream >> id;
    return id;
} 

//Struktura do przechowywania filmu
struct movie {
    std::string movieID; // ID filmu
    std::string title; // Tytul filmu
    float rating; // Ocena filmu

    movie() { rating = 0; }
    ~movie() {}

    //Przeciazenia operatorow porownania - jesli ocena rozna od zera to porownuj po ratingu, a jesli rowna zero to po ID (domyslnie w konstruktorze ocena = 0)
    bool operator> (movie otherMovie) {
        if (rating !=0) return rating > otherMovie.rating;
        return getID(movieID) > getID(otherMovie.movieID); 
        } 
    bool operator< (movie otherMovie) {
        if (rating !=0) return rating < otherMovie.rating;
        return getID(movieID) < getID(otherMovie.movieID); 
        } 
};

//Funkcja sprawdzajaca, czy dana tablica jest posortowana w kolejnosci rosnacej
template <typename T>
bool checkIfSorted (T* array, int left, int right) {
    for (int i = left; i < right; i++) {
        if (array[i] > array[i+1]) return false;
    }
    return true;
}

//Funkcja liczaca mediane ocen z danej tablicy filmow
float medianOfRatings (movie* array, int left, int right) {
    int size = right - left + 1;
    if (size%2 == 0) { return (array[left + size/2 - 1].rating + array[left + size/2].rating)/2; }
    else { return (array[left+ size/2].rating); }

}

//Funkcja liczaca srednia wartosc ocen z danej tablicy
float averageOfRating (movie* array, int left, int right) {
    float sum = 0;
    for (int i = left; i <= right; i++) {
        sum += array[i].rating;
    }
    return sum/(right - left + 1);
}

//Funkcja wykonujaca rekurencyjne szukanie binarne po ID na tablicy filmow, zwraca indeks na ktorym znajduje sie szukany element, jesli elementu nie ma w tablicy zwroci -1 
int binarySearch(movie *REF, std::string movieID, int left, int right) {
    int ID = getID(movieID), mid, l = left, r = right;
    mid = (l+r)/2;
    if (ID == getID(REF[mid].movieID)) return mid;
    if (l < r) {
        if (ID > getID(REF[mid].movieID)) return binarySearch(REF,movieID,mid + 1,right);
        else return binarySearch(REF,movieID,left,mid-1);
    }
    return -1;
}

//Funkcja kopiujaca wartosci jednej tablicy do drugiej
template <typename T>
void copyArray(T* arrA, T*arrB, int left, int right) {
    for (int i = left; i <= right; i++) {
        arrB[i] = arrA[i];
    }
}

int main(int argc, char *argv[]) {
    bool saveTime, saveMedAvgValues, saveHeader, compareRef; 

    if (argc > 1) { //Sprawadzenie wybranych opcji
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] == 'T') saveTime = true; //Save Time - zapisz do bazy danych czas sortowan
            if (argv[i][0] == '-' && argv[i][1] == 'H') saveHeader = true; //Save Header - zapisz do pliku ilosc elementow oraz naglowki (MergeSort QuickSort IntroSort)
            if (argv[i][0] == '-' && argv[i][1] == 'V') saveMedAvgValues = true; //Save Values of median and average - zapisz wartosci median i srednie
            if (argv[i][0] == '-' && argv[i][1] == 'R') compareRef = true; //Save data with Reference - posortuj tablice referencyjna po ID, dopasuj odpowiednie nazwy filmow po ID do tablicy z filmami przy pomocy szukania binarnego
        }
    }

    std::ifstream data("data/data.tsv"); //Otwarcie pliku z danymi
    std::ofstream dataBase("data/data_base.tsv", std::ios::app); //Otwarcie pliku z baza danych
    std::string buf; // bufor do niepotrzebnych stringow
    int n = N; // przypisanie wartosci n i tworzenie trzech tablic
    movie* arrA = new movie[n];
    movie* arrB = new movie[n];
    movie* arrC = new movie[n];

    //Pobranie danych ID i ratingu do tablicy A
    getline(data,buf);
    int i = 0;
    while (i < n && data >> arrA[i].movieID) {
        data >> arrA[i].rating;
        getline(data,buf);
        i++;
    }
    if (i != n) n = i; // Jesli ilosc danych jest mniejsza niz n, to jest to wlasciwa ilosc danych

    if (saveHeader) dataBase << "Wyniki dla N = " << n << std::endl << "MergeSort   QuickSort   IntroSort" << std::endl; // Jesli opcja z naglowkiem wybrana, wypisz naglowek do pliku z baza danych

    //Kopiowanie tablic
    copyArray(arrA,arrB,0,n-1);
    copyArray(arrA,arrC,0,n-1);

    std::cout << "Czasy sortowan dla " << n << " elementow po ratingu:" << std::endl;

    auto begin = std::chrono::high_resolution_clock::now(); //Czas start
    mergeSort(arrA,0,n-1); //Sortowanie
    auto end = std::chrono::high_resolution_clock::now(); //Czas stop
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin); //Roznica czasow
    std::cout << "MergeSort: " << elapsed.count()*1e-9 << " s" << std::endl; //Wyswietl wartosc czasu sortowania w sekunach
    if (saveTime) dataBase << elapsed.count()*1e-9 << " "; //Jesli opcja zapisu czasu wybrana - zapisz czas do pliku z baza danych
    if (!checkIfSorted(arrA,0,n-1)) { std::cerr << "Tablica 'MergeSort' nieposortowana" << std::endl; return 1; } //Sprawdz czy tablica jest posortowana, jesli nie - zakoncz prace

    begin = std::chrono::high_resolution_clock::now(); //Analogicznie jak przy mergeSort
    quickSort(arrB,0,n-1);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "QuickSort: " << elapsed.count()*1e-9 << " s" << std::endl;
    if (saveTime) dataBase << elapsed.count()*1e-9 << " ";
    if (!checkIfSorted(arrB,0,n-1)) { std::cerr << "Tablica 'QuickSort' nieposortowana" << std::endl; return 1; }

    begin = std::chrono::high_resolution_clock::now(); //Analogicznie jak przy mergeSort
    introSort(arrC,0,n-1);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "IntroSort: " << elapsed.count()*1e-9 << " s" << std::endl;
    if (saveTime) dataBase << elapsed.count()*1e-9 << std::endl;
    if (!checkIfSorted(arrC,0,n-1)) { std::cerr << "Tablica 'IntroSort' nieposortowana" << std::endl; return 1; }

    if (saveMedAvgValues) { //Jesli opcja wybrana, pobierz wartosci mediany i sredniej z kazdej tablicy i zapisz ja do bazy danych
        dataBase << "Median" << std::endl;
        dataBase << medianOfRatings(arrA,0,n-1) << "    ";
        dataBase << medianOfRatings(arrB,0,n-1) << "    ";
        dataBase << medianOfRatings(arrC,0,n-1) << std::endl;
        dataBase << "Average" << std::endl;
        dataBase << averageOfRating(arrA,0,n-1) << "    ";
        dataBase << averageOfRating(arrB,0,n-1) << "    ";
        dataBase << averageOfRating(arrC,0,n-1) << std::endl;
    }

    //Jesli wszystkie tablice posortowane to zwolnij pamiec, dalsze operacje wystarczy przeprowadzic na jednej tablicy
    delete [] arrB;
    delete [] arrC;

    if (compareRef) { //Jesli opcja z tablica referencyjna wybrana
        int m = M; //Przypisz wielkosc tablicy 
        std::ifstream dataRef("data/data_ref.tsv", std::ios::binary); //Otworz plik z danymi referencyjnymi
        std::ofstream sortedData("data/data_sorted.tsv"); // Otworz plik, w ktorym zapiszemy wynik
        movie* REF = new movie[m]; //Przypisanie pamieci tablicy ref.
        
        //Pobierz wartosci ID i nazwy do tablicy referencyjnej
        i = 0;
        getline(dataRef,buf);
        while (i < m && dataRef >> REF[i].movieID) {
            dataRef >> buf;
            getline(dataRef, buf, '\t');
            getline(dataRef,REF[i].title, '\t');
            getline(dataRef,buf);
            i++;
        }
        if (i != m) m = i;

        //Pomiar czasu sortowania tablicy referencyjnej analogiczny jak przypadku sortowan
        begin = std::chrono::high_resolution_clock::now(); 
        introSort(REF,0,m-1);
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << "Czas sortowania dla " << m << " elementow tablicy referencyjnej po ID: " << elapsed.count()*1e-9 << " s" << std::endl;

        if (!checkIfSorted(REF,0,m-1)) { std::cerr << "Tablica referencyjna nieposortowana" << std::endl; return 1; } //Sprawdz czy tablica jest posortowana
        
        //Przypisanie odpowiednich wartosci nazw filmow do tablicy A, porownujac ID z tablica referencyjna za pomoca szukania binarnego
        begin = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < n; j++) {
            int index = binarySearch(REF,arrA[j].movieID,0,m-1); //Szukanie binarne
            if (index == -1) arrA[j].title = "  (no information)"; //Jesli nie znaleziono odpowiedniego indeksu to za tytul napisz o braku informacji
            else arrA[j].title = REF[index].title; //W przeciwnym razie przepisz tytul do tablicy A
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << "Czas dopasowania posortowanych filmow do danych referencyjnych za pomoca szukania binarnego: " << elapsed.count()*1e-9 << " s" << std::endl;

        delete [] REF; //Zwolnij pamiec 

        //Zapisz tablice A w posortowanym pliku wg klucza (Ocena - ID Filmu - Oryginalny tytul)
        sortedData << "Rating   tconst  originalTitle" << std::endl;
        for(int j = 0; j<n; j++) sortedData << arrA[j].rating << "  " << arrA[j].movieID << "   " << arrA[j].title << std::endl;
    }

    return 0;
}
