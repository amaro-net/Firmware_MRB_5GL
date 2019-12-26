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
#ifndef __18F45K22
#define __18F45K22 1
#endif
#include <xc.h>
#include <string.h>
//#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "porta_serial.h"
#include "protocolo.h"
#include "delays.h"
#include "servos.h"

/**
 * Realiza as inicializações necessárias ao protocolo de comunicação
 *
 */
void InitProtocolo()
{    
    LimparString(cmdIN1, sizeof(cmdIN1));
    LimparString(cmdIN2, sizeof(cmdIN2));
    LimparString(cmdLED, sizeof(cmdLED));
    LimparString(cmdPRT, sizeof(cmdPRT));
    LimparString(cmdGA, sizeof(cmdGA));
    LimparString(cmdGF, sizeof(cmdGF));
    LimparString(cmdCTZ, sizeof(cmdCTZ));
    LimparString(cmdJST, sizeof(cmdJST));
    LimparString(cmdRPS, sizeof(cmdRPS));
    LimparString(cmdDSL, sizeof(cmdDSL));
    LimparString(cmdTMX, sizeof(cmdTMX));
    LimparString(cmdTMN, sizeof(cmdTMN));
    LimparString(cmdT90, sizeof(cmdT90));
    LimparString(cmdTRP, sizeof(cmdTRP));
    LimparString(cmdVEL, sizeof(cmdVEL));
    LimparString(cmdACL, sizeof(cmdACL));
    LimparString(cmdFRS, sizeof(cmdFRS));
    LimparString(cmdCSB, sizeof(cmdCSB));
    LimparString(cmdEMM, sizeof(cmdEMM));
    LimparString(cmdGTP, sizeof(cmdGTP));
    LimparString(cmdGMS, sizeof(cmdGMS));
    LimparString(cmdSTT, sizeof(cmdSTT));
    LimparString(cmdRST, sizeof(cmdRST));
    LimparString(cmdECH, sizeof(cmdECH));

    cmdIN1[0] = 'I';
    cmdIN1[1] = 'N';
    cmdIN1[2] = '1';

    cmdIN2[0] = 'I';
    cmdIN2[1] = 'N';
    cmdIN2[2] = '2';

    cmdLED[0] = 'L';
    cmdLED[1] = 'E';
    cmdLED[2] = 'D';
    
    cmdPRT[0] = 'P';
    cmdPRT[1] = 'R';
    cmdPRT[2] = 'T';

    cmdGA[0] = 'G';
    cmdGA[1] = 'A';

    cmdGF[0] = 'G';
    cmdGF[1] = 'F';

    cmdCTZ[0] = 'C';
    cmdCTZ[1] = 'T';
    cmdCTZ[2] = 'Z';

    cmdJST[0] = 'J';
    cmdJST[1] = 'S';
    cmdJST[2] = 'T';

    cmdRPS[0] = 'R';
    cmdRPS[1] = 'P';
    cmdRPS[2] = 'S';
    
    cmdDSL[0] = 'D';
    cmdDSL[1] = 'S';
    cmdDSL[2] = 'L';

    cmdTMX[0] = 'T';
    cmdTMX[1] = 'M';
    cmdTMX[2] = 'X';

    cmdTMN[0] = 'T';
    cmdTMN[1] = 'M';
    cmdTMN[2] = 'N';

    cmdT90[0] = 'T';
    cmdT90[1] = '9';
    cmdT90[2] = '0';

    cmdTRP[0] = 'T';
    cmdTRP[1] = 'R';
    cmdTRP[2] = 'P';

    cmdVEL[0] = 'V';
    cmdVEL[1] = 'E';
    cmdVEL[2] = 'L';
    
    cmdACL[0] = 'A';
    cmdACL[1] = 'C';
    cmdACL[2] = 'L';

    cmdFRS[0] = 'F';
    cmdFRS[1] = 'R';
    cmdFRS[2] = 'S';

    cmdCSB[0] = 'C';
    cmdCSB[1] = 'S';
    cmdCSB[2] = 'B';

    cmdEMM[0] = 'E';
    cmdEMM[1] = 'M';
    cmdEMM[2] = 'M';

    cmdGTP[0] = 'G';
    cmdGTP[1] = 'T';
    cmdGTP[2] = 'P';

    cmdGMS[0] = 'G';
    cmdGMS[1] = 'M';
    cmdGMS[2] = 'S';

    cmdSTT[0] = 'S';
    cmdSTT[1] = 'T';
    cmdSTT[2] = 'T';

    cmdRST[0] = 'R';
    cmdRST[1] = 'S';
    cmdRST[2] = 'T';

    cmdECH[0] = 'E';
    cmdECH[1] = 'C';
    cmdECH[2] = 'H';
    
    TRISLEDS = 0; // Configura o port dos leds como saida
    PORTLEDS = 0; // todos os leds off...
}

/**
 * Função para reconhecer um comando recebido pela UART. Esta função reconhece apenas o nome do comando
 * mas não o formato dos seus parâmetros.
 * @param comando String contendo o nome do comando que será reconhecido (JST, LED etc).
 * @return 1 Se o comando for reconhecido, 0 caso contrário.
 */
short int Comando(const char* comando)
{
    unsigned int i, tam_buffer, tam_comando;
    tam_buffer = strlen(bufferProto);
    tam_comando = strlen(comando);

    // se o tamanho do buffer for menor que o tamanho do comando, retorna falso
    if (tam_buffer <= tam_comando)
    {
        return 0;
    }

    for (i=1; i < tam_buffer-1 && i-1 < tam_comando; i++)
    {
        // Se achar um caractere no buffer diferente do que tem no comando, retorna falso
        if(bufferProto[i] != comando[i-1])
        {
            return 0;
        }
    }
    // Chegando aqui, o comando contido no buffer é o comando requerido
    return 1;
}


/**
 * Função que envia para a UART uma resposta do comando. Normalmente uma resposta de que
 * o comando foi reconhecido, ou uma resposta contendo valores.
 *
 * @param str string contendo a resposta do comando.
 */
void RespostaComando(const char *str) {
    char* pntStr = (char*) str;
    SendEnterUART1();

    while (*pntStr != ']' && *pntStr != 13 && *pntStr != 0x00) {
        SendCharToUart1(*(pntStr++));
    }

    SendCharToUart1(*pntStr);
    SendEnterUART1();
}

/**
 * Envia para a UART a resposta de que o formato do comando não foi reconhecido.
 * Em outras palavras, a resposta que esta função envia significa que o nome do comando
 * foi reconhecido, mas os seus parâmetros (e/ou o formato dos mesmos) não foram reconhecidos.
 * @param comando String contendo o comando que dará a resposta NAK
 */
void RespostaComandoNAK(const char *comando)
{
    unsigned int i, tam_comando;
    tam_comando = strlen(comando);
    SendEnterUART1();
    SendCharToUart1('[');
    for(i = 0; i < tam_comando; i++)
        SendCharToUart1(comando[i]);
    SendCharToUart1(' ');
    SendCharToUart1('N');
    SendCharToUart1('A');
    SendCharToUart1('K');
    SendCharToUart1(']');
    SendEnterUART1();
}

/**
 * Envia para a UART a resposta de que houve um erro de conexão entre a Ready For PIC e a placa
 * de controle dos servos. Em outras palavras, a resposta que esta função envia significa que
 * o comando e seus parâmetros foram reconhecidos, mas o dispositivo que o comando aciona
 * não respondeu adequadamente. Esta função só deve ser chamada para comandos que controlem algum
 * dispositivo, tais como os comandos JST, GA, GF e CTZ, que controlam a placa de controle dos
 * servos.
 * @param comando String contendo o comando que dará a resposta ERR.
 */
