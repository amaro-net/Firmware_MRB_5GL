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
#include <xc.h>

#include "servos.h"
#include "protocolo.h"
#include "delays.h"
#include "porta_serial.h"
#include <string.h>
#include <stdio.h>


unsigned int TempoPulsoInicial(unsigned char canal)
{
    unsigned int auxGetPosition;
    auxGetPosition = GetPosition(canal);
    if(auxGetPosition == 0xFFFF)
    {
        LimparString(str, TAM_STR);
        str[0] = 'I';
        str[1] = 'N';
        str[2] = 'I';
        str[3] = 'T';
        str[4] = '_';
        str[5] = 'C';
        str[6] = 'A';
        str[7] = 'N';
        str[8] = 'A';
        str[9] = 'L';
        str[10] = '_';
        str[11] = ((canal/10)%10)+'0';
        str[12] = (canal%10)+'0';
        RespostaComandoERR(str);
        return 0;
    }
    return auxGetPosition;
}

/**
 * Inicia o funcionamento e as configurações dos servos. É necessário
 * INTCONbits.GIEH = 1 antes de chamar esta função.
 */
void IniciarServos()
{
    TMR0L = TMR0L_VAL;
    TMR0H = TMR0H_VAL;
    T0CON = T0CON_VAL;
    
    INTCONbits.TMR0IE = 1; // Habilita interrupção do TIMER0

    RST = 1; // Habilita (retira do reset) a placa de controle dos servos

    delay_ms(1000); // Aguarda 1 segundo para a Mini Maestro terminar de iniciar
   
    servos[0].tempoPulso = TempoPulsoInicial(0);
    servos[1].tempoPulso = TempoPulsoInicial(1);
    servos[2].tempoPulso = TempoPulsoInicial(2);
    servos[3].tempoPulso = TempoPulsoInicial(3);
    servos[4].tempoPulso = TempoPulsoInicial(4);
    servos[5].tempoPulso = TempoPulsoInicial(5);
    
    servos[0].tempoMax = SERVO0_TEMPOMAX;
    servos[1].tempoMax = SERVO1_TEMPOMAX;
    servos[2].tempoMax = SERVO2_TEMPOMAX;
    servos[3].tempoMax = SERVO3_TEMPOMAX;
    servos[4].tempoMax = SERVO4_TEMPOMAX;

    servos[0].tempoMin = SERVO0_TEMPOMIN;
    servos[1].tempoMin = SERVO1_TEMPOMIN;
    servos[2].tempoMin = SERVO2_TEMPOMIN;
    servos[3].tempoMin = SERVO3_TEMPOMIN;
    servos[4].tempoMin = SERVO4_TEMPOMIN;

    servos[0].tempoCtz = SERVO0_TEMPO90;
    servos[1].tempoCtz = SERVO1_TEMPO90;
    servos[2].tempoCtz = SERVO2_TEMPO90;
    servos[3].tempoCtz = SERVO3_TEMPO90;
    servos[4].tempoCtz = SERVO4_TEMPO90;

    servos[5].tempoMax = SERVO5_TEMPOMAX; // Garra aberta
    servos[5].tempoMin = SERVO5_TEMPOMIN; // Garra fechada
    servos[5].tempoCtz = SERVO5_TEMPO90; // Garra semi-aberta

    servos[0].tempoRepouso = SERVO0_TEMPOREP;
    servos[1].tempoRepouso = SERVO1_TEMPOREP;
    servos[2].tempoRepouso = SERVO2_TEMPOREP;
    servos[3].tempoRepouso = SERVO3_TEMPOREP;
    servos[4].tempoRepouso = SERVO4_TEMPOREP;
    servos[5].tempoRepouso = SERVO5_TEMPOREP;

    servos[0].velTmpPulso = SERVO0_VEL;
    servos[1].velTmpPulso = SERVO1_VEL;
    servos[2].velTmpPulso = SERVO2_VEL;
    servos[3].velTmpPulso = SERVO3_VEL;
    servos[4].velTmpPulso = SERVO4_VEL;
    servos[5].velTmpPulso = SERVO5_VEL;

    servos[0].acelTmpPulso = SERVO0_ACEL;
    servos[1].acelTmpPulso = SERVO1_ACEL;
    servos[2].acelTmpPulso = SERVO2_ACEL;
    servos[3].acelTmpPulso = SERVO3_ACEL;
    servos[4].acelTmpPulso = SERVO4_ACEL;
    servos[5].acelTmpPulso = SERVO5_ACEL;

    LimparString(servos[0].nome, sizeof(servos[0].nome));
    servos[0].nome[0] = 'J';
    servos[0].nome[1] = 'U';
    servos[0].nome[2] = 'N';
    servos[0].nome[3] = 'T';
    servos[0].nome[4] = 'A';
    servos[0].nome[5] = ' ';
    servos[0].nome[6] = '0';

    LimparString(servos[1].nome, sizeof(servos[1].nome));
    servos[1].nome[0] = 'J';
    servos[1].nome[1] = 'U';
    servos[1].nome[2] = 'N';
    servos[1].nome[3] = 'T';
    servos[1].nome[4] = 'A';
    servos[1].nome[5] = ' ';
    servos[1].nome[6] = '1';

    LimparString(servos[2].nome, sizeof(servos[2].nome));
    servos[2].nome[0] = 'J';
    servos[2].nome[1] = 'U';
    servos[2].nome[2] = 'N';
    servos[2].nome[3] = 'T';
    servos[2].nome[4] = 'A';
    servos[2].nome[5] = ' ';
    servos[2].nome[6] = '2';

    LimparString(servos[3].nome, sizeof(servos[3].nome));
    servos[3].nome[0] = 'J';
    servos[3].nome[1] = 'U';
    servos[3].nome[2] = 'N';
    servos[3].nome[3] = 'T';
    servos[3].nome[4] = 'A';
    servos[3].nome[5] = ' ';
    servos[3].nome[6] = '3';

    LimparString(servos[4].nome, sizeof(servos[4].nome));
    servos[4].nome[0] = 'J';
    servos[4].nome[1] = 'U';
    servos[4].nome[2] = 'N';
    servos[4].nome[3] = 'T';
    servos[4].nome[4] = 'A';
    servos[4].nome[5] = ' ';
    servos[4].nome[6] = '4';

    LimparString(servos[5].nome, sizeof(servos[5].nome));
    servos[5].nome[0] = 'G';
    servos[5].nome[1] = 'A';
    servos[5].nome[2] = 'R';
    servos[5].nome[3] = 'R';
    servos[5].nome[4] = 'A';

    
    servos[0].sigla[0] = 'J';
    servos[0].sigla[1] = '0';
    servos[0].sigla[2] = (char)0;

    servos[1].sigla[0] = 'J';
    servos[1].sigla[1] = '1';
    servos[1].sigla[2] = (char)0;

    servos[2].sigla[0] = 'J';
    servos[2].sigla[1] = '2';
    servos[2].sigla[2] = (char)0;

    servos[3].sigla[0] = 'J';
    servos[3].sigla[1] = '3';
    servos[3].sigla[2] = (char)0;

    servos[4].sigla[0] = 'J';
    servos[4].sigla[1] = '4';
    servos[4].sigla[2] = (char)0;

    servos[5].sigla[0] = 'G';
    servos[5].sigla[1] = 'R';
    servos[5].sigla[2] = (char)0;

    servos[0].idJST = 'A';
    servos[1].idJST = 'B';
    servos[2].idJST = 'C';
    servos[3].idJST = 'D';
    servos[4].idJST = 'E';
    servos[5].idJST = 'G';

    //GIE_bit = 1;
    // Habilita a interrupção do timer da contagem de tempo de espera
    
    CommitTempos(); // Para guardar os tempos antes do primeiro envio para a placa de controle
    //LimparString(idServos, sizeof(idServos));

    PosicaoRepouso();
}

