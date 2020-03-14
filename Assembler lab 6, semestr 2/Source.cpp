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

			LOOP2:	//������ ���� �������� ������� ������� M2 �� ������� ebx-1 � ������ buffer

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
			LOOP3:   //������� �������� ebx-1 ���� ������� ����� �������, � ������ ����� ��������� ���� ������� ������� � ���������
				
				mov eax, ecx    //����������� ������ 4 �������� ebx-1 ���� ������� ������� �2 � ecx-1 ��� ������ ������� �1
				dec eax
				mov dx, 32
				mul dx
				movups xmm1, [M1+eax]
				movups xmm2, [buffer]
				mulps xmm1, xmm2

				mov eax, ecx //����������� ���������  4 �������� ebx-1 ���� ������� ������� �2 � ecx-1 ��� ������ ������� �1
				dec eax
				mov dx, 32
				mul dx
				movups xmm2, [M1+eax+16]
				movups xmm3, [buffer+16]
				mulps xmm2, xmm3
				//���������� ��� �������� �� xmm1 � xmm2
				addps xmm1, xmm2
				//�������� �������� ���� float �� xmm1 � ������ �� � ������� M3[edx-1][ecx-1] 
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