void RespostaComandoERR(const char *comando)
{
    unsigned int i, tam_comando;
    tam_comando = strlen(comando);
    SendEnterUART1();
    SendCharToUart1('[');
    for(i = 0; i < tam_comando; i++)
        SendCharToUart1(comando[i]);
    SendCharToUart1(' ');
    SendCharToUart1('E');
    SendCharToUart1('R');
    SendCharToUart1('R');
    SendCharToUart1(']');
    SendEnterUART1();
}

/**
 * Função para enviar uma resposta que indica que a junta especificada em um comando que trate juntas não
 * foi reconhecida.
 */
void RespostaJNT_NAK()
{
    SendEnterUART1();
    SendCharToUart1('[');
    SendCharToUart1('J');
    SendCharToUart1('N');
    SendCharToUart1('T');
    SendCharToUart1(' ');
    SendCharToUart1('N');
    SendCharToUart1('A');
    SendCharToUart1('K');
    SendCharToUart1(']');
    SendEnterUART1();
}

/**
 * Limpa uma string.
 * @param str
 * @param tamanho
 */
void LimparString(char* str, unsigned int tamanho)
{
    unsigned int i;
    for (i=0; i < tamanho; i++)
    {
        str[i] = (char)0;
    }
}

/**
 * 
 * @param num
 * @param str
 */
void ConverteNumParaStr5(unsigned int num, char* str)
{
    int j;
    LimparString(str, 5);
    for(j=3; j >= 0; j--)
    {
        str[j] = (num % 10) + '0';
        num = num / 10;
    }
}

void ConverteNumHexaParaStr5(unsigned int num, char* str)
{
    int j;
    char digito;
    LimparString(str, 5);
    for(j=3; j >= 0; j--)
    {
        digito = num % 16;
        if(digito > 9)
            str[j] = digito - 10 + 'A';
        else
            str[j] = digito + '0';
        num = num / 10;
    }
}

/**
 *
 *
 */
void EnviaStatusLeds()
{
    SendCharToUart1('L');
    SendCharToUart1('E');
    SendCharToUart1('D');
    SendCharToUart1('S');
    SendCharToUart1(':');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('0');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P0 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('1');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P1 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('2');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P2 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('3');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P3 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');
    
    SendEnterUART1();

    SendCharToUart1(' ');
    SendCharToUart1(' ');
    SendCharToUart1(' ');
    SendCharToUart1(' ');
    SendCharToUart1(' ');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('4');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P4 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('5');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P5 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('6');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P6 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');

    SendCharToUart1('P');
    SendCharToUart1('7');
    SendCharToUart1(':');
    SendCharToUart1(' ');
    SendCharToUart1(P7 + '0');
    SendCharToUart1(' ');
    SendCharToUart1('|');
    SendCharToUart1(' ');
    SendEnterUART1();
}

/**
 *
 * @param srv
 */
void EnviaStatusServo(Servo *srv)
{
    int i;
    char strNum[5];

    for(i = 0; i < strlen(srv->nome); i++)
    {
        SendCharToUart1(srv->nome[i]);
    }
    SendCharToUart1(':');
    SendEnterUART1();
    SendCharToUart1(' ');
    SendCharToUart1('T');
    SendCharToUart1('M');
    SendCharToUart1('P');
    SendCharToUart1(' ');
    SendCharToUart1('P');
    SendCharToUart1('U');
    SendCharToUart1('L');
    SendCharToUart1('S');
    SendCharToUart1('O');
    SendCharToUart1(' ');
    SendCharToUart1('=');
    SendCharToUart1(' ');

    ConverteNumParaStr5(srv->tempoPulso, strNum);

    SendStringToUart1(strNum);

    SendEnterUART1();
    SendCharToUart1(' ');
    SendCharToUart1('T');
    SendCharToUart1('M');
    SendCharToUart1('P');
    SendCharToUart1(' ');

    if (strcmp(srv->sigla,"GR") == 0)
    {
        SendCharToUart1('G');
        SendCharToUart1('A');
        SendCharToUart1('R');
        SendCharToUart1('R');
        SendCharToUart1('A');
        SendCharToUart1(' ');
        SendCharToUart1('F');
        SendCharToUart1('E');
        SendCharToUart1('C');
        SendCharToUart1('H');
        SendCharToUart1('A');
        SendCharToUart1('D');
        SendCharToUart1('A');
    }
    else
    {
        SendCharToUart1('M');
        SendCharToUart1('I');
        SendCharToUart1('N');
        SendCharToUart1('I');
        SendCharToUart1('M');
        SendCharToUart1('O');
    }
    SendCharToUart1(':');
    SendCharToUart1(' ');

    ConverteNumParaStr5(srv->tempoMin, strNum);

    SendStringToUart1(strNum);

    SendEnterUART1();
    SendCharToUart1(' ');
    SendCharToUart1('T');
    SendCharToUart1('M');
    SendCharToUart1('P');
    SendCharToUart1(' ');

    if (strcmp(srv->sigla,"GR") == 0)
    {
        SendCharToUart1('G');
        SendCharToUart1('A');
        SendCharToUart1('R');
        SendCharToUart1('R');
        SendCharToUart1('A');
        SendCharToUart1(' ');
        SendCharToUart1('A');
        SendCharToUart1('B');
        SendCharToUart1('E');
        SendCharToUart1('R');
        SendCharToUart1('T');
        SendCharToUart1('A');
    }
    else
    {
        SendCharToUart1('M');
        SendCharToUart1('A');
        SendCharToUart1('X');
        SendCharToUart1('I');
        SendCharToUart1('M');
        SendCharToUart1('O');
    }
    SendCharToUart1(':');
    SendCharToUart1(' ');

    ConverteNumParaStr5(srv->tempoMax, strNum);

    SendStringToUart1(strNum);

    SendEnterUART1();
    SendCharToUart1(' ');
    SendCharToUart1('T');
    SendCharToUart1('M');
    SendCharToUart1('P');
    SendCharToUart1(' ');

    if (strcmp(srv->sigla,"GR") == 0)
    {
        SendCharToUart1('G');
        SendCharToUart1('A');
        SendCharToUart1('R');
        SendCharToUart1('R');
        SendCharToUart1('A');
        SendCharToUart1(' ');
        SendCharToUart1('S');
        SendCharToUart1('E');
        SendCharToUart1('M');
        SendCharToUart1('I');
        SendCharToUart1(' ');
        SendCharToUart1('A');
        SendCharToUart1('B');
        SendCharToUart1('E');
        SendCharToUart1('R');
        SendCharToUart1('T');
        SendCharToUart1('A');
    }
    else
    {
        SendCharToUart1('9');
        SendCharToUart1('0');
        SendCharToUart1(' ');
        SendCharToUart1('G');
        SendCharToUart1('R');
        SendCharToUart1('A');
        SendCharToUart1('U');
        SendCharToUart1('S');
    }
    SendCharToUart1(':');
    SendCharToUart1(' ');

    ConverteNumParaStr5(srv->tempoCtz, strNum);

    SendStringToUart1(strNum);
    
    SendEnterUART1();
}

