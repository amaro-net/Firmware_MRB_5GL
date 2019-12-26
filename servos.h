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
 * TRIS do PIC que definirá quais pinos serão entradas e saídas no PORT dos servos
 */
#define TRIS_SERVOS   TRISD

/* Configura TIMER0 para contar aproximadamente 267us*/
#define T0CON_VAL 0x88
#define TMR0H_VAL 0xFF
#define TMR0L_VAL 0xB0

/*
 * ESQUEMA DE LIGAÇÃO ENTRE A READY FOR PIC E A MINI MAESTRO 24
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
 * Pino de indicação de erro da placa Mini Maestro 24
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
     * Caractere que será usado como identificador no comando JST para o servo
     */
    unsigned char idJST;
    /**
     * valor corrente do tempo de pulso do servo em microssegundos. Se zero, significa que o servo está desativado.
     */
    unsigned int tempoPulso;
    /**
     * Tempo máximo de pulso que o servo deverá aceitar
     */
    unsigned int tempoMax;
    /**
     * Tempo mínimo de pulso que o servo deverá aceitar. Não pode ser zero, pois este valor desativa o servo.
     */
    unsigned int tempoMin;
    /**
     * Tempo de pulso para o servo assumir a posição central (neutra)
     */
    unsigned int tempoCtz;
    /**
     * Tempo de pulso para o servo assumir a posição de repouso. Útil para o caso de religamento ou reinicialização da placa de controle dos servos.
     */
    unsigned int tempoRepouso;
    /**
     * Velocidade máxima de variação do tempo de pulso em (0,25us)/(10ms). Se zero, o servo trabalha na sua velocidade máxima.
     */
    unsigned int velTmpPulso;
    /**
     * Aceleração da variação do tempo de pulso em (0,25us)/(10ms)/(80ms). Se zero, o servo trabalha com a sua máxima aceleração.
     */
    unsigned int acelTmpPulso;
} Servo;

/**
 * Vetor principal que armazena os dados confirmados de todos os servos.
 */
Servo servos[6];

/**
 * Vetor temporário de servos para guardar os dados dos servos
 * antes de serem alterados e enviados para a placa de controle
 * dos servos. Em caso de erro de comunicação com a placa de
 * controle dos servos, os valores armazenados neste vetor
 * voltam para o vetor principal (servos[6]).
 */
Servo servosTemp[6];

//char idServos[7]; // Identificadores dos servos que terão seus valores alterados.

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
 * Variável incrementada a cada 29us (aproximadamente).
 */
unsigned int conta_29us = 0;

/**
 * Tamanho máximo do buffer da UART 2
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
 * Ponteiro auxiliar que aponta para o último elemento de bufferUART2
 */
unsigned char* pntBufferUART2fim;

/**
 * Variável que recebe os caracteres da UART 2
 */
char rByteUART2;

/**
 * Variável que sinaliza quando o vetor bufferUART2 já recebeu todos os bytes.
 */
unsigned short DataReadyUART2 = 0;

/**
 * Quantidade de bytes que se espera receber da UART 2
 */
int byteCount = 0;

/**
 * Variável para ser usada como índice do servo a ser configurado
 */
char indexServo;

/**
 * Tenta obter da placa Mini Maestro os targets correntes dos servos. Função utilizada
 * na inicialização dos servos.
 * @param canal Canal do servo na Mini Maestro
 * @return O valor corrente do target do servo, se houve comunicação bem sucedida com a Mini Maestro.
 *         Valor zero, se ocorreu algum erro na comunicação com a Mini Maestro, ou se a mesma estiver
 *         desconectada.
 */
unsigned int TempoPulsoInicial(unsigned char canal);

/**
 * Inicia o funcionamento e as configurações dos servos. É necessário
 * INTCONbits.GIEH = 1 antes de chamar esta função.
 */
void IniciarServos();

/**
 * Configuração dos servos
 */
