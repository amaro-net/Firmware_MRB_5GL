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

#include <xc.h>
#include "delays.h"
#include "porta_serial.h"
#include "bluetooth.h"
#include "protocolo.h"

void ConfigBluetoothPort()
{
    TRISCbits.TRISC2 = 0; // Configura o bit RC2 como sa�da
    TRISCbits.TRISC3 = 1; // Configura o bit RC3 como entrada
    TRISCbits.TRISC4 = 0; // Configura o bit RC2 como sa�da
    TRISCbits.TRISC5 = 1; // Configura o bit RC3 como entrada
    
    BIT_ACIONA_RECFG_BTH_INI = 1;
    BIT_ACIONA_TESTE_BTH_CONN = 1;
}

char BTGetResponse()
{
    if (response_rcvd) {
        response_rcvd = 0;
        return responseID;
    } else
        return 0;
}

/**
 *  The responses expected from the EasyBT module:<br/>
 *         CMD<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         END<br/>
 *         BTHCONNECT<br/>
 */
void RespConfBtStateMachine()
{
    // Process reception through state machine
    // We are parsing CMD<cr><lf>, AOK<cr><lf>, CONN<cr> and END<cr><lf> responses
    switch (BT_state)
    {
        case 0:
        {
            response = 0; // Clear response
            if (recCharUART1 == 'C') // We have 'C', it could be CMD<cr><lf>  or CONN
                BT_state = 1; // Expecting 'M' or 'N'
            if (recCharUART1 == 'A') // We have 'A', it could be AOK<cr><lf>
                BT_state = 11; // expecting 'O'
            if (recCharUART1 == 'E') // We have 'E', it could be END<cr><lf>
                BT_state = 31; // expecting 'N'
            break; // ...
        }

        case 1:
        {
            if (recCharUART1 == 'M')
                BT_state = 2;
            else if (recCharUART1 == 'O')
                BT_state = 22;
            else
                BT_state = 0;
            break;
        }

        case 2:
        {
            if (recCharUART1 == 'D') {
                response = BT_CMD; // CMD
                BT_state = 40;
            } else
                BT_state = 0;
            break;
        }

        case 11:
        {
            if (recCharUART1 == 'O')
                BT_state = 12;
            else
                BT_state = 0;
            break;
        }

        case 12:
        {
            if (recCharUART1 == 'K') {
                response = BT_AOK; // AOK
                BT_state = 40;
            } else
                BT_state = 0;
            break;
        }

        case 22:
        {
            if (recCharUART1 == 'N')
                BT_state = 23;
            else
                BT_state = 0;
            break;
        }

        case 23:
        {
            if (recCharUART1 == 'N') {
                response = BT_CONN; // BTHCONNECT
                response_rcvd = 1;
                responseID = response;
            }
            BT_state = 0;
            break;
        }

        case 31:
        {
            if (recCharUART1 == 'N')
                BT_state = 32;
            else
                BT_state = 0;
            break;
        }

        case 32:
        {
            if (recCharUART1 == 'D') {
                response = BT_END; // END
                BT_state = 40;
            } else
                BT_state = 0;
            break;
        }

        case 40:
        {
            if (recCharUART1 == 13)
                BT_state = 41;
            else
                BT_state = 0;
            break;
        }

        case 41:
        {
            if (recCharUART1 == 10) {
                response_rcvd = 1;
                responseID = response;
            }
            BT_state = 0;
            break;
        }

        default:
        {
            BT_state = 0;
            break;
        }
    }
}


