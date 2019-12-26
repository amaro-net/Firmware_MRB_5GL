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


    Este arquivo é parte do firmware do MRB-5GL

    firmware do MRB-5GL é um software livre; você pode redistribuí-lo e/ou
    modificá-lo dentro dos termos da Licença Pública Geral GNU como
    publicada pela Free Software Foundation (FSF); na versão 3 da
    Licença, ou (a seu critério) qualquer versão posterior.

    firmware do MRB-5GL é distribuído na esperança de que possa ser útil,
    mas SEM NENHUMA GARANTIA; sem uma garantia implícita de ADEQUAÇÃO
    a qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a
    Licença Pública Geral GNU para maiores detalhes.

    Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
    com este programa, Se não, veja <http://www.gnu.org/licenses/>.
*/
/* 
 * File:   porta_serial.h
 * Author: Amaro
 *
 * Created on 26 de Janeiro de 2014, 15:25
 */

#ifndef PORTA_SERIAL_H
#define	PORTA_SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <plib/usart.h>


#define RECEBEU_CHAR_UART1   PIR1bits.RC1IF == 1

#define RECEBEU_BYTE_UART2   PIR3bits.RC2IF == 1

/**
 * Variável que armazena qualquer caractere recebido pela UART 1
 */
char recCharUART1;

/**
 * Variável que armazena o último caractere confirmado na UART1 (apenas alfanuméricos, abre colchete([) e fecha
 * colchete ([)
 */
char ultimoRecCharUART1;

/**
 * Váriável que armazena qualquer byte recebido pela UART 2
 */
char recByteUART2;

/**
 * Envia um caractere ASCII para a UART 1
 * @param c caractere ASCII a ser enviado
 */
void SendCharUART1(char c);

/**
 * Envia uma string para a UART 1
 * @param str string a ser enviada
 */
void SendStrUART1(const char* str);

/**
 * Envia os caracteres CR e LF (nesta ordem) para a UART 1
 */
void SendEnterUART1();

/**
 * Envia para a UART 1 um número inteiro sem sinal com até 4 dígitos, estes convertidos
 * para ASCII.
 */
void SendUnsignedIntToUART1(unsigned int num);

/**
 * Envia um byte para a 
 * @param b
 */
void SendByteUART2(char b);

/**
 * Inicializa a UART1 para o protocolo de comunicação da USB/Bluetooth
 */
void IniciarUART1();

/**
 * Inicializa a UART2 para a comunicação com a placa Mini Maestro de controle dos servos
 */
void IniciarUART2();

/**
 * Macro para enviar um caractere para a UART 1. Idêntica à função SendCharUART1()
 */
#define SendCharToUart1(x) SendCharUART1(x)

/**
 * Macro para enviar uma string para a UART 1. Idêntica à função SendStrUART1()
 */
#define SendStringToUart1(s) SendStrUART1(s)

/**
 * Macro para receber um caractere da UART 1. Idêntica à macro getc1USART
 */
#define GetCharFromUart1 getc1USART

/**
 * Macro para receber um caractere da UART 2. Idêntica à macro getc2USART
 */
#define GetByteFromUart2 getc2USART


#ifdef	__cplusplus
}
#endif



#endif	/* PORTA_SERIAL_H */

