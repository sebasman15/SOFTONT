/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*
*                           (c) Copyright 2009-2010; Micrium, Inc.; Weston, FL
*                    All rights reserved.  Protected by international copyright laws.
*
*                                           ARM Cortex-M4 Port
*
* File      : OS_CPU_C.C
* Version   : V2.92
* By        : JJL
*             BAN
*
* LICENSING TERMS:
* ---------------
*           uC/OS-II is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use uC/OS-II in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-II for its use in your
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-II.  The fact that the source is provided does NOT mean that you can use
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
*
* For       : ARMv7 Cortex-M4
* Mode      : Thumb-2 ISA
* Toolchain : GNU C Compiler
*********************************************************************************************************
*/

#define   OS_CPU_GLOBALS

/*$PAGE*/
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"
//#include <os_cpu.h>
//#include <os_core.c>
//#include  <lib_def.h>




#define  DEF_INT_CPU_NBR_BITS                           (4     * 8u)
#define  DEF_BIT_FIELD(bit_field, bit_shift)                                 ((((bit_field) >= DEF_INT_CPU_NBR_BITS) ? (4294967295u)     \
                                                                                                                     : ((1u << (bit_field)) - 1uL)) \
                                                                                                                            << (bit_shift))
#define  DEF_BIT_MASK(bit_mask, bit_shift)                                     ((bit_mask) << (bit_shift))
OS_STK SP_HighReady;

/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/

#if OS_TMR_EN > 0u
static  INT16U  OSTmrCtr;
#endif

/*
*********************************************************************************************************
*                                          SYS TICK DEFINES
*********************************************************************************************************
*/

#define  OS_CPU_CM4_NVIC_ST_CTRL    (*((volatile INT32U *)0xE000E010uL)) /* SysTick Ctrl & Status Reg. */
#define  OS_CPU_CM4_NVIC_ST_RELOAD  (*((volatile INT32U *)0xE000E014uL)) /* SysTick Reload  Value Reg. */
#define  OS_CPU_CM4_NVIC_ST_CURRENT (*((volatile INT32U *)0xE000E018uL)) /* SysTick Current Value Reg. */
#define  OS_CPU_CM4_NVIC_ST_CAL     (*((volatile INT32U *)0xE000E01CuL)) /* SysTick Cal     Value Reg. */
#define  OS_CPU_CM4_NVIC_SHPRI1     (*((volatile INT32U *)0xE000ED18uL)) /* System Handlers  4 to  7 Prio.       */
#define  OS_CPU_CM4_NVIC_SHPRI2     (*((volatile INT32U *)0xE000ED1CuL)) /* System Handlers  8 to 11 Prio.       */
#define  OS_CPU_CM4_NVIC_SHPRI3     (*((volatile INT32U *)0xE000ED20uL)) /* System Handlers 12 to 15 Prio.       */


#define  OS_CPU_CM4_NVIC_ST_CTRL_COUNT                    0x00010000uL   /* Count flag.                */
#define  OS_CPU_CM4_NVIC_ST_CTRL_CLK_SRC                  0x00000004uL   /* Clock Source.              */
#define  OS_CPU_CM4_NVIC_ST_CTRL_INTEN                    0x00000002uL   /* Interrupt enable.          */
#define  OS_CPU_CM4_NVIC_ST_CTRL_ENABLE                   0x00000001uL   /* Counter mode.              */
#define  OS_CPU_CM4_NVIC_PRIO_MIN                               0xFFu    /* Min handler prio.          */

//#define NVIC_INT_CTRL   0xE000ED04                              // Interrupt control state register.
//#define NVIC_SYSPRI14   0xE000ED22                              // System priority register (priority 14).
//#define NVIC_PENDSV_PRI 0xFF                                    // PendSV priority value (lowest).
//#define NVIC_PENDSVSET  0x10000000                              // Value to trigger PendSV exception.

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookBegin (void)
{
    INT32U   size;
    OS_STK  *pstk;

                                                                /* Clear exception stack for stack checking.            */
    pstk = &OS_CPU_ExceptStk[0];
    size = OS_CPU_EXCEPT_STK_SIZE;
    while (size > 0u) {
        size--;
       *pstk++ = (OS_STK)0;
    }

                                                                /* Align the ISR stack to 8-bytes                       */
    OS_CPU_ExceptStkBase = (OS_STK *)&OS_CPU_ExceptStk[OS_CPU_EXCEPT_STK_SIZE];
    OS_CPU_ExceptStkBase = (OS_STK *)((OS_STK)(OS_CPU_ExceptStkBase) & 0xFFFFFFF8);

#if OS_TMR_EN > 0u
    OSTmrCtr = 0u;
#endif
}
#endif