/**
 * Para entrar no m�do de f�brica do m�dulo Bluetooth RN-41 � necess�rio enviar
 *     pela UART os seguintes comandos:
 *     Obs:(ap�s start-up do bluetooth temos 60 segundos
 *     para enviar o comando $$$ para entrar no modo de configura��o.)
 *
 *     Comandos:
 *     $$$   ->  Entra no modo de configura��o do Bluetooth. Devemos enviar este
 *     comando em at� 60 segundos ap�s start-up bluetooth.
 *
 *     SN,Nome_do_Bluetooth  -> Nome que daremos para o m�dulo Bluetooth. Quando
 *     fizermos a busca pelo PC dos dispositivos bluetooth ativos iremos encontrar
 *     nosso dispositivo pelo nome adotado atrav�s desse comando. (20 caracteres no m�ximo)
 *
 *     SO,BTH  -> String que aparece quando verificamos o modo extendido do status do dispositivo.
 *
 *     SM, 0  -> (0 = Slave | 1 = Master) Em uma rede bluetooth devemos ter um Master e um Slave.
 *     Normalmente deixamos o m�dulo como slave e mantemos o PC ou Tablet como Master. Saiba que
 *     voc� poder� manter a comunica��o entre dois bluetooth, por�m um dever� ser configurado
 *     como Master e o outro como Slave.
 *
 *     SA,1  -> (0 = Desabilita autentica��o | 1 = Habilita autentica��o). Este comando � utilizado
 *     quando existe a necessidade de senha para a conex�o com o dispositivo bluetooth.
 *
 *     SP,senha  -> (security PinCode -M�ximo 20 caracteres) Informe a senha de autentica��o para conex�o
 *     com o Bluetooth. Evite caracteres especiais, acentos e espa�os em brancos.
 *
 *     ---   -> (tr�s sinais de menos) Encerra o modo de configura��o do bluetooth. Ap�s esse comando
 *     � recomendado realimentar o bluetooth.
 */
