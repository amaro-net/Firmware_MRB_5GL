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
 * File:   servos.h
 * Author: Amaro
 *
 * Created on 26 de Janeiro de 2014, 15:26
 */

#ifndef SERVOS_H
#define	SERVOS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <plib/timers.h>

/**
 * Indica quando a contagem de 29 microssegundos terminou (se diferente de zero)
 */
#define CONTOU_29US   INTCONbits.TMR0IF
/**
 * PORT do PIC a ser usada para os servos
 */
#define PORT_SERVOS   PORTD
/**
 * TRIS do PIC que definir� quais pinos ser�o entradas e sa�das no PORT dos servos
 */
#define TRIS_SERVOS   TRISD

/* Configura TIMER0 para contar aproximadamente 267us*/
#define T0CON_VAL 0x88
#define TMR0H_VAL 0xFF
#define TMR0L_VAL 0xB0

/*
 * ESQUEMA DE LIGA��O ENTRE A READY FOR PIC E A MINI MAESTRO 24
 * READY FOR PIC  |   MINI MAESTRO 24
 * ---------------------------------------
 *    RD4         |    RST (ativo baixo)
 *    RD5         |    ERR
 *    RD6 (TX2)   |    RX
 *    RD7 (RC2)   |    TX
 */

/**
 * Pino de Reset da placa Mini Maestro 24
 */
#define RST PORTDbits.RD4
/**
 * Pino de indica��o de erro da placa Mini Maestro 24
 */
#define ERR PORTDbits.RD5

/**
 * Tempo de espera para eventos, em unidades de 29us
 */
#define TEMPO_ESPERA 65000

/**
 * Tipo da struct que guarda os dados referentes aos servos
 */
typedef struct _servo{
    /**
     * Campo para o nome associado ao servo.
     */
    unsigned char nome[8];
    /**
     * Campo para a sigla associada ao servo.
     */
    unsigned char sigla[3];
    /**
     * Caractere que ser� usado como identificador no comando JST para o servo
     */
    unsigned char idJST;
    /**
     * valor corrente do tempo de pulso do servo em microssegundos. Se zero, significa que o servo est� desativado.
     */
    unsigned int tempoPulso;
    /**
     * Tempo m�ximo de pulso que o servo dever� aceitar
     */
    unsigned int tempoMax;
    /**
     * Tempo m�nimo de pulso que o servo dever� aceitar. N�o pode ser zero, pois este valor desativa o servo.
     */
    unsigned int tempoMin;
    /**
     * Tempo de pulso para o servo assumir a posi��o central (neutra)
     */
    unsigned int tempoCtz;
    /**
     * Tempo de pulso para o servo assumir a posi��o de repouso. �til para o caso de religamento ou reinicializa��o da placa de controle dos servos.
     */
    unsigned int tempoRepouso;
    /**
     * Velocidade m�xima de varia��o do tempo de pulso em (0,25us)/(10ms). Se zero, o servo trabalha na sua velocidade m�xima.
     */
    unsigned int velTmpPulso;
    /**
     * Acelera��o da varia��o do tempo de pulso em (0,25us)/(10ms)/(80ms). Se zero, o servo trabalha com a sua m�xima acelera��o.
     */
    unsigned int acelTmpPulso;
} Servo;

/**
 * Vetor principal que armazena os dados confirmados de todos os servos.
 */
Servo servos[6];

/**
 * Vetor tempor�rio de servos para guardar os dados dos servos
 * antes de serem alterados e enviados para a placa de controle
 * dos servos. Em caso de erro de comunica��o com a placa de
 * controle dos servos, os valores armazenados neste vetor
 * voltam para o vetor principal (servos[6]).
 */
Servo servosTemp[6];

//char idServos[7]; // Identificadores dos servos que ter�o seus valores alterados.

#define SERVO0_TEMPOMIN   480
#define SERVO1_TEMPOMIN   768
#define SERVO2_TEMPOMIN   800
#define SERVO3_TEMPOMIN   528
#define SERVO4_TEMPOMIN   512
#define SERVO5_TEMPOMIN   416