/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookEnd (void)
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskCreateHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskCreateHook(ptcb);
#else
    (void)ptcb;                                  /* Prevent compiler warning                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskDelHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskDelHook(ptcb);
#else
    (void)ptcb;                                  /* Prevent compiler warning                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskIdleHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskIdleHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskReturnHook (OS_TCB  *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskReturnHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif


/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskStatHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskStatHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              p_arg         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when your task starts executing.
*              2) All tasks run in Thread mode, using process stack.
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit (	OS_TASK_PTR task,
						void *p_arg,
						OS_STK *ptos,
						INT16U opt)
{
    OS_STK *p_stk;


    (void)opt;                                                  /* 'opt' is not used, prevent warning                   */

    p_stk      = ptos + 1u;                                     /* Load stack pointer                                   */
                                                                /* Align the stack to 8-bytes.                          */
    p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
                                                                /* Registers stacked as if auto-saved on exception      */
    *(--p_stk) = (OS_STK)0x01000000uL;                          /* xPSR                                                 */
    *(--p_stk) = (OS_STK)task;                                  /* Entry Point                                          */
    *(--p_stk) = (OS_STK)OS_TaskReturn;                         /* R14 (LR)                                             */
    *(--p_stk) = (OS_STK)0x12121212uL;                          /* R12                                                  */
    *(--p_stk) = (OS_STK)0x03030303uL;                          /* R3                                                   */
    *(--p_stk) = (OS_STK)0x02020202uL;                          /* R2                                                   */
    *(--p_stk) = (OS_STK)0x01010101uL;                          /* R1                                                   */
    *(--p_stk) = (OS_STK)p_arg;                                 /* R0 : argument                                        */

                                                                /* Remaining registers saved on process stack           */
    *(--p_stk) = (OS_STK)0x11111111uL;                          /* R11                                                  */
    *(--p_stk) = (OS_STK)0x10101010uL;                          /* R10                                                  */
    *(--p_stk) = (OS_STK)0x09090909uL;                          /* R9                                                   */
    *(--p_stk) = (OS_STK)0x08080808uL;                          /* R8                                                   */
    *(--p_stk) = (OS_STK)0x07070707uL;                          /* R7                                                   */
    *(--p_stk) = (OS_STK)0x06060606uL;                          /* R6                                                   */
    *(--p_stk) = (OS_STK)0x05050505uL;                          /* R5                                                   */
    *(--p_stk) = (OS_STK)0x04040404uL;                          /* R4                                                   */

    return (p_stk);
}


/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0u) && (OS_TASK_SW_HOOK_EN > 0u)
void  OSTaskSwHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskSwHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                           OS_TCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTCBInitHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TCBInitHook(ptcb);
#else
    (void)ptcb;                                  /* Prevent compiler warning                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0u) && (OS_TIME_TICK_HOOK_EN > 0u)
void  OSTimeTickHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TimeTickHook();
#endif

#if OS_TMR_EN > 0u
    OSTmrCtr++;
    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
        OSTmrCtr = 0;
        OSTmrSignal();
    }
#endif
}
#endif





//@********************************************************************************************************
//@                                       HANDLE PendSV EXCEPTION
//@                                   void OS_CPU_PendSVHandler(void)
//@
//@ Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
//@              context switches with Cortex-M4.  This is because the Cortex-M4 auto-saves half of the
//@              processor context on any exception, and restores same on return from exception.  So only
//@              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
//@              this way means that context saving and restoring is identical whether it is initiated from
//@              a thread or occurs due to an interrupt or exception.
//@
//@           2) Pseudo-code is:
//@              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
//@              b) Save remaining regs r4-r11 on process stack;
//@              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP;
//@              d) Call OSTaskSwHook();
//@              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
//@              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
//@              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
//@              h) Restore R4-R11 from new process stack;
//@              i) Perform exception return which will restore remaining context.
//@
//@           3) On entry into PendSV handler:
//@              a) The following have been saved on the process stack (by processor):
//@                 xPSR, PC, LR, R12, R0-R3
//@              b) Processor mode is switched to Handler mode (from Thread mode)
//@              c) Stack is Main stack (switched from Process stack)
//@              d) OSTCBCur      points to the OS_TCB of the task to suspend
//@                 OSTCBHighRdy  points to the OS_TCB of the task to resume
//@
//@           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
//@              know that it will only be run when no other exception or interrupt is active, and
//@              therefore safe to assume that context being switched out was using the process stack (PSP).
//@********************************************************************************************************
void PendSV_Handler (void)
{
__asm(  "CPSID   I                                        			@ Prevent interruption during context switch\n"
		"															\n"
	    "MRS     R0, PSP                                       		@ PSP is process stack pointer\n"
	    "CBZ     R0, OS_CPU_PendSVHandler_nosave                 	@ Skip register save the first time\n"
		"															\n"
	    "SUBS    R0, R0, #0x20                                   	@ Save remaining regs r4-11 on process stack\n"
	    "STM     R0, {R4-R11}\n"
		"\n"
	    "LDR     R1, =OSTCBCur                                   	@ OSTCBCur->OSTCBStkPtr = SP;\n"
	    "LDR     R1, [R1]\n"
	    "STR     R0, [R1]                                        	@ R0 is SP of process being switched out\n"
		"                                                       	@ At this point, entire context of process has been saved\n"
	"OS_CPU_PendSVHandler_nosave:\n"
//	    "PUSH    {R14}                                          	@ Save LR exc_return value\n"
//	    "LDR     R0, =OSTaskSwHook                              	@ OSTaskSwHook();\n"
//	    "BLX     R0\n"
//	    "POP     {R14}\n"

	    "LDR     R0, =OSPrioCur                                 	@ OSPrioCur = OSPrioHighRdy;\n"
	    "LDR     R1, =OSPrioHighRdy\n"
	    "LDRB    R2, [R1]\n"
	    "STRB    R2, [R0]\n"

	    "LDR     R0, =OSTCBCur                                   	@ OSTCBCur  = OSTCBHighRdy;\n"
	    "LDR     R1, =OSTCBHighRdy\n"
	    "LDR     R2, [R1]\n"
	    "STR     R2, [R0]\n"
		"\n"
	    "LDR     R0, [R2]                                       	@ R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;\n"
	    "LDM     R0, {R4-R11}                                 		@ Restore r4-11 from new process stack\n"
	    "ADDS    R0, R0, #0x20\n"
	    "MSR     PSP, R0                                        	@ Load PSP with new process SP\n"
	    "ORR     LR, LR, #0x04\n"
	    "MOVS	 R14, #0xfffffffd				@ Wisse: Load LR with 0xFFFFFFFD to get a correct switch with PSP\n"
	    "CPSIE   I\n"
	    "BX      LR                                               	@ Exception return will restore remaining context\n");
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          SYS TICK HANDLER
*
* Description: Handle the system tick (SysTick) interrupt, which is used to generate the uC/OS-II tick
*              interrupt.
*
* Arguments  : None.
*
* Note(s)    : 1) This function MUST be placed on entry 15 of the Cortex-M4 vector table.
*********************************************************************************************************
*/

