#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"
#include "idt_asm.h"
#include "terminal.h"
#include "keyboard.h"
#include "rtc.h"
#include "file_system.h"
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
 /* void: idt test(void)
 * Inputs: void
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if IDT is loaded */
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
/* void: divide_by_zero_test(void)
 * Inputs: void
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if div by zero generates interrupt*/
int divide_by_zero_test(){
	TEST_HEADER;
	int a = 21;
	int b = 0;
	int division = a/b;
	division++; // Gets rid of 'unused variable' warning
	int result = FAIL;
	return result;

}

/* void: paging_test(void)
 * Inputs: void
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if pointers to start of vga mem and kern mem are actually valid*/
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
/* void: page_fault_test(void)
 * Inputs: void
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if page faults if pointer not valid*/
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

/* void: test_debug_error(void)
 * Inputs: void
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if debug error happens*/
int test_debug_error(){
	do {                                    \
		asm volatile ("call Debug"          \
		);                                  \
	} while (0);

	return FAIL;
}
/* void: bound_range_exception(void)
 * Inputs: void
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests bound_range handler */
int bound_range_exception(){
	do {                                    \
		asm volatile ("call Bound_Range_Exceeded"          \
		);                                  \
	} while (0);

	return FAIL;
}

// add more tests here

/* Checkpoint 2 tests */
// int terminal_test()
// {
// 	TEST_HEADER;
// 	// int i;
// 	char buf[128];
	
// 	int nbytes = terminal_read(0, buf, 128);
// 	nbytes = terminal_write(0, buf, 128);

// 	return PASS;
// }
int rtc_write_read_test(){
	TEST_HEADER;
	int i;
	int result;
	int rate = 1024;
	while(rate >= 2)
	{
		result = rtc_write(NULL, &rate, 4); // 100 is a meaningless arbitrary number.
		printf("test %d HZ\n", rate);
		if(rate > 64)
		{
			for(i = 0; i < 40; i++)
			{
			result = rtc_read(NULL,NULL,NULL);
			printf("1");
			}
			printf("\n");
		}
		else 
		{
			for(i = 0; i < 20; i++)
			{
			result = rtc_read(NULL,NULL,NULL);
			printf("1");
			}
			printf("\n");
		}
		
		rate = rate / 2;
	}
	result = rtc_open(NULL);
	result = rtc_close(NULL);
	
	return PASS;

}

int terminal_test()
{
	TEST_HEADER;
	int i;
	for(i = 0; i < 10; i++)
	{
		keyboard_buffer[i] = 'a';
		
	}
	keyboard_buffer_index = 10;

	printf("\n");
	printf("%d characters in keyboard buffer: ", 10);
	for(i = 0; i < 10; i++)
	{
		putc(keyboard_buffer[i]);
		
	}
	char buffa[128];
	printf("\n");
	printf("calling read: with %d nbytes", 100000);
	printf("\n");
	terminal_read(0, buffa, 100000);
	printf("calling write: with %d nbytes", 100000);
	printf("\n");
	printf("terminal buffer: ");
	terminal_write(0, buffa, 100000);
	printf("\n");
	keyboard_buffer_index = 0;
	return PASS;
}
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
	// TEST_OUTPUT("bounds exception test", bound_range_exception());
	TEST_OUTPUT("terminal test", terminal_test());
	// TEST_OUTPUT("rtc_write_read test", rtc_write_read_test());

	// launch your tests here
}
