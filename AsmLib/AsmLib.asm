;								Filtr Gaussa v0.4
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
;	- 25.10.2020 - wersja 0.3.1 - naprawienie dzia³ania funkcji filtru gaussa w C++
;	- 30.10.2020 - wersja 0.4 - dodanie pomiaru czasu dla funkcji dll
;
;gauss(unsigned char* inputArray, unsigned char * outputArray, float** kernel, int32_t width, int32_t startHeight, int32_t stopHeight, char size, float sum);
;
.data
	startHeight dd ?
	stopHeight dd ?
	kernelSize dd ?
	sum	real4 ?
	sumG real4 ?
	sumB real4 ?
	sumR real4 ?
	boundary dd ?
	;inputIndex dd ?
	outputIndex dd ?

.code						
gauss proc					

	push rbp
	mov rbp, rsp

	; get function parameters
	mov eax, DWORD PTR [rbp + 48]			; start height
	mov startHeight, eax

	mov eax, DWORD PTR [rbp + 56]			; stop 
	mov stopHeight, eax

	mov eax, DWORD PTR [rbp + 64]			; kernel size
	mov kernelSize, eax
	mov boundary, eax
	shr	boundary, 1							; wyliczanie boundary

	mov eax, REAL4 PTR [rbp + 72]			; sum
	mov sum, eax

	mov r10d, startHeight			; zewnetrzny for 
	add r10d, boundary				; for(i = startHeight + boundary

	mov r11d, stopHeight
	add r11d, boundary				; warunek konca zewnetrznej petli

outer_main_loop:
	
	mov r12d, boundary				; j = boundary

	mov r13d, r9d					; width + boundary
	add r13d, boundary

inner_main_loop:
	
	mov sumB, 0
	mov sumG, 0
	mov sumR, 0

	mov r14d, boundary			; zewnetrzny for kernela k
	neg r14d


outer_kernel_loop:
	
	mov r15d, boundary
	neg r15d

inner_kernel_loop:
	
	; wyliczanie inputIndex  i = r10d | j = r12d | k = r14d | l = r15d
	xor rax, rax
	xor rbx, rbx
	mov eax, r10d
	add eax, r14d		; koniec i + k w eax
	mov ebx, r9d
	add ebx, kernelSize
	dec ebx				; koniec width + size - 1 w ebx
	imul eax, ebx		; koniec mno¿enia (i + k) * (width + size - 1) w eax
	xor rbx, rbx
	mov ebx, r12d
	add ebx, r15d		; koniec (j + l) w ebx
	add eax, ebx		; koniec dodawania 
	imul eax, 3			; gotowy inputIndex
	;mov inputIndex, eax

	; wczytanie do xmm0 B G R z wejsciowego obrazu
	movzx ebx, BYTE PTR [rcx + rax]		; inputArray[inputIndex]
	cvtsi2ss xmm0, ebx
	pshufd xmm0, xmm0, 11000111b

	movzx ebx, BYTE PTR [rcx + rax + 1]		; inputArray[inputIndex + 1]
	cvtsi2ss xmm0, ebx
	pshufd xmm0, xmm0, 11100011b

	movzx ebx, BYTE PTR [rcx + rax + 2]		; inputArray[inputIndex + 2]
	cvtsi2ss xmm0, ebx

	; wczytanie od xmm1 odpowiedniego mnoznika z kernela 
	xor ebx, ebx
	xor eax, eax
	mov eax, r14d
	add eax, boundary
	imul eax, kernelSize
	add eax, r15d
	add eax, boundary

	;movzx ebx, BYTE PTR [r8 + rax]		; kernel[k + boundary][l + boundary]
	;cvtsi2ss xmm1, ebx


	

	;---------- koniec ciala inner_kernel_loop ------------------;
	inc r15d
	
	cmp r15d, boundary
	jle inner_kernel_loop


	;---------- koniec ciala outer_kernel_loop ------------------;
	inc r14d

	cmp r14d, boundary
	jle outer_kernel_loop

	; wyliczanie outputIndex i = r10d | j = r12d | k = r14d | l = r15d
	xor rax, rax
	xor rbx, rbx
	mov eax, r10d
	sub	eax, boundary		; koniec i - boundary w eax
	imul eax, r9d
	mov ebx, r12d
	sub ebx, boundary
	add	eax, ebx
	imul eax, 3			; gotowy outputIndex

	; dzielenie sumB/sum i  zapsanie pod outputIndex

	; dzielenie sumG/sum i  zapsanie pod outputIndex

	; dzielenie sumR/sum i  zapsanie pod outputIndex
	
	;---------- koniec ciala inner_main_loop ------------------;
	inc r12d

	cmp r12d, r13d
	jl inner_main_loop

	;---------- koniec ciala outer_main_loop ------------------;
	inc r10d						; inkrementacja i

	cmp r10d, r11d
	jl outer_main_loop


	mov rsp, rbp
	pop rbp
	ret						
gauss endp					
end							