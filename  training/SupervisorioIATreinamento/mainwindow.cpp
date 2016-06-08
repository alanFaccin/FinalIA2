#include "mainwindow.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>

#include <math.h>
#include <QFile>
#include <QVector>
#include <QDebug>
#include <iostream>

#define WINDOW_WIDHT 900
#define WINDOW_HEIGHT 600

#define NUM_ENTRADAS 3
#define NUM_NEURONIOS_CAMADA_UM 15
#define NUM_NEURONIOS_CAMADA_DOIS 2


QVector< QVector<double> > amostras_treinamento;
QVector< QVector<double> > saidas_desejada;

//PESOS
double w1[NUM_NEURONIOS_CAMADA_UM*NUM_ENTRADAS];
double w2[NUM_NEURONIOS_CAMADA_DOIS*NUM_NEURONIOS_CAMADA_UM];

//Delta PESOS
double dw1[NUM_NEURONIOS_CAMADA_UM*NUM_ENTRADAS];
double dw2[NUM_NEURONIOS_CAMADA_DOIS*NUM_NEURONIOS_CAMADA_UM];

//Entradas
double Entrada_I1[NUM_NEURONIOS_CAMADA_UM] = {0};
double Entrada_I2[NUM_NEURONIOS_CAMADA_DOIS]  = {0};

//SAIDAS
double saida_y1[NUM_NEURONIOS_CAMADA_UM] = {0};
double saida_y2[NUM_NEURONIOS_CAMADA_DOIS]  = {0};

//Erros
double  erro_camada1[NUM_NEURONIOS_CAMADA_UM] = {0};
double  erro_camada2[NUM_NEURONIOS_CAMADA_DOIS] = {0};

double saidas[NUM_NEURONIOS_CAMADA_DOIS] = {0};

