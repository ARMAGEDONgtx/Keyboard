/*!
    @author Michal Wróblewski
    @file mainwindow.cpp
    @date 22-12-2017
    @brief SM PROJECT

*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent klasa bazowa
 *
 *
 * 1) Uzycie signalmapper'a do sygnalow od przyciskow, aby moc latwo przypisac wartosc do guzika
 * 2) Połączenie wszystkich sygnałów i slotów wykorzystywanych do komunikacji itp
 * 3) stworzenie list do parametryzacji komunikacji szeregowej i wypelnienie nimi QBox'ow
 * 4) Wstepny inicjalizacja gui do nagrywania i odtwarzania piosenek
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_serialmng(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //MAPOWANIE PRZYCISKÓW********************************************************************
    connect(&signalmapp, SIGNAL(mapped(int)),this, SIGNAL(klawClicked(int)));
    buttons.append(ui->klaw0); buttons.append(ui->klaw1); buttons.append(ui->klaw2); buttons.append(ui->klaw3);
    buttons.append(ui->klaw4); buttons.append(ui->klaw5); buttons.append(ui->klaw6); buttons.append(ui->klaw7);
    buttons.append(ui->klaw8); buttons.append(ui->klaw9); buttons.append(ui->klaw_hash); buttons.append(ui->klaw_star);
    for(int i =0; i < 12;i++)
    {
        signalmapp.setMapping(buttons[i], i);
        connect(buttons[i], SIGNAL(clicked()), &signalmapp,SLOT(map()));
    }
    connect(this, SIGNAL(klawClicked(int)), this, SLOT(handleklawClick(int)));
    for(int i =0; i < 12;i++)
        values.append(0);
    //******************************************************************************************


    //SYGNALY -> SLOTY******************************************************************
    connect(&m_serialmng, SIGNAL(ErrorMsg(QString)),this, SLOT(CommunicationExcetion(QString)));
    connect(&m_serialmng, SIGNAL(NewData(QString)), this, SLOT(DataRecieved(QString)));
    connect(ui->actionGoTo,SIGNAL(triggered(bool)), this, SLOT(PageUart()));
    connect(ui->actionGoTo_2,SIGNAL(triggered(bool)),this,SLOT(PageMixer()));
    connect(ui->actionSongs,SIGNAL(triggered(bool)), this, SLOT(PageSongs()));
    //************************************************************************************


    //stworzenie list do parametryzacji komunikacji szeregowej i wypelnienie nimi QBox'ow***********
    baudlist << QString::number(1200) << QString::number(2400)<< QString::number(4800)<< QString::number(9600);
    baudlist << QString::number(19200) << QString::number(38400)<< QString::number(57600)<< QString::number(115200);
    ui->BaudrateBox->addItems(baudlist);
    datalist << QString::number(5) << QString::number(6) << QString::number(7) << QString::number(8);
    ui->DataBox->addItems(datalist);
    paritylist << QString("None") << QString("Odd") << QString("Even") << QString("Mark") << QString("Space");
    ui->ParityBox->addItems(paritylist);
    stoplist << QString("One") << QString("Two")<< QString("OnePointFive");
    ui->StopBox->addItems(stoplist);
    ui->PortBox->addItems(m_serialmng.get_ports());
    //******************************************************************************************


    // Wstepny inicjalizacja gui do nagrywania i odtwarzania piosenek***********************************
    updateLCD();
    QPair<QString, QString> tmp;
    tmp.first = "song1";
    tmp.second = "lalalal";
    songs.append(tmp);
    QPair<QString, QString> tmp2;
    tmp2.first = "song2";
    tmp2.second = "hahahaha";
    songs.append(tmp2);
   // songs.append(qMakePair("song2","blbalbablbablabla"));
    ui->songsBox->addItem(songs[0].first);
    ui->songsBox->addItem(songs[1].first);
    //******************************************************************************************

    //Ustawienie QDiala zeby nie musiec skalowac przez wartosc ADC mikrokontrolera
    ui->volume_dial->setMinimum(0);
    ui->volume_dial->setMaximum(4095);

    //Zaladowanie piosnek
    /*
    QFile file("songs\in.txt");
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          return;

      QTextStream in(&file);
      while (!in.atEnd()) {
          QString line = in.readLine();
          process_line(line);
      }
      */

}


/*!
 * \brief MainWindow::~MainWindow
 *
 *
 * destruktor klasy, nic nie robimy
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::updateLCD
 *
 *
 *
 * SLOT DO UPDATE'owania wyswietlaczy, zebysmy wiedzieli jaka mamy konfiguracje
 */
void MainWindow::updateLCD()
{
    ui->lcd_0->display(values[0]); ui->lcd_1->display(values[1]); ui->lcd_2->display(values[2]);
    ui->lcd_3->display(values[3]); ui->lcd_4->display(values[4]); ui->lcd_5->display(values[5]);
    ui->lcd_6->display(values[6]); ui->lcd_7->display(values[7]); ui->lcd_8->display(values[8]);
    ui->lcd_9->display(values[9]); ui->lcd_hash->display(values[10]); ui->lcd_star->display(values[11]);
}

