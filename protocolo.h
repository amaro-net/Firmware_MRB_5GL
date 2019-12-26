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
 * TRIS associado à configuração de entrada/saída dos pinos de acionamento dos leds
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
 * Tamanho máximo do buffer de decodificação dos comandos
 */
#define TAMANHO_BUFFER_PROTO 39

/**
 * Tamanho máximo da fila de buffers de recepção dos comandos
 */
#define TAMANHO_FILA_BUFFER_PROTO  6

/**
 * Índice correspondente a um canal indefinido na mini maestro 24
 */    
#define CANAL_INDEFINIDO  255

    
/**************************************************/
/** Variáveis de recepção de caracteres/comandos **/
/**************************************************/    

/**
 * Buffer de decodificação de comandos do protocolo de comunicação
 */
char bufferProto[TAMANHO_BUFFER_PROTO];

/**
 * Fila de buffers de recepção dos comandos
 */
char filaCircularBufferProto[TAMANHO_FILA_BUFFER_PROTO][TAMANHO_BUFFER_PROTO];

/**
 * Ponteiro auxiliar que "aponta" para uma posição de bufferProto ou de um 
 * elemento de filaCircularBufferProto
 */
char *pntBufferProto;

/**
 * Índice que aponta para a cabeça da fila circular de buffers de recepção dos comandos
 */
unsigned idxCabecaFila = 0;

/**
 * Índice que aponta para a cauda da fila circular de buffers de recepção dos comandos
 */
unsigned idxCaudaFila = 0;
/**
 * Variável que indica quando os caracteres de um comando estão sendo recebidos, ou seja,
 * quando um abre-colchete ([) foi recebido. Valor zero indica que nenhum [ foi recebido.
 * Esta variável vai a zero, também, quando um fecha-colchete (]) é recebido.
 */
unsigned char flagProtocolo = 0;

/**
 * Variável que recebe os caracteres da UART 1 quando CMD_mode = 0
 */
char rChar;

/**
 * Variável para indicar quando um caractere qualquer foi recebido pela UART 1.
 * Valor zero indica que nenhum caractere foi recebido pela UART 1.
 */
unsigned char recebeuChar = 0;


/*************************************************************************/
/** Variáveis para controle de feedback das juntas/garra e parada total **/
/*************************************************************************/

/**
 * Variável para indicar qual o tipo de comando de movimentação de servos, por quantidade, por foi acionado.
 * os tipos são 0-nenhum, 1-comando de 1 servo, 2-comando de vários servos. Esta variável foi criada,
 * a princípio, para informar às rotinas não bloqueantes de feedback dos servos qual o tipo de comando
 * foi acionado, para que seja dado o feedback adequado.
 */
char tipoCmdMovServosPorQt = 0;

/**
 * Ponteiro para o último comando de movimentação de servos (cmdJST, cmdCTZ, cmdGA, cmdGF)
 */
char* ultCmdMovServos;

/**
 * Variável para guardar qual o canal do servo que foi usado como parâmetro em algum comando
 * de acionamento de 1 servo. Só deve ser usada para comandos como o CTZ, GA ou GF.
 */
unsigned char canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;

/**
 * Flag para indicar se uma parada total foi solicitada via comando PRT.
 */
short paradaTotalSolicitada = 0;


/********************************************/
/** Variáveis de configuração do protocolo **/
/********************************************/

/**
 * Variável para indicar se os comandos JST, CTZ, GA e GF irão ou não mandar para a
 * UART 1 as posições que os servos envolvidos assumem durante os movimentos dos mesmos.
 * Valor zero indica que os comandos não irão enviar feedback durante o movimento dos
 * servos.
 */
char feedbackRastrServos = 0;

/**
 * Variável para indicar se os comandos dos servos irão ou não impedir que novos comandos
 * sejam recebidos enquanto os servos estiverem se movendo.
 */
char comandosServosBloqueantes = 0;

/**
 * Variável para indicar se os caracteres recebidos pela UART serão enviados de volta.
 * Ativado por padrão para permitir melhor interação com uso de programas de terminal
 * de porta serial.
 */
char ecoCaracteresAtivado = 1;


/**************************/
/** Strings dos comandos **/
/**************************/

/**
 * String que guarda os caracteres do "comando" IN1
 * (INicialização tipo 1). O comando em si não existe. Esta
 * string é usada apenas para ser exibida na resposta
 * da inicialização/reinicialização das posições dos servos
 * quando o comando enviado para a placa Mini Maestro 24 for
 * para apenas 1 target.
 */
char cmdIN1[4];

/**
 * String que guarda os caracteres do "comando" IN2
 * (INicialização tipo 2). O comando em si não existe. Esta
 * string é usada apenas para ser exibida na resposta
 * da inicialização/reinicialização das posições dos servos
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
 * (PaRada Total - comando para solicitar a parada total dos servos do braço robô)
 */