void ConfigServos();

/**
 * Rotina de interrupção para contar um tempo de espera específico. Utilizado pelas rotinas que
 * lêem os bits de entrada envolvidos na comunicação com a placa de controle dos servos.
 */
void ContaTempoEspera();

/**
 * Função para guardar (commitar) os valores dos tempos dos servos antes de serem alterados.
 * Usada como confirmação de que os valores foram setados corretamente na placa de controle
 * dos servos. Estes valores podem ser recuperados pela função RollbackTempos().
 */
void CommitTempos();

/**
 * Função para recuperar os tempos anteriores dos servos em caso de erro.
 * Os valores recuperados são os mesmos gravados pela função CommitTempos().
 */
void RollbackTempos();

/**
 * Função para guardar (commitar) os valores das velocidades dos servos antes de serem alteradas.
 * Usada como confirmação de que os valores foram setados corretamente na placa de controle
 * dos servos. Estes valores podem ser recuperados pela função RollbackVelocidades().
 */
void CommitVelocidades();

/**
 * Função para recuperar as velocidades anteriores dos servos em caso de erro.
 * Os valores recuperados são os mesmos gravados pela função CommitVelocidades().
 */
void RollbackVelocidades();

/**
 * Função para guardar (commitar) os valores das acelerações dos servos antes de serem alteradas.
 * Usada como confirmação de que os valores foram setados corretamente na placa de controle
 * dos servos. Estes valores podem ser recuperados pela função RollbackAceleracoes().
 */
void CommitAceleracoes();

/**
 * Função para recuperar as velocidades anteriores dos servos em caso de erro.
 * Os valores recuperados são os mesmos gravados pela função CommitAceleracoes().
 */
void RollbackAceleracoes();

/**
 * Função para fazer o braço robô assumir a posição de repouso. O braço robô deve ser posto
 * manualmente em uma posição que seja a mais próxima possível da posição de repouso.
 * A posição das juntas 0 a 4 para o repouso, em graus, são, respectivamente, as seguintes:
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
 * Segundo os parâmetros de Denavit-Hatenberg feitos para o braço robô, os ângulos
 * de cada junta seguem as seguintes convenções:
 * J0: correspondente ao teta1, corresponde ao ângulo em torno de um eixo vertical que
 *     passa pela junta 0, em relação a um eixo que sai horizontalmente de um ponto do
 *     eixo da junta 0 (vertical) para frente do braço robô. Se girar para a esquerda,
 *     o ângulo é positivo; se girar para a direita, negativo.
 * J1: correspondente ao teta2, corresponde ao ângulo em torno de um eixo horizontal 
 *     (que vai da esquerda para a direita) que passa pelo eixo da junta 1, em relação
 *     a um eixo paralelo ao mesmo eixo horizontal descrito para o teta1. Se o giro levar
 *     o segmento L1 para frente, o ângulo diminui, se levar para trás, o ângulo aumenta.
 *     como o eixo de referência de teta2 é horizontal, teta2 será sempre positivo.
 * J2: correspondente ao teta3, corresponde ao ângulo em relação a um eixo paralelo
 *     ao segmento L1, em torno de um eixo horizontal que passa por J2. Se a inclinação
 *     for para frente, teta2 será negativo; se for para trás, será positivo.
 * J3: correspondente ao teta4, corresponde ao ângulo em torno do eixo de J3, em relação
 *     a um eixo que é perpendicular ao plano formado pelo eixo de J3 e a um eixo paralelo
 *     ao segmento L2. Se for para frente, teta4 será negativo; se for para trás, positivo.
 * J4: correspondente ao teta5, corresponde ao ângulo em torno do eixo de J4, em relação a um eixo
 *     que é perpendicular, ao mesmo tempo, a J3 e J4.
 */
void PosicaoRepouso();

///////////////////////////////////////////////////////
/// FUNÇÕES PARA A PLACA MINI MAESTRO 24 DA POLOLU ////
///////////////////////////////////////////////////////