/*
|--------------------------------------------------------------------------------
| Método construtor da classe.
|--------------------------------------------------------------------------------
*/
MainWindow::MainWindow()
{
    QRect propertyDisplay = QApplication::desktop()->screenGeometry();

    this->setGeometry(QRect((propertyDisplay.width() - WINDOW_WIDHT) / 2, (propertyDisplay.height() - WINDOW_HEIGHT) / 2, WINDOW_WIDHT, WINDOW_HEIGHT));

    this->setWindowTitle(QString::fromUtf8("Rede Neural - Treinamento"));

    this->_widget = new QWidget(this);

    this->setCentralWidget(this->_widget);

    this->_labelNumeroEpocas = new QLabel(QString::fromUtf8("Número de Épocas"), this->_widget);
    this->_lineEditNumeroEpocas = new QLineEdit(this->_widget);
    this->_lineEditNumeroEpocas->setText(QString::fromUtf8("1500"));

    this->_labelPrecisao = new QLabel(QString::fromUtf8("Precisão"), this->_widget);
    this->_lineEditPrecisao = new QLineEdit(this->_widget);
    this->_lineEditPrecisao->setText(QString::fromUtf8("0.000001"));

    this->_labelTaxaAprendizado = new QLabel(QString::fromUtf8("Taxa de Aprendizado"), this->_widget);
    this->_lineEditTaxaAprendizado = new QLineEdit(this->_widget);
    this->_lineEditTaxaAprendizado->setText(QString::fromUtf8("0.01"));

    this->_labelMomentum = new QLabel(QString::fromUtf8("Momentum"), this->_widget);
    this->_lineEditMomentum = new QLineEdit(this->_widget);
    this->_lineEditMomentum->setText(QString::fromUtf8("0.99"));

    this->_labelfuncao = new QLabel(QString::fromUtf8("Função"), this->_widget);
   // this->_labelLogistica = new QLabel(QString::fromUtf8("Logística"), this->_widget);
   //this->_labelTangenteHiperbolica = new QLabel(QString::fromUtf8("T. Hiperbólica"), this->_widget);
    this->_CB_funcaoL = new QCheckBox("Logística",this->_widget);
    this->_CB_funcaoTH = new QCheckBox("T. Hiperbólica",this->_widget);
    this->_CB_funcaoL->setChecked(true);

    this->_labelAmostraTreinamento = new QLabel(QString::fromUtf8("Amostras para treinamento"), this->_widget);
    this->_lineEditAmostraTreinamento = new QLineEdit(this->_widget);
    this->_lineEditAmostraTreinamento->setEnabled(false);

    this->_labelSaidaDesejada = new QLabel(QString::fromUtf8("Saídas Desejadas"), this->_widget);
    this->_lineEditSaidaDesejada = new QLineEdit(this->_widget);
    this->_lineEditSaidaDesejada->setEnabled(false);

    this->_labelGuardarPesos = new QLabel(QString::fromUtf8("Guardar Pesos"), this->_widget);
    this->_lineEditGuardarPesos = new QLineEdit(this->_widget);
    this->_lineEditGuardarPesos->setEnabled(false);

    this->_buttonSelecionarArquivoAmostas = new QPushButton(QString::fromUtf8("..."), this->_widget);
    this->_buttonSelecionarArquivoSaidas = new QPushButton(QString::fromUtf8("..."), this->_widget);
    this->_buttonSelecionarDiretorioGuardarPesos = new QPushButton(QString::fromUtf8("..."), this->_widget);

    this->_buttonIniciarTreinamento = new QPushButton(QString::fromUtf8("Iniciar Treinamento"), this->_widget);
    this->_buttonGuardarPesos = new QPushButton(QString::fromUtf8("Guardar Pesos"), this->_widget);

    this->_layoutForm = new QGridLayout(this->_widget);

    this->_layoutForm->addWidget(this->_labelNumeroEpocas, 0, 0);
    this->_layoutForm->addWidget(this->_lineEditNumeroEpocas, 0, 1);
    this->_layoutForm->addWidget(this->_labelPrecisao, 0, 2);
    this->_layoutForm->addWidget(this->_lineEditPrecisao, 0, 3);
    this->_layoutForm->addWidget(this->_labelTaxaAprendizado, 0, 4);
    this->_layoutForm->addWidget(this->_lineEditTaxaAprendizado, 0, 5);
    this->_layoutForm->addWidget(this->_labelMomentum, 0, 6);
    this->_layoutForm->addWidget(this->_lineEditMomentum, 0, 7);
    this->_layoutForm->addWidget(this->_labelfuncao, 0, 8);
    //this->_layoutForm->addWidget(this->_labelLogistica, 1, 9);
    this->_layoutForm->addWidget(this->_CB_funcaoL,1,8);
    //this->_layoutForm->addWidget(this->_labelTangenteHiperbolica, 2, 9);
    this->_layoutForm->addWidget(this->_CB_funcaoTH,2,8);

    this->_layoutForm->addWidget(this->_labelAmostraTreinamento, 1, 0);
    this->_layoutForm->addWidget(this->_lineEditAmostraTreinamento, 1, 1, 1, 6);
    this->_layoutForm->addWidget(this->_buttonSelecionarArquivoAmostas, 1, 7);

    this->_layoutForm->addWidget(this->_labelSaidaDesejada, 2, 0);
    this->_layoutForm->addWidget(this->_lineEditSaidaDesejada, 2, 1, 1, 6);
    this->_layoutForm->addWidget(this->_buttonSelecionarArquivoSaidas, 2, 7);


    this->_layoutForm->addWidget(this->_labelGuardarPesos, 3, 0);
    this->_layoutForm->addWidget(this->_lineEditGuardarPesos, 3, 1, 1, 6);
    this->_layoutForm->addWidget(this->_buttonSelecionarDiretorioGuardarPesos, 3, 7);

    this->_layoutForm->addWidget(this->_buttonIniciarTreinamento, 4, 0);
    this->_layoutForm->addWidget(this->_buttonGuardarPesos, 4, 1);

    this->_widget->setLayout(this->_layoutForm);

    this->_chart = new QCustomPlot(this->_widget);
    this->_chart->setGeometry(QRect(0, 0, 800, 500));
    this->_chart->addGraph();
    this->_chart->xAxis->setLabel("CONTAGEM DAS ÉPOCAS");
    this->_chart->yAxis->setLabel("ERRO");
    this->_chart->xAxis->setRange(-1, 1);
    this->_chart->yAxis->setRange(0, 1);
    this->_chart->replot();

    this->_layoutForm->addWidget(this->_chart, 5, 0, 5, 0);

    this->_buttonGuardarPesos->setEnabled(false);

    this->show();

    QObject::connect(this->_buttonSelecionarArquivoAmostas, SIGNAL(clicked()), this, SLOT(slotSelecionarArquivoAmostraTreinamento()));
    QObject::connect(this->_buttonSelecionarArquivoSaidas, SIGNAL(clicked()), this, SLOT(slotSelecionarArquivoSaidaDeseja()));
    QObject::connect(this->_buttonSelecionarDiretorioGuardarPesos, SIGNAL(clicked()), this, SLOT(slotSelecionarDiretorioGuardarPesos()));
    QObject::connect(this->_buttonIniciarTreinamento, SIGNAL(clicked()), this, SLOT(slotIniciarTreinamento()));
    QObject::connect(this->_buttonGuardarPesos, SIGNAL(clicked()), this, SLOT(slotGuardarPesos()));
}

