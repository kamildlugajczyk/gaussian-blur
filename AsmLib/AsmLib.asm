;								Filtr Gaussa v0.3
;		
;							 autor: Kamil D³ugajczk
;
;	Changelog:
;	- 12.10.2020 - wersja 0.1.0 - utoworzenie projektu wraz z repozytorium na Githubie
;	- 15.10.2020 - wersja 0.1.1 - utworzenie szkicu projektu: prostego GUI, stworzenie 
;				   dwóch dodatkowych projektów - biblioteki asemblerowej oraz C++ - 
;				   wraz z funkcjami "zaœlepkami". Skonfigurowanie oraz przetestowanie
;				   dynamicznego linkowania bibliotek.
;	- 17.10.2020 - wesja 0.1.2 - poprawa GUI: dodanie miniaturek wybranej bitmapy, 
;				   pola wyboru pliku decelowego, pola do zwrócenia czasu wykonania
;				   oraz pola do wyœwietlenia histogramów
;	- 19.10.2020 - wersja 0.2 - dodanie funkcji generuj¹cej macierz Gaussa do filtrowania
;	- 22.10.2020 - wersja 0.3 - dodanie wczytywania bitmpay oraz implemetacji funkcji
;				   filtru gaussa w C++ (dziala ale zle)
;


.code						; blok kodu
init proc					; procedura initAsm bêd¹ca funkcj¹ zaœlepk¹
	mov rax, 2				; przenieœ wartoœæ 2 do rejestru rex
	ret						; powrót z procedury
init endp					; koniec procedury
end							; koniec bloku kodu