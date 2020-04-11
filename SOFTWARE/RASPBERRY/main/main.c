/*
 *	Arquivo:	main.c
 *	Author: 	Alexandre Mascarenhas
 *	Criado em:	10/04/2020
 *
 *	Modificado:
 *
 *
*/

/*
	Descrição:	Programa em C para controlar o Caroknator
*/

/*******************************************************
INCLUDES
*******************************************************/
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <lcd1602.h>
#include <zabbix.h>
#include <logsGen.h>
#include <modPr.h>
#include <modAn.h>
#include <modDI.h>


/*******************************************************
DEFINES
*******************************************************/


/***************************************************************
	Descomentar os defines dos módulos analogicos que serao utilizados pelo programa
***************************************************************/
//#define modAn1
#define modAn2
//#define modAn3

#define numChModAn1 8		// CONFIGURA QUANTAS ENTRADAS ANALOGICAS DE CADA MODULO AN SERA UTILIZADA(1, 2, 4, 6 OU 8)
#define numChModAn2 8
#define numChModAn3 8

#define pureAD 0		// CASO QUEIRA O VALOR PURO LIDO NA CONVERSA A/D COLOQUE EM 1, CASO CONTRARIO O VALOR SERA CONVERTIDO EM TENSAO

#define chSPI 0			// CANAL SPI DA RASP UTILIZADO PARA FALAR COM OS MODULOS AN
#define speedSPI 7000000	// VELOCIDADE DA COMUNICACAO SPI COM O DSPIC DOS MODULOS AN
/**************************************************************/



/***************************************************************
	Descomentar os defines dos módulos digitais que serao utilizados pelo programa
***************************************************************/
#define modDI1
//#define modDI2
//#define modDI3


#define numChModDI1 16		// CONFIGURA QUANTAS ENTRADAS DIGITAIS DE CADA MODULO DI SERA UTILIZADA (0 a 16)
#define numChModDI2 16
#define numChModDI3 16
/**************************************************************/



/**************************************************************
	Descomentar o define lcd caso o mesmo for utilizado no programa
**************************************************************/
#define lcd
#define lcdAddress 0x27
/*************************************************************/


/**************************************************************
	Descomentar o define zabbix caso o mesmo for utilizado no programa
**************************************************************/
//#define zabbix				// HABILITA/DESABILITA ENVIO PARA ZABBIX
#define zabbixHost "AQUISINATOR_TESTE"	// HOSTNAME DA RASP
#define zabbixServer "10.177.9.210"	// IP DO SERVIDOR ZABBIX PARA QUAL ENVIARA OS DADOS
/*************************************************************/



/**************************************************************
	Descomentar o define gravalog caso o mesmo for utilizado no programa
**************************************************************/
//#define gravaLog  		// HABILITA/DESABILITA GRAVAÇÃO CSV
#define rowsToWriteLog 5000	// NUMEROS MAXIMO DE LINHAS PARA GRAVAR NO LOG POR SEGUNDO
#define gravaLogDivider 0	// FATOR DIVISIVO PARA GRAVACAO DOS LOGS
/*************************************************************/


#define showValues 1		// HABILITA/DESABILITA MOSTRAR VALORES NO SHELL
#define debugSerial 0		// HABILITA/DESABILITA DEBUGAÇÃO PELA SERIAL




/*******************************************************
Declaracao de Funcoes
*******************************************************/
void sendZabbix(char*, char*, char*, float, int);
void configurePins();
void clearGlobalVariables();
void readAnalog(int);
void readDigital(int);
void setupModules();
void initialScreen();
void lcdSetup();


/********************************************************
Variaveis Globais
********************************************************/
#ifdef modAn1			//Se o modulo estiver definido acima, seta a flag respectiva
	int flagModAn1 = 1;
#else
	int flagModAn1 = 0;
#endif
unsigned int ch1Mod1 = 0;	//Cria as variaveis globais para cada modulo
unsigned int ch2Mod1 = 0;
unsigned int ch3Mod1 = 0;
unsigned int ch4Mod1 = 0;
unsigned int ch5Mod1 = 0;
unsigned int ch6Mod1 = 0;
unsigned int ch7Mod1 = 0;
unsigned int ch8Mod1 = 0;
float medCh1Mod1 = 0.0;
float medCh2Mod1 = 0.0;
float medCh3Mod1 = 0.0;
float medCh4Mod1 = 0.0;
float medCh5Mod1 = 0.0;
float medCh6Mod1 = 0.0;
float medCh7Mod1 = 0.0;
float medCh8Mod1 = 0.0;
unsigned int nCh1Mod1 = 0;
unsigned int nCh2Mod1 = 0;
unsigned int nCh3Mod1 = 0;
unsigned int nCh4Mod1 = 0;
unsigned int nCh5Mod1 = 0;
unsigned int nCh6Mod1 = 0;
unsigned int nCh7Mod1 = 0;
unsigned int nCh8Mod1 = 0;


#ifdef modAn2
	int flagModAn2 = 1;
#else
	int flagModAn2 = 0;
#endif
int ch1Mod2 = 0;
int ch2Mod2 = 0;
int ch3Mod2 = 0;
int ch4Mod2 = 0;
int ch5Mod2 = 0;
int ch6Mod2 = 0;
int ch7Mod2 = 0;
int ch8Mod2 = 0;
float medCh1Mod2 = 0.0;
float medCh2Mod2 = 0.0;
float medCh3Mod2 = 0.0;
float medCh4Mod2 = 0.0;
float medCh5Mod2 = 0.0;
float medCh6Mod2 = 0.0;
float medCh7Mod2 = 0.0;
float medCh8Mod2 = 0.0;
unsigned int nCh1Mod2 = 1;
unsigned int nCh2Mod2 = 1;
unsigned int nCh3Mod2 = 1;
unsigned int nCh4Mod2 = 1;
unsigned int nCh5Mod2 = 1;
unsigned int nCh6Mod2 = 1;
unsigned int nCh7Mod2 = 1;
unsigned int nCh8Mod2 = 1;


