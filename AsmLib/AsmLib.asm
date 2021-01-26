;								Filtr Gaussa v0.8
;		
;							 autor: Kamil D³ugajczk
;
;	Changelog:
;	- 12.10.2020 - wersja 0.1.0 - utoworzenie projektu wraz z repozytorium na Githubie
;	- 15.10.2020 - wersja 0.1.1 - utworzenie szkicu projektu: prostego GUI, stworzenie 
;				   dwóch dodatkowych projektów - biblioteki asemblerowej oraz C++ - 
;				   wraz z funkcjami "zaslepkami". Skonfigurowanie oraz przetestowanie
;				   dynamicznego linkowania bibliotek.
;	- 17.10.2020 - wesja 0.1.2 - poprawa GUI: dodanie miniaturek wybranej bitmapy, 
;				   pola wyboru pliku decelowego, pola do zwrócenia czasu wykonania
;				   oraz pola do wyœwietlenia histogramów
;	- 19.10.2020 - wersja 0.2 - dodanie funkcji generuj¹cej macierz Gaussa do filtrowania
;	- 22.10.2020 - wersja 0.3 - dodanie wczytywania bitmpay oraz implemetacji funkcji
;				   filtru gaussa w C++ (dziala ale zle)
;	- 25.10.2020 - wersja 0.3.1 - naprawienie dzia³ania funkcji filtru gaussa w C++
;	- 30.10.2020 - wersja 0.4 - dodanie pomiaru czasu dla funkcji dll
;	- 14.01.2020 - wersja 0.5 - dodanie obslugi watkow; dodanie ramki wokol wejciowego obrazu
;	- 15.01.2020 - wersja 0.5.1 - zmiana UI
;	- 23.01.2020 - wersja 0.6 - dodanie dll napisanej w assemblerze x64. Zmiana kernela z 2D na 1D
;	- 24.01.2020 - wersja 0.6.1 - poprawa funkcji w assemblerze
;	- 25.01.2020 - wersja 0.7 - dodanie histogramow
;	- 26.01.2020 - wersja 0.8 -
;
;
.data
	startHeight dd ?						; zmienna odpowiadajaca poczatkowej wysokosci od ktorej rozpoczynam przetwarzanie obrazu
	stopHeight dd ?							; zmienna odpowiadajaca wyoskosci na ktorej przestaje przetwarzac obraz
	kernelSize dd ?							; zmienna odpowiadajaca rozmiarowi macierzy Gaussa
	boundary dd ?							; zmienna odpowiadajaca wartosci dzielenia calkowitego rozmiaru macierzy Gaussa 

