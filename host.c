#include "host.h"

typedef union param_t{
	int pdInt;
	void *pdPtr;
	char *pdChrPtr;
} param;


/*action will be in r0, and argv in r1*/
int host_call(enum HOST_SYSCALL action, void *argv)
{
    /* For Thumb-2 code use the BKPT instruction instead of SWI.
* Refer to:
* http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0471c/Bgbjhiea.html
* http://en.wikipedia.org/wiki/ARM_Cortex-M#Cortex-M4 */
    int result;
    __asm__(
      "bkpt 0xAB\n"
      "nop\n"
      "bx lr\n"
      :"=r" (result) ::
    );
    return result;
}

int host_system(char *cmd){
	fio_printf(1, "\r\n%s \r\n", cmd);
	return host_call(SYS_OPEN, (param []){{.pdChrPtr=cmd}, {.pdInt=strlen(cmd)}});
}

int host_savefile(char *cmd, char *data){
	int fd;
	fd = host_call(SYS_OPEN, (param []){{.pdChrPtr=cmd}, {.pdInt=WRITE}, {.pdInt=strlen(cmd)}});

	if( fd>0 ){
		return host_call(SYS_WRITE, (param []){{.pdInt=fd}, {.pdChrPtr=data}, {.pdInt=strlen(data)}});
	}
	else if( fd==-1 ){
		return -1;
	}
	
}