char cmdPRT[4];

/**
 * String que guarda os caracteres do comando GA
 * (Garra Abrir - abertura total da garra robótica)
 */
char cmdGA[3];

/**
 * String que guarda os caracteres do comando GF
 * (Garra Fechar - fechamendo total da garra robótica)
 */
char cmdGF[3];

/**
 * String que guarda os caracteres do comando CTZ
 * (CenTraliZar - juntas em posição neutra/central ou garra em posição semiaberta)
 */
char cmdCTZ[4];

/**
 * String que guarda os caracteres do comando JST
 * (Junta Setar Tempo - Setar valores de tempo de pulso dos servos)
 */
char cmdJST[4];

/**
 * String que guarda os caracteres do comando RPS
 * (RePouSo - Coloca o braço robô na posição configurada como repouso)
 */
char cmdRPS[4];

/**
 * String que guarda os caracteres do comando DSL
 * (DeSLiga - desliga as juntas do braço robô)
 */
char cmdDSL[4];

/**
 * String que guarda os caracteres do comando TMX
 * (Tempo MáXimo)
 */
char cmdTMX[4];

/**
 * String que guarda os caracteres do comando TMN
 * (Tempo MíNimo)
 */
char cmdTMN[4];

/**
 * String que guarda os caracteres do comando T90
 * (Tempo 90 graus - tempo de pulso da posição neutra dos servos)
 */
char cmdT90[4];

/**
 * String que guarda os caracteres do comando TRP
 * (Tempo RePouso - tempo de pulso da junta a ser considerado como posição de repouso)
 */
char cmdTRP[4];

/**
 * String que guarda os caracteres do comandos VEL
 * (VELocidade - velocidade de variação dos tempos dos servos)
 */
char cmdVEL[4];

/**
 * String que guarda os caracteres do comando ACL
 * (ACeLeração - aceleração da variação dos tempos dos servos)
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
 * (ReSeT - para resetar todo o hardware do braço robô)
 */
char cmdRST[4];

/**
 * String que guarda os caracteres do comando ECH
 * (Echo CHar - comando para habilitar/desabilitar o echo dos caracteres)
 */
char cmdECH[4];


/**
 * Tamanho máximo da string auxiliar str
 */
#define TAM_STR 20

/**
 * String auxiliar
 */
char str[TAM_STR];


/*************/
/** Funções **/
/*************/

/**
 * Realiza as inicializações necessárias ao protocolo de comunicação
 *
 */
void InitProtocolo();

/**
 * Função para reconhecer um comando recebido pela UART. Esta função reconhece apenas o nome do comando
 * mas não o formato dos seus parâmetros.
 * @param comando endereço da string contendo o nome do comando que será reconhecido (JST, LED etc).
 * @return 1 Se o comando for reconhecido, 0 caso contrário.
 */
short int Comando(const char* comando);

/**
 * Função que envia para a UART uma resposta do comando. Normalmente uma resposta de que
 * o comando foi reconhecido, ou uma resposta contendo valores.
 *
 * @param Matriz endereço da string contendo a resposta do comando.
 */
void RespostaComando(const char *str);

/**
 * Envia para a UART a resposta de que o formato do comando não foi reconhecido.
 * Em outras palavras, a resposta que esta função envia significa que o nome do comando
 * foi reconhecido, mas os seus parâmetros (e/ou o formato dos mesmos) não foram reconhecidos.
 * @param comando String contendo o comando que dará a resposta NAK
 */
void RespostaComandoNAK(const char *comando);

/**
 * Envia para a UART a resposta de que houve um erro de conexão entre a Ready For PIC e a placa
 * de controle dos servos. Em outras palavras, a resposta que esta função envia significa que
 * o comando e seus parâmetros foram reconhecidos, mas o dispositivo que o comando aciona
 * não respondeu adequadamente. Esta função só deve ser chamada para comandos que controlem algum
 * dispositivo, tais como os comandos JST, GA, GF e CTZ, que controlam a placa de controle dos
 * servos.
 * @param comando String contendo o comando que dará a resposta ERR.
 */
void RespostaComandoERR(const char *comando);

/**
 * Função para enviar uma resposta que indica que a junta especificada em um comando que trate juntas não
 * foi reconhecida.
 */
void RespostaJNT_NAK();

/**
 * Configura algum parâmetro de operação do servo (tempos máximo, mínimo ou central, velocidade, aceleracao etc), dependendo de qual comando chamar esta função.
 * Se o comando chamado não incluir o valor a ser setado como parâmetro, esta função trata, apenas, de exibir o valor setado.
 * A variável indexServo é setada neste comando caso sua execução seja bem sucedida.
 * @param cmdQueSeta Endereço da string contendo o comando que setou o valor (TMX, TMN, T90, VEL, ACL).
 * @return 1 se o comando for corretamente executado, 0 caso contrário
 */
