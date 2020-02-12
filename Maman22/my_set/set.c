#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "set.h"
#include "info.h"

/*bitwise operations:*/
/*  k/32 gets the 0-3rd cell in the set (0:0-31,1:32-63,2:64-95,3:96-127).
    1=0000000....1 (31 zeros, 1 one), 1<<k%32 = 0000....010....0000. The 1 is in the k%32 bit.
    Than, we either OR - to set the kth bit, NOT and AND - to clear the kth bit or AND to test whether the kth bit is ON.
*/
#define SetBit(S,k)     (S[(k/32)] |= (1 << (k%32))) /*Sets the Kth bit in S to 1*/
#define ClearBit(S,k)   (S[(k/32)] &= ~(1 << (k%32))) /*Sets the Kth bit in S to 0*/
#define TestBit(S,k)    (S[(k/32)] & (1 << (k%32))) /*Tests the Kth bit in S. returns 1 if the bit is ON (==1), 0 otherwise*/


extern set SETA;
extern set SETB;
extern set SETC;
extern set SETD;
extern set SETE;
extern set SETF;
extern set SETF;

/*puts the numers in @numbers in @SET*/
void read_set_f(set SET, int* numbers) {
	int i;
	if (SET == NULL || numbers == NULL)
		return;
	empty_set(SET);
	for (i = 0; i < MAX_LINE; i++) {
		if (numbers[i] == EOI)
			return;
		if (numbers[i] >= 0 && numbers[i] <= MAX_NUMBER)
			SetBit(SET, numbers[i]);
	}
	free(numbers);
}
/*prints the numbers in @SET*/
void print_set_f(set SET) {
	int i,count=0,prev=0;
	if (SET == NULL)
		return;
	for (i = 0; i <= MAX_NUMBER; i++) {
		if((count / MAX_DISPLAY) > prev){
			prev++;
			printf("\n");
		}
		if (TestBit(SET, i)) {
			count++;
			printf("%d ", i);
		}
	}
	if (count == 0)
		printf("Set is empty");
	printf("\n");
}
/*unions @SET1 and @SET2, stores resualt in @SET3*/
void union_set_f(set SET1,set SET2,set SET3) {
	int i;
	if (SET1 == NULL || SET2 == NULL || SET3 == NULL)
		return;
	for (i = 0; i < MAX_NUMBER; i++) {
		if (TestBit(SET1, i) || TestBit(SET2, i))
			SetBit(SET3, i);
		else
			ClearBit(SET3, i);
	}
}

/*intersects @SET1 and @SET2, stores resualt in @SET3*/
void intersect_set_f(set SET1, set SET2, set SET3) {
	int i;
	if (SET1 == NULL || SET2 == NULL || SET3 == NULL)
		return;
	for (i = 0; i < MAX_NUMBER; i++) {
		if (TestBit(SET1, i) && TestBit(SET2, i))
			SetBit(SET3, i);
		else
			ClearBit(SET3,i);
	}
}

/*subs @SET2 from @SET1, stores resualt in @SET3*/
void sub_set_f(set SET1, set SET2, set SET3) {
	int i;
	if (SET1 == NULL || SET2 == NULL || SET3 == NULL)
		return;
	for (i = 0; i < MAX_NUMBER; i++) {
		if (TestBit(SET1, i) && !TestBit(SET2, i))
			SetBit(SET3, i);
		else
			ClearBit(SET3, i);
	}
}

/*preforming symmetric diffrance from @SET1 and @SET2, stores resualt in @SET3*/
void symdiff_set_f(set SET1, set SET2, set SET3) {
	int i;
	if (SET1 == NULL || SET2 == NULL || SET3 == NULL)
		return;
	for (i = 0; i < MAX_NUMBER; i++) {
		if ((TestBit(SET1, i) || TestBit(SET2, i)) && !(TestBit(SET1, i) && TestBit(SET2, i)))
			SetBit(SET3, i);
		else
			ClearBit(SET3, i);
	}
}

/*empties @SET*/
void empty_set(set SET) {
	int i;
	if (SET == NULL)
		return;
	for (i = 0; i < MAX_NUMBER; i++) {
		ClearBit(SET, i);
	}
}
