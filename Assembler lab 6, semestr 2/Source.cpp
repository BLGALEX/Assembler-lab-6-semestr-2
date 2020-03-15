#include<iostream>

int main()
{
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
	float x;
	float input_x = 1.767574;
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
				
				cmp ecx, 0
				je LOOP3EXIT
				mov eax, ecx    //����������� ������ 4 �������� ebx-1 ���� ������� ������� �2 � ecx-1 ��� ������ ������� �1
				mov dx, 32
				mul dx
				movups xmm1, [M1+eax-32]
				movups xmm2, [buffer]
				mulps xmm1, xmm2

				mov eax, ecx //����������� ���������  4 �������� ebx-1 ���� ������� ������� �2 � ecx-1 ��� ������ ������� �1
				mov dx, 32
				mul dx
				movups xmm2, [M1+eax-16]
				movups xmm3, [buffer+16]
				mulps xmm2, xmm3
				//���������� ��� �������� �� xmm1 � xmm2
				addps xmm1, xmm2
				//�������� �������� ���� float �� xmm1 � ������ �� ����� � ������� M3[edx-1][ecx-1] 
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
				xor edi,edi //������ � ������ ������� ��������� ��������� ������ �� �������
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
		movups xmm2, [M1+64]
		addps xmm1, xmm2
		movups [M1], xmm1
		movups xmm1, [M1+16]
		movups xmm2, [M1+80]
		addps xmm1, xmm2
		movups [M1+16], xmm1
		
	ret

	matrixnumermul: //������� ��������� ����� input_x �� ������� �3
		
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


	return 0;
}