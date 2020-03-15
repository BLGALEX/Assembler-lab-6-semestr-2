#include<iostream>
#include <intrin.h>

int main()
{
	setlocale(LC_ALL, "Rus");
	float M1[8][8] = {
		 1, 0,0,0,0,0,0,0,
		 0, 1,0,0,0,0,0,0,
		 0, 0,1,0,0,0,0,0,
		 0, 0,0,1,0,0,0,0,
		 0, 0,0,0,1,0,0,0,
		 0, 0,0,0,0,1,0,0,
		 0, 0,0,0,0,0,1,0,
		 0, 0,0,0,0,0,0,1 };
	float M2[8][8] = {
		  1, 0,0,0,0,0,0,0,
		 0, 2,0,0,0,0,0,0,
		 0, 0,3,0,0,0,0,0,
		 0, 0,0,4,0,0,0,0,
		 0, 0,0,0,5,0,0,0,
		 0, 0,0,0,0,6,0,0,
		 0, 0,0,0,0,0,7,0,
		 0, 0,0,0,0,0,0,8 };
	float M3[8][8];
	float buffer[8];
	float buffer_n[8];
	float x;
	float input_x = 1.76757;
	float M1_n[8][8], M2_n[8][8], M3_n[8][8];
	for(int i=0; i< 8;i++)
		for (int j = 0; j < 8; j++)
		{
			M1_n[i][j] = M1[i][j];
			M2_n[i][j] = M2[i][j];
		}

	unsigned __int64 t1, t2;
	t1 = __rdtsc();
	__asm
	{
	jmp start

	matrixmul:
		mov ecx, 8
		LOOP1:

			cmp ecx, 0
			je LOOP1EXIT
			mov ebx, ecx
			mov ecx, 8

			LOOP2:	//Данный цикл копирует столбец матрицы M2 по индексу ebx-1 в массив buffer

				cld
				lea edi, [buffer+ecx*4-4]
				lea esi, [M2+ebx*4-4]
				mov eax, ecx
				dec eax
				mov dx, 32
				mul dx
				add esi, eax
				movsd
			loop LOOP2

			mov ecx, 8
			LOOP3:   //считаем элементы ebx-1 того столбца новой матрицы, в каждом цикле считается один элемент начиная с поледнего
				
				cmp ecx, 0
				je LOOP3EXIT
				mov eax, ecx    //перемножаем первые 4 элемента ebx-1 того столбца матрицы М2 и ecx-1 той строки матрицы М1
				mov dx, 32
				mul dx
				movups xmm1, [M1+eax-32]
				movups xmm2, [buffer]
				mulps xmm1, xmm2

				mov eax, ecx //перемножаем последние  4 элемента ebx-1 того столбца матрицы М2 и ecx-1 той строки матрицы М1
				mov dx, 32
				mul dx
				movups xmm2, [M1+eax-16]
				movups xmm3, [buffer+16]
				mulps xmm2, xmm3
				//складываем все элементы из xmm1 и xmm2
				addps xmm1, xmm2
				//дописать сложение всех float из xmm1 и запись их суммы в элемент M3[edx-1][ecx-1] 
				movss [x], xmm1
				fld [x]
				shufps xmm1,xmm1, 11100101b
				movss [x], xmm1
				fld [x]
			    shufps xmm1, xmm1, 11101010b
				movss [x], xmm1
				fld [x]
				shufps xmm1, xmm1, 11111111b
				movss [x], xmm1
				fld [x]
				fadd
				fadd
				fadd
				xor edi,edi //Кладем в нужный элемент результат умножения строки на столбец
				mov eax, ebx
				dec eax
				mov dx, 4
				mul dx
				add edi, eax
				mov eax, ecx
				dec eax
				mov dx, 32
				mul dx
				add edi, eax
				fstp [M3+edi]
				dec ecx
				jmp LOOP3
			LOOP3EXIT:

			mov ecx, ebx
			dec ecx
			jmp LOOP1
		LOOP1EXIT:

	ret

	matrixadd:	

		movups xmm1, [M1]
		movups xmm2, [M2+64]
		addps xmm1, xmm2
		movups [M1], xmm1
		movups xmm1, [M1+16]
		movups xmm2, [M2+80]
		addps xmm1, xmm2
		movups [M1+16], xmm1
		
	ret

	matrixnumermul: //функция умножения числа input_x на матрицу М3
		
		mov ecx, 16
		LOOP4:
			mov eax, ecx
			mov dx, 16
			mul dx
			movups xmm1, [M3+eax-16]
			movss xmm2, [input_x]
		    shufps xmm2, xmm2, 00000000b
			mulps xmm1, xmm2
			movups [M3+eax-16], xmm1
			loop LOOP4

	ret
	
	start:

		call matrixadd
		call matrixmul
		call matrixnumermul

	}
	t2 = __rdtsc();
	std::cout << "Для вычислений с использованием SSE понадобилось " << t2 - t1 << " Тактов процессора или " << (float)(t2 - t1) / 2400000000 << " миллисекунд" << std::endl;
	t1 = __rdtsc();
	__asm
	{
	jmp start_n

	matrixmul_n:
		
		mov ecx, 8
		LOOP7:

			cmp ecx, 0
			je LOOP7EXIT
			mov ebx, ecx
			mov ecx, 8

			LOOP8:	//Данный цикл копирует столбец матрицы M2 по индексу ebx-1 в массив buffer

				cld
				lea edi, [buffer+ecx*4-4]
				lea esi, [M2_n+ebx*4-4]
				mov eax, ecx
				dec eax
				mov dx, 32
				mul dx
				add esi, eax
				movsd
			loop LOOP8

			mov ecx, 8
			LOOP9:   //считаем элементы ebx-1 того столбца новой матрицы, в каждом цикле считается один элемент начиная с поледнего
				
				cmp ecx, 0
				je LOOP9EXIT
				mov eax, ecx    //перемножаем первые 4 элемента ebx-1 того столбца матрицы М2 и ecx-1 той строки матрицы М1
				mov dx, 32
				mul dx

				fld [M1_n+eax-32]
				fld [buffer]
				fmul
				fld [M1_n+eax-28]
				fld [buffer+4]
				fmul
				fadd
				fld [M1_n+eax-24]
				fld [buffer+8]
				fmul
				fadd
				fld [M1_n+eax-20]
				fld [buffer+12]
				fmul
				fadd
				fld [M1_n+eax-16]
				fld [buffer+16]
				fmul
				fadd
				fld [M1_n+eax-12]
				fld [buffer+20]
				fmul
				fadd
				fld [M1_n+eax-8]
				fld [buffer+24]
				fmul
				fadd
				fld [M1_n+eax-4]
				fld [buffer+28]
				fmul
				fadd
				fst [x]

				xor edi,edi //Кладем в нужный элемент результат умножения строки на столбец
				mov eax, ebx
				dec eax
				mov dx, 4
				mul dx
				add edi, eax
				mov eax, ecx
				dec eax
				mov dx, 32
				mul dx
				add edi, eax
				fstp [M3_n+edi]
				dec ecx
				jmp LOOP9
			LOOP9EXIT:

			mov ecx, ebx
			dec ecx
			jmp LOOP7
		LOOP7EXIT:
		
	ret

	matrixadd_n:
		
		lea esi, [M1_n]
		lea edi, [buffer]
		mov ecx, 8
		repe movsw
		mov ecx,8
		LOOP5:
			
			fld [buffer+ecx*4-4]
			fld [M2_n+ecx*4+60]
			fadd 
			fstp [M1_n+ecx*4-4]
			loop LOOP5

			
	ret

	matrixnumermul_n:

		mov ecx, 64
		LOOP6:
		
			fld [M3_n+ecx*4-4]
			fld [input_x]
			fmul
			fstp [M3_n+ecx*4-4]
			loop LOOP6
	ret

	start_n:
		
		call matrixadd_n
		call matrixmul_n
		call matrixnumermul_n
	}
	t2 = __rdtsc();
	std::cout << "Для вычислений без использования SSE понадобилось " << t2 - t1 << " Тактов процессора или " << (float)(t2 - t1) / 2400000000 << " миллисекунд" << std::endl;
	x = M3_n[1][1];
	float y = M3[1][1];
	return 0;
}