void CommitTempos()
{
    servosTemp[0].tempoPulso = servos[0].tempoPulso;
    servosTemp[1].tempoPulso = servos[1].tempoPulso;
    servosTemp[2].tempoPulso = servos[2].tempoPulso;
    servosTemp[3].tempoPulso = servos[3].tempoPulso;
    servosTemp[4].tempoPulso = servos[4].tempoPulso;
    servosTemp[5].tempoPulso = servos[5].tempoPulso;
}

void RollbackTempos()
{
    servos[0].tempoPulso = servosTemp[0].tempoPulso;
    servos[1].tempoPulso = servosTemp[1].tempoPulso;
    servos[2].tempoPulso = servosTemp[2].tempoPulso;
    servos[3].tempoPulso = servosTemp[3].tempoPulso;
    servos[4].tempoPulso = servosTemp[4].tempoPulso;
    servos[5].tempoPulso = servosTemp[5].tempoPulso;
}

void CommitVelocidades()
{
    servosTemp[0].velTmpPulso = servos[0].velTmpPulso;
    servosTemp[1].velTmpPulso = servos[1].velTmpPulso;
    servosTemp[2].velTmpPulso = servos[2].velTmpPulso;
    servosTemp[3].velTmpPulso = servos[3].velTmpPulso;
    servosTemp[4].velTmpPulso = servos[4].velTmpPulso;
    servosTemp[5].velTmpPulso = servos[5].velTmpPulso;
}

