
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

int strcmp(char *str1,char *str2)
{
	int i;
	for (i=0; i<strlen(str1); i++)
	{
		if (i==strlen(str2)) return 1;
		if (str1[i]>str2[i]) return 1;
		else if (str1[i]<str2[i]) return -1;
	}
	return 0;
}

void strlwr(char *str)
{
	int i;
	for (i=0; i<strlen(str); i++)
	{
		if ('A'<=str[i] && str[i]<='Z') str[i]=str[i]+'a'-'A';
	}
}

void addToQueue(PROCESS* p)
{
	p->state=kRUNNABLE;
	if (p->priority>=10)
	{
		firstQueue[firstLen]=p;
		firstLen++;
		p->ticks=2;
		p->whichQueue=1;
	}
	else
	{
		secondQueue[secondLen]=p;
		secondLen++;
		p->ticks=p->priority;
		p->whichQueue=2;
	}
}

/*======================================================================*
                            tinix_main
 *======================================================================*/
PUBLIC int tinix_main()
{
	//disp_str("-----\"tinix_main\" begins-----\n");
	init_clock();
	clearScreen();
	disp_color_str("\n",0xe);
	disp_color_str("-------------------------------------------------------------------------\n\n\n",0xe);	                                              
	disp_color_str("    		*         *              **                                         \n",0xe);
	disp_color_str("   	 	*         *              **                                       \n",0xe);
	disp_color_str("    		*         *                                                       \n",0xe);
	disp_color_str("    		*         *              **                                     \n",0xe);
	disp_color_str("    		***********              **                                      \n",0xe);
	disp_color_str("    		*         *              **                                  \n",0xe);
	disp_color_str("    		*         *              **                                          \n",0xe);
	disp_color_str("    		*         *              **                                         \n",0xe);
	disp_color_str("    		*         *              **                                          \n",0xe);

	milli_delay(16000);
	clearScreen(); 

	disp_color_str("-------------------------------------------------------------------------\n\n\n",0xe);
	disp_color_str("            *********        *           *       *         *             \n",0xe);
	disp_color_str("           *                 *           *        *       *               \n",0xe);
	disp_color_str("          *           	     *           *         *     *             \n",0xe);
	disp_color_str("         *                   *           *          *   *                 \n",0xe);
	disp_color_str("         *                   *           *           * *          \n",0xe);
	disp_color_str("         *         ****      *           *            *                \n",0xe);
	disp_color_str("         *            *      *           *            *                   \n",0xe);
	disp_color_str("          *           *       *         *             *                   \n",0xe);
	disp_color_str("           **********          *********              *               \n",0xe);
	milli_delay(6000);
	clearScreen();
	




















	disp_color_str("      ******************************************************* \n",0x2);
	disp_color_str("     **********************************************************\n",0x2);
	disp_color_str("     *                                                        *\n",0x2);
	disp_color_str("     *                                                        * \n",0x2);
	disp_color_str("     *                  ****     ****   *    *                *\n",0xe);
	disp_color_str("     *                  *   *     **    *    *                *\n",0xe);
	disp_color_str("     *                  ****      **    *    *                *\n",0xe);
	disp_color_str("     *                  *   *     **    *    *                *\n",0xe);
	disp_color_str("     *                  ****     ****    ****                 *\n",0xe);
	disp_color_str("     *                                                        * \n",0x2);
	disp_color_str("     *                by Wangchenyu   Chenzijian               * \n",0xe);
	disp_color_str("     **********************************************************\n",0x2);
	disp_color_str("      *******************************************************\n",0x2);


	TASK*		p_task;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	t_16		selector_ldt	= SELECTOR_LDT_FIRST;
	int		i;
	t_8		privilege;
	t_8		rpl;
	int		eflags;
	for(i=0;i<NR_TASKS+NR_PROCS;i++){
		if (i < NR_TASKS) {	/* \C8\CE\CE\F1 */
			p_task		= task_table + i;
			privilege	= PRIVILEGE_TASK;
			rpl		= RPL_TASK;
			eflags		= 0x1202;	/* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {			/* \D3û\A7\BD\F8\B3\CC */
			p_task		= user_proc_table + (i - NR_TASKS);
			privilege	= PRIVILEGE_USER;
			rpl		= RPL_USER;
			eflags		= 0x202;	/* IF=1, bit 2 is always 1 */
		}

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid	= i;			/* pid */

		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;	/* change the DPL */
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;/* change the DPL */
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p_proc->regs.eip	= (t_32)p_task->initial_eip;
		p_proc->regs.esp	= (t_32)p_task_stack;
		p_proc->regs.eflags	= eflags;

		p_proc->nr_tty		= 0;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	//\D0޸\C4\D5\E2\C0\EF\B5\C4\D3\C5\CFȼ\B6\BA\CDticks
	proc_table[0].priority = 15;
	proc_table[1].priority =  5;
	proc_table[2].priority =  5;
	proc_table[3].priority =  5;
	proc_table[4].priority =  7;
	proc_table[5].priority =  10;
	proc_table[6].priority =  10;
	proc_table[7].priority =  10;

	//\B6\D4\D3\C5\CFȶ\D3\C1г\F5ʼ\BB\AF
	firstLen=firstHead=secondLen=0;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		addToQueue(proc_table+i);
	}
	//ָ\B6\A8\BF\D8\D6\C6̨
	proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 1;
	proc_table[3].nr_tty = 1;
	proc_table[4].nr_tty = 1;
	proc_table[5].nr_tty = 1;
	proc_table[6].nr_tty = 2;
	proc_table[7].nr_tty = 3;

	k_reenter	= 0;
	ticks		= 0;

	p_proc_ready	= proc_table;

	init_clock();

	restart();

	while(1){}
}

