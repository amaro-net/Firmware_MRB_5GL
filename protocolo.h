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
 * File:   protocolo.h
 * Author: Amaro
 *
 * Created on 26 de Janeiro de 2014, 15:25
 */


#ifndef PROTOCOLO_H
#define	PROTOCOLO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "servos.h"

/**
 * PORT associado aos pinos de acionamento dos leds.
 */
#define PORTLEDS PORTB

/**
 * TRIS associado � configura��o de entrada/sa�da dos pinos de acionamento dos leds
 */
#define TRISLEDS TRISB

/**
 * Pino do led 0
 */
#define P0  PORTBbits.RB0

/**
 * Pino do led 1
 */
#define P1  PORTBbits.RB1

/**
 * Pino do led 2
 */
#define P2  PORTBbits.RB2

/**
 * Pino do led 3
 */
#define P3  PORTBbits.RB3

/**
 * Pino do led 4
 */
#define P4  PORTBbits.RB4

/**
 * Pino do led 5
 */
#define P5  PORTBbits.RB5

/**
 * Pino do led 6
 */
#define P6  PORTBbits.RB6

/**
 * Pino do led 7
 */
#define P7  PORTBbits.RB7

/**
 * Delay em milissegundos entre o envio de um comando de acionamento dos servos e o
 * monitoramento da resposta da Mini Maestro.
 */
#define DELAY_MS_RESPOSTA  500

/**
 * Tamanho m�ximo do buffer de decodifica��o dos comandos
 */
#define TAMANHO_BUFFER_PROTO 39

/**
 * Tamanho m�ximo da fila de buffers de recep��o dos comandos
 */
#define TAMANHO_FILA_BUFFER_PROTO  6

/**
 * �ndice correspondente a um canal indefinido na mini maestro 24
 */    
#define CANAL_INDEFINIDO  255

    
/**************************************************/
/** Vari�veis de recep��o de caracteres/comandos **/
/**************************************************/    

/**
 * Buffer de decodifica��o de comandos do protocolo de comunica��o
 */
char bufferProto[TAMANHO_BUFFER_PROTO];

/**
 * Fila de buffers de recep��o dos comandos
 */
char filaCircularBufferProto[TAMANHO_FILA_BUFFER_PROTO][TAMANHO_BUFFER_PROTO];

/**
 * Ponteiro auxiliar que "aponta" para uma posi��o de bufferProto ou de um 
 * elemento de filaCircularBufferProto
 */
char *pntBufferProto;

/**
 * �ndice que aponta para a cabe�a da fila circular de buffers de recep��o dos comandos
 */
unsigned idxCabecaFila = 0;

/**
 * �ndice que aponta para a cauda da fila circular de buffers de recep��o dos comandos
 */
unsigned idxCaudaFila = 0;
/**
 * Vari�vel que indica quando os caracteres de um comando est�o sendo recebidos, ou seja,
 * quando um abre-colchete ([) foi recebido. Valor zero indica que nenhum [ foi recebido.
 * Esta vari�vel vai a zero, tamb�m, quando um fecha-colchete (]) � recebido.
 */
unsigned char flagProtocolo = 0;

/**
 * Vari�vel que recebe os caracteres da UART 1 quando CMD_mode = 0
 */
char rChar;

/**
 * Vari�vel para indicar quando um caractere qualquer foi recebido pela UART 1.
 * Valor zero indica que nenhum caractere foi recebido pela UART 1.
 */
unsigned char recebeuChar = 0;


/*************************************************************************/
/** Vari�veis para controle de feedback das juntas/garra e parada total **/
/*************************************************************************/

/**
 * Vari�vel para indicar qual o tipo de comando de movimenta��o de servos, por quantidade, por foi acionado.
 * os tipos s�o 0-nenhum, 1-comando de 1 servo, 2-comando de v�rios servos. Esta vari�vel foi criada,
 * a princ�pio, para informar �s rotinas n�o bloqueantes de feedback dos servos qual o tipo de comando
 * foi acionado, para que seja dado o feedback adequado.
 */
char tipoCmdMovServosPorQt = 0;

/**
 * Ponteiro para o �ltimo comando de movimenta��o de servos (cmdJST, cmdCTZ, cmdGA, cmdGF)
 */
char* ultCmdMovServos;

/**
 * Vari�vel para guardar qual o canal do servo que foi usado como par�metro em algum comando
 * de acionamento de 1 servo. S� deve ser usada para comandos como o CTZ, GA ou GF.
 */