short int VALSET(unsigned int* valorASetar)
{
    int i;
    unsigned int valorObtido;
    char milhar, centena, dezena, unidade;
    char strValorSet[5];
    short int flagErro = 0;
    /**
     * [TMX<JX><TEMPO_4_DIGITOS>]
     * [TMXGR<TEMPO_4_DIGITOS>]
     * [TMX<JX>]
     * [TMXGR]
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     */
    if (strlen(bufferProto) == 11){
        milhar = bufferProto[6];
        centena = bufferProto[7];
        dezena = bufferProto[8];
        unidade = bufferProto[9];
        if(isdigit(milhar) && isdigit(centena) && isdigit(dezena) && isdigit(unidade)){
            valorObtido = 1000 * (milhar - '0') + 100 * (centena - '0') + 10 * (dezena - '0') + (unidade - '0');
            if (valorObtido == 0)
                *valorASetar = 1;
            else
                *valorASetar = valorObtido;
        }
        else
            flagErro = 1;
        
    }
    else if (strlen(bufferProto) != 7)
        flagErro = 1;

    if(flagErro)
    {
        SendEnterUART1();
        SendCharToUart1('[');
        SendCharToUart1(bufferProto[1]);
        SendCharToUart1(bufferProto[2]);
        SendCharToUart1(bufferProto[3]);
        SendCharToUart1(' ');
        SendCharToUart1('N');
        SendCharToUart1('A');
        SendCharToUart1('K');
        SendCharToUart1(']');
        SendEnterUART1();
        return 0;
    }

    ConverteNumParaStr5(*valorASetar, strValorSet);
    
    for(i=0; i < 4; i++)
    {
        bufferProto[i+6] = strValorSet[i];
    }
    bufferProto[10] = ']'; // Para o caso do tamanho do buffer ser diferente de 11
    //RespostaComando(bufferProto);
    return 1;
}

/**
 *
 * @param buffer
 * @param ordemServo
 * @param srv
 * @return
 */
short int JST(unsigned int ordemServo, int indiceServo)
{
    // ordemServo = 0, 1, 2, 3, 4, 5 (garra)
    unsigned int valor, index, tamBuffer;
    char milhar, centena, dezena, unidade;
    int i;
    char strValorSet[5];

    index = ordemServo * 5;
    tamBuffer = strlen(bufferProto);
    /**
     * [JSTA0010B0010C0040D0056E0041G0050]
     * [JSTC0040]
     * [JSTG0090B0031]
     * [JST]
     * [JSTA]
     * [JSTB]
     * [JSTG]
     */
    // tamanho máximo = 35 - tamanhos 5 6 10 15 20 25 30 35
    if(tamBuffer >= 10  &&  tamBuffer % 5 == 0) // Aqui o valor do tempo é setado, se for o caso
    {
        milhar = bufferProto[5+index];
        centena = bufferProto[6+index];
        dezena = bufferProto[7+index];
        unidade = bufferProto[8+index];
        if(isdigit(milhar) && isdigit(centena) && isdigit(dezena) && isdigit(unidade))
        {
            valor = 1000 * (milhar - '0') + 100 * (centena - '0') + 10 * (dezena - '0') + (unidade - '0');
            if (valor != 0)
            {
                if (valor > servos[indiceServo].tempoMax) valor = servos[indiceServo].tempoMax;
                if (valor < servos[indiceServo].tempoMin) valor = servos[indiceServo].tempoMin;
            }
            servos[indiceServo].tempoPulso = valor;
        }
        else
        {
            return 0;
        }
    }
    
    ConverteNumParaStr5(servos[indiceServo].tempoPulso, strValorSet);
    
    for(i=0; i < 4; i++)
    {
        bufferProto[5+i+index] = strValorSet[i];
    }

    bufferProto[4+index] = servos[indiceServo].idJST;

    return 1;
}


short ConfigParamServo(char* cmdQueSeta)
{
    unsigned int* pntTempoASetar;
    
    if(bufferProto[4] == 'J') // J0 a J4
    {
        if(bufferProto[5] >= '0' && bufferProto[5] <= '4')
        {
            indexServo = bufferProto[5] - '0';
        }
        else
        {
            RespostaJNT_NAK();
            return 0;
        }
    }
    else if (bufferProto[4] == 'G' && bufferProto[5] == 'R')
    {
        indexServo = 5;
    }
    else
    {
        RespostaComandoNAK(cmdQueSeta);
        return 0;
    }

    if(strcmp(cmdQueSeta,"TMX") == 0)
    {
        pntTempoASetar = &servos[indexServo].tempoMax;
    }
    else if(strcmp(cmdQueSeta,"TMN") == 0)
    {
        pntTempoASetar = &servos[indexServo].tempoMin;
    }
    else if(strcmp(cmdQueSeta,"T90") == 0)
    {
        pntTempoASetar = &servos[indexServo].tempoCtz;
    }
    else if (strcmp(cmdQueSeta, "TRP") == 0)
    {
        pntTempoASetar = &servos[indexServo].tempoRepouso;
    }
    else if(strcmp(cmdQueSeta,"VEL") == 0)
    {
        pntTempoASetar = &servos[indexServo].velTmpPulso;
    }
    else if(strcmp(cmdQueSeta,"ACL") == 0)
    {
        pntTempoASetar = &servos[indexServo].acelTmpPulso;
    }
    return VALSET(pntTempoASetar);
}

void EncerraRecepcaoDeCaracteresDoProtocolo()
{
    flagProtocolo = 0; //reinicia flag do protocolo
    if(++idxCabecaFila == TAMANHO_FILA_BUFFER_PROTO)
        idxCabecaFila = 0;
}

/**
 * Função utilizada pela rotina de interrupção para receber os caracteres enviados pela UART e armazená-los
 * em buffer caso sejam enviados entre os caracteres limites da mensagem do protocolo ([ e ]).
 */
void RecebeCaracteresDoProtocolo()
{
    rChar = recCharUART1;
    if (flagProtocolo == 0 && rChar == '[') //é o Byte de início de comando?
    {
        LimparString(filaCircularBufferProto[idxCabecaFila], TAMANHO_BUFFER_PROTO);
        pntBufferProto = &filaCircularBufferProto[idxCabecaFila]; //Salva endereço do buffer no Ponteiro.
        *pntBufferProto = rChar; // armazena na posição corrente do buffer o caractere recebido
        ++pntBufferProto; // Avança 1 posição do buffer
        flagProtocolo = 1; //Seta flag que indica que o protocolo começou
    }
    else if (flagProtocolo == 1 && rChar != '?') // se verdadeiro indica que já recebemos o byte de início e
    {                            // devemos armazenar o restante dos caracteres
        if (rChar == 8) // BackSpace (para quando estiver acessando a UART 1 via terminal de porta serial)
        {
            pntBufferProto--; // Recua 1 posição no buffer
            if(pntBufferProto == filaCircularBufferProto[idxCabecaFila]) flagProtocolo = 0;
            *pntBufferProto = (char)0; // Null - caractere terminador da string "Apaga" o caractere do buffer
        }
        else if (rChar == ']') // Chegou o byte de finalização do comando, logo devemos concluir a recepção dos caracteres
        {
            *pntBufferProto = rChar; 
            
            EncerraRecepcaoDeCaracteresDoProtocolo();
        }
        else if(rChar == '[') // Se chegou outro byte de início do comando
        {
            // Reinicia a recepção dos caracteres
            LimparString(filaCircularBufferProto[idxCabecaFila], TAMANHO_BUFFER_PROTO);
            pntBufferProto = &filaCircularBufferProto[idxCabecaFila]; //Salva endereço do buffer no Ponteiro.
            *pntBufferProto = rChar; // armazena na posição corrente do buffer o caractere recebido
            ++pntBufferProto; // Avança 1 posição do buffer
        }
        else
        {
            *pntBufferProto = rChar;
            
            if (pntBufferProto < &filaCircularBufferProto[idxCabecaFila][TAMANHO_BUFFER_PROTO - 2])
                ++pntBufferProto;
            else
                EncerraRecepcaoDeCaracteresDoProtocolo();            
        }
    }    
}