/*
|--------------------------------------------------------------------------------
| Método destrutor da classe.
|--------------------------------------------------------------------------------
*/
MainWindow::~MainWindow()
{

}

/*
|--------------------------------------------------------------------------------
| Método responsável pelo slot "SelecionarArquivoAmostraTreinamento"
|--------------------------------------------------------------------------------
*/
void MainWindow::slotSelecionarArquivoAmostraTreinamento()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("*.txt"));
    this->_lineEditAmostraTreinamento->setText(fileName);
}

/*
|--------------------------------------------------------------------------------
| Método responsável pelo slot "SelecionarArquivoSaidaDeseja"
|--------------------------------------------------------------------------------
*/
void MainWindow::slotSelecionarArquivoSaidaDeseja()
{
    QString fileNameSaida = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("*.txt"));
    this->_lineEditSaidaDesejada->setText(fileNameSaida);
}

/*
|--------------------------------------------------------------------------------
| Método responsável pelo slot "slotSelecionarDiretorioGuardarPesos"
|--------------------------------------------------------------------------------
*/
void MainWindow::slotSelecionarDiretorioGuardarPesos()
{
    QString fileNameSaida = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("*.txt"));
    this->_lineEditGuardarPesos->setText(fileNameSaida);
}


/*
|--------------------------------------------------------------------------------
| Método responsável por calcular a funcao de ativaçao 0-Logistica | 1- Tangente hiperbólica
|--------------------------------------------------------------------------------
*/
double MainWindow::FuncaoAtivacao(double net, int funcao, double a)
{
    if (!funcao)
    {

        /*
            1
            y(n) = ---------------
            1 + exp(-a.net)
            */

        return( 1.0 / (1.0 + exp(-a * net)) );

    }
    else
    {

        /*
            exp(a.net) - exp(-a.net)
            y(n) = ------------------------
            exp(a.net) + exp(-a.net)
            */

        return( (exp(a * net) - exp(-a * net)) / (exp(a * net) + exp(-a * net)) );
    }

}

