#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
#include "heap_priority_queue.h"
#include "array_priority_queue.h"

// Funkcja do generowania liczb losowych
int generujLiczbeLosowaInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Funkcja do wczytywania kolejki z pliku
template <typename PQ>
bool wczytajZPliku(PQ& pq, const std::string& nazwaPliku) {
    std::ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        std::cerr << "Blad : Nie mozna otworzyc pliku " << nazwaPliku << std::endl;
        return false;
    }

    // Wyczyść istniejącą kolejkę przed wczytaniem nowych danych
    while (!pq.isEmpty()) {
        pq.extractMax();
    }

    int element, priorytet;
    while (plik >> element >> priorytet) {
        pq.insert(element, priorytet);
    }

    plik.close();
    return true;
}

// Funkcja do zapisywania kolejki do pliku
template <typename PQ>
bool zapiszDoPliku(const PQ& pq, const std::string& nazwaPliku) {
    std::ofstream plik(nazwaPliku);
    if (!plik.is_open()) {
        std::cerr << "Blad : Nie mozna otworzyc pliku " << nazwaPliku << " do zapisu" << std::endl;
        return false;
    }

    // Tworzymy kopię kolejki, aby nie modyfikować oryginalnej
    PQ kopiaPQ = pq;

    // Zapisujemy elementy w kolejności od najwyższego priorytetu
    while (!kopiaPQ.isEmpty()) {
        int element = kopiaPQ.findMax();
        int priorytet = kopiaPQ.getPriority(element);
        plik << element << " " << priorytet << std::endl;
        kopiaPQ.extractMax();
    }

    plik.close();
    return true;
}

