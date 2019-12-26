

short int AguardaTR_OK(unsigned short int valor)
{
    unsigned int tempo;
    conta_29us = 0; // tempo incrementado na interrupção do TIMER0
    tempo = conta_29us;
    while (tempo < TEMPO_ESPERA_REC_BIT && TR_OK != valor) tempo = conta_29us;
//    sendCharToUart1('T');
//    sendCharToUart1('R');
//    sendCharToUart1('_');
//    sendCharToUart1('O');
//    sendCharToUart1('K');
//    sendCharToUart1('=');
//    if(TR_OK == 0)
//        sendCharToUart1('0');
//    else if (TR_OK == 1)
//        sendCharToUart1('1');
//    else
//        sendCharToUart1('X');
//    sendCharToUart1(13);
//    sendCharToUart1(10);


    if(/*tempo >= TEMPO_ESPERA_REC_BIT &&*/ TR_OK != valor)
    {
        SendCharToUart1('E');
        SendCharToUart1(13);
        SendCharToUart1(10);
        return 0;
    }
//    sendCharToUart1('C');
//    sendCharToUart1(13);
//    sendCharToUart1(10);
    return 1;
}

/**
 *
 * @param valor
 * @return
 */
short int AguardaD_OK(unsigned short int valor)
{
    unsigned int tempo;
    conta_29us = 0; // tempo incrementado na interrupção do TIMER0
    tempo = conta_29us;
    while (tempo < TEMPO_ESPERA_REC_BIT && D_OK != valor) tempo = conta_29us;
    SendCharToUart1('D');
    SendCharToUart1('_');
    SendCharToUart1('O');
    SendCharToUart1('K');
    SendCharToUart1('=');
    if(D_OK == 0)
        SendCharToUart1('0');
    else if (D_OK == 1)
        SendCharToUart1('1');
    else
        SendCharToUart1('X');
    SendCharToUart1(13);
    SendCharToUart1(10);
    if(/*tempo >= TEMPO_ESPERA_REC_BIT && */D_OK != valor)
    {
        SendCharToUart1('E');
        SendCharToUart1(13);
        SendCharToUart1(10);
        return 0;
    }
    SendCharToUart1('C');
    SendCharToUart1(13);
    SendCharToUart1(10);
    return 1;
}

/**
 * Transmite 1 bit para a placa de controle dos servos
 *
 * @param valor Parâmetro que recebe o valor da variável que contém os bits ()
 * @param idbit Índice do bit que será transmitido (0 a 7)
 * @return
 */
short int TransmiteBit(unsigned short int valor, int idbit)
{
    unsigned short b;

    b = testbit(valor, idbit);

    BIT_DT = b;

    if(!BIT_DT)
        SendCharToUart1('0');
    else if (BIT_DT)
        SendCharToUart1('1');
    else
        SendCharToUart1('X');
//    sendCharToUart1(13);
//    sendCharToUart1(10);

    CLK = 1;
    if(!AguardaTR_OK(1))
    {
        CLK = 0;
        return 0;
    }
    CLK = 0;
    if(!AguardaTR_OK(0))
        return 0;
    return 1;
}

/**
 *
 * @param vetor_servos  Vetor contendo os dados de todos os servos.
 * @param ids  Vetor contendo, em char, os índices dos servos que terão seus dados mandados para a placa de controle de servos.
 * @return 1 se a transmissão foi bem sucedida. 0 caso contrário.
 */
short int TransmiteTemposServos()
{
    int x = 0, i;
    int qtServos;
    unsigned short int idServo;

    qtServos = strlen(idServos);

    BIT_DT = 0;
    CLK = 0;

    BIT_TR = 0; // Caso tenha alguma transmissão anterior pendente

    //AguardaD_OK(0);

    BIT_TR = 1;

    if(!AguardaD_OK(0))
    {
        BIT_TR = 0;
        SendCharToUart1('R');
        RST = 0;
        delay_ms(200);
        RST = 1;
        delay_ms(1000);

        BIT_TR = 1;
        if(!AguardaD_OK(0))
        {
            BIT_TR = 0;
            SendCharToUart1('I');
            return 0;
        }
    }


    do{
        SendCharToUart1('(');
        SendCharToUart1('x');
        SendCharToUart1('=');
        SendCharToUart1(x+'0');
        SendCharToUart1(')');
        SendCharToUart1('(');
        i = 0;
        idServo = idServos[x] - '0';
        do{
            if(!TransmiteBit(idServo,i))
            {
                PORTD = 0b00000001;
                SendCharToUart1(idServos[x]);
                SendCharToUart1(i+'0');
                SendCharToUart1(idServo+'0');
                BIT_TR = 0;
                return 0;
            }
            i++;
        }while(i < 3);
        SendCharToUart1(')');

        i = 0;
        do{
            if(!TransmiteBit(servos[idServo].tempoPulso, i))
            {
                SendCharToUart1('d');
                PORTD = (char)i;
                BIT_TR = 0;
                return 0;
            }
            i++;
        }while(!((i == 7 && x == qtServos - 1) || (i == 8 && x < qtServos - 1)));
        x++;
    }while(x < qtServos);

    // O artifício a seguir permite o PIC16F84A da placa de controle setar os tempos enviados para os servos
    // Consiste, basicamente, de setar BIT_TR com 0 antes de setar CLK com 0.
    BIT_DT = testbit(servos[idServo].tempoPulso, 7); // Seta o último bit do valor do tempo do último servo
    if(BIT_DT == 0)
        SendCharToUart1('0');
    else if (BIT_DT == 1)
        SendCharToUart1('1');
    else
        SendCharToUart1('X');
    SendCharToUart1(13);
    SendCharToUart1(10);

    CLK = 1;
    if(!AguardaTR_OK(1))
    {
        SendCharToUart1('f');
        CLK = 0;
        BIT_TR = 0;
        return 0;
    }
    BIT_TR = 0; // É aqui que começa o artifício
    delay_ms(100);
    CLK = 0;
    if(!AguardaTR_OK(0)){
        SendCharToUart1('g');
        return 0;
    }

    if(!AguardaD_OK(1))
    {
        SendCharToUart1('D');
        return 0;
    }

    return 1;
}