void RollbackVelocidades()
{
    servos[0].velTmpPulso = servosTemp[0].velTmpPulso;
    servos[1].velTmpPulso = servosTemp[1].velTmpPulso;
    servos[2].velTmpPulso = servosTemp[2].velTmpPulso;
    servos[3].velTmpPulso = servosTemp[3].velTmpPulso;
    servos[4].velTmpPulso = servosTemp[4].velTmpPulso;
    servos[5].velTmpPulso = servosTemp[5].velTmpPulso;
}

void CommitAceleracoes()
{
    servosTemp[0].acelTmpPulso = servos[0].acelTmpPulso;
    servosTemp[1].acelTmpPulso = servos[1].acelTmpPulso;
    servosTemp[2].acelTmpPulso = servos[2].acelTmpPulso;
    servosTemp[3].acelTmpPulso = servos[3].acelTmpPulso;
    servosTemp[4].acelTmpPulso = servos[4].acelTmpPulso;
    servosTemp[5].acelTmpPulso = servos[5].acelTmpPulso;
}

void RollbackAceleracoes()
{
    servos[0].acelTmpPulso = servosTemp[0].acelTmpPulso;
    servos[1].acelTmpPulso = servosTemp[1].acelTmpPulso;
    servos[2].acelTmpPulso = servosTemp[2].acelTmpPulso;
    servos[3].acelTmpPulso = servosTemp[3].acelTmpPulso;
    servos[4].acelTmpPulso = servosTemp[4].acelTmpPulso;
    servos[5].acelTmpPulso = servosTemp[5].acelTmpPulso;
}