#ifdef modAn3
	int flagModAn3 = 1;
#else
	int flagModAn3 = 0;
#endif
unsigned int ch1Mod3 = 0;
unsigned int ch2Mod3 = 0;
unsigned int ch3Mod3 = 0;
unsigned int ch4Mod3 = 0;
unsigned int ch5Mod3 = 0;
unsigned int ch6Mod3 = 0;
unsigned int ch7Mod3 = 0;
unsigned int ch8Mod3 = 0;
float medCh1Mod3 = 0.0;
float medCh2Mod3 = 0.0;
float medCh3Mod3 = 0.0;
float medCh4Mod3 = 0.0;
float medCh5Mod3 = 0.0;
float medCh6Mod3 = 0.0;
float medCh7Mod3 = 0.0;
float medCh8Mod3 = 0.0;
unsigned int nCh1Mod3 = 0;
unsigned int nCh2Mod3 = 0;
unsigned int nCh3Mod3 = 0;
unsigned int nCh4Mod3 = 0;
unsigned int nCh5Mod3 = 0;
unsigned int nCh6Mod3 = 0;
unsigned int nCh7Mod3 = 0;
unsigned int nCh8Mod3 = 0;


#ifdef modDI1
	int flagModDI1 = 1;
#else
	int flagModDI1 = 0;
#endif
int ch1ModDI1 = 0;
int ch2ModDI1 = 0;
int ch3ModDI1 = 0;
int ch4ModDI1 = 0;
int ch5ModDI1 = 0;
int ch6ModDI1 = 0;
int ch7ModDI1 = 0;
int ch8ModDI1 = 0;
int ch9ModDI1 = 0;
int ch10ModDI1 = 0;
int ch11ModDI1 = 0;
int ch12ModDI1 = 0;
int ch13ModDI1 = 0;
int ch14ModDI1 = 0;
int ch15ModDI1 = 0;
int ch16ModDI1 = 0;

#ifdef modDI2
	int flagModDI2 = 1;
#else
	int flagModDI2 = 0;
#endif
int ch1ModDI2 = 0;
int ch2ModDI2 = 0;
int ch3ModDI2 = 0;
int ch4ModDI2 = 0;
int ch5ModDI2 = 0;
int ch6ModDI2 = 0;
int ch7ModDI2 = 0;
int ch8ModDI2 = 0;
int ch9ModDI2 = 0;
int ch10ModDI2 = 0;
int ch11ModDI2 = 0;
int ch12ModDI2 = 0;
int ch13ModDI2 = 0;
int ch14ModDI2 = 0;
int ch15ModDI2 = 0;
int ch16ModDI2 = 0;

#ifdef modDI3
	int flagModDI3 = 1;
#else
	int flagModDI3 = 0;
#endif
int ch1ModDI3 = 0;
int ch2ModDI3 = 0;
int ch3ModDI3 = 0;
int ch4ModDI3 = 0;
int ch5ModDI3 = 0;
int ch6ModDI3 = 0;
int ch7ModDI3 = 0;
int ch8ModDI3 = 0;
int ch9ModDI3 = 0;
int ch10ModDI3 = 0;
int ch11ModDI3 = 0;
int ch12ModDI3 = 0;
int ch13ModDI3 = 0;
int ch14ModDI3 = 0;
int ch15ModDI3 = 0;
int ch16ModDI3 = 0;


#ifdef lcd
	int flagLcd = 1;
#else
	int flagLcd = 0;
#endif


#ifdef zabbix
	int flagZabbix = 1;
#else
	int flagZabbix = 0;
#endif


#ifdef gravalog
	int flagGravaLog = 1;
#else
	int flagGravaLog = 0;
#endif