short ConfigParamServo(char* cmdQueSeta);

/**
 * Encerra a recepção de caracteres para armazenamento em buffer pela UART 1.
 */
void EncerraRecepcaoDeCaracteresDoProtocolo();

/**
 * Função utilizada pela rotina de interrupção para receber os caracteres enviados pela UART e armazená-los
 * em buffer caso sejam enviados entre os caracteres limites da mensagem do protocolo ([ e ]).
 */
void RecebeCaracteresDoProtocolo();

/**
 * Obtém um comando da fila de recepção de comandos, se houver algum.
 * @return 1, se foi obtido comando na fila, 0 caso não haja nenhum novo comando na fila
 */
int ObterComandoDaFila();

/**
 * Envia para a UART string descrevendo o status de um servo, tais como tempo de pulso corrente, tempo máximo, tempo mínimo etc.
 * @param srv Ponteiro da estrutura (struct) contendo os dados referentes ao servo.
 */
void EnviaStatusServo(Servo* srv);

/**
 * Envia para a UART string descrevendo o status dos leds da placa Ready For PIC.
 */
void EnviaStatusLeds();

///**
// * Incrementa ou decrementa o valor do tempo de pulso corrente de um servo.
// * O sinal enviado pelo comando será avaliado nesta função, que avisará se o
// * mesmo foi ou não reconhecido.
// * @param sinal Ponteiro para a variável do tipo char que contém o sinal. Este
// * sinal deve ser +,- ou 0 (zero) para as juntas. Para a garra, o sinal pode
// * ser A (correspondente ao +), F(correspondente ao -), +, - ou 0 (zero).
// * @param idxServo índice do servo que terá seu tempo incrementado (0 a 5).
// * @return 1 se o sinal foi reconhecido, ou 0, caso contrário.
// */
//short int JIT(const char* sinal, int idxServo);

/**
 * Função que trata de setar um valor de configuração de um servo e/ou de obter
 * o valor dessa configuração e enviar para a UART 1 como resposta.
 * @param valorASetar ponteiro para a variável a ser setada.
 * @return 1 caso o comando (e os parâmetros do mesmo) sejam reconhecidos, 0 caso contrário
 */
short int VALSET(unsigned int* valorASetar);

/**
 * Função que seta o tempo de pulso do servo na estrutura que guarda os tempos dos servos. Caso o comando JST não venha
 * com parâmetros, esta função apenas trata de montar a resposta do comando para que seja enviado pela UART os valores
 * dos tempos de pulso do servo. Caso o JST venha apenas com 1 junta como parâmetro (sem o tempo), este comando monta a resposta
 * do comando JST para enviar apenas o tempo de pulso corrente da junta especificada.
 * @param ordemServo valor inteiro (a partir de zero) que representa em que "ordem" o servo foi especificado no comando, para
 * que a função possa encontrar a posição correta no buffer em que se encontra o valor a ser setado.
 * @param indiceServo índice do servo no vetor de servos.
 * @return 1 caso os parâmetros do comando sejam reconhecidos, 0 caso contrário.
 */
short int JST(unsigned int ordemServo, int indiceServo);

/**
 * Apaga o conteúdo de uma variável string.
 * @param str endereço da string a ter seu conteúdo apagado
 * @param tamanho tamanho da memória que a variável string ocupa. Pode ser usado sizeof(<nome_da_string>) como parâmetro na chamada.
 */
void LimparString(char* str, unsigned int tamanho);

/**
 * Converte um número inteiro não negativo para uma string.
 * @param num Número inteiro não negativo a ser convertido. O valor máximo deste número deve ser 9999.
 * @param str String que irá conter os dígitos do número em ASCII.
 */
void ConverteNumParaStr5(unsigned int num, char* str);

/**
 * Converte um número inteiro não negativo para uma string representando o valor do número inteiro em hexadecimal.
 * @param num Número inteiro não negativo a ser convertido. O valor máximo deste número deve ser 0xFFFF (65535).
 * @param str String que irá conter os dígitos hexadecimais do número convertido.
 */
void ConverteNumHexaParaStr5(unsigned int num, char* str);

/**
 * Função responsável por enviar as respostas dos comandos dos servos de forma não bloqueante.
 * Ou seja, quaisquer novos comandos podem ser recebidos durante o envio da resposta.
 * Esta função pode ser usada de forma bloqueante se ela for executada dentro de um loop que
 * teste se tipoCmdMovServosPorQt > 0.
 */
void RespEFeedbackMovServos();

/**
 * Função responsável pela decodificação dos comandos, acionamento geral e configurações.
 */
void DecodificaProtocolo();

#ifdef	__cplusplus
}
#endif

#endif	/* PROTOCOLO_H */

