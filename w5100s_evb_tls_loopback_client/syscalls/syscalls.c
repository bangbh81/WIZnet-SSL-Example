/**************************************************************************//*****
 * @file     stdio.c
 * @brief    Implementation of newlib syscall
 ********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "w5100s_evb.h"
#include "stm32f10x_conf.h"

#undef errno
extern int errno;
extern int  _end;

/*This function is used for handle heap option*/
__attribute__ ((used))
caddr_t _sbrk ( int incr )
{
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL) {
        heap = (unsigned char *)&_end;
    }
    prev_heap = heap;

    heap += incr;

    return (caddr_t) prev_heap;
}

__attribute__ ((used))
int link(char *old, char *new)
{
    return -1;
}

__attribute__ ((used))
int _close(int file)
{
    return -1;
}

__attribute__ ((used))
int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

__attribute__ ((used))
int _isatty(int file)
{
    return 1;
}

__attribute__ ((used))
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

///*Low layer read(input) function*/
__attribute__ ((used))
int _read(int file, char *ptr, int len)
{

#if 0
     //user code example
     int i;
     (void)file;

     for(i = 0; i < len; i++)
     {
        // UART_GetChar is user's basic input function
        *ptr++ = UART_GetChar();
     }

#endif

    return len;
}


/*Low layer write(output) function*/
__attribute__ ((used))
int _write(int file, char *ptr, int len)
{
    //user code example
	int i;
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	for(i = 0 ; i < len ; i++)
	{
		USART_SendData(PRINTF_USART, (uint8_t) *ptr);
		ptr++;
		while (USART_GetFlagStatus(PRINTF_USART, USART_FLAG_TXE) == RESET){}
		USART_ClearFlag(PRINTF_USART,USART_FLAG_TXE);
	}
   return len;
}

__attribute__ ((used))
void abort(void)
{
    /* Abort called */
    while(1);
}

/* --------------------------------- End Of File ------------------------------ */