// Funkcja wyświetlająca menu dla konkretnej kolejki priorytetowej
template <typename PQ>
void wyswietlMenu(PQ& pq, const std::string& nazwaKolejki) {
    int wybor;
    bool dziala = true;

    while (dziala) {
        std::cout << "\n===== Menu " << nazwaKolejki << " =====" << std::endl;
        std::cout << "1. Dodaj element" << std::endl;
        std::cout << "2. Usun element o najwyzszym priorytecie" << std::endl;
        std::cout << "3. Znajdz element o najwyzszym priorytecie" << std::endl;
        std::cout << "4. Zmien priorytet elementu" << std::endl;
        std::cout << "5. Zwieksz priorytet elementu" << std::endl;
        std::cout << "6. Zmniejsz priorytet elementu" << std::endl;
        std::cout << "7. Pokaz rozmiar kolejki" << std::endl;
        std::cout << "8. Wyswietl kolejke" << std::endl;
        std::cout << "9. Stworz losową kolejke" << std::endl;
        std::cout << "10. Wczytaj z pliku" << std::endl;
        std::cout << "11. Zapisz do pliku" << std::endl;
        std::cout << "12. Wyczysc kolejkę" << std::endl;
        std::cout << "13. Powrot " << std::endl;
        std::cout << "Wybierz opcje: ";
        std::cin >> wybor;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nieprawidlowe dane. Sprobuj ponownie." << std::endl;
            continue;
        }

        switch (wybor) {
        case 1: {
            // Dodaj element
            int element, priorytet;
            std::cout << "Podaj wartosc elementu: ";
            std::cin >> element;
            std::cout << "Podaj priorytet: ";
            std::cin >> priorytet;

            auto start = std::chrono::high_resolution_clock::now();
            pq.insert(element, priorytet);
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::cout << "Element dodany. Czas operacji: " << czas << " ns" << std::endl;
            break;
        }
        case 2: {
            // Usuń element o najwyższym priorytecie
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta." << std::endl;
                break;
            }

            auto start = std::chrono::high_resolution_clock::now();
            int maxElement = pq.extractMax();
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::cout << "Usuniety element: " << maxElement << ". Czas operacji: " << czas << " ns" << std::endl;
            break;
        }
        case 3: {
            // Znajdź element o najwyższym priorytecie
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta." << std::endl;
                break;
            }

            auto start = std::chrono::high_resolution_clock::now();
            int maxElement = pq.findMax();
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::cout << "Element o najwyzszym priorytecie: " << maxElement << ". Czas operacji: " << czas << " ns" << std::endl;
            break;
        }
        case 4: {
            // Zmień priorytet elementu
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta." << std::endl;
                break;
            }

            int element;
            std::cout << "Podaj wartosc elementu do modyfikacji: ";
            std::cin >> element;

            try {
                // Pobierz i wyświetl aktualny priorytet
                int aktualnyPriorytet = pq.getPriority(element);
                std::cout << "Aktualny priorytet elementu " << element << " to: " << aktualnyPriorytet << std::endl;

                int nowyPriorytet;
                std::cout << "Podaj nowy priorytet: ";
                std::cin >> nowyPriorytet;

                auto start = std::chrono::high_resolution_clock::now();
                pq.modifyKey(element, nowyPriorytet);
                auto end = std::chrono::high_resolution_clock::now();

                auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                std::cout << "Priorytet zmieniony z " << aktualnyPriorytet << " na " << nowyPriorytet
                    << ". Czas operacji: " << czas << " ns" << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Blad: " << e.what() << std::endl;
            }
            break;
        }
        case 5: {
            // Zwiększ priorytet elementu
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta." << std::endl;
                break;
            }

            int element;
            std::cout << "Podaj wartosc elementu do zwiekszenia priorytetu: ";
            std::cin >> element;

            try {
                // Pobierz i wyświetl aktualny priorytet
                int aktualnyPriorytet = pq.getPriority(element);
                std::cout << "Aktualny priorytet elementu " << element << " to: " << aktualnyPriorytet << std::endl;

                int nowyPriorytet;
                std::cout << "Podaj nowy (wyzszy) priorytet: ";
                std::cin >> nowyPriorytet;

                auto start = std::chrono::high_resolution_clock::now();
                pq.increaseKey(element, nowyPriorytet);
                auto end = std::chrono::high_resolution_clock::now();

                auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                std::cout << "Priorytet zwiekszony z " << aktualnyPriorytet << " na " << nowyPriorytet
                    << ". Czas operacji: " << czas << " ns" << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Blad: " << e.what() << std::endl;
            }
            break;
        }
        case 6: {
            // Zmniejsz priorytet elementu
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta." << std::endl;
                break;
            }

            int element;
            std::cout << "Podaj wartosc elementu do zmniejszenia priorytetu: ";
            std::cin >> element;

            try {
                // Pobierz i wyświetl aktualny priorytet
                int aktualnyPriorytet = pq.getPriority(element);
                std::cout << "Aktualny priorytet elementu " << element << " to: " << aktualnyPriorytet << std::endl;

                int nowyPriorytet;
                std::cout << "Podaj nowy (nizszy) priorytet: ";
                std::cin >> nowyPriorytet;

                auto start = std::chrono::high_resolution_clock::now();
                pq.decreaseKey(element, nowyPriorytet);
                auto end = std::chrono::high_resolution_clock::now();

                auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                std::cout << "Priorytet zmniejszony z " << aktualnyPriorytet << " na " << nowyPriorytet
                    << ". Czas operacji: " << czas << " ns" << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Blad: " << e.what() << std::endl;
            }
            break;
        }
        case 7: {
            // Pokaż rozmiar kolejki
            auto start = std::chrono::high_resolution_clock::now();
            int rozmiar = pq.returnSize();
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::cout << "Rozmiar kolejki: " << rozmiar << " elementow. Czas operacji: " << czas << " ns" << std::endl;
            break;
        }
        case 8: {
            // Wyświetl kolejkę
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta." << std::endl;
            }
            else {
                pq.print();
                std::cout << "Rozmiar kolejki: " << pq.returnSize() << " elementow" << std::endl;
            }
            break;
        }
        case 9: {
            // Utwórz losową kolejkę
            int rozmiar, minPriorytet, maxPriorytet;
            std::cout << "Podaj rozmiar kolejki: ";
            std::cin >> rozmiar;
            std::cout << "Podaj minimalny priorytet: ";
            std::cin >> minPriorytet;
            std::cout << "Podaj maksymalny priorytet: ";
            std::cin >> maxPriorytet;

            // Wyczyść istniejącą kolejkę
            pq.clear();

            // Wygeneruj losową kolejkę
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < rozmiar; i++) {
                int priorytet = generujLiczbeLosowaInt(minPriorytet, maxPriorytet);
                pq.insert(i, priorytet);
            }
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Utworzono losowa kolejke z " << rozmiar << " elementami. Czas operacji: " << czas << " ms" << std::endl;
            break;
        }
        case 10: {
            // Wczytaj z pliku
            std::string nazwaPliku;
            std::cout << "Podaj nazwe pliku do wczytania: ";
            std::cin >> nazwaPliku;

            auto start = std::chrono::high_resolution_clock::now();
            bool sukces = wczytajZPliku(pq, nazwaPliku);
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (sukces) {
                std::cout << "Wczytano dane z pliku. Czas operacji: " << czas << " ms" << std::endl;
                std::cout << "Rozmiar kolejki po wczytaniu: " << pq.returnSize() << " elementów" << std::endl;
            }
            break;
        }
        case 11: {
            // Zapisz do pliku
            if (pq.isEmpty()) {
                std::cout << "Kolejka jest pusta, nie ma co zapisac." << std::endl;
                break;
            }

            std::string nazwaPliku;
            std::cout << "Podaj nazwe pliku do zapisu: ";
            std::cin >> nazwaPliku;

            auto start = std::chrono::high_resolution_clock::now();
            bool sukces = zapiszDoPliku(pq, nazwaPliku);
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (sukces) {
                std::cout << "Zapisano dane do pliku. Czas operacji: " << czas << " ms" << std::endl;
            }
            break;
        }
        case 12: {
            // Wyczyść kolejkę
            auto start = std::chrono::high_resolution_clock::now();
            pq.clear();
            auto end = std::chrono::high_resolution_clock::now();

            auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::cout << "Kolejka zostala wyczyszczona. Czas operacji: " << czas << " ns" << std::endl;
            break;
        }
        case 13:
            dziala = false;
            break;
        default:
            std::cout << "Nieprawidlowy wybor. Sprobuj ponownie." << std::endl;
        }
    }
}

