#define NUM_ENTRADAS 3
#define NUM_NEURONIOS_CAMADA_UM 15
#define NUM_NEURONIOS_CAMADA_DOIS 2


//Configuracoes da Rede Perceptron Multicamadas.
int i = 0, j = 0, k = 0, n = 0, funcao = 0;
float soma = 0;

//PESOS
//double w1[NUM_NEURONIOS_CAMADA_UM * NUM_ENTRADAS] = {5.13864,-0.0536648,-3.58246,0.290506,0.338857,2.70735,5.46337,-2.90328,-3.26122,-10.6041,2.71847,8.80497,1.87464,1.62409,-7.4105,4.03173,-1.09139,-3.49394,-3.85585,2.24793,2.25873,-5.81032,2.82986,3.67874,-3.96045,0.553318,3.63752,-0.20594,-0.781021,4.26075,4.4361,-1.37263,-3.69798,-8.3933,-1.2443,7.54047,0.256702,0.579697,-4.11264,4.54055,-0.34553,-2.85339,-0.571247,0.144011,-2.89625};
//double w2[NUM_NEURONIOS_CAMADA_DOIS * NUM_NEURONIOS_CAMADA_UM] = {-3.69477,4.28876,3.25895,-7.65781,-9.32284,0.735775,-1.90571,-3.26943,0.039697,7.20552,1.40079,8.32873,-7.38657,-2.59816,-6.44104,3.9239,-0.741849,4.35578,-8.48837,3.4581,5.23473,-4.03212,-4.87115,-5.73272,-0.158789,5.28275,-5.984,-0.756126,4.03223,-2.13219};
double w1[NUM_NEURONIOS_CAMADA_UM * NUM_ENTRADAS] = {0.18337,0.298979,-0.673358,2.12613,0.656635,-1.85446,-1.5987,0.272904,0.948731,-0.485829,0.379211,-0.0252502,-0.233838,0.513929,-0.532496,-0.653475,0.539528,-0.153874,-1.74246,0.377184,0.864891,-1.60056,0.172366,1.02452,0.110069,0.466148,-0.656099,-0.915732,0.579357,-0.0486327,1.02001,0.75708,-1.57916,-1.00827,0.190603,0.815342,0.867957,0.272004,-1.05548,-0.764655,0.586315,-0.06769,1.76225,0.925875,-1.93713};
double w2[NUM_NEURONIOS_CAMADA_DOIS * NUM_NEURONIOS_CAMADA_UM] = {-0.401287,-2.26771,1.45163,0.334223,0.0885903,0.535191,1.60819,1.42374,-0.25601,0.803319,-1.14156,0.857742,-1.09396,0.663443,-1.83577,0.595095,2.11548,-1.06943,0.00221137,0.527177,0.173431,-0.950588,-1.1802,0.641158,0.0815628,1.75436,-0.911095,0.997676,0.0991697,2.28312};


//Delta PESOS
double dw1[NUM_NEURONIOS_CAMADA_UM * NUM_ENTRADAS];
double dw2[NUM_NEURONIOS_CAMADA_DOIS * NUM_NEURONIOS_CAMADA_UM];

//Entradas
double Entrada_I1[NUM_NEURONIOS_CAMADA_UM] = {0};
double Entrada_I2[NUM_NEURONIOS_CAMADA_DOIS]  = {0};

//SAIDAS
double saida_y1[NUM_NEURONIOS_CAMADA_UM] = {0};
double saida_y2[NUM_NEURONIOS_CAMADA_DOIS]  = {0};

double saidas[NUM_NEURONIOS_CAMADA_DOIS] = {0};
// porta de entarda do sinal
int sensor[NUM_ENTRADAS]  = {0, 1, 2};

//armazena valores lidos pelo sesnor
double ReadSensor[NUM_ENTRADAS] = {0};//{0.00,0.00,0.80,0.20,0.00};//{0.1,0.3,1.0,0.4,0.2}

// valores de calibracao
double valorMax  = 819;

// pwm motores
int LeftMotor   = 9;
int RightMotor  = 10;
int la1 = 2;
int la2 = 5;
int ra1 = 6;
int ra2 = 7;


//motores



String Recive         = "";



void setup() {

  Serial.begin(9600);
  pinMode(LeftMotor, OUTPUT);
  pinMode(RightMotor, OUTPUT);

  pinMode(la1, OUTPUT); 
  pinMode(la2, OUTPUT); 
  pinMode(ra1, OUTPUT); 
  pinMode(ra2, OUTPUT); 

}



float funcao_ativacao(float net, int funcao, float a)
{
  if (!funcao)
  {

    /*
                    1
      y(n) = ---------------
             1 + exp(-a.net)
    */

    return ( 1.0 / (1.0 + exp(-a * net)) );

  }
  else
  {

    /*
             exp(a.net) - exp(-a.net)
      y(n) = ------------------------
             exp(a.net) + exp(-a.net)
    */

    return ( (exp(a * net) - exp(-a * net)) / (exp(a * net) + exp(-a * net)) );
  }
}

void startReadSensor() {

 for (int i = 0; i < NUM_ENTRADAS; i++) {
  
    ReadSensor[i] = analogRead(sensor[i])/ valorMax;
 }

  Serial.println("%Valores Sensores:");
  for (int i = 0 ;i < NUM_ENTRADAS; i++) {
    //Serial.print(i);
    Serial.println(ReadSensor[i]);
  }
}

void loop() {

  digitalWrite(la1, HIGH);
  digitalWrite(la2, LOW); //LOW
  digitalWrite(ra1, HIGH);
  digitalWrite(ra2, LOW); //LOW
  
  startReadSensor();


  //Cálculo para camada NUM_NEURONIOS_CAMADA_UM.
  n = 0;
  for (j = 0; j < NUM_NEURONIOS_CAMADA_UM; j++)
  {
    soma = 0;
    for (i = 0; i < NUM_ENTRADAS; i++)
    {
      soma += w1[n] * ReadSensor[i];
      n += 1;
    }
    Entrada_I1[j] = soma;
    saida_y1[j] = funcao_ativacao(Entrada_I1[j], funcao, 1.0);
  }

  //Cálculo para camada NUM_NEURONIOS_CAMADA_DOIS.
  n = 0;
  for (j = 0; j < NUM_NEURONIOS_CAMADA_DOIS; j++)
  {
    soma = 0;
    for (i = 0; i < NUM_NEURONIOS_CAMADA_UM; i++)
    {
      soma += w2[n] * saida_y1[i];
      n += 1;
    }
    Entrada_I2[j] = soma;
    saida_y2[j] = funcao_ativacao(Entrada_I2[j], funcao, 1.0);
  }


  analogWrite(LeftMotor , saida_y2[0] * 254);
  analogWrite(RightMotor, saida_y2[1] * 254);


  Serial.print("#Saida da rede: ");
  Serial.print(saida_y2[0]* 254);
  Serial.print(" | ");
  Serial.println(saida_y2[1]* 254);



  delay(1000);

}