/**
 * Envia para a placa Mini Maestro um comando para setar o tempo de pulso (target) do servo
 * @param canal Canal ao qual o servo está conectado na placa Mini Maestro
 * @param target Valor correspondente ao tempo de pulso do servo, em microssegundos
 * @return 1 se o comando foi executado corretamente, 0 caso haja erros.
 */
short SetTarget(unsigned char canal, unsigned int target);

/**
 * Envia para a placa Mini Maestro um comando para setar, ao mesmo tempo, os tempos de pulso (targets) de uma sequência
 * de servos, determinada pelos parâmetros numTargets e primeiroCanal. Ex.: Se a quantidade de servos for 3, e o primeiro
 * canal for 2, serão setados os targets dos canais 2, 3 e 4. Os tempos a serem setados deverão ser armazenados no
 * vetor de servos antes desta função ser chamada.
 * @param numTargets Quantidade de servos na sequência que terão suas posições modificadas (seus targets setados)
 * @param primeiroCanal Primeiro canal da sequência de servos
 * @return 1 se o comando foi executado corretamente, 0 caso contrário
 */
short SetMultipleTargets(unsigned short numTargets, unsigned char primeiroCanal);

/**
 * Envia para a placa Mini Maestro um comando para setar a velocidade de variação
 * do pulso do servo, em (0,25us)/(10ms). Valor zero (ou valores muito altos) significa
 * que o servo irá assumir sua máxima velocidade.
 * @param canal Canal ao qual o servo está conectado na placa Mini Maestro
 * @param vel Velocidade a ser setada
 * @return 1 se a velocidade foi corretamente setada, 0 caso contrário.
 */
short SetSpeed(unsigned char canal, unsigned int vel);

/**
 * Envia para a placa Mini Maestro um comando para setar a aceleração do pulso
 * do servo, em (o,25us)/(10ms)/(80ms). Valor zero (ou valores muito altos) significa
 * que o servo assumirá sua máxima aceleração/desaceleração ao iniciar/concluir
 * um movimento.
 * @param canal Canal ao qual o servo está conectado na placa Mini Maestro
 * @param accel Aceleração a ser setada
 * @return 1 se a aceleração foi corretamente setada, 0 caso contrário.
 */
short SetAcceleration(unsigned char canal, unsigned int accel);

/**
 * Obtém da placa Mini Maestro o valor corrente da posição (tempo de pulso ou target) do servo
 * @param canal Canal ao qual está conectado o servo do qual se deseja saber a posição
 * @return Valor do tempo de pulso do servo em microssegundos. Caso haja desconexão da Mini Maestro, retorna 0xFFFF.
 */
unsigned int GetPosition(unsigned char canal);

/**
 * Verifica na placa Mini Maestro se há algum servo em movimento.
 * @return 1 se a placa Mini Maestro responder que há servos em movimento, 0 caso contrário.
 *          Caso haja desconexão da placa Mini Maestro, retorna 0xFF.
 */
unsigned char GetMovingState();

/**
 * Obtém bytes que representam o tipo de erro ocorrido na placa Mini Maestro. 
 * Mais detalhes sobre os valores do erro na documentação da placa Mini Maestro.
 * @return Valor correspondente ao erro. Em caso de desconexão com a Mini Maestro, retorna 0xFFFF.
 */
unsigned int GetErrors();

/**
 * Ativa a interrupção da UART 2 para recepção dos bytes da placa Mini Maestro, especificando quantos bytes se espera receber.
 * @param qtdBytesAReceber Quantidade de bytes que se espera receber pela UART 2.
 */
void AtivaRecepcaoUART2(int qtdBytesAReceber);

/**
 * Função para aguardar a recepção de bytes pela UART 2 durante um tempo limite.
 * @return 1 se todos os bytes foram recebidos a tempo, 0 caso contrário.
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

