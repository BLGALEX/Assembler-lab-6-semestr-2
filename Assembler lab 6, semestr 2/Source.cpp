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
	float buffer[4];
	__asm
	{
	jmp start

	matrixmul:





	ret

	matrixadd:	

		movups xmm1, [M1]
		movups xmm2, [M2]
		addps xmm1, xmm2
		movups [M1], xmm1
		movups xmm1, [M1+16]
		movups xmm2, [M2+16]
		addps xmm1, xmm2
		movups [M1+16], xmm1

	ret
	
	start:

		call matrixadd


	}


	return 0;
}