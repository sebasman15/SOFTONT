/*
Authors		: 	W Pielage & E Helmond J.F. van der Bent
CreatDate	:		13-9-2013

    buzzer.c:
          Buzzer-driver for ARM-board v5

    pin-info:
           PC9 - Buzzer

07-07-2014 aanpassing van compatabiliteit van buzzer_put
*/

void BUZZER_init(void);
void BUZZER_set(int counter);
void BUZZER_put(unsigned int time);

