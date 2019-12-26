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
/* 
 * File:   bluetooth.h
 * Author: Amaro
 *
 * Created on 26 de Janeiro de 2014, 15:25
 */

/*
No m�dulo Bluetooth "EasyBluetooth" manter os jumpers SW1.1 e SW4 na posi��o ON e os
demais na posi��o OFF.
J1 na posi��o 5V
PI03 OFF
P104 OFF
PI06 OFF
P107 ON

Conectar m�dulo no PORTC da placa Ready for PIC.

*/


#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Bit para acionamento da reconfigura��o do m�dulo bluetooth
 */
#define BIT_ACIONA_RECFG_BTH_INI PORTCbits.RC2
    
/**
 * Bit para verificar se a reconfigura��o do m�dulo bluetooth est� ativada
 */
#define RECFG_BTH_INI_ATIVADO PORTCbits.RC3
    
/**
 * Bit para acionamento do teste de conex�o do m�dulo bluetooth
 */
#define BIT_ACIONA_TESTE_BTH_CONN PORTCbits.RC4
    
/**
 * Bit para verificar se o teste de conex�o do m�dulo bluetooth est� ativado.
 */
#define TESTE_BTH_CONN_ATIVADO PORTCbits.RC5
    
/**
 * Constante para indicar que foi recebida uma resposta CMD do m�dulo EasyBluetooth
 */
const int BT_CMD = 1;

/**
 * Constante para indicar que foi recebida uma resposta AOK do m�dulo EasyBluetooth
 */
const int BT_AOK = 2;

/**
 * Constante para indicar que foi recebida uma resposta CONN do m�dulo EasyBluetooth
 */
const int BT_CONN = 3;

/**
 * Constante para indicar que foi recebida uma resposta END do m�dulo EasyBluetooth
 */
const int BT_END = 4;

/**
 * Vari�vel para indicar o modo de comunica��o pela UART 1. Valor 1 indica que est�
 * havendo comunica��o da UART 1 com o m�dulo EasyBluetooth para configura��o deste.
 * Valor 0 indica que est� havendo comunica��o da UART 1 utilizando o protocolo dos
 * comandos do bra�o rob�. Esta �ltima pode ocorrer via m�dulo EasyBluetooth ou
 * via USB serial.
 */
char CMD_mode = 1;

/**
 * Vari�vel da m�quina de estados que reconhece as respostas do m�dulo EasyBluetooth
 * aos comandos de configura��o
 */
char BT_state = 0;

/**
 * Vari�vel para indicar quando uma resposta do m�dulo EasyBluetooth foi recebida. Valor
 * 0 indica que nenhuma resposta foi recebida ainda.
 */
char response_rcvd = 0;

/**
 * Vari�vel que guarda um id representando qual resposta foi obtida do m�dulo EasyBluetooth
 * Os valores assumidos por esta vari�vel podem ser qualquer das seguintes constantes:<br/>
 * BT_CMD: Quando � recebida a sequ�ncia de caracteres CMD(cr)(lf)<br/>
 * BT_AOK: Quando � recebida a sequ�ncia de caracteres AOK(cr)(lf)<br/>
 * BT_CONN: Quando � recebida a sequ�ncia de caracteres CONN(cr)<br/>
 * BT_END: Quando � recebida a sequ�ncia de caracteres END(cr)(lf)<br/>
 * Note que esta vari�vel, ao contr�rio da vari�vel response, guarda o id da resposta
 * completamente reconhecida com seus caracteres terminadores.
 */
char responseID = 0;

/**
 * Vari�vel que guarda um id representando qual resposta foi obtida do m�dulo EasyBluetooth
 * Os valores assumidos por esta vari�vel podem ser qualquer das seguintes constantes:<br/>
 * BT_CMD: Quando � recebida a sequ�ncia de caracteres CMD<br/>
 * BT_AOK: Quando � recebida a sequ�ncia de caracteres AOK<br/>
 * BT_CONN: Quando � recebida a sequ�ncia de caracteres CONN<br/>
 * BT_END: Quando � recebida a sequ�ncia de caracteres END<br/>
 * Note que esta vari�vel, ao contr�rio da vari�vel responseID, guarda  o id da resposta
 * reconhecida sem seus caracteres terminadores. Portanto, ela n�o deve ser usada por
 * nenhuma outra rotina al�m da m�quina de estados na fun��o RespConfBtStateMachine().
 */
char response = 0;

/**
 * Configura��o da porta de comunica��o com o m�dulo bluetooth
 */
void ConfigBluetoothPort();

/**
 * Obt�m a resposta do m�dulo bluetooth. Mais precisamente, o valor de qualquer das constantes
 * que representam uma resposta do m�dulo bluetooth, ou zero caso n�o haja nenhuma resposta.
 * @return Retorna o valor de qualquer das seguintes constantes:<br/>
 * BT_CMD (1);<br/>
 * BT_AOK (2);<br/>
 * BT_CONN (3);<br/>
 * BT_END (4);<br/>
 * zero (0) caso n�o haja nenhuma resposta
 */
char BTGetResponse();

/**
 * Fun��o que configura o m�dulo EasyBluetooth (m�dulo RN-171).
 * Para entrar no m�do de f�brica do m�dulo Bluetooth RN-171 � necess�rio enviar
 * pela UART os seguintes comandos:<br/>
 *     Obs: ap�s start-up do bluetooth temos 60 segundos
 *     para enviar o comando $$$ para entrar no modo de configura��o.<br/>
 *
 *     Comandos:<br/>
 *     $$$   ->  Entra no modo de configura��o do Bluetooth. Devemos enviar este
 *     comando em at� 60 segundos ap�s start-up bluetooth.<br/>
 *
 *     SN,Nome_do_Bluetooth  -> Nome que daremos para o m�dulo Bluetooth. Quando
 *     fizermos a busca pelo PC dos dispositivos bluetooth ativos iremos encontrar
 *     nosso dispositivo pelo nome adotado atrav�s desse comando. (20 caracteres no m�ximo)<br/>
 *
 *     SO,Slave  -> String que aparece quando verificamos o modo extendido do status do dispositivo.<br/>
 *
 *     SM, 0  -> (0 = Slave | 1 = Master) Em uma rede bluetooth devemos ter um Master e um Slave.
 *     Normalmente deixamos o m�dulo como slave e mantemos o PC ou Tablet como Master. Saiba que
 *     voc� poder� manter a comunica��o entre dois bluetooth, por�m um dever� ser configurado
 *     como Master e o outro como Slave.<br/>
 *
 *     SA,1  -> (0 = Desabilita autentica��o | 1 = Habilita autentica��o). Este comando � utilizado
 *     quando existe a necessidade de senha para a conex�o com o dispositivo bluetooth.<br/>
 *
 *     SP,senha  -> (security PinCode -M�ximo 20 caracteres) Informe a senha de autentica��o para conex�o
 *     com o Bluetooth. Evite caracteres especiais, acentos e espa�os em brancos.<br/>
 *
 *     ---   -> (tr�s sinais de menos) Encerra o modo de configura��o do bluetooth. Ap�s esse comando
 *     � recomendado realimentar o bluetooth.<br/>
 */
void ConfigBluetooth();

/**
 *  The responses expected from the EasyBT module:<br/>
 *         CMD<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         AOK<br/>
 *         END<br/>
 *         SlaveCONNECT<br/>         
 */
void RespConfBtStateMachine();


#ifdef	__cplusplus
}
#endif

#endif	/* BLUETOOTH_H */