unsigned char canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;

/**
 * Flag para indicar se uma parada total foi solicitada via comando PRT.
 */
short paradaTotalSolicitada = 0;


/********************************************/
/** Vari�veis de configura��o do protocolo **/
/********************************************/

/**
 * Vari�vel para indicar se os comandos JST, CTZ, GA e GF ir�o ou n�o mandar para a
 * UART 1 as posi��es que os servos envolvidos assumem durante os movimentos dos mesmos.
 * Valor zero indica que os comandos n�o ir�o enviar feedback durante o movimento dos
 * servos.
 */
char feedbackRastrServos = 0;

/**
 * Vari�vel para indicar se os comandos dos servos ir�o ou n�o impedir que novos comandos
 * sejam recebidos enquanto os servos estiverem se movendo.
 */
char comandosServosBloqueantes = 0;

/**
 * Vari�vel para indicar se os caracteres recebidos pela UART ser�o enviados de volta.
 * Ativado por padr�o para permitir melhor intera��o com uso de programas de terminal
 * de porta serial.
 */
char ecoCaracteresAtivado = 1;


/**************************/
/** Strings dos comandos **/
/**************************/

/**
 * String que guarda os caracteres do "comando" IN1
 * (INicializa��o tipo 1). O comando em si n�o existe. Esta
 * string � usada apenas para ser exibida na resposta
 * da inicializa��o/reinicializa��o das posi��es dos servos
 * quando o comando enviado para a placa Mini Maestro 24 for
 * para apenas 1 target.
 */
char cmdIN1[4];

/**
 * String que guarda os caracteres do "comando" IN2
 * (INicializa��o tipo 2). O comando em si n�o existe. Esta
 * string � usada apenas para ser exibida na resposta
 * da inicializa��o/reinicializa��o das posi��es dos servos
 * quando o comando enviado para a placa Mini Maestro 24 for
 * para 2 ou mais targets.
 */
char cmdIN2[4];

/**
 * String que guarda os caracteres do comando LED
 * (acionamento/status dos leds)
 */
char cmdLED[4];

/**
 * String que guarda os caracteres do comando PRT
 * (PaRada Total - comando para solicitar a parada total dos servos do bra�o rob�)
 */
char cmdPRT[4];

/**
 * String que guarda os caracteres do comando GA
 * (Garra Abrir - abertura total da garra rob�tica)
 */
char cmdGA[3];

/**
 * String que guarda os caracteres do comando GF
 * (Garra Fechar - fechamendo total da garra rob�tica)
 */
char cmdGF[3];

/**
 * String que guarda os caracteres do comando CTZ
 * (CenTraliZar - juntas em posi��o neutra/central ou garra em posi��o semiaberta)
 */
char cmdCTZ[4];

/**
 * String que guarda os caracteres do comando JST
 * (Junta Setar Tempo - Setar valores de tempo de pulso dos servos)
 */
char cmdJST[4];

/**
 * String que guarda os caracteres do comando RPS
 * (RePouSo - Coloca o bra�o rob� na posi��o configurada como repouso)
 */
char cmdRPS[4];

/**
 * String que guarda os caracteres do comando DSL
 * (DeSLiga - desliga as juntas do bra�o rob�)
 */
char cmdDSL[4];

/**
 * String que guarda os caracteres do comando TMX
 * (Tempo M�Ximo)
 */
char cmdTMX[4];

/**
 * String que guarda os caracteres do comando TMN
 * (Tempo M�Nimo)
 */
char cmdTMN[4];

/**
 * String que guarda os caracteres do comando T90
 * (Tempo 90 graus - tempo de pulso da posi��o neutra dos servos)
 */
char cmdT90[4];

/**
 * String que guarda os caracteres do comando TRP
 * (Tempo RePouso - tempo de pulso da junta a ser considerado como posi��o de repouso)
 */
char cmdTRP[4];

/**
 * String que guarda os caracteres do comandos VEL
 * (VELocidade - velocidade de varia��o dos tempos dos servos)
 */
char cmdVEL[4];

/**
 * String que guarda os caracteres do comando ACL
 * (ACeLera��o - acelera��o da varia��o dos tempos dos servos)
 */
char cmdACL[4];

/**
 * String que guarda os caracteres do comando FRS
 * (Feedback de Rastreamento dos Servos)
 */
char cmdFRS[4];

/**
 * String que guarda os caracteres do comando CSB
 * (Comandos Servos Bloqueantes)
 */
