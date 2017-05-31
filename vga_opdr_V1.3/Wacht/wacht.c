// Rekening gehouden met de doelgroep en de hoeveelheid scherm verwisselingen:
// Een afwijking van 0.7 seconden op 1 minuut.
//

void wacht_ms (int tijd_ms)
{
	int teller_tijd;
	int max_clock=80000000; // PLL
	int clock_tijd =6;		// klokslagen voor ++
	int i=0;
	int clock_to_ms = max_clock/clock_tijd/1000;	//klok naar tijd in ms
	teller_tijd = clock_to_ms*tijd_ms;

	for (i=0;i<teller_tijd;i++)
	{}
}
