#include<iostream>

int main()
{
	float M1[8][8] = {
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7,
		 0, 1,2,3,4,5,6,7 };
	float M2[8][8] = {
	      0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7,
		  0, 1,2,3,4,5,6,7 };
	float M3[8][8];
	float buffer[8];
	char a = 0b00000001;
	int x;
	__asm
	{
	jmp start

	matrixmul:
		mov ecx, 8

		LOOP1:

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
				
				mov eax, ecx    //перемножаем первые 4 элемента ebx-1 того столбца матрицы М2 и ecx-1 той строки матрицы М1
				dec eax
				mov dx, 32
				mul dx
				movups xmm1, [M1+eax]
				movups xmm2, [buffer]
				mulps xmm1, xmm2

				mov eax, ecx //перемножаем последние  4 элемента ebx-1 того столбца матрицы М2 и ecx-1 той строки матрицы М1
				dec eax
				mov dx, 32
				mul dx
				movups xmm2, [M1+eax+16]
				movups xmm3, [buffer+16]
				mulps xmm2, xmm3
				//складываем все элементы из xmm1 и xmm2
				addps xmm1, xmm2
				//дописать сложение всех float из xmm1 и запись их в элемент M3[edx-1][ecx-1] 
			loop LOOP3
			
		loop LOOP1

	ret

	matrixadd:	

		movups xmm1, [M1]
		movups xmm2, [M1+64]
		addps xmm1, xmm2
		movups [M1], xmm1
		movups xmm1, [M1+16]
		movups xmm2, [M1+80]
		addps xmm1, xmm2
		movups [M1+16], xmm1
		
	ret
	
	start:

		call matrixadd
		call matrixmul


	}


	return 0;
}