char cmdCSB[4];

/**
 * String que guarda os caracteres do comando EMM
 * (Erros da Mini Maestro)
 */
char cmdEMM[4];

/**
 * String que guarda os caracteres do comando GTP
 * (GeT Position - comando Get Position para a Mini Maestro 24)
 */
char cmdGTP[4];

/**
 * String que guarda os caracteres do comando GMS
 * (Get Moving State - comando Get Moving State para a Mini Maestro 24)
 */
char cmdGMS[4];

/**
 * String que guarda os caracteres do comando STT
 * (STaTus - status dos servos)
 */
char cmdSTT[4];

/**
 * String que guarda os caracteres do comando RST
 * (ReSeT - para resetar todo o hardware do bra�o rob�)
 */
char cmdRST[4];

/**
 * String que guarda os caracteres do comando ECH
 * (Echo CHar - comando para habilitar/desabilitar o echo dos caracteres)
 */
char cmdECH[4];


/**
 * Tamanho m�ximo da string auxiliar str
 */
#define TAM_STR 20

/**
 * String auxiliar
 */
char str[TAM_STR];


/*************/
/** Fun��es **/
/*************/

/**
 * Realiza as inicializa��es necess�rias ao protocolo de comunica��o
 *
 */
void InitProtocolo();

/**
 * Fun��o para reconhecer um comando recebido pela UART. Esta fun��o reconhece apenas o nome do comando
 * mas n�o o formato dos seus par�metros.
 * @param comando endere�o da string contendo o nome do comando que ser� reconhecido (JST, LED etc).
 * @return 1 Se o comando for reconhecido, 0 caso contr�rio.
 */
short int Comando(const char* comando);

/**
 * Fun��o que envia para a UART uma resposta do comando. Normalmente uma resposta de que
 * o comando foi reconhecido, ou uma resposta contendo valores.
 *
 * @param Matriz endere�o da string contendo a resposta do comando.
 */
void RespostaComando(const char *str);

/**
 * Envia para a UART a resposta de que o formato do comando n�o foi reconhecido.
 * Em outras palavras, a resposta que esta fun��o envia significa que o nome do comando
 * foi reconhecido, mas os seus par�metros (e/ou o formato dos mesmos) n�o foram reconhecidos.
 * @param comando String contendo o comando que dar� a resposta NAK
 */
void RespostaComandoNAK(const char *comando);

/**
 * Envia para a UART a resposta de que houve um erro de conex�o entre a Ready For PIC e a placa
 * de controle dos servos. Em outras palavras, a resposta que esta fun��o envia significa que
 * o comando e seus par�metros foram reconhecidos, mas o dispositivo que o comando aciona
 * n�o respondeu adequadamente. Esta fun��o s� deve ser chamada para comandos que controlem algum
 * dispositivo, tais como os comandos JST, GA, GF e CTZ, que controlam a placa de controle dos
 * servos.
 * @param comando String contendo o comando que dar� a resposta ERR.
 */
void RespostaComandoERR(const char *comando);

/**
 * Fun��o para enviar uma resposta que indica que a junta especificada em um comando que trate juntas n�o
 * foi reconhecida.
 */
void RespostaJNT_NAK();

/**
 * Configura algum par�metro de opera��o do servo (tempos m�ximo, m�nimo ou central, velocidade, aceleracao etc), dependendo de qual comando chamar esta fun��o.
 * Se o comando chamado n�o incluir o valor a ser setado como par�metro, esta fun��o trata, apenas, de exibir o valor setado.
 * A vari�vel indexServo � setada neste comando caso sua execu��o seja bem sucedida.
 * @param cmdQueSeta Endere�o da string contendo o comando que setou o valor (TMX, TMN, T90, VEL, ACL).
 * @return 1 se o comando for corretamente executado, 0 caso contr�rio
 */
short ConfigParamServo(char* cmdQueSeta);

/**
 * Encerra a recep��o de caracteres para armazenamento em buffer pela UART 1.
 */
void EncerraRecepcaoDeCaracteresDoProtocolo();

/**
 * Fun��o utilizada pela rotina de interrup��o para receber os caracteres enviados pela UART e armazen�-los
 * em buffer caso sejam enviados entre os caracteres limites da mensagem do protocolo ([ e ]).
 */
void RecebeCaracteresDoProtocolo();

