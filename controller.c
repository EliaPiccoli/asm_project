#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

/* Inserite eventuali extern modules qui */

/* ************************************* */

enum { MAXLINES = 400 };
enum { LIN_LEN = 15 };
enum { LOUT_LEN = 8 };

long long current_timestamp() {
    struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	/* te.tv_nsec nanoseconds divide by 1000 to get microseconds*/
	long long nanoseconds = tp.tv_sec*1000LL + tp.tv_nsec; // caculate nanoseconds
    return nanoseconds;
}

int main(int argc, char *argv[]) {
    int i = 0;
    char bufferin[MAXLINES*LIN_LEN+1] ;
    char line[1024];
    long long tic_c, toc_c, tic_asm, toc_asm;

    char bufferout_c[MAXLINES*LOUT_LEN+1] = "" ;
    char bufferout_asm[MAXLINES*LOUT_LEN+1] = "" ;

    FILE *inputFile = fopen(argv[1], "r");

    if(argc != 3)
    {
        fprintf(stderr, "Syntax ./test <input_file> <output_file>\n");
        exit(1);
    }

    if (inputFile == 0)
    {
        fprintf(stderr, "failed to open the input file. Syntax ./test <input_file> <output_file>\n");
        exit(1);
    }

    while (i < MAXLINES && fgets(line, sizeof(line), inputFile))
    {
        i = i + 1;
        strcat( bufferin, line) ;
    }

    bufferin[MAXLINES*LIN_LEN] = '\0' ;

    fclose(inputFile);

    /* ELABORAZIONE in C */
    tic_c = current_timestamp();

    /* è possibile inserire qui il codice per l'elaborazione in C (non richiesto per l'elaborato) */
    /* questo pezzo di codice è solo una base di partenza per fare dei check sui dati */
    /*
    int c = 0;
    char tmpout[LOUT_LEN];
    strcpy( tmpout, "000-00\n");
    while ( bufferin[c] != '\0') {
      printf( "%d\n", bufferin[c]);
      strcat( bufferout_asm, tmpout);
      c = c + LIN_LEN ;
    }
    */
	const int cons[10] = { 200, 30, 120, 100, 200, 180, 24, 40, 20, 40 };
	int k;
	int res_gen, res_wm, res_dw, load[10], ck = 0, cons_tot;
	char tmpout[LOUT_LEN];

	tmpout[0] = '0';
	tmpout[1] = '0';
	tmpout[2] = '0';

	for (k = 0; bufferin[k] != '\0'; k += LIN_LEN)
	{
		cons_tot = 0;
		res_gen = bufferin[k] - 48;
		res_dw = bufferin[k + 1] - 48;
		res_wm = bufferin[k + 2] - 48;
		for (i = 3; i < 13; i++)
			load[i - 3] = bufferin[k + 1 + i] - 48;

		if (res_gen == 1)
		{
			if (tmpout[0] == '0')
			{
				tmpout[0] = '1';
				tmpout[2] = '1';
				tmpout[1] = '1';
				for (i = 0; i < 10; i++)
				{
					cons_tot = cons_tot + (load[i] * cons[i]);
				}
				if (cons_tot <= 150)
				{
					strcpy((tmpout + 4), "F1");
					ck = 0;
				}
				else if (cons_tot <= 300)
				{
					strcpy((tmpout + 4), "F2");
					ck = 0;
				}
				else if (cons_tot <= 450)
				{
					strcpy((tmpout + 4), "F3");
					ck = 0;
				}
				else
				{
					ck++;
					strcpy((tmpout + 4), "OL");
				}
			}
			else
			{
				if (res_wm == 1)
				{
					tmpout[2] = '1';
				}
				if (res_dw == 1)
				{
					tmpout[1] = '1';
				}
				for (i = 0; i < 10; i++)
				{
					cons_tot = cons_tot + (load[i] * cons[i]);
				}
				if (tmpout[2] == '0' && load[5] == 1)
				{
					cons_tot = cons_tot - 180;
				}
				if (tmpout[1] == '0' && load[4] == 1)
				{
					cons_tot = cons_tot - 200;
				}
				if (cons_tot <= 150)
				{
					strcpy((tmpout + 4), "F1");
					ck = 0;
				}
				else if (cons_tot <= 300)
				{
					strcpy((tmpout + 4), "F2");
					ck = 0;
				}
				else if (cons_tot <= 450)
				{
					strcpy((tmpout + 4), "F3");
					ck = 0;
				}
				else
				{
					ck++;
					strcpy((tmpout + 4), "OL");
				}
			}
		}
		else
		{
			if (tmpout[0] == '1')
			{
				if (res_wm == 1)
				{
					tmpout[2] = '1';
				}
				if (res_dw == 1)
				{
					tmpout[1] = '1';
				}
				for (i = 0; i < 10; i++)
				{
					cons_tot = cons_tot + (load[i] * cons[i]);
				}
				if (tmpout[2] == '0' && load[5] == 1)
				{
					cons_tot = cons_tot - 180;
				}
				if (tmpout[1] == '0' && load[4] == 1)
				{
					cons_tot = cons_tot - 200;
				}
				if (cons_tot <= 150)
				{
					strcpy((tmpout + 4), "F1");
					ck = 0;
				}
				else if (cons_tot <= 300)
				{
					strcpy((tmpout + 4), "F2");
					ck = 0;
				}
				else if (cons_tot <= 450)
				{
					strcpy((tmpout + 4), "F3");
					ck = 0;
				}
				else
				{
					ck++;
					strcpy((tmpout + 4), "OL");
				}
			}
			else
			{
				strcpy((tmpout + 4), "00");
			}
		}

		if (ck == 4)
		{
			tmpout[1] = '0';
		}
		else if (ck == 5)
		{
			tmpout[1] = '0';
			tmpout[2] = '0';
		}
		else if (ck == 6)
		{
			tmpout[0] = '0';
			tmpout[1] = '0';
			tmpout[2] = '0';
			strcpy((tmpout + 4), "00");
			ck = 0;
		}

		tmpout[3] = '-';
		tmpout[6] = '\n';
		strcat(bufferout_c, tmpout);
	}

    toc_c = current_timestamp();

  	long long c_time_in_nanos = toc_c - tic_c;

    /* FINE ELABORAZIONE C */

    /* INIZIO ELABORAZIONE ASM */

    tic_asm = current_timestamp();

    /* Assembly inline:
    Inserite qui il vostro blocco di codice assembly inline o richiamo a funzioni assembly.
    Il blocco di codice prende come input 'bufferin' e deve restituire una variabile stringa 'bufferout_asm' che verrà poi salvata su file. */
	
	__asm__(
		"cmpb $48, (%[in]);"		//controlliamo il valore di res_gen
		"je OFF;" 					//res gen = 0 il sistema rimane spento -> OFF
		"xorb %%cl, %%cl;"			//azzeriamo il contatore dei cicli in overload
	"ON:"
		"movb $49, (%[out]);"		//il sistema si accende, mettiamo quindi tutti gli interuttori a 1 e passiamo successivamente al calcolo della fascia di consumo
		"movb $49, 1(%[out]);"
		"movb $49, 2(%[out]);"
		"jmp CALCOLAFASCIA;"

	"OFF:"
		"xorb %%cl, %%cl;"			//il sistema è spento o deve spegnersi per eventuale superamento del sesto ciclo di OL, poniamo quindi tutti gli interuttori a 0 e anche la fascia viene posta a "00" e azzerato il contatore
		"movb $48, (%[out]);"		
		"movb $48, 1(%[out]);"
		"movb $48, 2(%[out]);"
		"movb $45, 3(%[out]);"
		"movb $48, 4(%[out]);"
		"movb $48, 5(%[out]);"
		"jmp NEXTLINE;"				//e passiamo quindi a controllare l'input successivo

	"RESGEN1:"						//se res_gen è uguale a 1 controlliamo se il sistema era spento e lo accendiamo tramite il passaggio da ON, altrimenti proseguiamo con il controllo di res_dw e res_wm
		"cmpb $48, %%bh;"
		"je ON;"
		"movb %%bh, (%[out]);"

	"RESDW:"						//controlliamo il valore di res_dw, se vale 1 poniamo a 1 l'interuttore relativo, altrimenti controlliamo res_wm
		"cmpb $49, 1(%[in]);"
		"jne RESWM;"
		"movb $49, %%bl;"

	"RESWM:"						//controlliamo il valore di res_wm, se vale 1 poniamo a 1 l'interruttore relativo, altrimenti passiamo a un check del load per eliminare dal load
		"cmpb $49, 2(%[in]);"		//l'eventuale carico di DW e WM se sono spente
		"jne CHANGELDW;"
		"movb $49, %%ch;"

	"CHANGELDW:"					//controlliamo se DW risulta accesa altrimenti andiamo a modificare bufferin mettendo uno 0 al relativo bit di load di quell'input
		"movb %%bl, 1(%[out]);"		//copiamo i nuovi valori effettivi di int_dw e int_wm sul buffer di output
		"movb %%ch, 2(%[out]);"
		"cmpb $48, %%bl;"
		"jne CHANGELWM;"
		"movb $48, 8(%[in]);"		//azzero l'eventuale conteggio del carico della lavastoviglie

	"CHANGELWM:"					//come per DW controlliamo ora se è necessario azzerare il bit di load relativo a WM nel caso in cui essa risulti spenta
		"cmpb $48, %%ch;"			//in entrambi i casi si passerà poi a calcolare la fascia di consumo dedl sistema in base al load eventualmente corretto
		"jne CALCOLAFASCIA;"
		"movb $48, 9(%[in]);"
		"jmp CALCOLAFASCIA;"

	"INIZIO:"
		"cmpb $48, (%[in]);"		//controlliamo il valore di res_gen, se 1 vado a controllare lo stato del sistema se 0 controllo il valore precedente di int_gen contenuto in "bh"
		"jne RESGEN1;"
		"cmpb $49, %%bh;"			//controllo int_gen del ciclo precedente
		"jne OFF;"
		"movb %%bh, (%[out]);"
		"jmp RESDW;"				//se il sistema era già acceso copia il valore di int_gen e passa al controllo degli altri reset

	"CALCOLAFASCIA:"
		"movb $45, 3(%[out]);"		//scriviamo il "-" sulla stringa di output
		"xorw %%ax, %%ax;"			//azzeriamo "ax" che conteneva il valore di consumo precedente e nel caso in cui il bit del load risulti 1 sommiamo il carico corrispondente
		"cmpb $49, 4(%[in]);"
		"jne FRIGO;"
		"addw $200, %%ax;"

		"FRIGO:"
		"cmpb $49, 5(%[in]);"
		"jne ASPIRA;"
		"addw $30, %%ax;"

		"ASPIRA:"
		"cmpb $49, 6(%[in]);"
		"jne PHON;"
		"addw $120, %%ax;"

		"PHON:"
		"cmpb $49, 7(%[in]);"
		"jne DW;"
		"addw $100, %%ax;"

		"DW:"
		"cmpb $49, 8(%[in]);"
		"jne WM;"
		"addw $200, %%ax;"

		"WM:"
		"cmpb $49, 9(%[in]);"
		"jne L60;"
		"addw $180, %%ax;"

		"L60:"
		"cmpb $49, 10(%[in]);"
		"jne L100;"
		"addw $24, %%ax;"

		"L100:"
		"cmpb $49, 11(%[in]);"
		"jne HIFI;"
		"addw $40, %%ax;"

		"HIFI:"
		"cmpb $49, 12(%[in]);"
		"jne TV;"
		"addw $20, %%ax;"

		"TV:"
		"cmpb $49, 13(%[in]);"
		"jne CHECK;"
		"addw $40, %%ax;"

	"CHECK:"						//confrontiamo ora il valore del consumo totale contenuto in "ax" con i limiti di ogni fascia ottenendo cosi lo stato del sistema che viene successivamente scritto sulla stringa di output
		"cmpw $150, %%ax;"			//confrontiamo con 150 daW -> Fascia 1: [0;150]
		"jg ELSE1;"
		"movb $70, 4(%[out]);"
		"movb $49, 5(%[out]);"
		"addb $1, %%cl;"
		"jmp CHECKCICLI;"

		"ELSE1:"
		"cmpw $300, %%ax;"			//confrontiamo con 300daW -> Fascia 2: ]150;300]
		"jg ELSE2;"
		"movb $70, 4(%[out]);"
		"movb $50, 5(%[out]);"
		"xorb %%cl, %%cl;"
		"jmp NEXTLINE;"

		"ELSE2:"
		"cmpw $450, %%ax;"			//confrontiamo con 450daW -> Fascia 3: ]300;450]
		"jg ELSE3;"
		"movb $70, 4(%[out]);"
		"movb $51, 5(%[out]);"
		"xorb %%cl, %%cl;"
		"jmp NEXTLINE;"

		"ELSE3:"					//se si effettua il salto a questa etichetta il sistema si trova in Overload (OL) e si passa poi al contollo del numero di cicli passati in questo stato
		"movb $79, 4(%[out]);"
		"movb $76, 5(%[out]);"
		"xorb %%cl, %%cl;"
		"jmp NEXTLINE;"			//incremento di 1 il contatore dei cicli in OL

	"CHECKCICLI:"
		"cmpb $4, %%cl;"
		"jl NEXTLINE;"				//se minore di 4 passo all'input successivo
		"jne CK5;"					//se maggiore di 4 controllo se sono a 5 o 6 cicli in OL
		"movb $48, 1(%[out]);"		//se uguale a 4 spengo int_dw aggiornando la stringa di output che andrà poi ad aggiornare la parte di registro contenente il valore di int_dw
		"jmp NEXTLINE;"

		"CK5:"
		"cmpb $5, %%cl;"
		"jne OFF;"					//se sono al sesto ciclo spengo il sistema, salto ad OFF che sovrascriverà l'intero buffer di output aggiornandolo con "000-00"
		"movb $48, 1(%[out]);"		//se uguale a 5 spengo int_dw e int_wm
		"movb $48, 2(%[out]);"

	"NEXTLINE:"
		"addl $15, %[in];"			//passo all'input successivo spostando di 15 caselle il puntatore alla stringa di input
		"movb $10, 6(%[out]);"		//aggiungo \n al termine della riga di output
		"movb (%[out]), %%bh;"		//salviamo il valore di int_gen di questo input ("bh" -> int_gen)
		"movb 1(%[out]), %%bl;"		//salviamo il valore di int_dw di questo input ("bl" -> int_dw)
		"movb 2(%[out]), %%ch;"		//salviamo il valore di int_wm di questo input ("ch" -> int_wm)
		"addl $7, %[out];"			//spostiamo il puntatore alla stringa di output a 7 caratteri dopo in modo da puntare alla prossima casella libera dove inserire il successivo output
		"cmpb $0, (%[in]);"			//controliamo se siamo arrivati al termine della stringa di input ovvero abbiamo incontrato il valore '\0'
		"jne INIZIO;"
		: //output
		: [in]"r"(bufferin), [out]"r"(bufferout_asm)
		: "eax", "ebx", "ecx"
	);
	
    toc_asm = current_timestamp();

  	long long asm_time_in_nanos = toc_asm - tic_asm;

    /* FINE ELABORAZIONE ASM */

    printf("C time elapsed: %lld ns\n", c_time_in_nanos);
    printf("ASM time elapsed: %lld ns\n", asm_time_in_nanos);

    /* Salvataggio dei risultati ASM */
  	FILE *outputFile;
    outputFile = fopen (argv[2], "w");
    fprintf (outputFile, "%s", bufferout_asm);
    fclose (outputFile);

    return 0;
}