/********************************************************
FUNCAO PRINCIPAL (MAIN)
********************************************************/
int main(int argc, char *argv[]){
	printf("****************************************************************************\n");
	char sz[] = "\n              MONITORAMENTO 'COLOQUE O NOME DO ATIVO MONITORADO AQUI'\n";
	printf(sz);
	printf("\n****************************************************************************\n");

	char local[100] = "/home/pi/projetos/generico";	// Pasta para os logs

	unsigned int nSamples = 0;	// Numero de samples no total
	int numErrors = 0;		// Numero de erros por segundo

	char nice[500] = "";		// Local exato do arquivo de log
	char extensao[10] = "csv";	// Extensao do arquivo de log
	char filename[300] = "";	// Nome do arquivo de log
	FILE *fp;

	int wroteLinesModAn1 = 0;	// Variaveis que contam o numero de linhas escritas em cada log
	int wroteLinesModAn2 = 0;
	int wroteLinesModAn3 = 0;

	time_t tempo; 			// Variável que guarda um tempo
	struct tm *sTempo;  		// Estrutura que guarda um tempo

	int hour;			// Variaveis de tempo
	int min;
	int sec;
	int oldSec;
	int day = 0;
	int mon = 0;
	int year = 0;

	setupModules();			// Funcao que incia e configura os modulos utilizados

	tempo = time(NULL); 		// Recebe tempo
	sTempo = localtime(&tempo);
	sec = sTempo->tm_sec;		// Pega o segundo
	oldSec = sec;

	if(flagGravaLog){			// Se for gravar o log, cria a pasta e o arquivo
		day = sTempo->tm_mday;
		mon = (sTempo -> tm_mon)+1;
		year = (sTempo -> tm_year)+1900;
		hour = sTempo-> tm_hour;
		sprintf(filename, "%d-%d-%d", day, mon, year);
		configLog(local,filename);
	}

	releOn(1);			// Liga o rele 1
	printf("\nRodando...\n");	// Inicia o loop principal
	for(;;){

		/****************************************************
		Escreva a logica do código aqui
		****************************************************/


		readAnalog(2);		// Le o modulo Analogico 2
		readDigital(1);		// Le o modulo de entrada Digitais 1



		/**************************************************/

		nSamples++;
		tempo = time(NULL);	// Pega o tempo atualizado
		sTempo = localtime(&tempo);
		hour = sTempo->tm_hour;
		min = sTempo->tm_min;
		sec = sTempo->tm_sec;
		if(flagGravaLog){
			if(flagModAn1){
				sprintf(nice, "%s/%s/%s.%s",local,filename,filename,extensao);
				fp = fopen(nice, "a+");
				if(fp == NULL){
					printf("\nDEU RUIM NA ABERTURA DO ARQUIVO DE LOG\n");
				}else{
					fprintf(fp,"\n%d:%d:%d;%d;%d;%d;%d;%d;%d;%d;%d",hour,min,sec,ch1Mod1, ch2Mod1, ch3Mod1, ch4Mod1, ch5Mod1, ch6Mod1, ch7Mod1, ch8Mod1);
					fclose(fp);
				}
				wroteLinesModAn1++;
			}
			if(flagModAn2){
				sprintf(nice, "%s/%s/%s.%s",local,filename,filename,extensao);
				fp = fopen(nice, "a+");
				if(fp == NULL){
					printf("\nDEU RUIM NA ABERTURA DO ARQUIVO DE LOG\n");
				}else{
					fprintf(fp,"\n%d:%d:%d;%d;%d;%d;%d;%d;%d;%d;%d",hour,min,sec,ch1Mod2, ch2Mod2, ch3Mod2, ch4Mod2, ch5Mod2, ch6Mod2, ch7Mod2, ch8Mod2);
					fclose(fp);
				}
				wroteLinesModAn2++;
			}
			if(flagModAn3){
				sprintf(nice, "%s/%s/%s.%s",local,filename,filename,extensao);
				fp = fopen(nice, "a+");
				if(fp == NULL){
					printf("\nDEU RUIM NA ABERTURA DO ARQUIVO DE LOG\n");
				}else{
					fprintf(fp,"\n%d:%d:%d;%d;%d;%d;%d;%d;%d;%d;%d",hour,min,sec,ch1Mod3, ch2Mod3, ch3Mod3, ch4Mod3, ch5Mod3, ch6Mod3, ch7Mod3, ch8Mod3);
					fclose(fp);
				}
				wroteLinesModAn3++;
			}
		}


		if(sec != oldSec){	// Mudou o segundo
			oldSec = sec;

			if(flagModDI1){					// Caso esteja utilizando o modulo entra aqui
				if(showValues){				// Se ativo para mostrar os valores no shell
					printf("\n******* MODULO DIGITAL 1  ********");
					printf("\nCanal1   = %d Canal2   = %d  Canal3  = %d Canal4  = %d",ch1ModDI1, ch2ModDI1, ch3ModDI1, ch4ModDI1);
					printf("\nCanal5   = %d Canal6   = %d  Canal7  = %d Canal8  = %d",ch5ModDI1, ch6ModDI1, ch7ModDI1, ch8ModDI1);
					printf("\nCanal9   = %d Canal10  = %d  Canal11 = %d Canal12 = %d",ch5ModDI1, ch6ModDI1, ch7ModDI1, ch8ModDI1);
					printf("\nCanal13  = %d Canal14  = %d  Canal15 = %d Canal16 = %d",ch13ModDI1, ch14ModDI1, ch15ModDI1, ch16ModDI1);
					printf("\n**********************************\n");
				}
			}

			if(flagModDI2){
				if(showValues){
					printf("\n******* MODULO DIGITAL 2  ********");
					printf("\nCanal1   = %d Canal2   = %d  Canal3  = %d Canal4  = %d",ch1ModDI2, ch2ModDI2, ch3ModDI2, ch4ModDI2);
					printf("\nCanal5   = %d Canal6   = %d  Canal7  = %d Canal8  = %d",ch5ModDI2, ch6ModDI2, ch7ModDI2, ch8ModDI2);
					printf("\nCanal9   = %d Canal10  = %d  Canal11 = %d Canal12 = %d",ch5ModDI2, ch6ModDI2, ch7ModDI2, ch8ModDI2);
					printf("\nCanal13  = %d Canal14  = %d  Canal15 = %d Canal16 = %d",ch13ModDI2, ch14ModDI2, ch15ModDI2, ch16ModDI2);
					printf("\n**********************************\n");
				}
			}

			if(flagModDI3){
				if(showValues){
					printf("\n******* MODULO DIGITAL 3  ********");
					printf("\nCanal1   = %d Canal2   = %d  Canal3  = %d Canal4  = %d",ch1ModDI3, ch2ModDI3, ch3ModDI3, ch4ModDI3);
					printf("\nCanal5   = %d Canal6   = %d  Canal7  = %d Canal8  = %d",ch5ModDI3, ch6ModDI3, ch7ModDI3, ch8ModDI3);
					printf("\nCanal9   = %d Canal10  = %d  Canal11 = %d Canal12 = %d",ch5ModDI3, ch6ModDI3, ch7ModDI3, ch8ModDI3);
					printf("\nCanal13  = %d Canal14  = %d  Canal15 = %d Canal16 = %d",ch13ModDI3, ch14ModDI3, ch15ModDI3, ch16ModDI3);
					printf("\n**********************************\n");
				}
			}


			if(flagModAn1){
				medCh1Mod1 /= nCh1Mod1;			// Faz a media dos valores lidos de cada canal do modulo no segundo
				medCh2Mod1 /= nCh2Mod1;
				medCh3Mod1 /= nCh3Mod1;
				medCh4Mod1 /= nCh4Mod1;
				medCh5Mod1 /= nCh5Mod1;
				medCh6Mod1 /= nCh6Mod1;
				medCh7Mod1 /= nCh7Mod1;
				medCh8Mod1 /= nCh8Mod1;

				if(showValues){
					printf("\n****** MODULO ANALOGICO 1  *******");
					printf("\nCanal1  = %.2f spsCh1 = %dsps  Canal2  = %.2f  spsCh2 = %dsps",medCh1Mod1, nCh1Mod1, medCh2Mod1, nCh2Mod1);
					printf("\nCanal3  = %.2f spsCh3 = %dsps  Canal4  = %.2f  spsCh4 = %dsps",medCh3Mod1, nCh3Mod1, medCh4Mod1, nCh4Mod1);
					printf("\nCanal5  = %.2f spsCh5 = %dsps  Canal6  = %.2f  spsCh6 = %dsps",medCh5Mod1, nCh5Mod1, medCh6Mod1, nCh6Mod1);
					printf("\nCanal7  = %.2f spsCh7 = %dsps  Canal8  = %.2f  spsCh8 = %dsps",medCh7Mod1, nCh7Mod1, medCh8Mod1, nCh8Mod1);
				}
			}
			if(flagModAn2){
				medCh1Mod2 /= nCh1Mod2;
				medCh2Mod2 /= nCh2Mod2;
				medCh3Mod2 /= nCh3Mod2;
				medCh4Mod2 /= nCh4Mod2;
				medCh5Mod2 /= nCh5Mod2;
				medCh6Mod2 /= nCh6Mod2;
				medCh7Mod2 /= nCh7Mod2;
				medCh8Mod2 /= nCh8Mod2;

				if(showValues){
					printf("\n****** MODULO ANALOGICO 2  *******");
					printf("\nCanal1  = %.2f spsCh1 = %dsps  Canal2  = %.2f  spsCh2 = %dsps",medCh1Mod2, nCh1Mod2, medCh2Mod2, nCh2Mod2);
					printf("\nCanal3  = %.2f spsCh3 = %dsps  Canal4  = %.2f  spsCh4 = %dsps",medCh3Mod2, nCh3Mod2, medCh4Mod2, nCh4Mod2);
					printf("\nCanal5  = %.2f spsCh5 = %dsps  Canal6  = %.2f  spsCh6 = %dsps",medCh5Mod2, nCh5Mod2, medCh6Mod2, nCh6Mod2);
					printf("\nCanal7  = %.2f spsCh7 = %dsps  Canal8  = %.2f  spsCh8 = %dsps",medCh7Mod2, nCh7Mod2, medCh8Mod2, nCh8Mod2);
				}
			}
			if(flagModAn3){
				medCh1Mod3 /= nCh1Mod3;
				medCh2Mod3 /= nCh2Mod3;
				medCh3Mod3 /= nCh3Mod3;
				medCh4Mod3 /= nCh4Mod3;
				medCh5Mod3 /= nCh5Mod3;
				medCh6Mod3 /= nCh6Mod3;
				medCh7Mod3 /= nCh7Mod3;
				medCh8Mod3 /= nCh8Mod3;

				if(showValues){
					printf("\n****** MODULO ANALOGICO 3  *******");
					printf("\nCanal1  = %.2f spsCh1 = %dsps  Canal2  = %.2f  spsCh2 = %dsps",medCh2Mod3, nCh2Mod3, medCh1Mod3, nCh1Mod3);
					printf("\nCanal3  = %.2f spsCh3 = %dsps  Canal4  = %.2f  spsCh4 = %dsps",medCh4Mod3, nCh4Mod3, medCh3Mod3, nCh3Mod3);
					printf("\nCanal5  = %.2f spsCh5 = %dsps  Canal6  = %.2f  spsCh6 = %dsps",medCh6Mod3, nCh6Mod3, medCh5Mod3, nCh5Mod3);
					printf("\nCanal7  = %.2f spsCh7 = %dsps  Canal8  = %.2f  spsCh8 = %dsps",medCh8Mod3, nCh8Mod3, medCh7Mod3, nCh7Mod3);
				}
			}

			if(showValues){
				printf("\n**********************************");
				printf("\nTaxa de erro na comunicacao SPI: %.4f%%", ((float)numErrors/nSamples)*100);
				printf("\nTaxa de amostragem: %d sps/ch", (nSamples));
				printf("\nGravando LOG: ");
				if(flagGravaLog){
					if(flagModAn1){
						printf("Sim\nLinhas escritas no LOG do Modulo An1: %d linhas \n", wroteLinesModAn1);
					}
					if(flagModAn2){
						printf("Sim\nLinhas escritas no LOG do Modulo An2: %d linhas \n", wroteLinesModAn2);
					}
					if(flagModAn3){
						printf("Sim\nLinhas escritas no LOG do Modulo An3: %d linhas \n", wroteLinesModAn3);
					}
				}else{
					printf("Nao\n");
				}
				printf("Hora: %d:%d:%d\n\n",hour, min, sec );
			}

			if(flagZabbix){
				//sendZabbix(ipDoServer, nomeDoHost, key, value, 0);
				sendZabbix(zabbixServer, zabbixHost, "medCh1Mod2", medCh1Mod2, 0);
			}

			nSamples = 0;
			numErrors = 0;
			wroteLinesModAn1 = 0;
			wroteLinesModAn2 = 0;
			wroteLinesModAn3 = 0;
			clearGlobalVariables();
		}
	}
	fclose(fp);

	return 0;
}


