/*
    MRB-5GL firmware - manages the commands of MRB-5GL, a 5 DOF Robot Arm prototype

    Copyright (C) 2019  Amaro Duarte de Paula Neto

    This file is part of MRB-5GL firmware.

    MRB-5GL firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MRB-5GL firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MRB-5GL firmware.  If not, see <https://www.gnu.org/licenses/>.

    contact e-mail: amaro.net80@gmail.com


    Este arquivo � parte do firmware do MRB-5GL

    firmware do MRB-5GL � um software livre; voc� pode redistribu�-lo e/ou
    modific�-lo dentro dos termos da Licen�a P�blica Geral GNU como
    publicada pela Free Software Foundation (FSF); na vers�o 3 da
    Licen�a, ou (a seu crit�rio) qualquer vers�o posterior.

    firmware do MRB-5GL � distribu�do na esperan�a de que possa ser �til,
    mas SEM NENHUMA GARANTIA; sem uma garantia impl�cita de ADEQUA��O
    a qualquer MERCADO ou APLICA��O EM PARTICULAR. Veja a
    Licen�a P�blica Geral GNU para maiores detalhes.

    Voc� deve ter recebido uma c�pia da Licen�a P�blica Geral GNU junto
    com este programa, Se n�o, veja <http://www.gnu.org/licenses/>.
*/
#ifndef __18F45K22
#define __18F45K22 1
#endif

#include <xc.h>
#include <plib/usart.h>
#include "delays.h"
#include "porta_serial.h"


void SendCharUART1(char c)
{
    while(Busy1USART());
    putc1USART(c);
}


void SendStrUART1(const char* str)
{
    char *ptrStr = (char*)str;
    while (*ptrStr) {
        SendCharToUart1(*(ptrStr++));
    }
}

void SendEnterUART1()
{
    SendCharToUart1(13);
    SendCharToUart1(10);
}

void SendUnsignedIntToUART1(unsigned int num)
{
    SendCharToUart1((num/1000)%10+'0');
    SendCharToUart1((num/100)%10+'0');
    SendCharToUart1((num/10)%10+'0');
    SendCharToUart1(num%10+'0');
}

void IniciarUART1()
{
    ultimoRecCharUART1 = '?'; // Para indicar que nenhum caractere foi recebido ainda
    TRISC = 0b11000000;
    PORTC = 0b00000000;

    TRISCbits.RC3 = 0; // Reseta pino RTS do m�dulo Bluetooth visto que iremos
    PORTCbits.RC3 = 0; // utilizar somente os pinos RX e TX

    //    UART1_Init(9600);   // Initialize UART module at 9600 bps   (Do MikroC PRO)

    /* "Configura o baudrate a 9600"
     * As seguintes instru��es equivalem, no datasheet do PIC18F45K22,
       a configurar o baud rate a 2400, com 16 bits
       ver datasheet na p�g. 283, na �ltima tabela (SPBRGHx:SPBRGx = 832)*/
    //BSF         BAUDCON+0, 3, 0
    BAUDCONbits.BRG16 = 1;
    //MOVLW       3
    //MOVWF       SPBRGH+0
    SPBRGH = 3;
    //        MOVLW       64
    //        MOVWF       SPBRG+0
    SPBRG = 64;
    //        BSF         TXSTA+0, 2, 0
    TXSTAbits.BRGH = 1;
    /**/

    delay_ms(100); // Wait for UART module to stabilize

    //CONFIGURA��O DA INTERRUP��O SERIAL
    //**************************************************************
    RC1IF = 0; //Limpa Flag de Interrup��o na recep��o Serial
    IPR1bits.RC1IP = 1; //Define como Alta Prioridade de interrup��o
    PIE1bits.RC1IE = 1; //Habilita a interrup��o Serial

}


void IniciarUART2()
{
//    BSF         BAUDCON2+0, 3, 0
    BAUDCON2bits.BRG16 = 1;
    //BAUDCON2bits.BRG16 = 0;
//        MOVLW       3
//        MOVWF       SPBRGH2+0
    SPBRGH2 = 3;
    //SPBRGH2 = 0;
//        MOVLW       64
//        MOVWF       SPBRG2+0
    SPBRG2 = 64;
    //SPBRG2 =
//        BSF         TXSTA2+0, 2, 0
    TXSTA2bits.BRGH = 1;

    TXSTA2bits.TXEN = 1;

    TXSTA2bits.SYNC = 0;

    RCSTA2bits.SPEN = 1;

    RCSTA2bits.CREN = 1;

    delay_ms(100); // Wait for UART module to stabilize

    RC2IF = 0;
    IPR3bits.RC2IP = 1;
    PIE3bits.RC2IE = 1;
}


void SendByteUART2(char b)
{
    //SendStringToUart1("[AntesDoWhile]");
    while(Busy2USART());
    //delay_ms(100);
    //SendStringToUart1("[DepoisDoWhile_putc2USART]");
    putc2USART(b);
}