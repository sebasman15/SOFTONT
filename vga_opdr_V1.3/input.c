//application lay for input user
//#include <stdio.h>
//#include <uart.h>
#include <includes.h>
// to do functies koppelen aan de input



//void welkomtekst()
//{
//
//
//}

typedef enum{figure,text,pixel,bitmap, katja};
typedef enum{lijn, vierkant, elips, driehoek};
typedef enum{omhoog,omlaag,links,rechts};
typedef enum{ja,nee};
typedef enum{font1, font2}; // to do font 1 en 2 veranderen in de gekozen fonts
typedef enum{bold, knipperen};
typedef enum{klein, groot};
typedef enum{schrijven, uitlezen};
typedef enum{pijl, smiley};

#define CR 13 // carriage return char
#define LF 10 // linefeed char

void keuze_menu()
{
	int choice_1;
	int choice_2;
	int start_x_pos;
	int start_y_pos;
	int richting;
	int lengte;
	int breedte;
	int vulling;
	int lijndikte;
	int kleur;
	int font;
	int stijl;
	int grootte;
    char tekst;
    char temp[8];
    temp[8]='\0';
    char *s;
    int i=0;

    *s = UART_get();
    if(*s != CR)
    {
    	temp[i]=*s;
    	i++;
    }
    else {


//    UART_gets(temp, TRUE);
    UART_puts(temp);
	//{
	UART_puts("\n\rwelkom bij de festival vga controller");
	UART_puts("\n\rtyp een comando in de onderstaande regel om iets op het scherm uit te voeren");
	UART_puts("\n\rals u de commando's niet weet typ dan -H om de commado lijst te zien");
    }

	//}
	//choice_1 = atoi(temp);

	switch (choice_1) {
		case	katja: // functie voor bitmap van katja schuurman
		case figure: UART_puts("\n\r\n\rwilt u een lijn,vierkant,elips of driehoek maken?");
		UART_puts("\n\r\n\rvoer uw keuze hier onder in");
		switch (choice_2)
		{
		case lijn: UART_puts("\n\ru heeft lijn gekozen, voer eerst het start x_coördinaat en daarna het start y_coördinaat in");
					start_x_pos = atoi(UART_gets(temp,TRUE));
					// to do hoe moet het wachten op invoer commando ook alweer
					start_y_pos = atoi(UART_gets(temp,TRUE));
					// to do hoe moet het wachten op invoer commando ook alweer
					UART_puts("\n\rx_coördinaat ="); UART_putint(start_x_pos); UART_puts("\n\r\n\r___"); UART_puts("\n\r\n\ry_coördinaat =");UART_putint(start_y_pos);
					UART_puts("\n\rrvoer gewenste richting in");
					richting = atoi(UART_gets(temp,TRUE));
					// to do hoe moet het wachten op invoer commando ook alweer
					switch (richting)
					{
						case omhoog:
							UART_puts("\n\r\n\rde gekozen richting is "); UART_puts(richting);
							UART_puts("\n\r\n\rvoer de gewenste lengte in");
							lengte = atoi(UART_gets(temp,TRUE));
							if(start_y_pos-lengte < 0)
								UART_puts("\n\r\n\rlijn even inkorten, en opnieuw invoeren");
							// to do terug naar invoer lengte
							else
							UART_puts("\n\r\n\rlengte is "); UART_putint(lengte);

							break;
						case omlaag:
							UART_puts("\n\r\n\rde gekozen richting is "); UART_puts(richting);
							UART_puts("\n\r\n\rvoer de gewenste lengte in");
							lengte = atoi(UART_gets(temp,TRUE));
							if(start_y_pos+lengte > 279)
								UART_puts("\n\r\n\rlijn even inkorten, en opnieuw invoeren");
							else
							UART_puts("\n\r\n\rlengte is "); UART_putint(lengte);
							// to do terug naar invoer lengte

						break;

						case rechts:
							UART_puts("\n\r\n\rde gekozen richting is "); UART_puts(richting);
							UART_puts("\n\r\n\rvoer de gewenste lengte in");
							lengte = atoi(UART_gets(temp,TRUE));
							if(start_x_pos+lengte > 339)
								UART_puts("\n\r\n\rlijn even inkorten, en opnieuw invoeren");
							// to do terug naar invoer lengte
							else
								UART_puts("\n\r\n\rlengte is "); UART_putint(lengte);

							break;
						case links:
							UART_puts("\n\r\n\rde gekozen richting is "); UART_puts( richting);
							UART_puts("\n\r\n\rvoer de gewenste lengte in");
							lengte = atoi(UART_gets(temp,TRUE));
							if(start_x_pos-lengte < 0)
								UART_puts("\n\r\n\rlijn even inkorten, en opnieuw invoeren");
							else
								UART_puts("\n\r\n\rlengte is "); UART_putint(lengte);

							break;
						default:UART_puts("\n\r\n\rdeze invoer is onjuist, kom op man hoe moeilijk is het om een richting in te typen");
							break;
					}

					UART_puts("\n\r\n\rvoer de gewenste lengte in");


			break;

		case vierkant: UART_puts("\n\r\n\ru heeft vierkant gekozen, voer eerst het start_x coördinaat en start y coördinaat in ");
		start_x_pos = atoi(UART_gets(temp,TRUE));
		// to do wacht op inpout commando
		start_y_pos = atoi(UART_gets(temp,TRUE));
		// to do wacht op inpout commando
		UART_puts("\n\r\n\rx coördinaat is");UART_putint(start_x_pos); UART_puts("\n\r\n\r__"); UART_puts("\n\r\n\ry coördinaat is "); UART_putint(start_y_pos);
		UART_puts("\n\r\n\rvoer nu eerst de gewenste lengte in en daarna de breedte");
		lengte = atoi(UART_gets(temp,TRUE)); // to do UART gets fixen
		breedte = atoi(UART_gets(temp,TRUE)); // to do UART_gets fixen
		UART_puts("\n\r\n\rlengte is");UART_putint(lengte); UART_puts("\n\r\n\r__");UART_puts("\n\r\n\rbreedte is ");UART_putint(breedte);
		UART_puts("\n\r\n\rwilt u het vierkant vullen? vul in ja of nee");
		vulling = atoi(UART_gets(temp,TRUE));
		// to do wacht op inpout commando
		if (vulling == ja)
		lijndikte = 1;
		UART_puts("\n\r\n\rkies een kleur");
		kleur = UART_gets(temp,TRUE);
		// to do wacht op inpout commando
		UART_puts("\n\r\n\ruw gekozen input is");
		UART_puts("\n\r\n\rstart_x =");UART_putint(start_x_pos); UART_puts("\n\r\n\rstart_y =" ); UART_putint(start_y_pos);
		UART_puts("\n\r\n\rlengte =");UART_putint(lengte); UART_puts("\n\r\n\rbreedte");UART_putint(breedte);
		UART_puts("\n\r\n\rvulling ="); UART_puts(vulling); UART_puts("\n\r\n\rlijndikte =" );UART_putint(lijndikte);
		UART_puts("\n\r\n\rkleur ="); UART_puts(kleur);

			break;

		case elips:
			UART_puts("\n\r\n\ru heeft elips gekozen, voer eerst het start_x coördinaat en start y coördinaat ");
					start_x_pos = atoi(UART_gets(temp,TRUE));
					// to do wacht op input commando
					start_y_pos = atoi(UART_gets(temp,TRUE));
					// to do wacht op input commando
					UART_puts("\n\rx coördinaat is"); UART_putint(start_x_pos); UART_puts("\n\r\n\r__"); UART_puts("\n\r\n\ry coördinaat is "); UART_putint(start_y_pos);
					UART_puts("\n\rvoer nu eerst de gewenste lengte in en daarna de breedte");
					lengte = atoi(UART_gets(temp,TRUE));
					breedte = atoi(UART_gets(temp,TRUE));
					UART_puts("\n\rlengte is"); UART_putint(lengte); UART_puts("\n\r\n\r__");UART_puts("\n\r\n\rbreedte is "); UART_putint(breedte );
					UART_puts("\n\rwilt u de elips vullen? vul in ja of nee");
					vulling = atoi(UART_gets(temp,TRUE));
					// to do wacht op inpout commando
					if (vulling == ja)
					lijndikte = 1;
					UART_puts("\n\rkies een kleur");
					kleur = UART_gets(temp,TRUE);
					// to do wacht op inpout commando
					UART_puts("\n\ruw gekozen input is");
					UART_puts("\n\rstart_x ="); UART_putint(start_x_pos); UART_puts("\n\r\n\rstart_y ="); UART_putint(start_y_pos );
					UART_puts("\n\rlengte =");UART_putint(lengte); UART_puts("\n\r\n\rbreedte" ); UART_putint(breedte);
					UART_puts("\n\rvulling =" ); UART_putint(vulling); UART_puts("\n\r\n\rlijndikte ="  ); UART_putint(lijndikte);
					UART_puts("\n\rkleur ="); UART_puts(kleur);


			break;

		case driehoek:
			UART_puts("\n\r\n\ru heeft driehoek gekozen, voer eerst het start_x coördinaat en start y coördinaat in ");
					start_x_pos = atoi(UART_gets(temp,TRUE));
					// to do wacht op inpout commando
					start_y_pos = atoi(UART_gets(temp,TRUE));
					// to do wacht op inpout commando
					UART_puts("\n\rx coördinaat is"); UART_putint(start_x_pos); UART_puts("\n\r__"); UART_puts("\n\ry coördinaat is " );UART_putint(start_y_pos);
					UART_puts("\n\rvoer nu eerst de gewenste lengte in en daarna de breedte");
					lengte = atoi(UART_gets(temp,TRUE));
					breedte = atoi(UART_gets(temp,TRUE));
					UART_puts("\n\rlengte is"); UART_putint(lengte); UART_puts("\n\r__");UART_puts("\n\rbreedte is " ); UART_putint(breedte);
					UART_puts("\n\rwilt u de driehoek vullen? vul in ja of nee");
					vulling = atoi(UART_gets(temp,TRUE));
					// to do wacht op inpout commando
					if (vulling == ja)
					lijndikte = 1;
					UART_puts("\n\rkies een kleur");
					kleur = UART_gets(temp,TRUE);
					// to do wacht op inpout commando
					UART_puts("\n\ruw gekozen input is");
					UART_puts("\n\rstart_x ="); UART_putint(start_x_pos); UART_puts("\n\rstart_y =" ); UART_putint(start_y_pos);
					UART_puts("\n\rlengte ="); UART_putint(lengte); UART_puts("\n\rbreedte");UART_putint(breedte );
					UART_puts("\n\rvulling ="); UART_puts(vulling); UART_puts("\n\rlijndikte ="); UART_putint(lijndikte );
					UART_puts("\n\rkleur ="); UART_puts( kleur);

			break;

		}

			break;
		case text: UART_puts("\n\rkies een van de onderstaande fonts");
					UART_puts("\n\rfont1,font2 "); // to do gekozen fots invullen
					UART_puts("\n\rvoer uw keuze hier onder in");
					font = atoi(UART_gets(temp,TRUE));
					UART_puts("\n\ruw gekozen font is"); UART_puts(font);
					UART_puts("\n\rwilt u de tekst bold maken of laten knipperen?");
					stijl=atoi(UART_gets(temp,TRUE));
					// to do wacht op input commando
					UART_puts("\n\ruw gekozen stijl is " ); UART_puts( stijl);
					UART_puts("\n\rwilt u de tekst klein of groot op het scherm uitvoeren");
					grootte = atoi(UART_gets(temp,TRUE));
					UART_puts("\n\rde gekozen grootte is " ); UART_puts(grootte);
					UART_puts("\n\rvoer nu de gewenste tekst in, maximaal aantal karakters = 40");
					tekst = UART_gets(temp,TRUE);
					UART_puts("\n\rde tekst die u wilt uitvoeren op het scherm is" ); UART_puts(tekst);
					UART_puts("\n\rgekozen font = " ); UART_puts(font);
					UART_puts("\n\rgekozen stijl = "); UART_puts(stijl);
					UART_puts("\n\rgekozen grootte = " ); UART_puts(grootte);


			break;
		case pixel: UART_puts("\n\rwilt u een pixel inlezen of uitlezen");
					UART_puts("\n\rvoer uw keuze hier onder in");
					choice_2 = atoi(UART_gets(temp,TRUE));
					UART_puts("\n\rvoer eerst het x coördinaat en vervolgens het y coördinaat");
					start_x_pos =atoi(UART_gets(temp,TRUE));
					// to do wacht commando voor input
					start_y_pos = atoi(UART_gets(temp,TRUE));
					UART_puts("\n\rx coördinaat = "); UART_putint(start_x_pos); UART_puts("\n\r y coördinaat = " ); UART_putint(start_y_pos);


			break;
		case bitmap: UART_puts("\n\rwilt u een smiley of een pijl op het scherm uitvoeren");
		   	   	   	 UART_puts("\n\rvoer uw keuze hier onder in");
		   	   	   	 choice_2 = atoi(UART_gets(temp,TRUE));
		   	   	   	 if (choice_2 == pijl)
		   	   	   	 {
		   	   	   		UART_puts("\n\rvoer een de richting van de pijl in");
		   	   	   		richting = atoi(UART_gets(temp,TRUE));
		   	   	   	 }

			break;

		default:
			UART_puts("\n\rvoer commando's hieronder in.");
			break;
	}


}