/********************************************************
Configuracao e Inicializao dos modulos
********************************************************/
void setupModules(){
	processorModuleSetup();		// Incia o modulo processador

	releOff(1);			// Desliga o rele 1
	releOff(2);			// Desliga o rele 2
	lcdSetup();
	printf("\nConfigurando SPI ...");
	if(flagLcd){
		lcd1602SetCursor(0, 0);
		lcd1602WriteString("Configurando SPI");
		lcd1602SetCursor(0, 1);
		lcd1602WriteString("                ");
	}
	startSPI(chSPI, speedSPI);	// Incia o modulo SPI
	if(flagLcd){
		delay(100);
		lcd1602SetCursor(0, 1);
		lcd1602WriteString("OK              ");
	}
	delay(1000);

	if(flagModAn1 || flagModAn2 || flagModAn3){
		printf("\nConfigurando Modulo(s) AN\n");
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Configurando    ");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("modulo(s) AN    ");
		}
	}

	if(flagModAn1){
//		analogModuleSetup(numeroDoModulo, numeroDeCanaisAnalogicos, ResetDoPIC(0-NAO, 1-SIM));
		analogModuleSetup(1, 8, 1);	// Inicia o modulo analogico 2, com 8 entradas analogicos e com reset no PIC
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Modulo AN 1 - OK");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("                ");
			delay(800);
		}
	}
	if(flagModAn2){
//		analogModuleSetup(numeroDoModulo, numeroDeCanaisAnalogicos, ResetDoPIC(0-NAO, 1-SIM));
		analogModuleSetup(2, 8, 1);	// Inicia o modulo analogico 2, com 8 entradas analogicos e com reset no PIC
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Modulo AN 2 - OK");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("                ");
			delay(800);
		}
	}
	if(flagModAn3){
//		analogModuleSetup(numeroDoModulo, numeroDeCanaisAnalogicos, ResetDoPIC(0-NAO, 1-SIM));
		analogModuleSetup(3, 8, 0);	// Inicia o modulo analogico 2, com 8 entradas analogicos e com reset no PIC
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Modulo AN 3 - OK");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("                ");
			delay(800);
		}
	}


	if(flagModDI1 || flagModDI2 || flagModDI3){
		printf("\nConfigurando Modulo(s) DI");
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Configurando    ");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("modulo(s) DI    ");
			delay(1000);
		}
	}
	if(flagModDI1){
//		digitalModuleSetup(numeroDoModulo)
		digitalModuleSetup(1);		// Inicia o modulo digital 1
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Modulo DI 1 - OK");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("                ");
			delay(800);
		}
	}
	if(flagModDI2){
//		digitalModuleSetup(numeroDoModulo)
		digitalModuleSetup(2);		// Inicia o modulo digital 1
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Modulo DI 2 - OK");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("                ");
			delay(800);
		}
	}
	if(flagModDI3){
//		digitalModuleSetup(numeroDoModulo)
		digitalModuleSetup(3);		// Inicia o modulo digital 1
		if(flagLcd){
			lcd1602SetCursor(0, 0);
			lcd1602WriteString("Modulo DI 3 - OK");
			lcd1602SetCursor(0, 1);
			lcd1602WriteString("                ");
			delay(800);
		}
	}

	initialScreen();
}