.code			
;
; gauss(unsigned char* inputArray, unsigned char * outputArray, float* kernel, int32_t width, int32_t startHeight, int32_t stopHeight, int32_t size, float sum);
;
; Parametry wejsciowe procedury:
;	- unsigned char* inputArray - wskaznik na jednowymiarowa tablice pikseli obrazu wejsciowego, w formacie B, G, R, B, G, R, ... Miesci sie w rejestrze RCX
;	- unsigned char * outputArray - wskaznik na jednowymiarowa tablice obrazu wyjsciowego do ktorej zapisany zostanie przetworzony obraz. Miesci sie w rejestrze RDX
;	- float* kernel - wskaznik na jednowymiarowa tablice wartosci zmiennoprzecinkowych odpowiadajacych wartosciom macierzy Gaussa. Miesci sie w rejestrze R8
;	- int32_t width - szerokosc przetwarzanego obrazu. Miesci sie w rejetrze R9
;	- int32_t startHeight - poczatkowa wysokosc od ktorej rozpoczyna sie przetwarzanie obrazu. Odkladana jest na stosie
;	- int32_t stopHeight - wyoskosc na ktorej przestaje byc przetwarzac obraz. Odkladana jest na stosie
;	- int32_t size - rozmiar macierzy Gaussa (np. wartosc 3 oznacza macierz 3x3, wartosc 5 oznacza macierz 5x5 itd.). Odkladana jest na stosie
;	- float sum - suma wartosci wszystkich elementow macierzy Gaussa. Odkladana jest na stosie
;
; Parametry wyjsciowe procedury:
;	- procedura modyfikuje tablice outputArray i nie zwraca zadnej wartosci
;	
; U¿ywane rejestry: RAX, RBX, RCX, RDX, R8, R9, R10, R11, R12, R13, R14, R15
;
gauss proc					

	push rbp								; odlkadam na stos wartosc rejestru rbp
	mov rbp, rsp							; wczytuje do rejestru rbp wartosc wskaznika stosu (rsp) 

	mov eax, DWORD PTR [rbp + 48]			; pobieram do eax 5 parametr funkcji (w odrozneniu od 4 pierwszych ten znajduje sie na stosie a nie w rejstrach)
	mov startHeight, eax					; przypisuje wartosc pobranego parametru (poczatkowej wysokosci) do zmiennej startHeight w celu lepszej czytelnosci

	mov eax, DWORD PTR [rbp + 56]			; pobieram do eax 6 parametr funkcji 
	mov stopHeight, eax						; przypisuje wartosc pobranego parametru (wyoskosci na ktorej przestaje przetwarzac obraz) do zmiennej stopHeight w celu lepszej czytelnosci

	mov eax, DWORD PTR [rbp + 64]			; pobieram do eax 7 parametr funkcji
	mov kernelSize, eax						; przypisuje wartosc pobranego parametru (rozmiaru macierzy Gaussa) do zmiennej kernelSize w celu lepszej czytelnosci
	mov boundary, eax						; przypisuje ta sama wartosc do zmiennej boundary
	shr	boundary, 1							; po to, aby przesunac ja bitowo w prawo w celu podzielenia jej na pol, uzyskujac tym samym prawidlowa wartosc

	movss xmm3, REAL4 PTR [rbp + 72]		; pobieram do najmlodszej czesci rejestru xmm3 8 parametr funkcji (bede z niego korzystal pozniej przy dzieleniu 
											; np. sumB/sum w ramach operacji wektorowych. Rejestr xmm3 wyglada w tym momencie nastepujaco: | sum | 0 | 0 | 0 |
	pshufd xmm3, xmm3, 11000000b			; za pomoca przetasowania umieszczam wartosc sumy na 3 pierwszych pozycjach rejestru xmm3
											; rejestr xmm3 wyglada w tym momencie nastepujaco: | sum | sum | sum | 0 |

	mov r10d, startHeight					; wczytuje do r10d startHeight. Rejestr bedzie sluzyl jako zewnetrzny iterator "i" glownej petli for 
	add r10d, boundary						; dodaje do iteratora boundary zgodnie z algorytmem   for(i = startHeight + boundary; ... ; ...)

	mov r11d, stopHeight					; wczytuje do r11d stopHeight. Rejestr bedzie sluzyl jako warunek koncowy glownej zewnetrznej petli for
	add r11d, boundary						; dodaje do warunku konca boundary zgodnie z algorytmem   for(... ; i < stopHeight + boundary; ...)

outer_main_loop:							; etykieta zewnetrznej glownej petli for
	
	mov r12d, boundary						; wczytuje do r12d boundary. Rejestr bedzie sluzyl jako wewnetrzny iterator "j" glownej petli for    for (int j = boundary; ... ; ...)

	mov r13d, r9d							; wczytuje do r13d szerokosc bitmapy (width, umieszczony w r9d zgodnie z calling convention x64). Rejestr bedzie sluzyl jako warunek koncowy glownej wewnetrznej petli for
	add r13d, boundary						; dodaje do warunku konca boundary zgodnie z algorytmem   for(... ; j < width + boundary; ...)

inner_main_loop:							; etykieta wewnetrznej glownej petli for
	
	xorps xmm2, xmm2						; zeruje rejestr xmm2 w ktorym przechowywane sa wartosci sumB, sumG, sumR, tak aby dla kazdego piksela suma RGB liczona byla poprawnie

	mov r14d, boundary						; wczytuje do r14d boundary. Rejestr bedzie sluzyl jako zewnetrzny iterator "k" "macierzowej" petli for 
	neg r14d								; neguje wartosc rejestru tak jak przewiduje algorytm   for(k = -boundary; ... ; ...) 

outer_kernel_loop:							; etykieta zewnetrznej "macierzowej" petli for
	
	mov r15d, boundary						; wczytuje do r15d boundary. Rejestr bedzie sluzyl jako wewnetrzny iterator "l" "macierzowej" petli for
	neg r15d								; neguje wartosc rejestru tak jak przewiduje algorytm   for(l = -boundary; ... ; ...) 

inner_kernel_loop:							; etykieta wewnetrznej "macierzowej" petli for
	
											; iteratory petli: i = r10d | j = r12d | k = r14d | l = r15d
	xor rax, rax							; zeruje rejestr rax. Zaczynam wyliczanie inputIndex na podstawie wzoru:   inputIndex = 3 * ((i + k) * (width + size - 1) + (j + l))
	xor rbx, rbx							; zeruje takze rejestr rbx. Oba posluza do obliczen, a gotowy inputIndex zostanie w rax
	mov eax, r10d							; wczytuje "i" z rejestru r10d do rejestru eax
	add eax, r14d							; dodaje do niego "k" wartosc z r14d. W rezultacie w eax znajduje sie wartosc wyrazenia (i + k)  
	mov ebx, r9d							; wczytuje width z r9d do rejestru ebx 
	add ebx, kernelSize						; dodaje do niego kernelSize
	dec ebx									; odejmuje od rejestru 1 w rezultacie otrzymujac w ebx wartosc wyrazenia (width + size - 1)
	imul eax, ebx							; mnoze rejestru eax oraz ebx otrzymujac w eax wartosc wyrazenia ((i + k) * (width + size - 1))
	xor rbx, rbx							; zeruje rejestr rbx
	mov ebx, r12d							; wczytuje "j" z r12d do ebx
	add ebx, r15d							; dodaje do niego "l" z r15d, otrzymujac w ebx (j + l)
	add eax, ebx							; dodaje do siebie wartosci z eax oraz ebx. W eax znajduje sie aktualnie ((i + k) * (width + size - 1) + (j + l)) 
	imul eax, 3								; mnoze zawartosc rejestru eax przez 3 otrzymujac gotowy inputIndex w rejestrze eax

											; zaczynam wczytywanie do rejestru xmm0 wartosci B G R pikseli z wejsciowego obrazu
	movzx ebx, BYTE PTR [rcx + rax]			; wczytuje do ebx wartosc spod inputArray[inputIndex], czyli wartosci skladowej B piksela. W rcx znajduje sie wskaznik
											; na poczatek wejsciowej tablicy, natomiast w eax wyliczony przed chwila odpowiedni indeks. 
	cvtsi2ss xmm0, ebx						; zamieniam wczytana wartosc skladowej B piksela na liczbê zmiennopozycyjn¹ pojedynczej precyzji i zapisuje w rejestrze xmm0: | B | 0 | 0 | 0 |
	pshufd xmm0, xmm0, 11000111b			; za pomoca przetasowania umieszczam wartosc B na 3 pozycji w xmm0: | 0 | 0 | B | 0 |

	movzx ebx, BYTE PTR [rcx + rax + 1]		; wczytuje do ebx wartosc spod inputArray[inputIndex + 1], czyli wartosci skladowej G piksela.
	cvtsi2ss xmm0, ebx						; zamieniam wczytana wartosc skladowej G piksela na liczbê zmiennopozycyjn¹ pojedynczej precyzji i zapisuje w rejestrze xmm0: | G | 0 | B | 0 |
	pshufd xmm0, xmm0, 11100011b			; za pomoca przetasowania umieszczam wartosc G na 2 pozycji w xmm0: | 0 | G | B | 0 |

	movzx ebx, BYTE PTR [rcx + rax + 2]		; wczytuje do ebx wartosc spod inputArray[inputIndex + 2], czyli wartosci skladowej R piksela
	cvtsi2ss xmm0, ebx						; zamieniam wczytana wartosc skladowej R piksela na liczbê zmiennopozycyjn¹ pojedynczej precyzji i zapisuje w rejestrze xmm0: | R | G | B | 0 |
											; wczytane skladowe kolorow piksela znajduja sie na poprawnych pozycjach w xmm0, dlatego przetasowanie nie jest juz konieczne
 
	xor ebx, ebx							; zeruje rejestr ebx. Bede teraz wyliczal odpowiednia wartosc indeksu z macierzy Gaussa zgodnie ze wzorem:   kernelIndex = (k + boundary) * size + l + boundary;
	xor eax, eax							; zeruje rejestr eax
	mov eax, r14d							; wczytuje "k" z r14d do rejestru eax 
	add eax, boundary						; dodaje do niego wartosc boundary. W eax znajduje sie teraz wartosc wyrazenia (k + boundary)
	imul eax, kernelSize					; mnoze uzyskana wartosc przez rozmiar macierzy Gaussa. Stan eax: (k + boundary) * size
	add eax, r15d							; dodaje do rejestru eax "l" znajdujace sie w r15d. Stan eax: (k + boundary) * size + l
	add eax, boundary						; dodaje boundary zgodnie ze wzorem. W tym momencie w eax znajduje sie wyliczony kernelIndex
	imul eax, 4								; mnoze uzyskany kernelIndex przez 4, poniewaz potrzebuje odwolac sie do tablicy zmiennych typu float

	movss xmm1, REAL4 PTR [r8 + rax]		; wczytuje do najmlodszej czesci rejestru xmm1 wartosc spod wyliczonego indeksu: kernel[(k + boundary) * size + l + boundary]
											; rejestr xmm1 wyglada w tym momencie nastepujaco: | kernel[...] | 0 | 0 | 0 |
	pshufd xmm1, xmm1, 11000000b			; za pomoca przetasowania umieszczam wartosc na 3 pierwszych pozycjach rejestru xmm1: | kernel[...] | kernel[...] | kernel[...] | 0 |
	
	mulps xmm0, xmm1						; mnoze rejestry xmm0 oraz xmm1, tak aby w xmm0 otrzymac wartosci skladowe kolorow przemnozone przez odpowiedni mnoznik z macierzy Gaussa
											; xmm0:   |      R      |      G      |       B     | 0 |
											; xmm1: * | kernel[...] | kernel[...] | kernel[...] | 0 |
	addps xmm2, xmm0						; dodaje otrzymane wartosci skladowych do xmm2 zawierajace sume skladowych dla calej iteracji petli "macierzowej"
											; xmm2:    |        sumR      |        sumG      |         sumB     | 0 |
											; xmm0: +  | R *  kernel[...] | R *  kernel[...] | R *  kernel[...] | 0 |

											; w tym miejscu nastepuje koniec ciala wewnetrznej "macierzowej" petli for 
	inc r15d								; inkrementuje o 1 wartosc wewnetrznego iteratora "l" "macierzowej" petli for    for(... ; ... ; l++)
	cmp r15d, boundary						; porownuje wartosc iteratora "l" z warunkiem koncowym boundary    for(...; l <= boundary; ...)
	jle inner_kernel_loop					; jesli wartosc iteratora jest mniejsza lub rowna, skacze z powrotem do etykiety wewnetrznej "macierzowej" petli for

											; w tym miejscu nastepuje koniec ciala zewnetrznej "macierzowej" petli for
	inc r14d								; inkrementuje o 1 wartosc zewnetrznego iteratora "k" "macierzowej" petli for    for(... ; ... ; k++)
	cmp r14d, boundary						; porownuje wartosc iteratora "k" z warunkiem koncowym boundary    for(...; k <= boundary; ...)
	jle outer_kernel_loop					; jesli wartosc iteratora jest mniejsza lub rowna, skacze z powrotem do etykiety zewnetrznej "macierzowej" petli for

	xor rax, rax							; zeruje rejestr rax. Zaczynam wyliczanie inputIndex na podstawie wzoru: outputIndex = 3 * ((i - boundary) * width + (j - boundary))
	xor rbx, rbx							; zeruje rejestr rbx
	mov eax, r10d							; wczytuje do rejestru eax wartosc iteratora "i" z rejestru r10d
	sub	eax, boundary						; odejmuje od eax wartosc boundary. W eax znajduje sie aktualnie (i - boundary)
	imul eax, r9d							; mnoze otrzymana wartosc przez szerokosc obrazu znajdujaca sie w r9d. Stan eax: (i - boundary) * width
	mov ebx, r12d							; wczytuje do ebx wartosc iteratora "j" z rejstru r12d. 
	sub ebx, boundary						; odejmuje od niego wartosc boundary. W ebx znajduje sie aktualnie (j - boundary)
	add	eax, ebx							; dodaje do siebie wartosci rejestrow eax i ebx, otrzymujac w eax wartosc wyrazenia: (i - boundary) * width + (j - boundary)
	imul eax, 3								; mnoze zawartosc rejestru eax przez 3 otrzymujac gotowy outputIndex

	divps xmm2, xmm3						; dziele rejestru xmm2 oraz xmm3 w celu znormalizowania wartosci. Wynik dzielenia znajduje sie w xmm2
											; xmm2:   | sumR | sumG | sumB | 0 |
											; xmm3: / | sum  | sum  | sum  | 0 |

											; stan rejestru xmm2: | R | G | B | 0 |
	cvtss2si ebx, xmm2						; zamieniam zmiennopozycyjn¹ pojedynczej precyzji z najmlodszej pozycji z xmm2 (wartosc R) na liczbe calkowita i zapisuje w rejestrze ebx
	mov BYTE PTR [rdx + rax + 2], bl 		; zapisuje pod odpowiednie miejsce w tablicy wynikowej wczytana przed chwila do ebx wartosc skladowej R (outputArray[outputIndex + 2])
	pshufd xmm2, xmm2, 11100101b			; za pomoca przetasowania kopiuje wartosc G na najmlodsza pozycje xmm2: | G | G | B | 0 |

	cvtss2si ebx, xmm2						; zamieniam zmiennopozycyjn¹ pojedynczej precyzji z najmlodszej pozycji z xmm2 (wartosc G) na liczbe calkowita i zapisuje w rejestrze ebx
	mov BYTE PTR [rdx + rax + 1], bl 		; zapisuje pod odpowiednie miejsce w tablicy wynikowej wczytana przed chwila do ebx wartosc skladowej G (outputArray[outputIndex + 1])
	pshufd xmm2, xmm2, 11100110b			; za pomoca przetasowania kopiuje wartosc B na najmlodsza pozycje xmm2: | B | G | B | 0 |

	cvtss2si ebx, xmm2						; zamieniam zmiennopozycyjn¹ pojedynczej precyzji z najmlodszej pozycji z xmm2 (wartosc B) na liczbe calkowita i zapisuje w rejestrze ebx
	mov BYTE PTR [rdx + rax], bl			; zapisuje pod odpowiednie miejsce w tablicy wynikowej wczytana przed chwila do ebx wartosc skladowej B (outputArray[outputIndex])
	
											; w tym miejscu nastepuje koniec ciala wewnetrznej glownej petli for
	inc r12d								; inkrementuje o 1 wartosc wewnetrznego iteratora "j" glownej petli for    for(... ; ... ; j++)
	cmp r12d, r13d							; porownuje wartosc iteratora "j" z warunkiem koncowym zapisanym w r13d    for(...; j < width + boundary; ...)
	jl inner_main_loop						; jesli wartosc iteratora jest mniejsza, skacze z powrotem do etykiety wewnetrznej glownej petli for

											; w tym miejscu nastepuje koniec ciala zewnetrznej glownej petli for 
	inc r10d								; inkrementuje o 1 wartosc zewnetrznego iteratora "i" glownej petli for    for(... ; ... ; i++)
	cmp r10d, r11d							; porownuje wartosc iteratora "i" z warunkiem koncowym zapisanym w r11d    for(...; i < stopHeight + boundary; ...)
	jl outer_main_loop						; jesli wartosc iteratora jest mniejsza, skacze z powrotem do etykiety zewnetrznej glownej petli for

	mov rsp, rbp							; wczytuje do wskaznika stosu wartosc rbp
	pop rbp									; sciagam rbp ze stosu
	ret										; powrot z procedury
gauss endp					
end							