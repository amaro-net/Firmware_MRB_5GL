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
 * File:   main.c
 * Author: Amaro
 *
 * Created on 11 de Janeiro de 2014, 04:02
 */

#include "main.h"
//#include <p18f45k22.h>
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "porta_serial.h"
#include "bluetooth.h"
#include "protocolo.h"
#include "delays.h"
#include "servos.h"


/**
 * 
 */
void interrupt interrupcoes()
{
    if (CONTOU_29US)
    {
        ContaTempoEspera();
        
        INTCONbits.TMR0IF = 0; // Limpa o flag de interrupção do timer 0
        return;
    }
    else if (RECEBEU_CHAR_UART1)
    {
        recCharUART1 = GetCharFromUart1(); // Get received byte
        if(isdigit(recCharUART1) || isalpha(recCharUART1) || recCharUART1 == '[' || recCharUART1 == ']')
        {
            ultimoRecCharUART1 = recCharUART1;
            recCharUART1 = toupper(recCharUART1);
        }
        recebeuChar = ecoCaracteresAtivado; // 1 se o echo estiver ativado, 0 caso contrário

        if (CMD_mode)
        {
            // Máquina de estados de recepção de caracteres do módulo bluetooth.
            RespConfBtStateMachine();
            
            PIR1bits.RC1IF = 0; //Apaga Flag de interrupção serial da UART 1
            return;
        }
        else
        {
            /*
             * Recepção de caracteres componentes do protocolo de comunicação do braço robô.
             * Pode ser via bluetooth ou por USB-Serial.
             */
            RecebeCaracteresDoProtocolo();
            
            PIR1bits.RC1IF = 0; //Apaga Flag de interrupção serial da UART 1
            return;
        }
    }
    else if(RECEBEU_BYTE_UART2)
    {
        /*
         * Recepção de bytes da UART 2, que são respostas aos comandos enviados para a placa Mini Maestro.
         */
        recByteUART2 = GetByteFromUart2();
        RecebeBytesUART2();
        
        PIR3bits.RC2IF = 0; //Apaga Flag de interrupção serial da UART 1
        return;
    }
}
/**
 *
 */
void ConfigIO() {

    /* Speed to 64 MHz */
//    OSCCONbits.IRCF = 0b111;
//    PLLEN = 1;

    ANSELB = 0x00; // Configura PORTB pins como digital
    ANSELC = 0x00; // Configura PORTC pins como digital
    ANSELD = 0x00; // Configura PORTD pins como digital

    C1ON = 0; //Desabilita os comparadores internos
    C2ON = 0; //Desabilita os comparadores internos

    ConfigServos();

    TRISAbits.RA0 = 1; // Canal AD configurado como Entrada
    PORTAbits.RA0 = 1;
}

/**
 * Função principal
 * @return 0
 */
int main() {
    unsigned int i = 0;

    ConfigIO(); //Configura os pinos de IO
    PORTLEDS = 0x18;

    IniciarUART1();
    InitProtocolo();
    //CONFIGURAÇÃO GERAL DAS INTERRUPÇÕES
    //**************************************************************
    INTCONbits.GIEH = 1; //Habilita chave geral das interrupções
    //Habilita o vetor de alta prioridade de interrupcao
    INTCONbits.GIEL = 1; //Habilita somente o vetor de baixa prioridade de interrupção
    RCONbits.IPEN = 1; //1 ->Define 2 level de prioridade presente no PIC18Fxx
    //0->Herança da família PIC16F -> 1 vetor de interrup

    //**************************************************************
    
    delay_ms(5000);
    PORTLEDS = 0x42;
    ConfigBluetooth(); //Chama rotina de configuração do bluetooth
    CMD_mode = 0;

    i = 0;
    
    while (i < 5) {
        delay_ms(200);
        PORTLEDS = ~PORTLEDS;
        delay_ms(200);
        PORTLEDS = ~PORTLEDS;
        i++;
    }
    
    IniciarUART2();

    IniciarServos();

    PORTLEDS = 0b00000000;
    
    SendStringToUart1("[PRONTO]");
    SendEnterUART1();
    
    recebeuChar = 0; // Para não receber caractere espúrio ao iniciar o primeiro ciclo
    while (1) {
        /* Echo dos caracteres */
        if (recebeuChar) {
            recebeuChar = 0;
            if (recCharUART1 == 8) { // Se recebeu um backspace
                SendCharToUart1(8); // <BS> (BackSpace)
                SendCharToUart1(' '); // Espaço
                SendCharToUart1(8); // <BS> (BackSpace)
            } else if (recCharUART1 == 13) {
                SendEnterUART1();
            }
            else if(recCharUART1 == '?'){
                SendCharToUart1(ultimoRecCharUART1);
            }else
                SendCharToUart1(recCharUART1);
        }

        /* Tratamento do Buffer de Recepção */
        if (ObterComandoDaFila())
        {
            DecodificaProtocolo(); 
        }
        // Resposta e feedback dos servos - forma não bloqueante
        if (!comandosServosBloqueantes)
        {
            RespEFeedbackMovServos();
        }
    }
    return 0;
}