// Funkcja do utworzenia wypełnionej kolejki priorytetowej opartej na kopcu
HeapPriorityQueue<int> utworzWypelnionaKolejkeKopca(int rozmiar, int powtorzenie, const int* losoweElementy, const int* losowePriorytety) {
    HeapPriorityQueue<int> pq;
    for (int i = 0; i < rozmiar; i++) {
        pq.insert(losoweElementy[i], losowePriorytety[i]);
    }
    return pq;
}

// Funkcja do utworzenia wypełnionej kolejki priorytetowej opartej na tablicy
ArrayPriorityQueue<int> utworzWypelnionaKolejkeTablicy(int rozmiar, int powtorzenie, const int* losoweElementy, const int* losowePriorytety) {
    ArrayPriorityQueue<int> pq;
    for (int i = 0; i < rozmiar; i++) {
        pq.insert(losoweElementy[i], losowePriorytety[i]);
    }
    return pq;
}

// Funkcja do generowania losowych tablic do testów
void generujLosoweTablice(int** losoweElementy, int** losowePriorytety, int** losoweElementyDoModyfikacji, int powtorzenia, int maxRozmiar) {
    // Alokacja pamięci dla tablic
    *losoweElementy = new int[maxRozmiar];
    *losowePriorytety = new int[maxRozmiar];
    *losoweElementyDoModyfikacji = new int[maxRozmiar / 10];

    // Wygeneruj losowe elementy i priorytety
    for (int i = 0; i < maxRozmiar; i++) {
        (*losoweElementy)[i] = i; // Użyj indeksu jako wartości elementu
        (*losowePriorytety)[i] = std::rand() % 1000000; // Losowy priorytet
    }

    // Wygeneruj losowe elementy do modyfikacji (podzbiór elementów)
    for (int i = 0; i < maxRozmiar / 10; i++) {
        (*losoweElementyDoModyfikacji)[i] = std::rand() % maxRozmiar;
    }
}

