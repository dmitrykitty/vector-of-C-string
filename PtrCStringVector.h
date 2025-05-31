#ifndef MYSTRING_H
#define MYSTRING_H

#include <cstddef> // std::size_t

/** @file PtrCStringVector.h
 * @date 9 czerwca 2021
 * @brief W ramach kolokwium trzeba zaimplementowac wszystkie opisane metody klasy PtrCStringVector.
 * Do ponizszych metod **sa testy** w pliku @ref PtrCStringVectorTests.cpp. **/


/** class PtrCStringVector
 * @brief Klasa PtrCStringVector, stanowiaca wektor wskaźników do niemodyfikowalnych tekstów.
 *        Wektor ten moze sie powiekszac o nowe elementy dokonujac kopiowania dotychczasowej zawartosci.
 *        Schemat znajduje się tutaj:
 *        @image html diagramKolokwium1.png
 *        @image latex diagramKolokwium1.png
 *
 * @note Nie wolno uzywac typu [std::string](https://en.cppreference.com/w/cpp/string), nalezy uzyc tablicy char
 * @note Nie wolno uzywac [std::vector](https://en.cppreference.com/w/cpp/container/vector), ma byc reczne zarzadzanie pamiecia
 *
 * @note Można utworzyć dowolną ilość metod pomocniczych lub funkcji w pliku źródłowym.
 *       Niektóre metody pomocnicze są zadeklarowane, ale nie trzeba ich implementować.
 *
 * @param size_ ilosc elementow w kontenerze, tworzac pusty ma byc 0, po kazdym zawolaniu `push_back` powinno byc o jeden wiecej
 * @param capacity_ ilosc zaallokowanej pamieci w kontenerze na elementy, wywolania `push_back` w razie potrzeby maja dokonac reallokacji powiekszajacej
 * @param data_ dynamicznie allokowana tablica wskaznikow do niemodyfikowalnych tekstow **/
class PtrCStringVector {

    std::size_t size_;
    std::size_t capacity_;
    char** data_;

public:
    /** @brief konstruktor domyslny, jego zadaniem jest ustawienie size_, capacity_ i data_ na brak elementow **/
    PtrCStringVector();
    //IMPLEMENTED

    /** @brief konstruktor kopiujacy, dokonujacy **gleboka kopie**, czyli nie tylko tablica wskaznikow na tekst musi zostac skopiowana ale rowniez wszystkie wskazywane teksty
     *  @param source - kontener zrodkowy, z ktorego musza byc skopiowane wszystkie dane **/
    PtrCStringVector(const PtrCStringVector& source);
    //IMPLEMENTED

    /** @brief destruktor, ktory musi koniecznie zwolnic pamiec i inne zasoby **/
    ~PtrCStringVector();
    //IMPLEMENTED


    /** @brief operator przypisania, ktory ma za zadanie przeniesc zawartosc z obiektu zrodlowego
     *  @note prosze sie upewnic, ze zadziala przypisanie na samego siebie: @code{.cpp}
           PtrCStringVector a;
           PtrCStringVector& b = a;
           a = std::move(b);
        @endcode
        @details Operator przypisania przenoszacy powinien **zwolnic dotychczasowa pamiec**, aby nie dopuscic do wyciekow pamieci.
            Powinien tez zostawic obiekt zrodlowy w stanie jak po zawolaniu konstruktora domyslnego. **/
    PtrCStringVector& operator=(PtrCStringVector&& source) noexcept;
    //IMPLEMENTED

    /** @brief operator przypisania, ktory ma za zadanie skopiowac doglebnie tresc, analogicznie jak konstruktor kopiujacy \ref PtrCStringVector(const PtrCStringVector&)
     *  @note prosze sie upewnic, ze zadziala przypisanie na samego siebie: @code{.cpp}
           PtrCStringVector a;
           PtrCStringVector& b = a;
           a = b;
        @endcode
        @note prosze sie upewnic, ze zadziala przypisanie kaskadowe: @code{.cpp}
           PtrCStringVector a, b, c;
           a = b = c;
        @endcode
        @details Operator przypisania powinien **zwolnic pamiec** w razie potrzeby, aby nie dopuscic do wyciekow pamieci. **/
    PtrCStringVector& operator=(const PtrCStringVector& source);
    //IMPLEMENTED