/**
 * Obt�m um comando da fila de recep��o de comandos, se houver algum.
 * @return 1, se foi obtido comando na fila, 0 caso n�o haja nenhum novo comando na fila
 */
int ObterComandoDaFila();

/**
 * Envia para a UART string descrevendo o status de um servo, tais como tempo de pulso corrente, tempo m�ximo, tempo m�nimo etc.
 * @param srv Ponteiro da estrutura (struct) contendo os dados referentes ao servo.
 */
void EnviaStatusServo(Servo* srv);

/**
 * Envia para a UART string descrevendo o status dos leds da placa Ready For PIC.
 */
void EnviaStatusLeds();

///**
// * Incrementa ou decrementa o valor do tempo de pulso corrente de um servo.
// * O sinal enviado pelo comando ser� avaliado nesta fun��o, que avisar� se o
// * mesmo foi ou n�o reconhecido.
// * @param sinal Ponteiro para a vari�vel do tipo char que cont�m o sinal. Este
// * sinal deve ser +,- ou 0 (zero) para as juntas. Para a garra, o sinal pode
// * ser A (correspondente ao +), F(correspondente ao -), +, - ou 0 (zero).
// * @param idxServo �ndice do servo que ter� seu tempo incrementado (0 a 5).
// * @return 1 se o sinal foi reconhecido, ou 0, caso contr�rio.
// */
//short int JIT(const char* sinal, int idxServo);

/**
 * Fun��o que trata de setar um valor de configura��o de um servo e/ou de obter
 * o valor dessa configura��o e enviar para a UART 1 como resposta.
 * @param valorASetar ponteiro para a vari�vel a ser setada.
 * @return 1 caso o comando (e os par�metros do mesmo) sejam reconhecidos, 0 caso contr�rio
 */
short int VALSET(unsigned int* valorASetar);

/**
 * Fun��o que seta o tempo de pulso do servo na estrutura que guarda os tempos dos servos. Caso o comando JST n�o venha
 * com par�metros, esta fun��o apenas trata de montar a resposta do comando para que seja enviado pela UART os valores
 * dos tempos de pulso do servo. Caso o JST venha apenas com 1 junta como par�metro (sem o tempo), este comando monta a resposta
 * do comando JST para enviar apenas o tempo de pulso corrente da junta especificada.
 * @param ordemServo valor inteiro (a partir de zero) que representa em que "ordem" o servo foi especificado no comando, para
 * que a fun��o possa encontrar a posi��o correta no buffer em que se encontra o valor a ser setado.
 * @param indiceServo �ndice do servo no vetor de servos.
 * @return 1 caso os par�metros do comando sejam reconhecidos, 0 caso contr�rio.
 */
short int JST(unsigned int ordemServo, int indiceServo);

/**
 * Apaga o conte�do de uma vari�vel string.
 * @param str endere�o da string a ter seu conte�do apagado
 * @param tamanho tamanho da mem�ria que a vari�vel string ocupa. Pode ser usado sizeof(<nome_da_string>) como par�metro na chamada.
 */
void LimparString(char* str, unsigned int tamanho);

/**
 * Converte um n�mero inteiro n�o negativo para uma string.
 * @param num N�mero inteiro n�o negativo a ser convertido. O valor m�ximo deste n�mero deve ser 9999.
 * @param str String que ir� conter os d�gitos do n�mero em ASCII.
 */
void ConverteNumParaStr5(unsigned int num, char* str);

/**
 * Converte um n�mero inteiro n�o negativo para uma string representando o valor do n�mero inteiro em hexadecimal.
 * @param num N�mero inteiro n�o negativo a ser convertido. O valor m�ximo deste n�mero deve ser 0xFFFF (65535).
 * @param str String que ir� conter os d�gitos hexadecimais do n�mero convertido.
 */
void ConverteNumHexaParaStr5(unsigned int num, char* str);

/**
 * Fun��o respons�vel por enviar as respostas dos comandos dos servos de forma n�o bloqueante.
 * Ou seja, quaisquer novos comandos podem ser recebidos durante o envio da resposta.
 * Esta fun��o pode ser usada de forma bloqueante se ela for executada dentro de um loop que
 * teste se tipoCmdMovServosPorQt > 0.
 */
void RespEFeedbackMovServos();

/**
 * Fun��o respons�vel pela decodifica��o dos comandos, acionamento geral e configura��es.
 */
void DecodificaProtocolo();

#ifdef	__cplusplus
}
#endif

#endif	/* PROTOCOLO_H */