/********************************************************
Incia LCD
********************************************************/
void lcdSetup(){
	int rc;
	rc = lcd1602Init(1, lcdAddress);
	if(rc){
		printf("\nLCD INICIALIZATION FAILED !! CHECK THIS\n");
	}
	lcd1602WriteString("Aquisinator V1.0");
	lcd1602SetCursor(0, 1);
	lcd1602WriteString(" METRO  GMT/SMA ");
	lcd1602Control(1, 0, 0);	// Backligh ligado, underline desligado, blink block ligado
	delay(1000);
}

/********************************************************
Tela Inicial do programa LCD
********************************************************/
void initialScreen(){
	lcd1602SetCursor(0, 0);
	lcd1602WriteString("Aquisinator V1.0");
	lcd1602SetCursor(0, 1);
	lcd1602WriteString(" METRO  GMT/NMA ");
}

/********************************************************
Limpa as variaveis globais
********************************************************/
void clearGlobalVariables(){
	if(flagModAn1){
		medCh1Mod1 = 0.0;
		medCh2Mod1 = 0.0;
		medCh3Mod1 = 0.0;
		medCh4Mod1 = 0.0;
		medCh5Mod1 = 0.0;
		medCh6Mod1 = 0.0;
		medCh7Mod1 = 0.0;
		medCh8Mod1 = 0.0;
		nCh1Mod1 = 1;
		nCh2Mod1 = 1;
		nCh3Mod1 = 1;
		nCh4Mod1 = 1;
		nCh5Mod1 = 1;
		nCh6Mod1 = 1;
		nCh7Mod1 = 1;
		nCh8Mod1 = 1;
	}
	if(flagModAn2){
		medCh1Mod2 = 0.0;
		medCh2Mod2 = 0.0;
		medCh3Mod2 = 0.0;
		medCh4Mod2 = 0.0;
		medCh5Mod2 = 0.0;
		medCh6Mod2 = 0.0;
		medCh7Mod2 = 0.0;
		medCh8Mod2 = 0.0;
		nCh1Mod2 = 1;
		nCh2Mod2 = 1;
		nCh3Mod2 = 1;
		nCh4Mod2 = 1;
		nCh5Mod2 = 1;
		nCh6Mod2 = 1;
		nCh7Mod2 = 1;
		nCh8Mod2 = 1;
	}
	if(flagModAn3){
		medCh1Mod3 = 0.0;
		medCh2Mod3 = 0.0;
		medCh3Mod3 = 0.0;
		medCh4Mod3 = 0.0;
		medCh5Mod3 = 0.0;
		medCh6Mod3 = 0.0;
		medCh7Mod3 = 0.0;
		medCh8Mod3 = 0.0;
		nCh1Mod3 = 1;
		nCh2Mod3 = 1;
		nCh3Mod3 = 1;
		nCh4Mod3 = 1;
		nCh5Mod3 = 1;
		nCh6Mod3 = 1;
		nCh7Mod3 = 1;
		nCh8Mod3 = 1;
	}
}



