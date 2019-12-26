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
 * File:   bluetooth.h
 * Author: Amaro
 *
 * Created on 26 de Janeiro de 2014, 15:25
 */

/*
No módulo Bluetooth "EasyBluetooth" manter os jumpers SW1.1 e SW4 na posição ON e os
demais na posição OFF.
J1 na posição 5V
PI03 OFF
P104 OFF
PI06 OFF
P107 ON

Conectar módulo no PORTC da placa Ready for PIC.

*/


#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Bit para acionamento da reconfiguração do módulo bluetooth
 */
#define BIT_ACIONA_RECFG_BTH_INI PORTCbits.RC2
    
/**
 * Bit para verificar se a reconfiguração do módulo bluetooth está ativada
 */
#define RECFG_BTH_INI_ATIVADO PORTCbits.RC3
    
/**
 * Bit para acionamento do teste de conexão do módulo bluetooth
 */
#define BIT_ACIONA_TESTE_BTH_CONN PORTCbits.RC4
    
/**
 * Bit para verificar se o teste de conexão do módulo bluetooth está ativado.
 */
#define TESTE_BTH_CONN_ATIVADO PORTCbits.RC5
    
/**
 * Constante para indicar que foi recebida uma resposta CMD do módulo EasyBluetooth
 */
const int BT_CMD = 1;

/**
 * Constante para indicar que foi recebida uma resposta AOK do módulo EasyBluetooth
 */
const int BT_AOK = 2;

/**
 * Constante para indicar que foi recebida uma resposta CONN do módulo EasyBluetooth
 */
const int BT_CONN = 3;

/**
 * Constante para indicar que foi recebida uma resposta END do módulo EasyBluetooth
 */
const int BT_END = 4;

/**
 * Variável para indicar o modo de comunicação pela UART 1. Valor 1 indica que está
 * havendo comunicação da UART 1 com o módulo EasyBluetooth para configuração deste.
 * Valor 0 indica que está havendo comunicação da UART 1 utilizando o protocolo dos
 * comandos do braço robô. Esta última pode ocorrer via módulo EasyBluetooth ou
 * via USB serial.
 */
char CMD_mode = 1;

/**
 * Variável da máquina de estados que reconhece as respostas do módulo EasyBluetooth
 * aos comandos de configuração
 */
char BT_state = 0;

/**
 * Variável para indicar quando uma resposta do módulo EasyBluetooth foi recebida. Valor
 * 0 indica que nenhuma resposta foi recebida ainda.
 */
char response_rcvd = 0;

/**
 * Variável que guarda um id representando qual resposta foi obtida do módulo EasyBluetooth
 * Os valores assumidos por esta variável podem ser qualquer das seguintes constantes:<br/>
 * BT_CMD: Quando é recebida a sequência de caracteres CMD(cr)(lf)<br/>
 * BT_AOK: Quando é recebida a sequência de caracteres AOK(cr)(lf)<br/>
 * BT_CONN: Quando é recebida a sequência de caracteres CONN(cr)<br/>
 * BT_END: Quando é recebida a sequência de caracteres END(cr)(lf)<br/>
 * Note que esta variável, ao contrário da variável response, guarda o id da resposta
 * completamente reconhecida com seus caracteres terminadores.
 */
char responseID = 0;

/**
 * Variável que guarda um id representando qual resposta foi obtida do módulo EasyBluetooth
 * Os valores assumidos por esta variável podem ser qualquer das seguintes constantes:<br/>
 * BT_CMD: Quando é recebida a sequência de caracteres CMD<br/>
 * BT_AOK: Quando é recebida a sequência de caracteres AOK<br/>
 * BT_CONN: Quando é recebida a sequência de caracteres CONN<br/>
 * BT_END: Quando é recebida a sequência de caracteres END<br/>
 * Note que esta variável, ao contrário da variável responseID, guarda  o id da resposta
 * reconhecida sem seus caracteres terminadores. Portanto, ela não deve ser usada por
 * nenhuma outra rotina além da máquina de estados na função RespConfBtStateMachine().
 */
char response = 0;

/**
 * Configuração da porta de comunicação com o módulo bluetooth
 */
void ConfigBluetoothPort();

/**
 * Obtém a resposta do módulo bluetooth. Mais precisamente, o valor de qualquer das constantes
 * que representam uma resposta do módulo bluetooth, ou zero caso não haja nenhuma resposta.
 * @return Retorna o valor de qualquer das seguintes constantes:<br/>
 * BT_CMD (1);<br/>
 * BT_AOK (2);<br/>
 * BT_CONN (3);<br/>
 * BT_END (4);<br/>
 * zero (0) caso não haja nenhuma resposta
 */
char BTGetResponse();

/**
 * Função que configura o módulo EasyBluetooth (módulo RN-171).
 * Para entrar no módo de fábrica do módulo Bluetooth RN-171 é necessário enviar
 * pela UART os seguintes comandos:<br/>
 *     Obs: após start-up do bluetooth temos 60 segundos
 *     para enviar o comando $$$ para entrar no modo de configuração.<br/>
 *
 *     Comandos:<br/>
 *     $$$   ->  Entra no modo de configuração do Bluetooth. Devemos enviar este
 *     comando em até 60 segundos após start-up bluetooth.<br/>
 *
 *     SN,Nome_do_Bluetooth  -> Nome que daremos para o módulo Bluetooth. Quando
 *     fizermos a busca pelo PC dos dispositivos bluetooth ativos iremos encontrar
 *     nosso dispositivo pelo nome adotado através desse comando. (20 caracteres no máximo)<br/>
 *
 *     SO,Slave  -> String que aparece quando verificamos o modo extendido do status do dispositivo.<br/>
 *
 *     SM, 0  -> (0 = Slave | 1 = Master) Em uma rede bluetooth devemos ter um Master e um Slave.
 *     Normalmente deixamos o módulo como slave e mantemos o PC ou Tablet como Master. Saiba que
 *     você poderá manter a comunicação entre dois bluetooth, porém um deverá ser configurado
 *     como Master e o outro como Slave.<br/>
 *
 *     SA,1  -> (0 = Desabilita autenticação | 1 = Habilita autenticação). Este comando é utilizado
 *     quando existe a necessidade de senha para a conexão com o dispositivo bluetooth.<br/>
 *
 *     SP,senha  -> (security PinCode -Máximo 20 caracteres) Informe a senha de autenticação para conexão
 *     com o Bluetooth. Evite caracteres especiais, acentos e espaços em brancos.<br/>
 *
 *     ---   -> (três sinais de menos) Encerra o modo de configuração do bluetooth. Após esse comando
 *     é recomendado realimentar o bluetooth.<br/>
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