#define SERVO0_TEMPOMAX   2432
#define SERVO1_TEMPOMAX   2256
#define SERVO2_TEMPOMAX   2208
#define SERVO3_TEMPOMAX   2480
#define SERVO4_TEMPOMAX   2432
#define SERVO5_TEMPOMAX   2000

#define SERVO0_TEMPO90    1405
#define SERVO1_TEMPO90    1798
#define SERVO2_TEMPO90    2208
#define SERVO3_TEMPO90    1251
#define SERVO4_TEMPO90    1472
#define SERVO5_TEMPO90    (SERVO5_TEMPOMAX+SERVO5_TEMPOMIN)/2

#define SERVO0_TEMPOREP   SERVO0_TEMPOMIN
#define SERVO1_TEMPOREP   SERVO1_TEMPOMAX
#define SERVO2_TEMPOREP   959
#define SERVO3_TEMPOREP   1836
#define SERVO4_TEMPOREP   1472
#define SERVO5_TEMPOREP   SERVO5_TEMPOMAX

#define SERVO0_VEL    128
#define SERVO1_VEL    128
#define SERVO2_VEL    92
#define SERVO3_VEL    92
#define SERVO4_VEL    128
#define SERVO5_VEL    100

#define SERVO0_ACEL   16
#define SERVO1_ACEL   16
#define SERVO2_ACEL   24
#define SERVO3_ACEL   32
#define SERVO4_ACEL   32
#define SERVO5_ACEL   0

/**
 * Vari�vel incrementada a cada 29us (aproximadamente).
 */
unsigned int conta_29us = 0;

/**
 * Tamanho m�ximo do buffer da UART 2
 */
#define TAMANHO_BUFFER_UART2 3
/**
 * Vetor que acumula os bytes que chegam pela UART2
 */
unsigned char bufferUART2[TAMANHO_BUFFER_UART2];

/**
 * Ponteiro auxiliar que aponta para os elementos de bufferUART2
 */
unsigned char *pntBufferUART2;

/**
 * Ponteiro auxiliar que aponta para o �ltimo elemento de bufferUART2
 */
unsigned char* pntBufferUART2fim;

/**
 * Vari�vel que recebe os caracteres da UART 2
 */
char rByteUART2;

/**
 * Vari�vel que sinaliza quando o vetor bufferUART2 j� recebeu todos os bytes.
 */
unsigned short DataReadyUART2 = 0;

/**
 * Quantidade de bytes que se espera receber da UART 2
 */
int byteCount = 0;

/**
 * Vari�vel para ser usada como �ndice do servo a ser configurado
 */
char indexServo;

/**
 * Tenta obter da placa Mini Maestro os targets correntes dos servos. Fun��o utilizada
 * na inicializa��o dos servos.
 * @param canal Canal do servo na Mini Maestro
 * @return O valor corrente do target do servo, se houve comunica��o bem sucedida com a Mini Maestro.
 *         Valor zero, se ocorreu algum erro na comunica��o com a Mini Maestro, ou se a mesma estiver
 *         desconectada.
 */
unsigned int TempoPulsoInicial(unsigned char canal);

/**
 * Inicia o funcionamento e as configura��es dos servos. � necess�rio
 * INTCONbits.GIEH = 1 antes de chamar esta fun��o.
 */
void IniciarServos();

/**
 * Configura��o dos servos
 */
void ConfigServos();

/**
 * Rotina de interrup��o para contar um tempo de espera espec�fico. Utilizado pelas rotinas que
 * l�em os bits de entrada envolvidos na comunica��o com a placa de controle dos servos.
 */
void ContaTempoEspera();

/**
 * Fun��o para guardar (commitar) os valores dos tempos dos servos antes de serem alterados.
 * Usada como confirma��o de que os valores foram setados corretamente na placa de controle
 * dos servos. Estes valores podem ser recuperados pela fun��o RollbackTempos().
 */
void CommitTempos();

/**
 * Fun��o para recuperar os tempos anteriores dos servos em caso de erro.
 * Os valores recuperados s�o os mesmos gravados pela fun��o CommitTempos().
 */