int ObterComandoDaFila()
{
    if(idxCaudaFila == idxCabecaFila)
        return 0; // Não tem comando novo na fila
    
    LimparString(bufferProto, sizeof(bufferProto));
    strcpy(bufferProto, filaCircularBufferProto[idxCaudaFila]);
    
    if(++idxCaudaFila == TAMANHO_FILA_BUFFER_PROTO)
        idxCaudaFila = 0;
    
    return 1; // Foi obtido comando da fila
}

void ParadaTotal()
{
    unsigned int posicao;
    
    for(char canal = 0; canal < 6; canal++)
    {
        posicao = GetPosition(canal);
        SetTarget(canal, posicao);
    }
    
}

void RetomarVelocidadesAposParadaTotal1Servo()
{
    if(paradaTotalSolicitada)
    {
        paradaTotalSolicitada = 0;
        for(char canal = 0; canal < 6; canal++)
        {
            SetSpeed(canal, servos[canal].velTmpPulso);
        }
    }
}

void ResetaVariaveisDeFeedBack()
{
    tipoCmdMovServosPorQt = 0;
    ultCmdMovServos = (char*)NULL;
    canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;
}

void RespEFeedbackMovServos()
{
    unsigned char result;
    unsigned int posicao;
    
    switch(tipoCmdMovServosPorQt)
    {
        case 1: // Acionamento de 1 servo apenas
        {
            result = GetMovingState();
            if (result == 0x01) // Em movimento            
            {
                switch(feedbackRastrServos)
                {
                case 0: // sem feedback
                    if(paradaTotalSolicitada)
                        ParadaTotal();
                    break;
                case 1: // Tempos dos servos
                    if(paradaTotalSolicitada)
                        ParadaTotal();
                    LimparString(str, TAM_STR);
                    posicao = GetPosition(canalCmd1ServoNaoBloq);
                    SendEnterUART1();
                    SendStringToUart1("[MOV");
                    SendStringToUart1(servos[canalCmd1ServoNaoBloq].sigla);
                    SendUnsignedIntToUART1(posicao);
                    SendCharToUart1(']');
                    break;
                case 2: // Sinal de movimento
                    if(paradaTotalSolicitada)
                        ParadaTotal();
                    SendEnterUART1();
                    SendStringToUart1("[MOV]");
                    break;
                }
            }
            else if (result == 0x00) // Parado
            {
                CommitTempos();
                RetomarVelocidadesAposParadaTotal1Servo();
                SendEnterUART1();
                SendCharToUart1('[');
                SendStringToUart1(ultCmdMovServos);
                if(ultCmdMovServos == cmdCTZ || ultCmdMovServos == cmdIN1)
                {
                    SendStringToUart1(servos[canalCmd1ServoNaoBloq].sigla);
                }
                posicao = GetPosition(canalCmd1ServoNaoBloq);
                SendUnsignedIntToUART1(posicao);
                SendCharToUart1(']');
                SendEnterUART1();
                ResetaVariaveisDeFeedBack();
            }
            else // Erro
            {
                RollbackTempos();
                RetomarVelocidadesAposParadaTotal1Servo();
                RespostaComandoERR(ultCmdMovServos);
                if (result != 0xFF)
                {
                    ConverteNumHexaParaStr5(result, str);
                    SendEnterUART1();
                    SendCharToUart1('[');
                    SendStringToUart1("ERR");
                    SendCharToUart1(str[0]);
                    SendCharToUart1(str[1]);
                    SendCharToUart1(']');
                    SendEnterUART1();
                }
                ResetaVariaveisDeFeedBack();
            }
            break;
        }
        
        case 2: // Acionamento de vários servos
        {
            unsigned char canal;

            result = GetMovingState();
            if (result == 0x01) // Em movimento            
            {
                switch(feedbackRastrServos)
                {
                case 0: // Sem feedback
                    if(paradaTotalSolicitada)
                    {
                        ParadaTotal();
                    }
                    break;

                case 1: // Feedback com valores de posição (tempos) dos servos
                    SendEnterUART1();
                    SendStringToUart1("[MOV");
                    for(canal = 0; canal < 6; canal++)
                    {
                        posicao = GetPosition(canal);
                        if(paradaTotalSolicitada)
                            SetTarget(canal, posicao);
                        SendCharToUart1(servos[canal].idJST);
                        SendUnsignedIntToUART1(posicao);
                    }

                    SendCharToUart1(']');
                    break;

                case 2: // feedback aviso de movimento apenas
                    if(paradaTotalSolicitada)
                    {
                        ParadaTotal();
                    }
                    SendStringToUart1("[MOV]");
                    SendEnterUART1();
                    break;
                }
            }
            else if (result == 0x00) // Parado
            {
                CommitTempos();
                RetomarVelocidadesAposParadaTotal1Servo();
                SendEnterUART1();
                SendCharToUart1('[');
                SendStringToUart1(ultCmdMovServos);
                for(canal = 0; canal < 6; canal++)
                {
                    posicao = GetPosition(canal);
                    SendCharToUart1(servos[canal].idJST);
                    SendUnsignedIntToUART1(posicao);
                }
                SendCharToUart1(']');
                SendEnterUART1();
                ResetaVariaveisDeFeedBack();
            }
            else // Erro
            {
                RollbackTempos();
                RetomarVelocidadesAposParadaTotal1Servo();
                RespostaComandoERR(ultCmdMovServos);
                if (result != 0xFF)
                {
                    ConverteNumHexaParaStr5(result, str);
                    SendEnterUART1();
                    SendCharToUart1('[');
                    SendStringToUart1("ERR");
                    SendCharToUart1(str[0]);
                    SendCharToUart1(str[1]);
                    SendCharToUart1(']');
                    SendEnterUART1();
                }
                ResetaVariaveisDeFeedBack();
            }
            break;
        }
        
        default:
            break;
    }
}


/**
 * Função responsável pela decodificação dos comandos, acionamento geral e configurações.
 */