/*!
 * \brief MainWindow::on_ConnectButton_clicked
 *
 *
 *
 * SLOT do przyciusku 'connect' przesylalmy wsyzstkie wybrane aktualnie parametry w QBox'ach
 */
void MainWindow::on_ConnectButton_clicked()
{
    m_serialmng.set_port(ui->PortBox->currentText(),ui->BaudrateBox->currentText(), ui->DataBox->currentText(),
                         ui->ParityBox->currentText(),ui->StopBox->currentText());
}

/*!
 * \brief MainWindow::CommunicationExcetion
 * \param msg - wiadomosc z bledem
 *
 *
 * wyswietlamy wszystkie komunikaty o komunikacji szeregowej
 */
void MainWindow::CommunicationExcetion(QString msg)
{
    ui->ExceptionText->document()->setPlainText(msg);
}

/*!
 * \brief MainWindow::on_DisconnectButton_clicked
 *
 *
 * SLOT do przyciusku 'disconnect', zamykamy port
 */
void MainWindow::on_DisconnectButton_clicked()
{
    m_serialmng.close_port();
}

/*!
 * \brief MainWindow::on_SendButton_clicked
 *
 *
 *
 * przesylanie wszystkiego po UART'cie co wpisalismy do pola PlainText
 */
void MainWindow::on_SendButton_clicked()
{
    m_serialmng.send_msg(ui->SendPlainText->document()->toPlainText());
}

/*!
 * \brief MainWindow::DataRecieved
 * \param data - dane z UART'a
 *
 *
 * dodawanie odebranych danych po UART'cie do PlainText'u
 */
void MainWindow::DataRecieved(QString data)
{
    //przeslane dane potencjometra
    if(data[0] == '@')
    {
        QString dial_val = data.mid(1,4);
        ui->volume_dial->setValue(dial_val.toInt());
        ui->lcd_volume->display(dial_val.toDouble()/4095.0 * 100.0);
    }
    //przeslane dane o nacisnieciu klawisza do nagrywania pisoenek - nie zaimplementowane
    if(data[0] == '!')
    {
        QString freq = data.mid(1,3);


    }
    ui->RecievedPlainText->moveCursor(QTextCursor::End);
    ui->RecievedPlainText->insertPlainText(data);
    ui->RecievedPlainText->moveCursor(QTextCursor::End);
}

/*!
 * \brief MainWindow::on_ClearButton_clicked
 *
 *
 *
 * SLOT od przycisku 'clear'
 * czyscimy zawartosc odbernaych danych
 */
void MainWindow::on_ClearButton_clicked()
{
    ui->RecievedPlainText->document()->clear();
}

/*!
 * \brief MainWindow::PageUart
 *
 *
 *
 * SLOT do zmiany strony w QStackedWidget
 * na storne od konfiguarcji UART'a
 */
void MainWindow::PageUart()
{
    ui->stackedWidget->setCurrentWidget(ui->Page1);
}

/*!
 * \brief MainWindow::PageMixer
 *
 *
 *
 * SLOT do zmiany strony w QStackedWidget
 * na strone od konfiguracji czestotliwosci
 */
void MainWindow::PageMixer()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

/*!
 * \brief MainWindow::PageSongs
 *
 *
 *
 * SLOT do zmiany strony w QStackedWidget
 * na strone od piosenek - nie zaimplementowane piosenki
 */
void MainWindow::PageSongs()
{
    ui->stackedWidget->setCurrentWidget(ui->pageSongs);
}

/*!
 * \brief MainWindow::on_pushButton_clicked
 *
 *
 * SLOT od przesylania konfiguracji,
 * przeysylamy cala tablice zmiennych konfigarayjnych od keypada
 */
void MainWindow::on_pushButton_clicked()
{
    QString tmp;
    for(int i =0;i < 12; i++)
    {
        tmp.append(QString::number(values[i]));
        tmp += ",";
    }
    tmp+= "!";
    qDebug() << tmp;
    m_serialmng.send_msg(tmp);
}

/*!
 * \brief MainWindow::handleklawClick
 * \param numb numer identyfikujacy, ktory przycisk kliknieto
 *
 *
 * po kliknieciu danego przycisku, otwieramy dialog do zmianny parametrow
 * tutaj wykorzystujemy funkcjonalnosc mapera sygnalow
 */
void MainWindow::handleklawClick(int numb)
{
    dialog.set_value_to_change(&(values[numb]));
    dialog.exec();
    updateLCD();
}

/*!
 * \brief MainWindow::on_songsBox_activated
 * \param arg1 nazwa piosenki
 *
 *
 *
 * wyswietlanie textu wybranej piosenki - piosenki nie zaimplementowany
 */
void MainWindow::on_songsBox_activated(const QString &arg1)
{
    for(int i = 0; i< songs.size();i++)
    {
        if(songs[i].first == arg1)
            ui->textSong->document()->setPlainText(songs[i].second);

    }
}