void RollbackTempos();

/**
 * Fun��o para guardar (commitar) os valores das velocidades dos servos antes de serem alteradas.
 * Usada como confirma��o de que os valores foram setados corretamente na placa de controle
 * dos servos. Estes valores podem ser recuperados pela fun��o RollbackVelocidades().
 */
void CommitVelocidades();

/**
 * Fun��o para recuperar as velocidades anteriores dos servos em caso de erro.
 * Os valores recuperados s�o os mesmos gravados pela fun��o CommitVelocidades().
 */
void RollbackVelocidades();

/**
 * Fun��o para guardar (commitar) os valores das acelera��es dos servos antes de serem alteradas.
 * Usada como confirma��o de que os valores foram setados corretamente na placa de controle
 * dos servos. Estes valores podem ser recuperados pela fun��o RollbackAceleracoes().
 */
void CommitAceleracoes();

/**
 * Fun��o para recuperar as velocidades anteriores dos servos em caso de erro.
 * Os valores recuperados s�o os mesmos gravados pela fun��o CommitAceleracoes().
 */
void RollbackAceleracoes();

/**
 * Fun��o para fazer o bra�o rob� assumir a posi��o de repouso. O bra�o rob� deve ser posto
 * manualmente em uma posi��o que seja a mais pr�xima poss�vel da posi��o de repouso.
 * A posi��o das juntas 0 a 4 para o repouso, em graus, s�o, respectivamente, as seguintes:
 * J0 = -90
 * J1 = 130
 * J2 = -117
 * J3 = 30
 * J4 = 0
 *
 *   o-----o
 *    \     \
 *     \     \
 *      o
 *      |
 *      |
 *  ----------
 * Segundo os par�metros de Denavit-Hatenberg feitos para o bra�o rob�, os �ngulos
 * de cada junta seguem as seguintes conven��es:
 * J0: correspondente ao teta1, corresponde ao �ngulo em torno de um eixo vertical que
 *     passa pela junta 0, em rela��o a um eixo que sai horizontalmente de um ponto do
 *     eixo da junta 0 (vertical) para frente do bra�o rob�. Se girar para a esquerda,
 *     o �ngulo � positivo; se girar para a direita, negativo.
 * J1: correspondente ao teta2, corresponde ao �ngulo em torno de um eixo horizontal 
 *     (que vai da esquerda para a direita) que passa pelo eixo da junta 1, em rela��o
 *     a um eixo paralelo ao mesmo eixo horizontal descrito para o teta1. Se o giro levar
 *     o segmento L1 para frente, o �ngulo diminui, se levar para tr�s, o �ngulo aumenta.
 *     como o eixo de refer�ncia de teta2 � horizontal, teta2 ser� sempre positivo.
 * J2: correspondente ao teta3, corresponde ao �ngulo em rela��o a um eixo paralelo
 *     ao segmento L1, em torno de um eixo horizontal que passa por J2. Se a inclina��o
 *     for para frente, teta2 ser� negativo; se for para tr�s, ser� positivo.
 * J3: correspondente ao teta4, corresponde ao �ngulo em torno do eixo de J3, em rela��o
 *     a um eixo que � perpendicular ao plano formado pelo eixo de J3 e a um eixo paralelo
 *     ao segmento L2. Se for para frente, teta4 ser� negativo; se for para tr�s, positivo.
 * J4: correspondente ao teta5, corresponde ao �ngulo em torno do eixo de J4, em rela��o a um eixo
 *     que � perpendicular, ao mesmo tempo, a J3 e J4.
 */
void PosicaoRepouso();

///////////////////////////////////////////////////////
/// FUN��ES PARA A PLACA MINI MAESTRO 24 DA POLOLU ////
///////////////////////////////////////////////////////

/**
 * Envia para a placa Mini Maestro um comando para setar o tempo de pulso (target) do servo
 * @param canal Canal ao qual o servo est� conectado na placa Mini Maestro
 * @param target Valor correspondente ao tempo de pulso do servo, em microssegundos
 * @return 1 se o comando foi executado corretamente, 0 caso haja erros.
 */
