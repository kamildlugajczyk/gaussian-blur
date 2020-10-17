;			Filtr Gaussa v0.1.2
;		
;		autor: Kamil D�ugajczk
;
;	Changelog:
;	- 12.10.2020 - wersja 0.1.0 - utoworzenie projektu wraz z repozytorium na Githubie
;	- 15.10.2020 - wersja 0.1.1 - utworzenie szkicu projektu: prostego GUI, stworzenie 
;				   dw�ch dodatkowych projekt�w - biblioteki asemblerowej oraz C++ - 
;				   wraz z funkcjami "za�lepkami". Skonfigurowanie oraz przetestowanie
;				   dynamicznego linkowania bibliotek.
;	- 17.10.2020 - wesja 0.1.2 - poprawa GUI: dodanie miniaturek wybranej bitmapy, 
;				   pola wyboru pliku decelowego, pola do zwr�cenia czasu wykonania
;				   oraz pola do wy�wietlenia histogram�w
;
;
;


.code						; blok kodu
initAsm proc				; procedura initAsm b�d�ca funkcj� za�lepk�
	mov rax, 2				; przenie� warto�� 2 do rejestru rex
	ret						; powr�t z procedury
initAsm endp				; koniec procedury
end							; koniec bloku kodu