// Szablon funkcji do testowania wydajności dowolnego typu kolejki priorytetowej
template <typename PQ, typename FunkcjaUtworzWypelnionaKolejke>
void testujWydajnoscKolejki(std::ofstream& plikWynikow,
    const std::string& nazwaKolejki,
    FunkcjaUtworzWypelnionaKolejke utworzWypelnionaKolejke,
    const int* losoweElementy,
    const int* losowePriorytety,
    const int* losoweElementyDoModyfikacji) {
    // Stałe dla testów
    const int POWTORZENIA = 100;
    const int ROZMIARY_TESTOW[] = { 5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000 };
    const int LICZBA_ROZMIAROW = sizeof(ROZMIARY_TESTOW) / sizeof(ROZMIARY_TESTOW[0]);

    std::cout << "\n=== TESTOWANIE " << nazwaKolejki << " ===" << std::endl;
    plikWynikow << "\n=== " << nazwaKolejki << " ===" << std::endl;

    // Nagłówki CSV
    plikWynikow << "Rozmiar,Insert(ns),ExtractMax(ns),FindMax(ns),ModifyKey(ns),ReturnSize(ns)" << std::endl;

    for (int r = 0; r < LICZBA_ROZMIAROW; r++) {
        int rozmiar = ROZMIARY_TESTOW[r];
        std::cout << "Testowanie dla rozmiaru: " << rozmiar << std::endl;

        // Zmienne do przechowywania czasów
        double sumaInsert = 0;
        double sumaExtractMax = 0;
        double sumaFindMax = 0;
        double sumaModifyKey = 0;
        double sumaReturnSize = 0;

        // Utwórz bazową kolejkę dla danego rozmiaru
        PQ bazowaKolejka = utworzWypelnionaKolejke(rozmiar, 0, losoweElementy, losowePriorytety);

        for (int rep = 0; rep < POWTORZENIA; rep++) {
            // Test operacji insert
            {
                PQ kopiaKolejki = bazowaKolejka; // Kopia do testowania
                int element = rozmiar + rep; // Nowy element
                int priorytet = losowePriorytety[rep % 100];

                auto start = std::chrono::high_resolution_clock::now();
                kopiaKolejki.insert(element, priorytet);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::nano> czas = end - start;
                sumaInsert += czas.count();
            }

            // Test operacji extract-max
            {
                PQ kopiaKolejki = bazowaKolejka;
                if (!kopiaKolejki.isEmpty()) {
                    auto start = std::chrono::high_resolution_clock::now();
                    kopiaKolejki.extractMax();
                    auto end = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<double, std::nano> czas = end - start;
                    sumaExtractMax += czas.count();
                }
            }

            // Test operacji find-max
            {
                if (!bazowaKolejka.isEmpty()) {
                    auto start = std::chrono::high_resolution_clock::now();
                    bazowaKolejka.findMax();
                    auto end = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<double, std::nano> czas = end - start;
                    sumaFindMax += czas.count();
                }
            }

            // Test operacji modify-key
            {
                PQ kopiaKolejki = bazowaKolejka;
                if (!kopiaKolejki.isEmpty()) {
                    int elementDoModyfikacji = losoweElementyDoModyfikacji[rep % 10] % rozmiar;
                    int nowyPriorytet = std::rand() % 1000000;

                    try {
                        auto start = std::chrono::high_resolution_clock::now();
                        kopiaKolejki.modifyKey(elementDoModyfikacji, nowyPriorytet);
                        auto end = std::chrono::high_resolution_clock::now();

                        std::chrono::duration<double, std::nano> czas = end - start;
                        sumaModifyKey += czas.count();
                    }
                    catch (...) {
                        // Ignoruj błędy (element może nie zostać znaleziony)
                    }
                }
            }

            // Test operacji return-size
            {
                auto start = std::chrono::high_resolution_clock::now();
                bazowaKolejka.returnSize();
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::nano> czas = end - start;
                sumaReturnSize += czas.count();
            }
        }

        // Oblicz średnie czasy (w nanosekundach)
        double sredniaInsert = sumaInsert / POWTORZENIA;
        double sredniaExtractMax = sumaExtractMax / POWTORZENIA;
        double sredniaFindMax = sumaFindMax / POWTORZENIA;
        double sredniaModifyKey = sumaModifyKey / POWTORZENIA;
        double sredniaReturnSize = sumaReturnSize / POWTORZENIA;

        // Wyniki
        std::cout << "Rozmiar: " << rozmiar << std::endl;
        std::cout << "  Insert: " << sredniaInsert << " ns" << std::endl;
        std::cout << "  Extract Max: " << sredniaExtractMax << " ns" << std::endl;
        std::cout << "  Find Max: " << sredniaFindMax << " ns" << std::endl;
        std::cout << "  Modify Key: " << sredniaModifyKey << " ns" << std::endl;
        std::cout << "  Return Size: " << sredniaReturnSize << " ns" << std::endl;

        // Zapisz do pliku
        plikWynikow << rozmiar << ","
            << sredniaInsert << ","
            << sredniaExtractMax << ","
            << sredniaFindMax << ","
            << sredniaModifyKey << ","
            << sredniaReturnSize << std::endl;
    }
}