void DecodificaProtocolo()
{
    //unsigned int Leitura_AD;
    //unsigned char ADStr[10];
    int i;
    short idxSrv;

    /*
     * [LED11111111]
     * [LED10101010]
     * [LED00000000]
     * [LED]
    RESPOSTA: O mesmo comando aplicado, se especificado os bits, ou o comando com os bits no estado corrente

    Acende ou apaga os leds do PORTLEDS
     */
    if (Comando(cmdLED)) {
        if (strlen(bufferProto) == 13) { // SETA OS LEDS
            unsigned char bits_leds, i, c;
            bits_leds = 0;

            for (i = 0; i < 8; i++) {
                c = bufferProto[i + 4];
                if (c == '0' || c == '1') {
                    bits_leds = 2 * bits_leds + (c - '0');
                } else {
                    bits_leds = 2 * bits_leds;
                    bufferProto[i + 4] = '0';
                }
            }

            PORTLEDS = bits_leds;
            RespostaComando(bufferProto);
        }else if (strlen(bufferProto) == 5){ // DEVOLVE O STATUS DOS LEDS
            SendEnterUART1();
            SendCharToUart1('[');
            SendCharToUart1('L');
            SendCharToUart1('E');
            SendCharToUart1('D');
            SendCharToUart1(P7 + '0');
            SendCharToUart1(P6 + '0');
            SendCharToUart1(P5 + '0');
            SendCharToUart1(P4 + '0');
            SendCharToUart1(P3 + '0');
            SendCharToUart1(P2 + '0');
            SendCharToUart1(P1 + '0');
            SendCharToUart1(P0 + '0');
            SendCharToUart1(']');
            SendEnterUART1();
        }
        else{ // COMANDO [LED] NÃO RECONHECIDO
            RespostaComandoNAK(cmdLED);
        }
    }
    /**
     * [PRT]
     * PaRada Total = Para total e imediatamente o braço robô
     * Resposta: [PRT OK] se o braço robô já estiver parado
     *           
     */
    else if(Comando(cmdPRT))
    {
        if(GetMovingState() == 1)
        {
            paradaTotalSolicitada = 1;
            for(char canal = 0; canal < 6; canal++)
            {
                SetSpeed(canal, 1);
            }
        }
        else
        {
            RespostaComando("[PRT OK]");
        }
        
    }    
    /**
     * [GA]
     * Garra Abrir = Abre a garra completamente
     * Resposta: [GAXXXX] onde XXXX = tempo de pulso do servo da garra (em us) ao estar completamente aberta.
     *           Se o feedback for tipo 1 (tempo dos servos): [MOVGRXXXX], onde XXXX = posição corrente do servo (em us)
     *           Se o feedback for tipo 2 (sinal de movimento): [MOV]
     *           Os feedbacks sempre virão antes da resposta [GAXXXX]. Se a garra já estiver completamente aberta,
     *           apenas a resposta [GAXXXX] será enviada.
     */
    else if (Comando(cmdGA))
    {
        if(strlen(bufferProto) == 4)
        {
            servos[5].tempoPulso = servos[5].tempoMax;

            SetTarget(5,servos[5].tempoPulso);
            
            if(tipoCmdMovServosPorQt < 2)
            {
                if(canalCmd1ServoNaoBloq != 5 && canalCmd1ServoNaoBloq != CANAL_INDEFINIDO)
                {
                    tipoCmdMovServosPorQt = 2;
                    ultCmdMovServos = cmdJST;
                    canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;
                }
                else
                {
                    ultCmdMovServos = cmdGA;
                    tipoCmdMovServosPorQt = 1;
                    canalCmd1ServoNaoBloq = 5;
                }
                delay_ms(DELAY_MS_RESPOSTA);
            }
            
            if (comandosServosBloqueantes)
            {
                while(tipoCmdMovServosPorQt > 0)
                {
                    RespEFeedbackMovServos();
                }
            }
        }
        else
        {
            RespostaComandoNAK(cmdGA);
        }
    }
    /**
     * [GF]
     * Garra Fechar = Fecha a garra completamente
     * * Resposta: [GFXXXX] onde XXXX = tempo de pulso do servo da garra (em us) ao estar completamente fechada.
     *           Se o feedback for tipo 1 (tempo dos servos): [MOVGRXXXX], onde XXXX = posição corrente do servo (em us)
     *           Se o feedback for tipo 2 (sinal de movimento): [MOV]
     *           Os feedbacks sempre virão antes da resposta [GFXXXX]. Se a garra já estiver completamente fechada,
     *           apenas a resposta [GFXXXX] será enviada.
     */
    else if (Comando(cmdGF))
    {
        if(strlen(bufferProto) == 4)
        {
            servos[5].tempoPulso = servos[5].tempoMin;

            SetTarget(5, servos[5].tempoPulso);

            if(tipoCmdMovServosPorQt < 2)
            {
                if(canalCmd1ServoNaoBloq != 5 && canalCmd1ServoNaoBloq != CANAL_INDEFINIDO)
                {
                    tipoCmdMovServosPorQt = 2;
                    ultCmdMovServos = cmdJST;
                    canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;
                }
                else
                {
                    ultCmdMovServos = cmdGF;
                    tipoCmdMovServosPorQt = 1;
                    canalCmd1ServoNaoBloq = 5;
                }
                delay_ms(DELAY_MS_RESPOSTA);
            }
            
            if (comandosServosBloqueantes)
            {
                while(tipoCmdMovServosPorQt > 0)
                {
                    RespEFeedbackMovServos();
                }
            }
        }
        else
        {
            RespostaComandoNAK(cmdGF);
        }
    }

    /**
     * [CTZ<JX>]
     * [CTZGR]
     *
     * Sendo <JX> = J0,...,J4
     *        GR = Garra
     * Centraliza em 90 graus a junta correspondente ou deixa a garra semiaberta
     * Resposta: [CTZ<JX>XXXX]
     *           [CTZGRXXXX]
     *           onde XXXX é o tempo de pulso corrente do servo da junta (ou da garra) em us.
     *           Se o feedback for do tipo 1 (tempo dos servos):
     *             [MOV<JX>XXXX]
     *             [MOVGRXXXX]
     *           Se o feedback for do tipo 2(sinal de movimento): [MOV]
     *
     */
    else if (Comando(cmdCTZ))
    {
        if(strlen(bufferProto) != 7)
        {
            RespostaComandoNAK(cmdCTZ);
        }
        else if(bufferProto[4] == 'J')
        {
            if (bufferProto[5] >= '0' && bufferProto[5] <= '4')
            {
                unsigned char canal;
                canal = bufferProto[5] - '0';
                servos[canal].tempoPulso = servos[canal].tempoCtz;
                
                SetTarget(canal, servos[canal].tempoPulso);
                                
                if(tipoCmdMovServosPorQt < 2)
                {                
                    if(canalCmd1ServoNaoBloq != canal && canalCmd1ServoNaoBloq != CANAL_INDEFINIDO)
                    {
                        tipoCmdMovServosPorQt = 2;
                        ultCmdMovServos = cmdJST;
                        canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;
                    }
                    else
                    {
                        ultCmdMovServos = cmdCTZ;
                        tipoCmdMovServosPorQt = 1;
                        canalCmd1ServoNaoBloq = canal;
                    }                
                    delay_ms(DELAY_MS_RESPOSTA);
                }
                
                if (comandosServosBloqueantes)
                {
                    while(tipoCmdMovServosPorQt > 0)
                    {
                        RespEFeedbackMovServos();
                    }
                }
            }
            else
            {
                RespostaJNT_NAK();
            }
        }
        else if (bufferProto[4] == 'G' && bufferProto[5] == 'R')
        {
            servos[5].tempoPulso = servos[5].tempoCtz;
            
            SetTarget(5, servos[5].tempoPulso);

            if(tipoCmdMovServosPorQt < 2)
            {
                if(canalCmd1ServoNaoBloq != 5 && canalCmd1ServoNaoBloq != CANAL_INDEFINIDO)
                {
                    tipoCmdMovServosPorQt = 2;
                    ultCmdMovServos = cmdJST;
                    canalCmd1ServoNaoBloq = CANAL_INDEFINIDO;
                }
                else
                {
                    ultCmdMovServos = cmdCTZ;
                    tipoCmdMovServosPorQt = 1;
                    canalCmd1ServoNaoBloq = 5;
                }
                delay_ms(DELAY_MS_RESPOSTA);
            }
            if (comandosServosBloqueantes)
            {
                while(tipoCmdMovServosPorQt > 0)
                {
                    RespEFeedbackMovServos();
                }
            }
        }
        else
        {
            RespostaJNT_NAK();
        }
    }
    
    /**
     * [JSTA0010B0010C0040D0056E0041G0050]
     * [JSTC0040]
     * [JSTG0090B0031]
     * [JST]
     * [JSTA]
     * [JSTB]
     * [JSTG]
     *
     * RESPOSTA: O mesmo comando, com os valores de tempo das juntas especificadas realmente setados, se for aplicado com as juntas e o tempo.
     *           Os tempos de todas as juntas, se for aplicado apenas o [JST]
     *           O tempo da junta especificada, se for aplicado [JST<J>], onde <J> = A,...,E,G
     * onde:
     * JST: Comando de Junta Setar Tempo (Tempo de pulso dos servos das juntas e de abertura/fechamento da garra são setados dos valores
     *       especificados no pacote)
     * A, B, C, D, E: Representam as juntas. Na resposta, o tempo estará sempre entre o tempo máximo e o tempo mínimo configurados.
     * G: Representa a garra (abertura). Na resposta, o tempo estará sempre entre o tempo máximo e o tempo mínimo configurados para a garra.
     * Se for aplicado o comando JST com pelo menos 1 junta e um tempo de pulso:
     *    - Se o feedback for do tipo 1 (tempos dos servos): [MOVAXXXXBXXXXCXXXXDXXXXEXXXXGXXXX]
     *      onde XXXX é tempo do servo corrente correspondente
     *    - Se o feedback for do tipo 2 (sinal de movimento): [MOV]
     *    Se todos os servos especificados no comando JST (com junta e tempo de pulso) já estiverem
     *    nas posições especificadas (ou em suas posições limites, de modos que não podem se mover além delas)
     *    apenas a resposta [JSTAXXXXBXXXXCXXXXDXXXXEXXXXGXXXX] (com os valores de tempo em us no lugar dos XXXX) será enviada.
     */
    else if(Comando(cmdJST))
    {
        unsigned int i, iServo, tamBuffer;

        tamBuffer = strlen(bufferProto); // tamanho máximo = 35 - tamanhos 5 6 10 15 20 25 30 35
        if (tamBuffer > 35 || tamBuffer < 5 || (tamBuffer % 5 != 0 && tamBuffer != 6))
        {
            RespostaComandoNAK(cmdJST);
        }
        else if(tamBuffer == 5) //[JST]
        {
            for(i=0; i < 6; i++){
                /* Ao chamar o [JST] sem nenhum parâmetro, nenhum valor será setado nas variáveis */
                JST(i, i);
            }
            bufferProto[34] = ']';
            RespostaComando(bufferProto);
        }
        else if(tamBuffer == 6) // [JST<J>], onde <J> = A,...,E,G
        {
            /* Ao chamar o JST com uma junta (sem o valor do tempo) será mostrado o tempo correspondente da junta especificada,
               sem que nenhum valor seja setado */           
            if(bufferProto[4] >= 'A' && bufferProto[4] <= 'E')
            {
                idxSrv = bufferProto[4] - 'A';
                JST(0, idxSrv);
                bufferProto[9] = ']';
                RespostaComando(bufferProto);
            }
            else if(bufferProto[4] == 'G')
            {
                JST(0, 5);
                bufferProto[9] = ']';
                RespostaComando(bufferProto);
            }
            else
            {
                RespostaJNT_NAK();
            }
        }
        else // JST com as juntas e os valores de tempo
        {
            unsigned char primeiroCanal;

            i = 0;
            iServo = 4;
            primeiroCanal = 5;
            while(i < 6 && iServo < tamBuffer - 1)
            {
                if(bufferProto[iServo] >= 'A' && bufferProto[iServo] <= 'E')
                {
                    idxSrv = bufferProto[iServo] - 'A';
                    if ( primeiroCanal > (unsigned char)idxSrv ) primeiroCanal = (unsigned char)idxSrv;
                    if(! JST(i, idxSrv) ) i = 6;
                }
                else if(bufferProto[iServo] == 'G')
                {
                    if(! JST(i, 5) ) i = 6;
                }
                else
                {
                    i = 6;
                }

                i++;
                //iServo = 4+5*i;
                iServo += 5;
            }

            if (i > 6)
            {
                RespostaComandoNAK(cmdJST);
            }
            else 
            {
                // Transmissão dos tempos para a placa Mini Maestro.
                SetMultipleTargets(6-primeiroCanal, primeiroCanal);
                
                ultCmdMovServos = cmdJST;
                tipoCmdMovServosPorQt = 2;
                delay_ms(DELAY_MS_RESPOSTA);
                if (comandosServosBloqueantes)
                {
                    while(tipoCmdMovServosPorQt > 0)
                    {
                        RespEFeedbackMovServos();
                    }
                }
            }
        }
    }

    /*
     * [RPS]
     * Resposta: [RPSAXXXXBXXXXCXXXXDXXXXEXXXXGXXXX], onde XXXX é o valor em microssegundos da posição
     * dos servos
     *
     * Coloca o braço robô na posição de repouso configurada com o comando TRP. Notar que a garra não é manipulada neste caso.
     * - Se o feedback for do tipo 1 (tempos dos servos): [MOVAXXXXBXXXXCXXXXDXXXXEXXXXGXXXX]
     *   onde XXXX é tempo do servo corrente correspondente
     * - Se o feedback for do tipo 2 (sinal de movimento): [MOV]
     * Se todos os servos estiverem na posição de repouso,
     * apenas a resposta [JSTAXXXXBXXXXCXXXXDXXXXEXXXXGXXXX] (com os valores de tempo em us no lugar dos XXXX) será enviada.
     */
    else if(Comando(cmdRPS))
    {
        unsigned int i;

        if(strlen(bufferProto) == 5)
        {
            for(i = 0; i < 5; i++)
            {
                servos[i].tempoPulso = servos[i].tempoRepouso;
            }

            // Transmissão dos tempos para a placa Mini Maestro.
            SetMultipleTargets(5, 0);

            ultCmdMovServos = cmdRPS;
            tipoCmdMovServosPorQt = 2;
            delay_ms(DELAY_MS_RESPOSTA);
            if (comandosServosBloqueantes)
            {
                while(tipoCmdMovServosPorQt > 0)
                {
                    RespEFeedbackMovServos();
                }
            }
        }
        else
        {
            RespostaComandoNAK(cmdRPS);
        }
    }
    
    /*
     * [DSL]
     * Resposta: [DSLA0000B0000C0000D0000E0000]
     *
     * Desliga as juntas e a garra do braço robô, de forma que não haja controle
     * de posicionamento nas mesmas.
     */
    else if(Comando(cmdDSL))
    {
        unsigned int i;

        if(strlen(bufferProto) == 5)
        {
            for(i = 0; i < 6; i++)
            {
                servos[i].tempoPulso = 0;
            }

            // Transmissão dos tempos para a placa Mini Maestro.
            SetMultipleTargets(6, 0);

            ultCmdMovServos = cmdDSL;
            tipoCmdMovServosPorQt = 2;
            delay_ms(DELAY_MS_RESPOSTA);
            if (comandosServosBloqueantes)
            {
                while(tipoCmdMovServosPorQt > 0)
                {
                    RespEFeedbackMovServos();
                }
            }
        }
        else
        {
            RespostaComandoNAK(cmdDSL);
        }
    }

    /**
     * [TMX<JX><TEMPO_4_DIGITOS>]
     * [TMXGR<TEMPO_4_DIGITOS>]
     * [TMX<JX>]
     * [TMXGR]
     * Onde:
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * <TEMPO_4_DIGITOS>: tempo de pulso para o servomotor em us
     *
     * RESPOSTA: [TMX<JX><TEMPO_4_DIGITOS_SETADO>]
     *           [TMXGR<TEMPO_4_DIGITOS_SETADO>]
     * onde <TEMPO_4_DIGITOS_SETADO>: Tempo máximo setado (não ultrapassa
     *                                o limite máximo de 50% do periodo da onda).
     *
     */
    else if(Comando(cmdTMX))
    {
        if(ConfigParamServo(cmdTMX))
        {
            RespostaComando(bufferProto);
        }
    }

    /**
     * [TMN<JX><TEMPO_4_DIGITOS>]
     * [TMNGR<TEMPO_4_DIGITOS>]
     * [TMN<JX>]
     * [TMNGR]
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * <TEMPO_4_DIGITOS>: tempo de pulso para o servomotor em us
     *
     * RESPOSTA: [TMN<JX><TEMPO_4_DIGITOS_SETADO>]
     *           [TMNGR<TEMPO_4_DIGITOS_SETADO>]
     * onde <TEMPO_4_DIGITOS_SETADO>: Tempo mínimo realmente setado
     */
    else if(Comando(cmdTMN))
    {
        if (ConfigParamServo(cmdTMN))
        {
            RespostaComando(bufferProto);
        }
    }

    /**
     * [T90<JX><TEMPO_4_DIGITOS>]
     * [T90GR<TEMPO_4_DIGITOS>]
     * [T90<JX>]
     * [T90GR]
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * <TEMPO_4_DIGITOS>: tempo de pulso para o servomotor em us
     *
     * RESPOSTA: [TMN<JX><TEMPO_4_DIGITOS_SETADO>]
     *           [TMNGR<TEMPO_4_DIGITOS_SETADO>]
     * onde <TEMPO_4_DIGITOS_SETADO>: Tempo do servo realmente setado quando estiver na posição considerada central
     */
    else if(Comando(cmdT90))
    {
        if (ConfigParamServo(cmdT90))
        {
            RespostaComando(bufferProto);
        }

    }

    /**
     * [TRP<JX><TEMPO_4_DIGITOS>]
     * [TRPGR<TEMPO_4_DIGITOS>]
     * [TRP<JX>]
     * [TRPGR]
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * <TEMPO_4_DIGITOS>: tempo de pulso para o servomotor em us
     *
     * RESPOSTA: [TRP<JX><TEMPO_4_DIGITOS_SETADO>]
     *           [TMNGR<TEMPO_4_DIGITOS_SETADO>]
     * onde <TEMPO_4_DIGITOS_SETADO>: Tempo do servo realmente setado considerado como a posição de repouso
     */
    else if(Comando(cmdTRP))
    {
        if (ConfigParamServo(cmdTRP))
        {
            RespostaComando(bufferProto);
        }
    }

    /**
     * [VEL<JX><VEL_4_DIGITOS>]
     * [VELGR<VEL_4_DIGITOS>]
     * [VEL<JX>]
     * [VELGR]
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * <VEL_4_DIGITOS>: velocidade de variação do tempo de pulso para o servomotor
     *
     * RESPOSTA: [VEL<JX><VEL_4_DIGITOS_SETADA>]
     *           [TMNGR<VEL_4_DIGITOS_SETADA>]
     * onde <VEL_4_DIGITOS_SETADO>: Valor de velocidade de variação do tempo de pulso do servo (4 dígitos)
     */
    else if(Comando(cmdVEL))
    {
        if (ConfigParamServo(cmdVEL))
        {
            if (SetSpeed(indexServo, servos[indexServo].velTmpPulso))
            {
                CommitVelocidades();
                RespostaComando(bufferProto);
            }
            else
            {
                RollbackVelocidades();
                RespostaComandoERR(cmdVEL);
            }
        }
    }

    /**
     * [ACL<JX><ACEL_4_DIGITOS>]
     * [ACLGR<ACEL_4_DIGITOS>]
     * [ACL<JX>]
     * [ACLGR]
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * <ACEL_4_DIGITOS>: aceleração de variação do tempo de pulso para o servomotor
     *
     * RESPOSTA: [VEL<JX><ACEL_4_DIGITOS_SETADA>]
     *           [TMNGR<ACEL_4_DIGITOS_SETADA>]
     * onde <ACEL_4_DIGITOS_SETADO>: Valor de aceleração do tempo de pulso do servo
     */
    else if(Comando(cmdACL))
    {
        if (ConfigParamServo(cmdACL))
        {
            if (SetAcceleration(indexServo, servos[indexServo].acelTmpPulso))
            {
                CommitAceleracoes();
                RespostaComando(bufferProto);
            }
            else
            {
                RollbackAceleracoes();
                RespostaComandoERR(cmdACL);
            }
        }
    }

    /**
     * [FRS<OP>]
     * [FRS]
     * Onde <OP> = 0, 1 ou 2, sendo que:
     *        0 = sem feedback
     *        1 = valores de posição (tempos) dos servos enquanto em movimento
     *        2 = sinal de movimentação dos servos
     * Resposta: o mesmo comando com o mesmo valor de <OP> setado.
     *
     * Feedback de Rastreamento dos Servos
     * Configuração para qual tipo de feedback os comandos JST, CTZ, GA e GF enviarão
     * a UART 1 durante o movimento dos servos.
     */
    else if(Comando(cmdFRS))
    {
        int tam;
        tam = strlen(bufferProto);

        if (tam == 6)
        {
            char b;
            b = bufferProto[4] - '0';

            switch(b)
            {
            case 0: // Sem Feedback
            case 1: // Posição dos servos
            case 2: // sinal de movimentação
                feedbackRastrServos = b;
                RespostaComando(bufferProto);
                break;
            default:
                RespostaComandoNAK(cmdFRS);
                break;
            }
        }
        else if (tam == 5)
        {
            bufferProto[4] = feedbackRastrServos + '0';
            bufferProto[5] = ']';
            RespostaComando(bufferProto);
        }
        else
        {
            RespostaComandoNAK(cmdFRS);
        }
    }

    /**
     * [CSB<B>]
     * [CSB]
     * Onde <B> = 0, 1
     * Resposta: o mesmo comando com o mesmo valor de <B> setado.
     *
     * Comando Servos Bloqueantes
     * Configuração para permitir ou não que os comandos JST, CTZ, GA e GF permitam a recepção
     * de novos comandos pela UART 1 enquanto os servos estiverem se movimentando.
     */
    else if(Comando(cmdCSB))
    {
        int tam;
        tam = strlen(bufferProto);
        
        if (tam == 6)
        {
            char op;
            op = bufferProto[4] - '0';

            switch(op)
            {
            case 0: 
            case 1:
                comandosServosBloqueantes = op;
                RespostaComando(bufferProto);
                break;
            default:
                RespostaComandoNAK(cmdCSB);
                break;
            }
        }
        else if (tam == 5)
        {
            bufferProto[4] = comandosServosBloqueantes + '0';
            bufferProto[5] = ']';
            RespostaComando(bufferProto);
        }
        else
        {
            RespostaComandoNAK(cmdCSB);
        }
    }

    /**
     * [EMM]
     * Erros da Mini Maestro
     * RESPOSTA: [EMM<valor_em_hexa_4_digitos>]
     *
     * onde <valor_em_hexa_4_digitos>: Valor correspondente ao erro ocorrido na mini maestro.
     *                                 FFFF significa que a Mini Maestro está desconectada
     *                                 enquanto 0000 significa ausência de erros
     */
    else if(Comando(cmdEMM))
    {
        unsigned int erros;

        erros = GetErrors();

        ConverteNumHexaParaStr5(erros, &bufferProto[4]);
        bufferProto[8] = ']';
        RespostaComando(bufferProto);
    }


    /**
     * [GTP<JX>]
     * [GTPGR]
     * [GTP<JX>S]
     * [GTPGRS]
     * Onde:
     * GTP: comando GeT Position da placa Mini Maestro
     * <JX>: junta X, onde X = 0,1,2,3,4
     * GR: garra
     * S: Usa-se o S no final do comando para sinalizar para a Ready For PIC que o valor
     * obtido da placa Mini Maestro deve ser atualizado no campo de tempo de pulso do
     * servo correspondente.
     * Resposta:
     * [GTP<JX><VALOR_4_DIGITOS>]
     * [GTPGR<VALOR_4_DIGITOS>]
     * [GTP<JX><VALOR_4_DIGITOS>S]
     * [GTPGR<VALOR_4_DIGITOS>S]
     *
     * Onde:
     * <VALOR_4_DIGITOS>: Valor do tempo de pulso (target) obtido da placa Mini Maestro
     * S: Para indicar que o valor do target foi sincronizado com o tempo de pulso do servo
     *    registrado na estrutura do servo correspondente.
     */
    else if(Comando(cmdGTP))
    {
        unsigned int position;
        char sincroniza;
        
        if(strlen(bufferProto) < 7 || strlen(bufferProto) > 8 || (strlen(bufferProto) == 8 && bufferProto[6] != 'S'))
        {
            RespostaComandoNAK(cmdGTP);
        }
        else if(bufferProto[4] == 'J')
        {
            if (bufferProto[5] >= '0' && bufferProto[5] <= '4')
            {
                unsigned char canal;
                canal = bufferProto[5] - '0';

                position = GetPosition(canal);

                if (position != 0xFFFF)
                {
                    sincroniza = bufferProto[6];
                    ConverteNumParaStr5(position, &bufferProto[6]);
                    if (sincroniza == 'S')
                    {
                        servos[canal].tempoPulso = position;
                        CommitTempos();
                        bufferProto[10] = 'S';
                        bufferProto[11] = ']';
                    }
                    else
                    {
                        bufferProto[10] = ']';
                    }
                    
                    RespostaComando(bufferProto);
                }
                else
                {
                    RespostaComandoERR(cmdGTP);
                }
            }
            else
            {
                RespostaJNT_NAK();
            }
        }
        else if (bufferProto[4] == 'G' && bufferProto[5] == 'R')
        {
            position = GetPosition(5);

            if (position != 0xFFFF)
            {
                sincroniza = bufferProto[6];
                ConverteNumParaStr5(position, &bufferProto[6]);
                if (sincroniza == 'S')
                {
                    servos[5].tempoPulso = position;
                    CommitTempos();
                    bufferProto[10] = 'S';
                    bufferProto[11] = ']';
                }
                else
                {
                    bufferProto[10] = ']';
                }

                RespostaComando(bufferProto);
            }
            else
            {
                RespostaComandoERR(cmdGTP);
            }
        }
        else
        {
            RespostaJNT_NAK();
        }
    }

    /**
     * [GMS]
     * Comando Get Moving State da placa Mini Maestro
     * RESPOSTA: [GMS1]  quando está havendo movimento de algum servo
     *           [GMS0]  quando não há movimento de nenhum servo
     *           [GMS ERR] quando a placa está desconectada
     */
    else if(Comando(cmdGMS))
    {
        unsigned char ms;

        ms = GetMovingState();

        switch(ms)
        {
        case 0x00:
            bufferProto[4] = '0';
            bufferProto[5] = ']';
            RespostaComando(bufferProto);
            break;
        case 0x01:
            bufferProto[4] = '1';
            bufferProto[5] = ']';
            RespostaComando(bufferProto);
            break;
        default:
            RespostaComandoERR(cmdGMS);
            break;
        }
    }

    /**
     * [STT]
     * STatus de Todos os dispositivos
     */
    else if(Comando(cmdSTT))
    {
        int i;
        SendEnterUART1();
        SendCharToUart1('T');
        SendCharToUart1('E');
        SendCharToUart1('M');
        SendCharToUart1('P');
        SendCharToUart1('O');
        SendCharToUart1(' ');
        SendCharToUart1('D');
        SendCharToUart1('O');
        SendCharToUart1('S');
        SendCharToUart1(' ');
        SendCharToUart1('S');
        SendCharToUart1('E');
        SendCharToUart1('R');
        SendCharToUart1('V');
        SendCharToUart1('O');
        SendCharToUart1('S');
        SendCharToUart1(':');
        SendEnterUART1();

        for (i=0; i < 6; i++)
        {
            EnviaStatusServo(&(servos[i]));
        }
        EnviaStatusLeds();
    }
    
    /**
     *
     * [RST]
     * ReSeT em todos os hardwares conectados à Ready For PIC
     *
     * [RSTM]
     * Reset apenas na placa Mini Maestro 24
     */
    else if (Comando(cmdRST))
    {
        if (strlen(bufferProto) == 6 && bufferProto[4] == 'M')
        {
            SendEnterUART1();
            SendStringToUart1("[RESETANDO MINI MAESTRO 24]");
            SendEnterUART1();
            RST = 0; // Reset da placa Mini Maestro 24
            IniciarServos(); // Retira a Mini Maestro do reset e reinicia os servos.
            SendEnterUART1();
            SendStringToUart1("[PRONTO]");
            SendEnterUART1();
        }
        else if (strlen(bufferProto) == 5)
        {
            unsigned int k;
            RespostaComando(bufferProto);
            SendEnterUART1();
            SendStringToUart1("[RESETANDO]");
            SendEnterUART1();

            PORTLEDS = 0;
            for (i = 0; i < 8; i++) {
                PORTLEDS = PORTLEDS << 1 | 0x01;
                delay_ms(200);
            }

            k = 1;
            for (i = 254; i >= 0; i -= k) {
                PORTLEDS = PORTLEDS & i;
                k *= 2;
                delay_ms(200);
            }

            RESET(); // Reset do PIC18F45K22. Os demais hardwares serão resetados durante a reinicialização.
        }
        else
        {
            RespostaComandoNAK(cmdRST);
        }
    }

    /**
     * [ECH<B>]
     * [ECH]
     * Onde <B> = 0, 1
     * Resposta: o mesmo comando com o mesmo valor de <B> setado.
     *
     * Eco Char
     * Configuração para habilitar (<B> = 1) ou desabilitar (<B> = 0) o echo
     * dos caracteres recebidos pela UART1.
     */
    else if(Comando(cmdECH))
    {
        int tam;
        tam = strlen(bufferProto);

        if (tam == 6)
        {
            char op;
            op = bufferProto[4] - '0';

            switch(op)
            {
            case 0:
            case 1:
                ecoCaracteresAtivado = op;
                RespostaComando(bufferProto);
                break;
            default:
                RespostaComandoNAK(cmdECH);
                break;
            }
        }
        else if (tam == 5)
        {
            bufferProto[4] = ecoCaracteresAtivado + '0';
            bufferProto[5] = ']';
            RespostaComando(bufferProto);
        }
        else
        {
            RespostaComandoNAK(cmdECH);
        }
    }
    /**
     * 
     * Quando comando não é reconhecido, cai neste else
     * 
     */
    else
    {
        SendEnterUART1();
        SendCharToUart1('[');
        SendCharToUart1('C');
        SendCharToUart1('M');
        SendCharToUart1('D');
        SendCharToUart1(' ');
        SendCharToUart1('N');
        SendCharToUart1('A');
        SendCharToUart1('K');
        SendCharToUart1(']');
        SendEnterUART1();
    }
    
} // Fim da função DecodificaProtocolo