void PosicaoRepouso()
{
    unsigned char i;
    char frsTemp;

    frsTemp = feedbackRastrServos;
    feedbackRastrServos = 1; // Inicialização de posição ocorrerá com feedback

    /* Reduz velocidade e aceleração para todas as juntas */
    for (i = 0; i < 6; i++)
    {
        SetAcceleration(i, 2);
        SetSpeed(i,10);
    }

    /* Coloca o braço na posição de repouso */
    for(i = 0; i < 6; i++)
    {
        servos[i].tempoPulso = servos[i].tempoRepouso;
    }
    // Junta 0
    SetTarget(0, servos[0].tempoPulso);
    ultCmdMovServos = cmdIN1;
    tipoCmdMovServosPorQt = 1;
    canalCmd1ServoNaoBloq = 0;
    delay_ms(DELAY_MS_RESPOSTA);
    while(tipoCmdMovServosPorQt > 0)
    {
        RespEFeedbackMovServos();
    }

    // Juntas 1 a 4 e Garra
    SetMultipleTargets(6, 0);
    ultCmdMovServos = cmdIN2;
    tipoCmdMovServosPorQt = 2;
    delay_ms(DELAY_MS_RESPOSTA);
    while(tipoCmdMovServosPorQt > 0)
    {
        RespEFeedbackMovServos();
    }

    // Seta as velocidades e acelerações para os seus valores configurados
    for (i = 0; i < 6; i++)
    {
        SetAcceleration(i, servos[i].acelTmpPulso);
        SetSpeed(i, servos[i].velTmpPulso);
    }

    feedbackRastrServos = frsTemp;
}

/*
 * Configuração dos servos
 */
void ConfigServos() {
    /*
     * ESQUEMA DE LIGAÇÃO ENTRE A READY FOR PIC E A MINI MAESTRO 24
     * READY FOR PIC  |   MINI MAESTRO 24
     * ---------------------------------------
     *    RD4 (SAÍDA) |    RST (ativo baixo)
     *    RD5         |    ERR
     *    RD6 (TX2)   |    RX
     *    RD7 (RC2)   |    TX
     */
    TRIS_SERVOS = 0b11100000; // Configura pinos de entrada/saída
    PORT_SERVOS = 0b00000000; // Todos os pinos em zero. Lembrar que o pino RD4 reseta a Mini Maestro com valor zero.

    //INTCON2bits.NOT_RBPU = 0; // Habilita a configuração de pull-ups do PORTB
    //WPUBbits.WPUB3 = 1; // Habilita pull-up no RB3
    //WPUBbits.WPUB4 = 1; // Habilita pull-up no RB4
}

void ContaTempoEspera() {
    CONTOU_29US = 0;
    conta_29us++;
    //INTCONbits.TMR0IE = 1;              //seta novamente a interrupção do TIMER0
    TMR0H = TMR0H_VAL;
    TMR0L = TMR0L_VAL;
}


void AtivaRecepcaoUART2(int qtdBytesAReceber)
{
    if(RCSTA2bits.OERR == 1)
    {
        //SendCharToUart1('E');
        RCSTA2bits.CREN = 0;
        delay_ms(10);
        RCSTA2bits.CREN = 1;
    }
    //pntBufferUART2 = &bufferUART2;
    pntBufferUART2 = &bufferUART2[0];
    byteCount = qtdBytesAReceber;
    pntBufferUART2fim = &bufferUART2[byteCount-1];
    LimparString(bufferUART2, TAMANHO_BUFFER_UART2);
    PIE3bits.RC2IE = 1;  // Habilita a interrupção do pino de recepção da UART 2
}

unsigned short AguardaReceberBytesUART2()
{
    unsigned int tempo;
    conta_29us = 0; // tempo incrementado na interrupção do TIMER0
    tempo = conta_29us;
    while (tempo < TEMPO_ESPERA && DataReadyUART2 != 1) tempo = conta_29us;
    if (DataReadyUART2)
    {
        DataReadyUART2 = 0;
        return 1;
    }
    return 0;
}

/**
 * Para receber os bytes da UART2 e armazenar em buffer. A função AtivaRecepcaoUART2 deve ser chamada antes de
 * realizar qualquer recepção de bytes pela UART 2.
 */
void RecebeBytesUART2()
{
    if (byteCount > 0 && pntBufferUART2 <= pntBufferUART2fim)
    {
        *pntBufferUART2 = recByteUART2;
        if (pntBufferUART2 == pntBufferUART2fim) // Último byte?
        {
            DataReadyUART2 = 1;
            byteCount = 0;
            PIE3bits.RC2IE = 0;
        }
        else
        {
            ++pntBufferUART2;
        }
    }
    
    RC2IF = 0; 
}