/*
|--------------------------------------------------------------------------------
| Método responsável por calcular a derivada
|--------------------------------------------------------------------------------
*/
double MainWindow::derivada(double net, int funcao, double a)
{

    if (!funcao){

        /*
         1                       1
         y(n) = --------------- * ( 1 - --------------- )
         1 - exp(-a.net)         1 - exp(-a.net)
         */

        return( (1.0 / (1.0 + exp(-a * net))) * (1.0 - (1.0 / (1.0 + exp(-a * net)))) );

    }
    else
    {

        /*
         exp(a.net) - exp(-a.net)
         y(n) = 1 - ( ------------------------ )≤
         exp(a.net) + exp(-a.net)

         */

        return( 1.0 - pow((exp(a * net) - exp(-a * net)) / (exp(a * net) + exp(-a * net)),2) );
    }
}

/*
|--------------------------------------------------------------------------------
| Funcao para dar um reset nos arrays dos pesos da camada 1 (PesosCamada1) e da camada 2 (PesosCamada2)
|--------------------------------------------------------------------------------
*/
void MainWindow::zeraPesos(int camada){
    // Zera os pesos da camada de entrada (input) e da camada 1
    if (camada ==1) {
        for (int j = 0; j < (NUM_NEURONIOS_CAMADA_UM*NUM_ENTRADAS); j++)
        {
            w1[j] = 0.0;
        }
    }else if (camada==2){
        // Zera o vetor de pesos da camada 1 e da camada 2.
        for (int j = 0; j < (NUM_NEURONIOS_CAMADA_DOIS*NUM_NEURONIOS_CAMADA_UM); j++)
        {
            w2[j] = 0.0;
        }
    }


}

/*
|--------------------------------------------------------------------------------
| Funcao responsavel por dar um reset nos vetores que ennvolvem os neoronios da camada 1  e da camada 2
|--------------------------------------------------------------------------------
*/
void MainWindow::zeraVetoresNeuronios(int camada){

    if (camada ==1) {
        // Zera os vetores envolvidos aos neuronios da camada 1.
        for (int j = 0; j < NUM_NEURONIOS_CAMADA_UM; j++)
        {
            Entrada_I1[j] = 0.0;
            saida_y1[j] = 0.0;
            erro_camada1[j] = 0.0;
        }
    }else if (camada==2){

        // Zera os vetores envolvidos aos neuronios da camada 2.
        for (int j = 0; j < NUM_NEURONIOS_CAMADA_DOIS; j++)
        {
            Entrada_I2[j] = 0.0;
            saida_y2[j] = 0.0;
            erro_camada2[j] = 0.0;
        }
    }


}

/*
|--------------------------------------------------------------------------------
|Funcao responsavel por randomizar os pesos iniciais de cada camada
|--------------------------------------------------------------------------------
*/
void MainWindow:: randomizaPesos(int camada){

    if (camada ==1) {
        // Randomiza os pesos para a camada 1.
        for (int j = 0; j < (NUM_NEURONIOS_CAMADA_UM*NUM_ENTRADAS); j++)
        {
            w1[j] = (float)(rand() % 100) / 1000;
        }

    }else if (camada==2){
        // Randomiza os pesos para a camada 2.
        for (int j = 0; j < (NUM_NEURONIOS_CAMADA_DOIS*NUM_NEURONIOS_CAMADA_UM); j++)
        {
            w2[j] = (float)(rand() % 100) / 1000;
        }

    }

}