// Funkcja do uruchamiania testów wydajności
void uruchomTestyWydajnosci() {
    // Inicjalizacja generatora liczb losowych
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Stałe dla testów
    const int POWTORZENIA = 100;
    const int ROZMIARY_TESTOW[] = { 5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000 };
    const int LICZBA_ROZMIAROW = sizeof(ROZMIARY_TESTOW) / sizeof(ROZMIARY_TESTOW[0]);
    const int MAX_ROZMIAR = ROZMIARY_TESTOW[LICZBA_ROZMIAROW - 1];

    // Tablice do przechowywania wygenerowanych losowych danych
    int* losoweElementy = nullptr;
    int* losowePriorytety = nullptr;
    int* losoweElementyDoModyfikacji = nullptr;

    // Wygeneruj losowe tablice przed uruchomieniem testów
    std::cout << "Generowanie losowych tablic..." << std::endl;
    generujLosoweTablice(&losoweElementy, &losowePriorytety, &losoweElementyDoModyfikacji, POWTORZENIA, MAX_ROZMIAR);

    std::cout << "TESTY WYDAJNOSCI KOLEJEK " << std::endl;
    std::cout << "=====================================" << std::endl;

    // Otwórz plik do zapisywania wyników
    std::ofstream plikWynikow("wyniki_kolejek_priorytetowych.csv");
    if (!plikWynikow.is_open()) {
        std::cerr << "Blad  Nie mozna otworzyc pliku wynikow." << std::endl;

        // Zwolnij zaalokowaną pamięć
        delete[] losoweElementy;
        delete[] losowePriorytety;
        delete[] losoweElementyDoModyfikacji;

        return;
    }

    // Zapisz nagłówek
    plikWynikow << "TESTY WYDAJNOSCI KOLEJEK PRIORYTETOWYCH" << std::endl;

    try {
        // Uruchom testy dla każdego typu kolejki
        std::cout << "Testowanie kolejki priorytetowej opartej na kopcu..." << std::endl;
        testujWydajnoscKolejki<HeapPriorityQueue<int>>(
            plikWynikow,
            "KOLEJKA PRIORYTETOWA OPARTA NA KOPCU",
            utworzWypelnionaKolejkeKopca,
            losoweElementy,
            losowePriorytety,
            losoweElementyDoModyfikacji
        );
        // Upewnij się, że dane są zapisane do pliku
        plikWynikow.flush();

        std::cout << "Testowanie kolejki priorytetowej opartej na tablicy..." << std::endl;
        testujWydajnoscKolejki<ArrayPriorityQueue<int>>(
            plikWynikow,
            "KOLEJKA PRIORYTETOWA OPARTA NA TABLICY",
            utworzWypelnionaKolejkeTablicy,
            losoweElementy,
            losowePriorytety,
            losoweElementyDoModyfikacji
        );
        plikWynikow.flush();
    }
    catch (const std::exception& e) {
        std::cerr << "Wystapil blad podczas testowania: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Wystapil nieznany blad podczas testowania." << std::endl;
    }

    // Zamknij plik
    plikWynikow.close();

    // Zwolnij zaalokowaną pamięć
    delete[] losoweElementy;
    delete[] losowePriorytety;
    delete[] losoweElementyDoModyfikacji;

    std::cout << "\nWyniki zostaly zapisane do pliku wyniki_kolejek_priorytetowych.csv" << std::endl;
}

int main() {
    // Inicjalizacja generatora liczb losowych
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Utwórz kolejki priorytetowe
    HeapPriorityQueue<int> kolejkaKopca;
    ArrayPriorityQueue<int> kolejkaTablicy;

    int wybor;
    bool dziala = true;

    while (dziala) {
        std::cout << "\n===== Tester Kolejek Priorytetowych =====" << std::endl;
        std::cout << "1. Kolejka priorytetowa oparta na kopcu" << std::endl;
        std::cout << "2. Kolejka priorytetowa oparta na tablicy" << std::endl;
        std::cout << "3. Uruchom testy wydajnosci" << std::endl;
        std::cout << "0. Wyjscie" << std::endl;
        std::cout << "Wybierz opcje: ";
        std::cin >> wybor;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nieprawidlowe dane. Sprobuj ponownie." << std::endl;
            continue;
        }

        switch (wybor) {
        case 0:
            std::cout << "Wyjscie z programu." << std::endl;
            dziala = false;
            break;
        case 1:
            wyswietlMenu(kolejkaKopca, "Kolejka Priorytetowa Oparta na Kopcu");
            break;
        case 2:
            wyswietlMenu(kolejkaTablicy, "Kolejka Priorytetowa Oparta na Tablicy");
            break;
        case 3:
            uruchomTestyWydajnosci();
            break;
        default:
            std::cout << "Nieprawidlowy wybor. Sprobuj ponownie." << std::endl;
        }
    }

    return 0;
}
