#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"
#include "idt_asm.h"
#include "terminal.h"
#include "keyboard.h"
#include "rtc.h"
#include "syscall.h"
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

/* void: test_debug_error()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if debug error happens*/
int test_debug_error(){
	do {                                    \
		asm volatile ("call Debug"          \
		);                                  \
	} while (0);

	return FAIL;
}
/* void: bound_range_exception()
 * Inputs: None
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

/* void: rtc_write_read_test()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if rtc_read and rtc_write run without
 * 			 causing any errors. */
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

/* void: terminal_test()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if terminal_read and terminal_write correctly
 * 			 read from a predefined buffer and then displays that 
 * 		     message on screen. */
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

/* void: file_system_test()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if a normal sized file can be read and
 * 			 and have it's content displayed. Tests the 
 * 		     read_dentry_by_name and read_data functions. */
int file_system_test()
{
	int i;
	int result = 0;
	dentry_t entry;
	unsigned char bluffa[187];
	TEST_HEADER;
	// char fname[4] = {'g','r','e','p'};
	char fname[10] = {'f','r','a','m','e','0','.','t','x','t'};
	result += read_dentry_by_name((uint8_t*)fname,&entry);
	printf("\n");
	for(i = 0; i < 10; i++)
	{
		printf("%c", entry.file_name[i]);
	}
	result += read_data(entry.node_num,0,(uint8_t*)bluffa,187);
	for(i = 0; i < 187; i++)
	{
		printf("%c",bluffa[i]);
	}
	printf("\n %d \n", result);
	return PASS;
}

/* void: file_system_test_large()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: tests if a file spanning multiple memory blocks 
 * 		 	 can be read and have it's content displayed. */
int file_system_test_large()
{
	int i;
	int result = 0;
	dentry_t entry;
	unsigned char bluffa[6149];
	TEST_HEADER;
	char fname[4] = {'g','r','e','p'};
	// char fname[10] = {'f','r','a','m','e','0','.','t','x','t'};
	result += read_dentry_by_name((uint8_t*)fname,&entry);
	printf("\n");
	for(i = 0; i < 4; i++)
	{
		printf("%c", entry.file_name[i]);
	}
	result += read_data(entry.node_num,0,(uint8_t*)bluffa,6149);
	for(i = 0; i < 6149; i++)
	{
		printf("%c",bluffa[i]);
	}
	printf("\n %d \n", result);
	return PASS;
}

/* void: list_file_systems()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: Lists all the files in fsdir, along with 
 * 			 their file types and file sizes. Checks
 * 		     the read_dentry_by_index function. */
int list_file_systems()
{
	TEST_HEADER;
	int i,j;
	int name_length;
	dentry_t files;
	printf("\n");
	for(i = 0; i < 17; i++)
	{
		read_dentry_by_index(i, &files);
		name_length = strlen((int8_t*)files.file_name);
		for(j = 0; j < name_length && j < 32;j++)
		{
			printf("%c", files.file_name[j]);

		}
		printf("   file type:%d", files.file_type);
		printf("  size: %d", (inodes_ptr + files.node_num)->length);
		printf("\n");

	}
	return PASS;
	
}

/* void: file_dne()
 * Inputs: None
 * Return Value: FAIL if FAIL, SUCCESS if worked
 * Function: Check if file_open works for files that
 * 			 don't exist. */
int file_dne()
{
	TEST_HEADER;
	char fname[6] = {'e','c','e','3','9','1'};
	int result = file_open((uint8_t*)fname);

	if (result == -1){
		printf("\n");
		printf("file not found");
		printf("\n");
		return PASS;
	}

	return FAIL;
	
}
/* Checkpoint 3 tests */

int Basic_syscall_test()
{
	TEST_HEADER;

	//void (*addr)() = idt[0x80].offset_31_16 * 0x10000 + idt[0x80].offset_15_00;
	//addr();

	//uint8_t Filename[14] = {'r','u','n',' ','e','c','e','3','9','1','.','t','x','t'};
	//execute(Filename);
	// int i = 0;
	char Filename2[4] = {'c','a','t','\0'};
	// printf("\n");
    // for(i = 0; i < strlen(Filename2); i ++){
    //     printf("%c", Filename2[i]);
    // }
    // printf("\n");

	execute((uint8_t*)Filename2);
	return PASS;
}

int Get_args_test(){
	TEST_HEADER;

	char Filename2[13] = {'c','a','t',' ','-','r',' ','-','-','A','A','\0'};
	execute((uint8_t*)Filename2);

	char arg_buf[13] = {'\0'};
	getargs((uint8_t*)arg_buf, 13);

	int i;
	printf("\n->");
	for(i = 0; i < 8; i++){
		putc(arg_buf[i]);
		if(arg_buf[i] != Filename2[3+i]){
			return FAIL;
		}
	}
	printf("<-\n");

	return PASS;
}
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
	// TEST_OUTPUT("terminal test", terminal_test());
	// TEST_OUTPUT("rtc_write_read test", rtc_write_read_test());
	// TEST_OUTPUT("fs test", file_system_test());
	// TEST_OUTPUT("large fs test", file_system_test_large());
	// TEST_OUTPUT("list files", list_file_systems());
	// TEST_OUTPUT("test file doesnt exist", file_dne());
	// TEST_OUTPUT("Basic syscalls", Basic_syscall_test());
	TEST_OUTPUT("GetArgs", Get_args_test());
	// launch your tests here
}