short SetTarget(unsigned char canal, unsigned int target);

/**
 * Envia para a placa Mini Maestro um comando para setar, ao mesmo tempo, os tempos de pulso (targets) de uma sequ�ncia
 * de servos, determinada pelos par�metros numTargets e primeiroCanal. Ex.: Se a quantidade de servos for 3, e o primeiro
 * canal for 2, ser�o setados os targets dos canais 2, 3 e 4. Os tempos a serem setados dever�o ser armazenados no
 * vetor de servos antes desta fun��o ser chamada.
 * @param numTargets Quantidade de servos na sequ�ncia que ter�o suas posi��es modificadas (seus targets setados)
 * @param primeiroCanal Primeiro canal da sequ�ncia de servos
 * @return 1 se o comando foi executado corretamente, 0 caso contr�rio
 */
short SetMultipleTargets(unsigned short numTargets, unsigned char primeiroCanal);

/**
 * Envia para a placa Mini Maestro um comando para setar a velocidade de varia��o
 * do pulso do servo, em (0,25us)/(10ms). Valor zero (ou valores muito altos) significa
 * que o servo ir� assumir sua m�xima velocidade.
 * @param canal Canal ao qual o servo est� conectado na placa Mini Maestro
 * @param vel Velocidade a ser setada
 * @return 1 se a velocidade foi corretamente setada, 0 caso contr�rio.
 */
short SetSpeed(unsigned char canal, unsigned int vel);

/**
 * Envia para a placa Mini Maestro um comando para setar a acelera��o do pulso
 * do servo, em (o,25us)/(10ms)/(80ms). Valor zero (ou valores muito altos) significa
 * que o servo assumir� sua m�xima acelera��o/desacelera��o ao iniciar/concluir
 * um movimento.
 * @param canal Canal ao qual o servo est� conectado na placa Mini Maestro
 * @param accel Acelera��o a ser setada
 * @return 1 se a acelera��o foi corretamente setada, 0 caso contr�rio.
 */
short SetAcceleration(unsigned char canal, unsigned int accel);

/**
 * Obt�m da placa Mini Maestro o valor corrente da posi��o (tempo de pulso ou target) do servo
 * @param canal Canal ao qual est� conectado o servo do qual se deseja saber a posi��o
 * @return Valor do tempo de pulso do servo em microssegundos. Caso haja desconex�o da Mini Maestro, retorna 0xFFFF.
 */
unsigned int GetPosition(unsigned char canal);

/**
 * Verifica na placa Mini Maestro se h� algum servo em movimento.
 * @return 1 se a placa Mini Maestro responder que h� servos em movimento, 0 caso contr�rio.
 *          Caso haja desconex�o da placa Mini Maestro, retorna 0xFF.
 */
unsigned char GetMovingState();

/**
 * Obt�m bytes que representam o tipo de erro ocorrido na placa Mini Maestro. 
 * Mais detalhes sobre os valores do erro na documenta��o da placa Mini Maestro.
 * @return Valor correspondente ao erro. Em caso de desconex�o com a Mini Maestro, retorna 0xFFFF.
 */
unsigned int GetErrors();

/**
 * Ativa a interrup��o da UART 2 para recep��o dos bytes da placa Mini Maestro, especificando quantos bytes se espera receber.
 * @param qtdBytesAReceber Quantidade de bytes que se espera receber pela UART 2.
 */
void AtivaRecepcaoUART2(int qtdBytesAReceber);

/**
 * Fun��o para aguardar a recep��o de bytes pela UART 2 durante um tempo limite.
 * @return 1 se todos os bytes foram recebidos a tempo, 0 caso contr�rio.
 */
unsigned short AguardaReceberBytesUART2();

/**
 * Recebe bytes vindos da UART 2 e armazena em bufferUART2
 */
void RecebeBytesUART2();

#define testbit(var, bit) ( ((var) & (1 <<(bit))) >> (bit))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

#ifdef	__cplusplus
}
#endif

#endif	/* SERVOS_H */