void clearScreen()
{
	int i;
	disp_pos=0;
	for(i=0;i<80*25;i++)
	{
		disp_str(" ");
	}
	disp_pos=0;
}


void help()
{
	printf("           *////////////////////////////////////////////*/\n");
	printf("                   design by Wangchenyu  Chenzijian       \n");
	printf("           *////////////////////////////////////////////*/\n");
	printf("\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("      *////  help         --------  shwo the help menu     ////*\n");
	printf("      *////  clear        --------  clear screen           ////*\n");
	printf("      *////  F2           --------  show the process run   ////*\n");
	printf("      *////  F3           --------  Guess number           ////*\n");
	printf("      *////  F4           --------  Be joyful              ////*\n");
	printf("      *////  kill 2~5     --------  kill the process 2~5   ////*\n");
	printf("      *////  start 2~5    --------  start the process 2~5  ////*\n");
	printf("      *////  show         --------  show the process state ////*\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("\n");
}

void show()
{
	PROCESS* p;
	int i;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		p=&proc_table[i];
		printf("process%d:",p->pid);
		switch (p->state)
		{
		case kRUNNABLE:
			printf("    Runnable\n");
			break;
		case kRUNNING:
			printf("    Running\n");
			break;
		case kREADY:
			printf("    Ready\n");
			break;
		}
	}
}

void readOneStringAndOneNumber(char* command,char* str,int* number)
{
	int i;
	int j=0;
	for (i=0; i<strlen(command); i++)
	{
		if (command[i]!=' ') break;
	}
	for (; i<strlen(command); i++)
	{
		if (command[i]==' ') break;
		str[j]=command[i];
		j++;
	}
	for (; i<strlen(command); i++)
	{
		if (command[i]!=' ') break;
	}

	*number=0;
	for (; i<strlen(command) && '0'<=command[i] && command[i]<='9'; i++)
	{
		*number=*number*10+(int) command[i]-'0';
	}
}

void dealWithCommand(char* command)
{
	strlwr(command);
	if (strcmp(command,"clear")==0)
	{
		clearScreen();
		sys_clear(tty_table);
		return ;
	}
	if (strcmp(command,"help")==0)
	{
		help();
		return ;
	}
	if (strcmp(command,"show")==0)
	{
		show();
		return ;
	}

	char str[100];
	int number;
	readOneStringAndOneNumber(command,str,& number);
	if (strcmp(str,"kill")==0)
	{
		if (number<0 || number>NR_TASKS+NR_PROCS)
		{
			printf("No found this process!!");
		}
		else if (number==0 || number==6)
		{
			printf("You do not have sufficient privileges\n");
		}
		else if (2<=number && number <=5)
		{
			proc_table[number].state=kREADY;
			printf("kill process %d successful\n",number);
		}
		return ;
	}
	if (strcmp(str,"start")==0)
	{
		if (number<0 || number>NR_TASKS+NR_PROCS)
		{
			printf("No found this process!!");
		}
		else if (number==0 || number==6)
		{
			printf("You do not have sufficient privileges\n");
		}
		else if (2<=number && number <=5)
		{
			proc_table[number].state=kRUNNABLE;
			printf("start process %d successful\n",number);
		}
		return ;
	}
	printf("can not find this command\n");
}

/*======================================================================*
                               Terminal
 *======================================================================*/
void Terminal()
{
	TTY *p_tty=tty_table;
	p_tty->startScanf=0;
	while(1)
	{
		printf("DB=>");
		//printf("<Ticks:%x>", get_ticks());
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		dealWithCommand(p_tty->str);
	}
}


/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	int i = 0;
	while(1){
		printf("B");
		milli_delay(1000);
	}
}



/*======================================================================*
                               TestC
 *======================================================================*/
void TestC()
{
	int i = 0;
	while(1){
		printf("C");
		milli_delay(1000);
	}
}

void TestD()
{
	int i=0;
	while (1)
	{
		printf("D");
		milli_delay(1000);
	}
}

void TestE()
{
	int i=0;
	while (1)
	{
		printf("E");
		milli_delay(1000);
	}
}
/*======================================================================*
                MyFunction
*=======================================================================*/

char number[]="3576";
TTY *goBangGameTty=tty_table+2;

int checkParameter(char x[])    //\BC\EC\B2\E9\CD\E6\BC\D2\CA\E4\C8\EB\B5Ĳ\CE\CA\FD\CAǷ\F1\D5\FDȷ
{
    int k=0;

    for(int i=0;i<4;i++){
        if (x[i]==number[i]){
            k++;
        }
    }
    return k;
}

void goBangGameStart()
{
        while (1)
        {
            printf("please input your number:\n");
            openStartScanf(goBangGameTty);
            while (goBangGameTty->startScanf) ;
            //readOneNumber(&x);
            char *x;
            x=goBangGameTty->str;
            int k=checkParameter(x);
            printf("You have %d right number.\n",k);
            if ( k==4 ){
                printf("Success.\n");
                break;
            }
        }
}
int y=1;

void halalala()
{	
	y=get_ticks();
	printf("Are you boring? \nMay be you can have a try :  ");
	
	while(0){printf("%d",get_ticks()-y);}
	switch (get_ticks()-y+3)
		{
		case 1:
			printf("Music\n");
			break;
		case 2:
			printf("LOL\n");
			break;
		case 3:
			printf("DOTA\n");
			break;
		case 4:
			printf("QQ\n");
			break;
		case 5:
			printf("WeChat\n");
			break;
		case 6:
			printf("Shopping\n");
			break;
		
		}while(1);
		
	

}