    /** @brief metoda, ktora skopiuje podany tekst i umiesci na koncu w Vectorze. W razie braku miejsca powinna dokonac powiekszenia kontenera.
     *  @param text2Add - tekst do skopiowania doglebnie (na nowa dynamiczna pamiec)
     *  @post Dodany tekst zostanie skopiowany i umieszczony na koncu kontenera. W razie potrzeby tablica wskaznikow powinna byc powiekszona. */
    void push_back(const char* text2Add);
    //IMPLEMENTED

    /** @brief Metoda zwracajaca aktualnie posiadana ilosc elementow w kontenerze.
     *  @return wartosc `size_` */
    [[nodiscard]] auto size() const {
        return size_;
    }

    /** @brief Metoda zwracajaca informacje ile elementow zmiesci sie w zaalokowanej tablicy.
     *  @return wartosc `capacity_` */
    [[nodiscard]] auto capacity() const {
        return capacity_;
    }

    /** @brief operator indeksowania, ktory otrzymawszy indeks, zwroci wskaznik do tekstu znajdujacego sie na danej pozycji w kontenerze
     *  @param index elementu tekstowego w kontenerze
     *  @throw std::out_of_range w razie, gdy `index >= size_` **/
    char* operator[](std::size_t index);
    //IMPLEMENTED

    /** @brief operator indeksowania, podobny do powyzszego @ref operator[], ale zwraca `const char*` i jest metoda stala
     *  @throw std::out_of_range w razie, gdy `index >= size_` **/
    const char* operator[](std::size_t index) const;
    //IMPLEMENTED

    /** @brief operator, ktory tworzy kontener zawierajacy wszystkie elementy z dowoch kontenerow (czyli dodaje kontenery)
     *  @param anotherVector kontener z ktorego maja byc dorzucone elementy
     *  @return nowo-utworzony kontener zawierajacy wszystkie elementy z `*this` i `anotherVector`
     *  @details wpierw beda skopiowane elementy z `*this`, nastepnie wszystkie z `anotherVector` */
    PtrCStringVector operator+(const PtrCStringVector& anotherVector) const;
    //IMPLEMENTED

    /** @brief operator& - ma za zadanie zwrocic nowo-utworzony kontener, ktory bedzie zawieral zawartosc obydwu kontenerow poprzez sklejenie tekstow na odpowiadajacych sobie pozycjach
     *  @param anotherVector kontener z ktorego maja byc doklejone elementy do tych z `*this`
     *  @return Nowo-utworzony kontener zawierajacy wszystkie elementy z `*this` i `anotherVector` posklejane na odpowiadajacych sobie pozycjach
     *  @details W razie braku odpowiadajacych sobie elementow (gdy rozmiary kontenerow sa inne) nalezy od miejsca gdzie elementy ma tylko jeden wziac element tylko z kontenera, ktory go posiada.
     *  Nie nalezy doklejac czegokolwiek laczac napisy (zadnej spacji, srednika itp). Przykladowo mamy dwa kontenery (zbieznosc imion przypadkowa):
     *  1. `PtrCStringVector kobietyWGrupie;`, ktory zawiera teksty:
     *      1. "Anastazja"
     *      2. "Bonifia"
     *      3. "Cecylia"
     *  2. `PtrCStringVector mezczyzniWGrupie;`, ktory zawiera teksty:
     *      1. "Ambrozy"
     *      2. "Bazyli"
     *      3. "Cezary"
     *      4. "Dionizy"
     *      5. "Elohim"
     *  3. Wynikowy `kobietyWGrupie & mezczyzniWGrupie` powinien zawierac:
     *      1. "AnastazjaAmbrozy"
     *      2. "BonifiaBazyli"
     *      3. "CecyliaCezary"
     *      4. "Dionizy"
     *      5. "Elohim" **/
    PtrCStringVector operator&(const PtrCStringVector& anotherVector) const;

protected: // methods:
    /** @brief metoda pomocnicza zwalniajaca wszystkie zasoby i zerujaca skladowe klasy **/
    void free();
    //IMPLEMENTED

    /** @brief metoda pomocnicza dokonujaca allokacji dynamicznej tablicy o okreslonym rozmiarze, nastepnie kopiujaca wszystkie elementy z dotychczasowej tablicy.
     *  @note nalezy pamietac o zwalnianiu zasobow
     *  @details dla uproszczenia zakladamy, ze metoda ta jedynie zwieksza zaalokowana pamiec, nie zmniejsza **/
    void reserve(std::size_t new_size);
    //IMPLEMENTED
};

#endif // MYSTRING_H