void ConfigBluetooth()
{
    unsigned int Cont = 0;
    char bt_response = 0;

    PORTCbits.RC1 = 0; // Reseta o m�dulo bluetooth. o SW1.7 deve estar ON
    delay_ms(5);
    PORTCbits.RC1 = 1; // Desativa o reset e espera 100 ms para o m�dulo estabilizar
    delay_ms(100);

    PORTLEDS = 0;
    
    // Reconfigura o m�dulo buetooth apenas se o jumper de reconfigura��o estiver acionado
    if(RECFG_BTH_INI_ATIVADO == 0) 
        return;
    
    SendEnterUART1();
    SendCharToUart1('[');
    SendCharToUart1('B');
    SendCharToUart1('T');
    SendCharToUart1('C');
    SendCharToUart1('F');
    SendCharToUart1('G');
    SendCharToUart1(']');
    SendEnterUART1();
    
    do {
        PORTLEDS = 0x81;
        SendCharToUart1('$');
        SendCharToUart1('$');
        SendCharToUart1('$'); // Enter command mode
        //__delay_ms(500UL);
        delay_ms(1000);

        Cont++;
        if (Cont == 10) {            
            SendEnterUART1();
            SendCharToUart1('[');
            SendCharToUart1('B');
            SendCharToUart1('T');
            SendCharToUart1('C');
            SendCharToUart1('F');
            SendCharToUart1('G');
            SendCharToUart1(' ');
            SendCharToUart1('E');
            SendCharToUart1('R');
            SendCharToUart1('R');
            SendCharToUart1(']');
            SendEnterUART1();
            
            PORTLEDS = 0x00;
            Cont = 0;
            while (Cont < 5) {
                delay_ms(500);
                PORTLEDS = ~PORTLEDS;
                delay_ms(500);
                PORTLEDS = ~PORTLEDS;
                Cont++;
            }
            return;            
        }
    } while (BTGetResponse() != BT_CMD);
    PORTLEDS = 1; // P0
    
    do {
        SendCharToUart1('S');
        SendCharToUart1('N');
        SendCharToUart1(',');
        SendCharToUart1('n');
        SendCharToUart1('o');
        SendCharToUart1('v');
        SendCharToUart1('a');
        SendCharToUart1('p');
        SendCharToUart1('r');
        SendCharToUart1('o');
        SendCharToUart1('t');
        SendCharToUart1('o');
        SendCharToUart1('c');
        SendCharToUart1('l'); // Name of device
        SendCharToUart1(13); // CR
        delay_ms(500);
    } while (BTGetResponse() != BT_AOK);

    PORTLEDS = PORTLEDS << 1; // P1
    
    do {
        SendCharToUart1('S');
        SendCharToUart1('O');
        SendCharToUart1(',');
        SendCharToUart1('B');
        SendCharToUart1('T');        
        SendCharToUart1('H'); // Extended status string
        SendCharToUart1(13); // CR
        delay_ms(800);
    } while (BTGetResponse() != BT_AOK);

    PORTLEDS = PORTLEDS << 1; // P2
  
    do {
        SendCharToUart1('S');
        SendCharToUart1('M');
        SendCharToUart1(',');
        SendCharToUart1('0'); // Set mode (0 = slave, 1 = master, 2 = trigger, 3 = auto, 4 = DTR, 5 = ANY)
        SendCharToUart1(13); // CR
        delay_ms(800);
    } while (BTGetResponse() != BT_AOK);

    PORTLEDS = PORTLEDS << 1; // P3

    do {
        SendCharToUart1('S');
        SendCharToUart1('A');
        SendCharToUart1(',');
        SendCharToUart1('1'); // Autentica��o (1 - enable, 0 - disable)
        SendCharToUart1(13); // CR
        delay_ms(800);
    } while (BTGetResponse() != BT_AOK);

    PORTLEDS = PORTLEDS << 1; // P4

    do {
        SendCharToUart1('S');
        SendCharToUart1('P');
        SendCharToUart1(',');
        SendCharToUart1('1');
        SendCharToUart1('2');
        SendCharToUart1('3');
        SendCharToUart1('4'); // Security pin code (1234)
        SendCharToUart1(13); // CR
        delay_ms(800);
    } while (BTGetResponse() != BT_AOK);

    PORTLEDS = PORTLEDS << 1; // P5
    
    do {
        SendCharToUart1('-');
        SendCharToUart1('-');
        SendCharToUart1('-'); // sai do modo de comando
        SendCharToUart1(13); // CR
        delay_ms(800);
    } while (BTGetResponse() != BT_END);
    
    SendEnterUART1();
    SendCharToUart1('[');
    SendCharToUart1('B');
    SendCharToUart1('T');
    SendCharToUart1('C');
    SendCharToUart1('F');
    SendCharToUart1('G');
    SendCharToUart1(' ');
    SendCharToUart1('O');
    SendCharToUart1('K');
    SendCharToUart1(']');
    SendEnterUART1();
    
    PORTLEDS = PORTLEDS << 1; // P6

    if(TESTE_BTH_CONN_ATIVADO == 1)
    {
        SendEnterUART1();
        SendCharToUart1('[');
        SendCharToUart1('B');
        SendCharToUart1('T');
        SendCharToUart1('C');
        SendCharToUart1(']');
        SendEnterUART1();
    }
    
    while (TESTE_BTH_CONN_ATIVADO == 1 && bt_response != BT_CONN)
        bt_response = BTGetResponse(); // Aguarda Conex�o
    
    SendEnterUART1();
    if(bt_response == BT_CONN)
    {        
        SendCharToUart1('[');
        SendCharToUart1('B');
        SendCharToUart1('T');
        SendCharToUart1('C');
        SendCharToUart1(' ');
        SendCharToUart1('O');
        SendCharToUart1('K');
        SendCharToUart1(']');
    }
    else
    {
        SendCharToUart1('[');
        SendCharToUart1('B');
        SendCharToUart1('T');
        SendCharToUart1('C');
        SendCharToUart1(' ');
        SendCharToUart1('D');
        SendCharToUart1('S');
        SendCharToUart1('B');
        SendCharToUart1(']');
    }
    SendEnterUART1();
    
    PORTLEDS = PORTLEDS << 1; // P7
    delay_ms(1000);
}
