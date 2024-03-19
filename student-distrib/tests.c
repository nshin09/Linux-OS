#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"
#include "idt_asm.h"
#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d  ", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

int divide_by_zero_test(){
	TEST_HEADER;
	int a = 21;
	int b = 0;
	int division = a/b;
	division++; // Gets rid of 'unused variable' warning
	int result = FAIL;
	return result;

}


int paging_test(){
	TEST_HEADER;
	int result = PASS;
	int *pointer = (int*)(VGA_MEM_START); 
	if(*pointer == NULL)
	{
		printf("\nvga_mem\n");
		result = FAIL;
	}
	pointer = (int*)(KERNEL_MEMORY);
	if((*pointer == NULL))
	{
		printf("kern_mem\n");
		result = FAIL;
	}
	printf(" p val: %d\n", *pointer);
	return result;
}

int page_fault_test()
{
	TEST_HEADER;
	int *pointer = (int*)(0x80000001);
	if(!(*pointer == NULL))
	{
		printf("out of range\n");
	}
	return FAIL; // should never get here
}


int test_debug_error(){
	do {                                    \
		asm volatile ("call Debug"          \
		);                                  \
	} while (0);

	return FAIL;
}

int bound_range_exception(){
	do {                                    \
		asm volatile ("call Bound_Range_Exceeded"          \
		);                                  \
	} while (0);

	return FAIL;
}

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("divide_by_zero_test", divide_by_zero_test());
	// TEST_OUTPUT("paging test", paging_test());
	// TEST_OUTPUT("paging range test", page_fault_test());
	// TEST_OUTPUT("debug exception test", test_debug_error());
	TEST_OUTPUT("bounds exception test", bound_range_exception());


	// launch your tests here
}
