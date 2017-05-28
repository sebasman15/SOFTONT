@
@********************************************************************************************************
@                                                uC/OS-II
@                                          The Real-Time Kernel
@
@
@                              (c) Copyright 2010@ Micrium, Inc.@ Weston, FL
@                    All rights reserved.  Protected by international copyright laws.
@
@                                           ARM Cortex-M4 Port
@
@ File      : OS_CPU_A.ASM
@ Version   : V2.92
@ By        : JJL
@             BAN
@
@ For       : ARMv7 Cortex-M4
@@Mode      : Thumb-2 ISA
@ Toolchain : GNU C Compiler
@
@********************************************************************************************************
@

@********************************************************************************************************
@                                          PUBLIC FUNCTIONS
@********************************************************************************************************

    .extern  OSRunning                                           @ External references
    .extern  OSPrioCur
    .extern  OSPrioHighRdy
    .extern  OSTCBCur
    .extern  OSTCBHighRdy
    .extern  OSIntExit
    .extern  OSTaskSwHook
    .extern  OS_CPU_ExceptStkBase

    .global  OS_CPU_SR_Save                                      @ Functions declared in this file
    .global  OS_CPU_SR_Restore
    .global  OSStartHighRdy
    .global  OSCtxSw
    .global  OSIntCtxSw
    .global  OS_CPU_PendSVHandler

@PAGE
@********************************************************************************************************
@                                               EQUATES
@********************************************************************************************************

.equ NVIC_INT_CTRL,   0xE000ED04                              @ Interrupt control state register.
.equ NVIC_SYSPRI14,   0xE000ED22                              @ System priority register (priority 14).
.equ NVIC_PENDSV_PRI, 0xFF                                    @ PendSV priority value (lowest).
.equ NVIC_PENDSVSET,  0x10000000                              @ Value to trigger PendSV exception.

@********************************************************************************************************
@                                     CODE GENERATION DIRECTIVES
@********************************************************************************************************

.text
.align 2
.thumb
.syntax unified

@********************************************************************************************************
@                                   CRITICAL SECTION METHOD 3 FUNCTIONS
@
@ Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
@              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
@              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
@              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
@              into the CPU's status register.
@
@ Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void)@
@                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr)@
@
@
@ Note(s)    : 1) These functions are used in general like this:
@
@                 void Task (void *p_arg)
@                 {
@                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
@                     OS_CPU_SR  cpu_sr;
@                 #endif
@
@                          :
@                          :
@                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
@                          :
@                          :
@                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
@                          :
@                          :
@                 }
@********************************************************************************************************
.thumb_func
OS_CPU_SR_Save:
	CPSID   I			@ Disable IRQ by setting PRIMASK
    BX      LR			@

.thumb_func
OS_CPU_SR_Restore:
	CPSIE	I			@ Enable IRQ by clearing PRIMASK
    BX      LR

@PAGE
@********************************************************************************************************
@                                         START MULTITASKING
@                                      void OSStartHighRdy(void)
@
@ Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
@              the first task to start.
@
@           2) OSStartHighRdy() MUST:
@              a) Setup PendSV exception priority to lowest;
@              b) Set initial PSP to 0, to tell context switcher this is first run;
@              c) Set the main stack to OS_CPU_ExceptStkBase
@              d) Set OSRunning to TRUE;
@              e) Trigger PendSV exception;
@              f) Enable interrupts (tasks will run with interrupts enabled).
@********************************************************************************************************

.thumb_func
OSStartHighRdy:
	CPSID   I

    LDR     R0, =NVIC_SYSPRI14                                  @ Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                                              @ Set the PSP to 0 for initial context switch call
    MSR     PSP, R0

    LDR     R0, =OS_CPU_ExceptStkBase                           @ Initialize the MSP to the OS_CPU_ExceptStkBase
    LDR     R1, [R0]
    MSR     MSP, R1

    LDR     R0, =OSRunning                                      @ OSRunning = TRUE
    MOVS    R1, #1
    STRB    R1, [R0]

    LDR     R0, =NVIC_INT_CTRL                                  @ Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                                                   @ Enable interrupts at processor level

OSStartHang:
    B       OSStartHang                                         @ Should never get here


@PAGE
@********************************************************************************************************
@                       PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
@
@ Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function
@              triggers the PendSV exception which is where the real work is done.
@********************************************************************************************************

.thumb_func
OSCtxSw:
	LDR     R0, =NVIC_INT_CTRL                                  @ Trigger the PendSV exception (causes context switch)
	LDR     R1, =NVIC_PENDSVSET
	STR     R1, [R0]
	BX      LR

@PAGE
@********************************************************************************************************
@                   PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
@
@ Note(s) : 1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
@              the result of an interrupt.  This function simply triggers a PendSV exception which will
@              be handled when there are no more interrupts active and interrupts are enabled.
@********************************************************************************************************

.thumb_func
OSIntCtxSw:
	LDR     R0, =NVIC_INT_CTRL		@ Trigger the PendSV exception (causes context switch)
	LDR     R1, =NVIC_PENDSVSET
	STR     R1, [R0]
	BX      LR

.end
