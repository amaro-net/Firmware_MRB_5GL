/* 
 * File:   servos_old.h
 * Author: Amaro
 *
 * Created on 12 de Junho de 2014, 13:24
 */

#ifndef SERVOS_OLD_H
#define	SERVOS_OLD_H

#ifdef	__cplusplus
extern "C" {
#endif

// Pinos de saída (do PIC18F45K22 para a placa de controle dos servos)
#define BIT_TR  LATBbits.LATB0
#define BIT_DT  LATBbits.LATB1
#define CLK     LATBbits.LATB2
#define RST     LATBbits.LATB5

// Pinos de entrada (da placa de controle dos servos para o PIC18F45K22)
#define TR_OK   PORTBbits.RB3
#define D_OK    PORTBbits.RB4

/* Esquema de ligação entre os pinos da Ready For PIC (PIC18F45K22) e a placa de controle dos servos (PIC16F84A)
 * LINHA    |   PIC18F45K22  |  PIC16F84A
 * -------------------------------------------
 *  BIT_TR  |      RB0       |  RA4  (PINO 3)
 *  BIT_DT  |      RB1       |  RA3  (PINO 2)
 *  CLK     |      RB2       |  RA2  (PINO 1)
 *  TR_OK   |      RB3       |  RA1  (PINO 18)
 *  D_OK    |      RB4       |  RA0  (PINO 17)
 *  RST     |      RB5       |  MCLR (PINO 4)
 */


/**
 * Função que aguarda o bit TR_OK assumir o valor especificado no parâmetro durante um tempo limite.
 * @param valor  valor que se espera que o bit TR_OK assuma (1 ou 0)
 * @return 1 se o bit TR_OK assumiu o valor do parâmetro valor. 0 caso contrário
 */
short int AguardaTR_OK(unsigned short int valor);

/**
 * Função que aguarda o bit D_OK assumir o valor especificado no parâmetro durante um tempo limite.
 * @param valor valor que se espera que o bit D_OK assuma (1 ou 0)
 * @return 1 se o bit D_OK assumiu o valor do parâmetro valor. 0 caso contrário.
 */
short int AguardaD_OK(unsigned short int valor);

/**
 * Realiza a transmissão de 1 bit para a placa de controle dos servos.
 * @param valor parâmetro que contém 8 bits, dos quais 1 será transmitido.
 * @param idbit posição do bit a ser transmitido do parâmetro valor. Aceita de 0 a 7,
 * @return 1 se a transmissão do bit for bem sucedida, 0 caso contrário.
 */
short int TransmiteBit(unsigned short int valor, int idbit);

/**
 * Transmite os tempos dos servos para a placa de controle dos servos.
 * @return 1 caso os tempos de todos os servos tenham sido transmitidos com sucesso. 0 caso contrário.
 */
short int TransmiteTemposServos();

#ifdef	__cplusplus
}
#endif

#endif	/* SERVOS_OLD_H */