/********************************************************
Le os canais Digitais configurados
********************************************************/
void readDigital(int module){
	switch(module){
		case 1:{
			switch(numChModDI1){
				case 16:{ch16ModDI1 = digitalModuleRead(1, 16);}
				case 15:{ch15ModDI1 = digitalModuleRead(1, 15);}
				case 14:{ch14ModDI1 = digitalModuleRead(1, 14);}
				case 13:{ch13ModDI1 = digitalModuleRead(1, 13);}
				case 12:{ch12ModDI1 = digitalModuleRead(1, 12);}
				case 11:{ch11ModDI1 = digitalModuleRead(1, 11);}
				case 10:{ch10ModDI1 = digitalModuleRead(1, 10);}
				case 9:{ch9ModDI1 = digitalModuleRead(1, 9);}
				case 8:{ch8ModDI1 = digitalModuleRead(1, 8);}
				case 7:{ch7ModDI1 = digitalModuleRead(1, 7);}
				case 6:{ch6ModDI1 = digitalModuleRead(1, 6);}
				case 5:{ch5ModDI1 = digitalModuleRead(1, 5);}
				case 4:{ch4ModDI1 = digitalModuleRead(1, 4);}
				case 3:{ch3ModDI1 = digitalModuleRead(1, 3);}
				case 2:{ch2ModDI1 = digitalModuleRead(1, 2);}
				case 1:{ch1ModDI1 = digitalModuleRead(1, 1);}
			}
		}break;

		case 2:{
			switch(numChModDI2){
				case 16:{ch16ModDI2 = digitalModuleRead(2, 16);}
				case 15:{ch15ModDI2 = digitalModuleRead(2, 15);}
				case 14:{ch14ModDI2 = digitalModuleRead(2, 14);}
				case 13:{ch13ModDI2 = digitalModuleRead(2, 13);}
				case 12:{ch12ModDI2 = digitalModuleRead(2, 12);}
				case 11:{ch11ModDI2 = digitalModuleRead(2, 11);}
				case 10:{ch10ModDI2 = digitalModuleRead(2, 10);}
				case 9:{ch9ModDI2 = digitalModuleRead(2, 9);}
				case 8:{ch8ModDI2 = digitalModuleRead(2, 8);}
				case 7:{ch7ModDI2 = digitalModuleRead(2, 7);}
				case 6:{ch6ModDI2 = digitalModuleRead(2, 6);}
				case 5:{ch5ModDI2 = digitalModuleRead(2, 5);}
				case 4:{ch4ModDI2 = digitalModuleRead(2, 4);}
				case 3:{ch3ModDI2 = digitalModuleRead(2, 3);}
				case 2:{ch2ModDI2 = digitalModuleRead(2, 2);}
				case 1:{ch1ModDI2 = digitalModuleRead(2, 1);}
			}
		}break;

		case 3:{
			switch(numChModDI3){
				case 16:{ch16ModDI3 = digitalModuleRead(3, 16);}
				case 15:{ch15ModDI3 = digitalModuleRead(3, 15);}
				case 14:{ch14ModDI3 = digitalModuleRead(3, 14);}
				case 13:{ch13ModDI3 = digitalModuleRead(3, 13);}
				case 12:{ch12ModDI3 = digitalModuleRead(3, 12);}
				case 11:{ch11ModDI3 = digitalModuleRead(3, 11);}
				case 10:{ch10ModDI3 = digitalModuleRead(3, 10);}
				case 9:{ch9ModDI3 = digitalModuleRead(3, 9);}
				case 8:{ch8ModDI3 = digitalModuleRead(3, 8);}
				case 7:{ch7ModDI3 = digitalModuleRead(3, 7);}
				case 6:{ch6ModDI3 = digitalModuleRead(3, 6);}
				case 5:{ch5ModDI3 = digitalModuleRead(3, 5);}
				case 4:{ch4ModDI3 = digitalModuleRead(3, 4);}
				case 3:{ch3ModDI3 = digitalModuleRead(3, 3);}
				case 2:{ch2ModDI3 = digitalModuleRead(3, 2);}
				case 1:{ch1ModDI3 = digitalModuleRead(3, 1);}
			}
		}break;


	}
}