/////////////////////////////////////////////
/// COMANDOS PARA A PLACA MINI MAESTRO 24 ///
/////////////////////////////////////////////

short SetTarget(unsigned char canal, unsigned int target)
{
    target = target * 4; // multiplica o target por 4. Exigência do Compact Protocol da placa Mini Maestro
    
    // Envio do comando
    SendByteUART2(0x84); // Comando set target, Compact Protocol
    SendByteUART2(canal); // Canal
    SendByteUART2(target & 0x7F); // Bits menos significativos do target
    SendByteUART2((target >> 7) & 0x7F);   
   
    return 1;
}

short SetMultipleTargets(unsigned short numTargets, unsigned char primeiroCanal)
{
    unsigned char i;
    unsigned int target;

    SendByteUART2(0x9F); // Comando Set Multiple Target, Compact Protocol
    SendByteUART2(numTargets);
    SendByteUART2(primeiroCanal);

    for (i = primeiroCanal; i < 6; i++)
    {
        target = servos[i].tempoPulso;
        target = target * 4; // multiplica o target por 4. Exigência do Compact Protocol da placa Mini Maestro
        SendByteUART2(target & 0x7F); // Bits menos significativos do target
        SendByteUART2((target >> 7) & 0x7F);
    }

    return 1;
}

short SetSpeed(unsigned char canal, unsigned int vel)
{
    // Envio do comando
    SendByteUART2(0x87); // Comando set speed, Compact Protocol
    SendByteUART2(canal); // Canal
    SendByteUART2(vel & 0x7F); // Bits menos significativos da velocidade
    SendByteUART2((vel >> 7) & 0x7F); // bits mais significativos
    
//    if (GetErrors() != 0x0000)
//        return 0;

    return 1;
}

short SetAcceleration(unsigned char canal, unsigned int accel)
{
    // Envio do comando
    SendByteUART2(0x89); // Comando set acceleration, Compact Protocol
    SendByteUART2(canal); // Canal
    SendByteUART2(accel & 0x7F); // Bits menos significativos da aceleração
    SendByteUART2((accel >> 7) & 0x7F); // bits mais significativos

//    if (GetErrors() != 0x0000)
//        return 0;

    return 1;
}

unsigned int GetPosition(unsigned char canal)
{
    unsigned int posicao = 0x0000;
    
    AtivaRecepcaoUART2(2); // Ativa recepção da UART 2, esperando receber 2 byteS como resposta
    SendByteUART2(0x90); //Comando Get Position, Compact Protocol
    SendByteUART2(canal);
    
    if (!AguardaReceberBytesUART2())
    {
        return 0xFFFF;
    }

//    posicao = posicao | (unsigned int)bufferUART2[1];
//    posicao = (posicao << 8) | (unsigned int)bufferUART2[0];

    posicao = bufferUART2[0] + 256 * bufferUART2[1];

    // O valor obtido vem multiplicado por 4 da placa Mini Maestro. Divide para obter o valor verdadeiro.
    posicao /= 4;
    
    return posicao;
}

unsigned char GetMovingState()
{
    AtivaRecepcaoUART2(1); // Ativa recepção da UART 2, esperando receber 1 byte como resposta
    SendByteUART2(0x93); // Comando get moving state, Compact Protocol
    if (!AguardaReceberBytesUART2())
    {
        return 0xFF;
    }
    return bufferUART2[0];
}

unsigned int GetErrors()
{
    unsigned int error = 0x0000;
    AtivaRecepcaoUART2(1); // Ativa recepção da UART 2, esperando receber 1 byte como resposta
    SendByteUART2(0xA1); // Comando get moving state, Compact Protocol
    if (!AguardaReceberBytesUART2())
    {
        return 0xFFFF;
    }

    error = error | (unsigned int)bufferUART2[1];
    error = (error << 8) | (unsigned int)bufferUART2[0];
    
    return error;
}