void  SysTick_Handler (void)
{
    OS_CPU_SR  cpu_sr;

    __asm(	"CPSID   I\n"
    		"PUSH    {R14}\n"
    		"@MRS     R0, PSP  \n"
    		"@SUBS    R0, R0, #0x20                                       @ Save remaining regs r4-11 on process stack\n"
    		"@STM     R0, {R4-R11}\n"

    		"@LDR     R1, =OSTCBCur                                       @ OSTCBCur->OSTCBStkPtr = SP;\n"
    		"@LDR     R1, [R1]\n"
    		"@STR     R0, [R1]                                            @ R0 is SP of process being switched out\n"
    		"                                                            @ At this point, entire context of process has been saved");

    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

    OSIntExit();

    __asm(	"@MOV	R14, #0xfffffffd				@ Wisse: Try to load LR with 0xFFFFFFFD to get a correct switch\n"
    		"POP     {R14}\n"

    		"@LDR     R1, =OSTCBCur\n"
    		"@LDR     R2, [R1]\n"

    		"@LDR     R0, [R2]                                            @ R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;\n"
    		"@LDM     R0, {R4-R11}                                        @ Restore r4-11 from new process stack\n"
    		"@ADDS    R0, R0, #0x20\n"
    		"@MSR     PSP, R0                                             @ Load PSP with new process SP\n"
    		"ORR     LR, LR, #0x04                                       @ Ensure exception return uses process stack\n"
    		"@MOV	R14, #0xfffffffd\n"
    		"CPSIE   I\n"
    		"@BX      LR                                                  @ Exception return will restore remaining context\n");
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         INITIALIZE SYS TICK
*
* Description: Initialize the SysTick.
*
* Arguments  : cnts         Number of SysTick counts between two OS tick interrupts.
*
* Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
*********************************************************************************************************
*/

void  OS_CPU_SysTickInit (INT32U  cnts)
{
    INT32U  prio;


    OS_CPU_CM4_NVIC_ST_RELOAD = cnts - 1u;

                                                            /* Set SysTick handler prio.                              */
    prio  = OS_CPU_CM4_NVIC_SHPRI3;
    prio &= DEF_BIT_FIELD(24, 0);
    prio |= DEF_BIT_MASK(OS_CPU_CFG_SYSTICK_PRIO, 24);

    OS_CPU_CM4_NVIC_SHPRI3 = prio;

                                                            /* Enable timer.                                          */
    OS_CPU_CM4_NVIC_ST_CTRL |= OS_CPU_CM4_NVIC_ST_CTRL_CLK_SRC |
                               OS_CPU_CM4_NVIC_ST_CTRL_ENABLE;
                                                            /* Enable timer interrupt.                                */
    OS_CPU_CM4_NVIC_ST_CTRL |= OS_CPU_CM4_NVIC_ST_CTRL_INTEN;
}