/********************************************************
Le os canais analogicos configurados
********************************************************/
void readAnalog(int module){
	switch(module){
		case 1:{
			switch(numChModAn1){
				case 1:{
					ch1Mod1 = analogModuleRead(1, 1, pureAD);
					if(ch1Mod1 >= 0){medCh1Mod1 += ch1Mod1;nCh1Mod1++;}
				}break;

				case 2:{
					ch1Mod1 = analogModuleRead(1, 0, pureAD);
					if(ch1Mod1 >= 0){medCh1Mod1 += ch1Mod1;nCh1Mod1++;}
					ch2Mod1 = analogModuleRead(1, 1, pureAD);
					if(ch2Mod1 >= 0){medCh2Mod1 += ch2Mod1;nCh2Mod1++;}
				}break;

				case 4:{
					ch1Mod1 = analogModuleRead(1, 0, pureAD);
					if(ch1Mod1 >= 0){medCh1Mod1 += ch1Mod1;nCh1Mod1++;}
					ch4Mod1 = analogModuleRead(1, 1, pureAD);
					if(ch4Mod1 >= 0){medCh4Mod1 += ch4Mod1;nCh4Mod1++;}
					ch3Mod1 = analogModuleRead(1, 2, pureAD);
					if(ch3Mod1 >= 0){medCh3Mod1 += ch3Mod1;nCh3Mod1++;}
					ch2Mod1 = analogModuleRead(1, 3, pureAD);
					if(ch2Mod1 >= 0){medCh2Mod1 += ch2Mod1;nCh2Mod1++;}
				}break;

				case 6:{
					ch3Mod1 = analogModuleRead(1, 0, pureAD);
					if(ch3Mod1 >= 0){medCh3Mod1 += ch3Mod1;nCh3Mod1++;}
					ch6Mod1 = analogModuleRead(1, 1, pureAD);
					if(ch6Mod1 >= 0){medCh6Mod1 += ch6Mod1;nCh6Mod1++;}
					ch5Mod1 = analogModuleRead(1, 2, pureAD);
					if(ch5Mod1 >= 0){medCh5Mod1 += ch5Mod1;nCh5Mod1++;}
					ch2Mod1 = analogModuleRead(1, 3, pureAD);
					if(ch2Mod1 >= 0){medCh2Mod1 += ch2Mod1;nCh2Mod1++;}
					ch1Mod1 = analogModuleRead(1, 4, pureAD);
					if(ch1Mod1 >= 0){medCh1Mod1 += ch1Mod1;nCh1Mod1++;}
					ch4Mod1 = analogModuleRead(1, 5, pureAD);
					if(ch4Mod1 >= 0){medCh4Mod1 += ch4Mod1;nCh4Mod1++;}
				}break;

				case 8:{
					ch5Mod1 = analogModuleRead(1, 0, pureAD);
					if(ch5Mod1 >= 0){medCh5Mod1 += ch5Mod1;nCh5Mod1++;}
					ch8Mod1 = analogModuleRead(1, 1, pureAD);
					if(ch8Mod1 >= 0){medCh8Mod1 += ch8Mod1;nCh8Mod1++;}
					ch7Mod1 = analogModuleRead(1, 2, pureAD);
					if(ch7Mod1 >= 0){medCh7Mod1 += ch7Mod1;nCh7Mod1++;}
					ch2Mod1 = analogModuleRead(1, 3, pureAD);
					if(ch2Mod1 >= 0){medCh2Mod1 += ch2Mod1;nCh2Mod1++;}
					ch1Mod1 = analogModuleRead(1, 4, pureAD);
					if(ch1Mod1 >= 0){medCh1Mod1 += ch1Mod1;nCh1Mod1++;}
					ch4Mod1 = analogModuleRead(1, 5, pureAD);
					if(ch4Mod1 >= 0){medCh4Mod1 += ch4Mod1;nCh4Mod1++;}
					ch3Mod1 = analogModuleRead(1, 6, pureAD);
					if(ch3Mod1 >= 0){medCh3Mod1 += ch3Mod1;nCh3Mod1++;}
					ch6Mod1 = analogModuleRead(1, 7, pureAD);
					if(ch6Mod1 >= 0){medCh6Mod1 += ch6Mod1;nCh6Mod1++;}
				}break;
			}
		}break;

		case 2:{
			switch(numChModAn2){
				case 1:{
					ch1Mod2 = analogModuleRead(2, 1, pureAD);
					if(ch1Mod2 >= 0){medCh1Mod2 += ch1Mod2;nCh1Mod2++;}
				}break;

				case 2:{
					ch1Mod2 = analogModuleRead(2, 0, pureAD);
					if(ch1Mod2 >= 0){medCh1Mod2 += ch1Mod2;nCh1Mod2++;}
					ch2Mod2 = analogModuleRead(2, 1, pureAD);
					if(ch2Mod2 >= 0){medCh2Mod2 += ch2Mod2;nCh2Mod2++;}
				}break;

				case 4:{
					ch1Mod2 = analogModuleRead(2, 0, pureAD);
					if(ch1Mod2 >= 0){medCh1Mod2 += ch1Mod2;nCh1Mod2++;}
					ch4Mod2 = analogModuleRead(2, 1, pureAD);
					if(ch4Mod2 >= 0){medCh4Mod2 += ch4Mod2;nCh4Mod2++;}
					ch3Mod2 = analogModuleRead(2, 2, pureAD);
					if(ch3Mod2 >= 0){medCh3Mod2 += ch3Mod2;nCh3Mod2++;}
					ch2Mod2 = analogModuleRead(2, 3, pureAD);
					if(ch2Mod2 >= 0){medCh2Mod2 += ch2Mod2;nCh2Mod2++;}
				}break;

				case 6:{
					ch3Mod2 = analogModuleRead(2, 0, pureAD);
					if(ch3Mod2 >= 0){medCh3Mod2 += ch3Mod2;nCh3Mod2++;}
					ch6Mod2 = analogModuleRead(2, 1, pureAD);
					if(ch6Mod2 >= 0){medCh6Mod2 += ch6Mod2;nCh6Mod2++;}
					ch5Mod2 = analogModuleRead(2, 2, pureAD);
					if(ch5Mod2 >= 0){medCh5Mod2 += ch5Mod2;nCh5Mod2++;}
					ch2Mod2 = analogModuleRead(2, 3, pureAD);
					if(ch2Mod2 >= 0){medCh2Mod2 += ch2Mod2;nCh2Mod2++;}
					ch1Mod2 = analogModuleRead(2, 4, pureAD);
					if(ch1Mod2 >= 0){medCh1Mod2 += ch1Mod2;nCh1Mod2++;}
					ch4Mod2 = analogModuleRead(2, 5, pureAD);
					if(ch4Mod2 >= 0){medCh4Mod2 += ch4Mod2;nCh4Mod2++;}
				}break;

				case 8:{
					ch5Mod2 = analogModuleRead(2, 0, pureAD);
					if(ch5Mod2 >= 0){medCh5Mod2 += ch5Mod2;nCh5Mod2++;}
					ch8Mod2 = analogModuleRead(2, 1, pureAD);
					if(ch8Mod2 >= 0){medCh8Mod2 += ch8Mod2;nCh8Mod2++;}
					ch7Mod2 = analogModuleRead(2, 2, pureAD);
					if(ch7Mod2 >= 0){medCh7Mod2 += ch7Mod2;nCh7Mod2++;}
					ch2Mod2 = analogModuleRead(2, 3, pureAD);
					if(ch2Mod2 >= 0){medCh2Mod2 += ch2Mod2;nCh2Mod2++;}
					ch1Mod2 = analogModuleRead(2, 4, pureAD);
					if(ch1Mod2 >= 0){medCh1Mod2 += ch1Mod2;nCh1Mod2++;}
					ch4Mod2 = analogModuleRead(2, 5, pureAD);
					if(ch4Mod2 >= 0){medCh4Mod2 += ch4Mod2;nCh4Mod2++;}
					ch3Mod2 = analogModuleRead(2, 6, pureAD);
					if(ch3Mod2 >= 0){medCh3Mod2 += ch3Mod2;nCh3Mod2++;}
					ch6Mod2 = analogModuleRead(2, 7, pureAD);
					if(ch6Mod2 >= 0){medCh6Mod2 += ch6Mod2;nCh6Mod2++;}
				}break;
			}
		}break;

		case 3:{
			switch(numChModAn3){
				case 1:{
					ch1Mod3 = analogModuleRead(3, 1, pureAD);
					if(ch1Mod3 >= 0){medCh1Mod3 += ch1Mod3;nCh1Mod3++;}
				}break;

				case 2:{
					ch1Mod3 = analogModuleRead(3, 0, pureAD);
					if(ch1Mod3 >= 0){medCh1Mod3 += ch1Mod3;nCh1Mod3++;}
					ch2Mod2 = analogModuleRead(3, 1, pureAD);
					if(ch2Mod3 >= 0){medCh2Mod3 += ch2Mod3;nCh2Mod3++;}
				}break;

				case 4:{
					ch1Mod3 = analogModuleRead(3, 0, pureAD);
					if(ch1Mod3 >= 0){medCh1Mod3 += ch1Mod3;nCh1Mod3++;}
					ch4Mod3 = analogModuleRead(3, 1, pureAD);
					if(ch4Mod3 >= 0){medCh4Mod3 += ch4Mod3;nCh4Mod3++;}
					ch3Mod3 = analogModuleRead(3, 2, pureAD);
					if(ch3Mod3 >= 0){medCh3Mod3 += ch3Mod3;nCh3Mod3++;}
					ch2Mod3 = analogModuleRead(3, 3, pureAD);
					if(ch2Mod3 >= 0){medCh2Mod3 += ch2Mod3;nCh2Mod3++;}
				}break;

				case 6:{
					ch3Mod3 = analogModuleRead(3, 0, pureAD);
					if(ch3Mod3 >= 0){medCh3Mod3 += ch3Mod3;nCh3Mod3++;}
					ch6Mod3 = analogModuleRead(3, 1, pureAD);
					if(ch6Mod3 >= 0){medCh6Mod3 += ch6Mod3;nCh6Mod3++;}
					ch5Mod3 = analogModuleRead(3, 2, pureAD);
					if(ch5Mod3 >= 0){medCh5Mod3 += ch5Mod3;nCh5Mod3++;}
					ch2Mod3 = analogModuleRead(3, 3, pureAD);
					if(ch2Mod3 >= 0){medCh2Mod3 += ch2Mod3;nCh2Mod3++;}
					ch1Mod3 = analogModuleRead(3, 4, pureAD);
					if(ch1Mod3 >= 0){medCh1Mod3 += ch1Mod3;nCh1Mod3++;}
					ch4Mod3 = analogModuleRead(3, 5, pureAD);
					if(ch4Mod3 >= 0){medCh4Mod3 += ch4Mod3;nCh4Mod3++;}
				}break;

				case 8:{
					ch5Mod3 = analogModuleRead(3, 0, pureAD);
					if(ch5Mod3 >= 0){medCh5Mod3 += ch5Mod3;nCh5Mod3++;}
					ch8Mod3 = analogModuleRead(3, 1, pureAD);
					if(ch8Mod3 >= 0){medCh8Mod3 += ch8Mod3;nCh8Mod3++;}
					ch7Mod3 = analogModuleRead(3, 2, pureAD);
					if(ch7Mod3 >= 0){medCh7Mod3 += ch7Mod3;nCh7Mod3++;}
					ch2Mod3 = analogModuleRead(3, 3, pureAD);
					if(ch2Mod3 >= 0){medCh2Mod3 += ch2Mod3;nCh2Mod3++;}
					ch1Mod3 = analogModuleRead(3, 4, pureAD);
					if(ch1Mod3 >= 0){medCh1Mod3 += ch1Mod3;nCh1Mod3++;}
					ch4Mod3 = analogModuleRead(3, 5, pureAD);
					if(ch4Mod3 >= 0){medCh4Mod3 += ch4Mod3;nCh4Mod3++;}
					ch3Mod3 = analogModuleRead(3, 6, pureAD);
					if(ch3Mod3 >= 0){medCh3Mod3 += ch3Mod3;nCh3Mod3++;}
					ch6Mod3 = analogModuleRead(3, 7, pureAD);
					if(ch6Mod3 >= 0){medCh6Mod3 += ch6Mod3;nCh6Mod3++;}
				}break;
			}
		}break;
	}
}