/*
|--------------------------------------------------------------------------------
| Método responsável pelo slot "IniciarTreinamento"
|--------------------------------------------------------------------------------
*/
void MainWindow::slotIniciarTreinamento()
{
    if(this->_lineEditNumeroEpocas->text() == ""){
        QMessageBox m;
        m.setText("Campo número de épocas está vazio.");
        m.exec();
        return;
    }

    if(this->_lineEditPrecisao->text() == ""){
        QMessageBox m;
        m.setText("Campo precisão está vazio.");
        m.exec();
        return;
    }

    if(this->_lineEditTaxaAprendizado->text() == ""){
        QMessageBox m;
        m.setText("Campo taxa de aprendizado está vazio.");
        m.exec();
        return;
    }

    if(this->_lineEditAmostraTreinamento->text() == ""){
        QMessageBox m;
        m.setText("Campo amostras de treinamento está vazio.");
        m.exec();
        return;
    }

    if(this->_lineEditSaidaDesejada->text() == ""){
        QMessageBox m;
        m.setText("Campo saídas desejadas está vazio.");
        m.exec();
        return;
    }

    if(this->_lineEditGuardarPesos->text() == ""){
        QMessageBox m;
        m.setText("Campo guardar pesos está vazio.");
        m.exec();
        return;
    }

    this->_lineEditNumeroEpocas->setEnabled(false);
    this->_lineEditPrecisao->setEnabled(false);
    this->_lineEditTaxaAprendizado->setEnabled(false);
    this->_lineEditAmostraTreinamento->setEnabled(false);
    this->_lineEditSaidaDesejada->setEnabled(false);
    this->_buttonSelecionarArquivoAmostas->setEnabled(false);
    this->_buttonSelecionarArquivoSaidas->setEnabled(false);
    this->_buttonIniciarTreinamento->setEnabled(false);
    this->_lineEditMomentum->setEnabled(false);

    /*
|--------------------------------------------------------------------------------
| Carrega Arquivo de Amostras
|--------------------------------------------------------------------------------
*/
    QString path_amostras = this->_lineEditAmostraTreinamento->text();

    QFile amostras(path_amostras);
    if(!amostras.open(QIODevice::ReadOnly)) {
        qDebug() << amostras.errorString();
        exit(0);
    }

    QTextStream in_a(&amostras);

    while(!in_a.atEnd()) {
        QString line_a = in_a.readLine();
        QStringList fields_a = line_a.split(",");

        QVector<double> amostra;

        for(int i = 0; i < fields_a.size(); i++) {
            double value_a = fields_a.at(i).toDouble();
            amostra.append(value_a);
        }

        amostras_treinamento.append(amostra);
    }

    amostras.close();

    /*
|--------------------------------------------------------------------------------
| Carrega Arquivo de Saidas
|--------------------------------------------------------------------------------
*/
    QString path_saidas = this->_lineEditSaidaDesejada->text();

    QFile saidas(path_saidas);
    if(!saidas.open(QIODevice::ReadOnly)) {
        qDebug() << saidas.errorString();
        exit(0);
    }

    QTextStream in_s(&saidas);

    while(!in_s.atEnd()) {
        QString line_s = in_s.readLine();
        QStringList fields_s = line_s.split(",");
        QVector<double> saida;

        for(int i = 0; i < fields_s.size(); i++) {
            double value_s = fields_s.at(i).toDouble();
            saida.append(value_s);
        }

        saidas_desejada.append(saida);
    }

    saidas.close();
    /*
|--------------------------------------------------------------------------------
| Treinamento
|--------------------------------------------------------------------------------
*/
    double precisao = this->_lineEditPrecisao->text().toDouble(); // pega precisao informada na interface
    double aprendizado = this->_lineEditTaxaAprendizado->text().toDouble(); // pega a taxa de aprendizado infomada da interface
    int numepocas = this->_lineEditNumeroEpocas->text().toInt(); // pega o numero de épocas informada na interaface
    double MOMENTUM = this->_lineEditMomentum->text().toDouble();           // Termo de momentum.

    QVector<double> gepocas(numepocas), gerro(numepocas);// vetores que guardam a epoca e seu respectivo erro

    int epocas = 0;
    double erro_medio_quadratico= 0;
    double erro_quadratico = 0;
    double somatorio = 0;
    int funcao = 0;

    if(this->_CB_funcaoL->isChecked()){
        funcao = 0;
        this->_CB_funcaoTH->setChecked(false);
    }
    if(this->_CB_funcaoTH->isChecked()){
        funcao = 1;
        this->_CB_funcaoL->setChecked(false);
    }

    zeraPesos(1);
    zeraPesos(2);

    zeraVetoresNeuronios(1);
    zeraVetoresNeuronios(2);

    randomizaPesos(1);
    randomizaPesos(2);




    // pesos randomizados para a camada 1.
    qDebug()  <<" pesos randomizados para a camada 1";
    for (int j = 0; j < (NUM_NEURONIOS_CAMADA_UM*NUM_ENTRADAS); j++)
    {
        qDebug() <<w1[j] ;
    }


    // pesos randomizados para a camada 2.
    qDebug()  <<" pesos randomizados para a camada 2";
    for (int j = 0; j < (NUM_NEURONIOS_CAMADA_DOIS*NUM_NEURONIOS_CAMADA_UM); j++)
    {
        qDebug() <<w2[j] ;
    }


    qDebug()  <<"**************************** Inico Treinamento ****************************";

    do{

        // Propaga os k padrıes pela rede.
        for (int k = 0; k < amostras_treinamento.size(); k++)
        {
            qDebug() <<"calc c1";
            //C·lculo para camada C1.
            int n = 0;
            for (int j = 0; j < NUM_NEURONIOS_CAMADA_UM; j++)
            {
                somatorio = 0;
                for (int i = 0; i < NUM_ENTRADAS; i++)
                {
                    somatorio += w1[n] * amostras_treinamento.at(k).at(i);//amostras_treinamento.at(k);
                    n += 1;
                }
                Entrada_I1[j] = somatorio;
                saida_y1[j] = FuncaoAtivacao(Entrada_I1[j],funcao,1.0);
            }
            qDebug() <<"calc c2";
            //C·lculo para camada C2.
            n = 0;
            for (int j = 0; j < NUM_NEURONIOS_CAMADA_DOIS; j++)
            {
                somatorio = 0;
                for (int i = 0; i < NUM_NEURONIOS_CAMADA_UM; i++)
                {
                    somatorio += w2[n] * saida_y1[i];
                    n += 1;
                }
                Entrada_I2[j] = somatorio;
                saida_y2[j] = FuncaoAtivacao(Entrada_I2[j],funcao,1.0);
            }

            //********************* C·lculo do Erro MÈdio Quadr·tico ************************
            qDebug() <<"Calculo do Erro Quadratico";
            //Calculo do Erro Quadratico.
            erro_quadratico = 0;
            for(int j = 0; j < NUM_NEURONIOS_CAMADA_DOIS; j++)
            {
                erro_quadratico += pow((saidas_desejada[k][j] - saida_y2[j]),2);
            }

            //Calculo do Erro Medio Quadratico (Criterio de Parada).
            erro_medio_quadratico += (0.5 * erro_quadratico);


            //**************************** Retropropagacao do Erro **************************

            //Calculo do erro para camada 2.
            for (int i = 0; i < NUM_NEURONIOS_CAMADA_DOIS; i++)
            {
                erro_camada2[i] = (saidas_desejada[k][i] - saida_y2[i]) * derivada(Entrada_I2[i],funcao,1.0);
            }

            //Atualizacao dos pesos para camada 2.
            for (int i = 0; i < NUM_NEURONIOS_CAMADA_UM; i++)
            {
                n = 0;
                for (int j = 0; j < NUM_NEURONIOS_CAMADA_DOIS; j++)
                {
                    dw2[n + i] = aprendizado * saida_y1[i] * erro_camada2[j] + (MOMENTUM * dw2[n + i]);
                    w2[n + i] = w2[n + i] + dw2[n + i];
                    n += NUM_NEURONIOS_CAMADA_UM;
                }
            }

            //Calculo do erro para camada 1.
            for (int i = 0; i < NUM_NEURONIOS_CAMADA_UM; i++)
            {
                n = 0;
                somatorio = 0;
                for (int j = 0; j < NUM_NEURONIOS_CAMADA_DOIS; j++)
                {
                    somatorio += (erro_camada2[j] * w2[n + i]);
                    n += NUM_NEURONIOS_CAMADA_UM;
                }
                erro_camada1[i] = somatorio * derivada(Entrada_I1[i],funcao,1.0);
            }

            //Atualizacao dos pesos para camada 1.
            for (int i = 0; i < NUM_ENTRADAS; i++)
            {
                n = 0;
                for (int j = 0; j < NUM_NEURONIOS_CAMADA_UM; j++)
                {
                    dw1[n + i] = aprendizado * amostras_treinamento.at(k).at(i) * erro_camada1[j] + (MOMENTUM * dw1[n + i]);
                    w1[n + i] = w1[n + i] + dw1[n + i];
                    n += NUM_ENTRADAS;
                }
            }
        }

        // C·lculo do erro mÈdio quadr·tico da Època de treinamento.
        erro_medio_quadratico = (1.0 / amostras_treinamento.size()) * erro_medio_quadratico;

        gerro.append(erro_medio_quadratico);
        gepocas.append(epocas);

        epocas++;

       // qDebug() << erro_medio_quadratico;
        //qDebug() << epocas;

        erro_medio_quadratico = 0;


    } while(epocas < numepocas ||erro_medio_quadratico >= precisao);
    qDebug()  <<"**************************** Fim Treinamento ****************************";
    /*
|--------------------------------------------------------------------------------
| Gráfico
|--------------------------------------------------------------------------------
*/

    this->_chart->graph(0)->setData(gepocas, gerro);
    this->_chart->xAxis->setLabel("CONTAGEM DAS ÉPOCAS");
    this->_chart->yAxis->setLabel("ERRO");
    this->_chart->xAxis->setRange(0, numepocas);
    this->_chart->yAxis->setRange(0, 1.5);
    this->_chart->replot();

    /*
|--------------------------------------------------------------------------------
| Habilitar Botões
|--------------------------------------------------------------------------------
*/
    this->_lineEditNumeroEpocas->setEnabled(true);
    this->_lineEditPrecisao->setEnabled(true);
    this->_lineEditTaxaAprendizado->setEnabled(true);
    this->_lineEditAmostraTreinamento->setEnabled(true);
    this->_lineEditSaidaDesejada->setEnabled(true);
    this->_buttonSelecionarArquivoAmostas->setEnabled(true);
    this->_buttonSelecionarArquivoSaidas->setEnabled(true);
    this->_buttonIniciarTreinamento->setEnabled(true);
    this->_buttonGuardarPesos->setEnabled(true);
    this->_lineEditMomentum->setEnabled(true);

}


/*
|--------------------------------------------------------------------------------
| Método responsável pelo slot "GuardarPesos"
|--------------------------------------------------------------------------------
*/
void MainWindow::slotGuardarPesos()
{
    QFile pesos(this->_lineEditGuardarPesos->text());

    if(!pesos.open(QIODevice::ReadWrite)) {
        qDebug() << pesos.errorString();
        exit(0);
    }

    QTextStream in_p(&pesos);

    in_p.operator <<("W1(Pesos Camada 1):");
    in_p.operator << ("{");
    for(int i = 0; i < NUM_NEURONIOS_CAMADA_UM * NUM_ENTRADAS; i++) {
        in_p.operator << (w1[i]);
        in_p.operator << (",");
    }
    in_p.operator << ("}, ");
    in_p.operator <<("\n");

    in_p.operator <<("W2 (Pesos Camada 2):");
    in_p.operator << ("{");
    for(int i = 0; i < NUM_NEURONIOS_CAMADA_UM * NUM_NEURONIOS_CAMADA_DOIS; i++) {
        in_p.operator << (w2[i]);
        in_p.operator << (",");
    }
    in_p.operator << ("}, ");
    in_p.operator <<("\n");

    pesos.close();
}
