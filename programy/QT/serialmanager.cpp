/*!
    @author Michal Wróblewski
    @file serialmanager.cpp
    @date 22-12-2017
    @brief SM PROJECT

*/

#include "serialmanager.h"

/*!
 * \brief SerialManager::SerialManager
 *
 *
 * 1) laczymy wszystkie SYGNALY->SLOTY
 * 2) startujemy timer
 */
SerialManager::SerialManager()
{
    connect(&ActivePort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(&ActivePort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));

    m_timer.start(1000);
}

/*!
 * \brief SerialManager::get_ports
 * \return lista portow
 *
 *
 *
 * pobieram wszystkie dostepne porty i je zwracamy w QStringList
 */
QStringList SerialManager::get_ports()
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
            PortsList.append(info.portName());
    }

    return PortsList;
}

/*!
 * \brief SerialManager::set_port
 * \param name nazwa portu
 * \param baud baudrate
 * \param data dlugosc danych
 * \param parity konfiguarcja bitow parzystosci
 * \param stop konfiguracja bitow stopu
 *
 *
 *
 * no podstawie parametrów wybranych z QBox'ów ustaiwamy komunikacje i otwieramy port
 */
void SerialManager::set_port(QString name, QString baud, QString data, QString parity, QString stop)
{
    ActivePort.setPortName(name);
    ActivePort.setBaudRate(baud.toInt());

    if(data == "5")
    {
        ActivePort.setDataBits(QSerialPort::Data5);
    }
    else if(data == "6")
    {
       ActivePort.setDataBits(QSerialPort::Data6);
    }
    else if(data == "7")
    {
        ActivePort.setDataBits(QSerialPort::Data7);
    }
    else if(data == "8")
    {
        ActivePort.setDataBits(QSerialPort::Data8);
    }

    if (parity == "None")
    {
        ActivePort.setParity(QSerialPort::NoParity);
    }
    else if(parity == "Odd")
    {
        ActivePort.setParity(QSerialPort::OddParity);
    }
    else if(parity == "Even")
    {
        ActivePort.setParity(QSerialPort::EvenParity);
    }
    else if(parity == "Space")
    {
        ActivePort.setParity(QSerialPort::SpaceParity);
    }
    else if(parity == "Mark")
    {
        ActivePort.setParity(QSerialPort::MarkParity);
    }

    if (stop == "One")
    {
        ActivePort.setStopBits(QSerialPort::OneStop);
    }
    else if(stop == "Two")
    {
       ActivePort.setStopBits(QSerialPort::TwoStop);
    }
    else if(stop == "OnePointFive")
    {
        ActivePort.setStopBits(QSerialPort::OneAndHalfStop);
    }

    if(QSerialPortInfo(ActivePort).isBusy())
    {
        //Sygnał z błedem
        //emit ErrorMsg("serial port is busy");
    }

    qDebug() << QSerialPortInfo(ActivePort).isBusy();

    close_port();

    if(!ActivePort.open(QIODevice::ReadWrite))
    {
        //emit ErrorMsg("couldn't open serial port");
    }
    qDebug() << "port opened";

    qDebug() << ActivePort.portName() + ActivePort.baudRate() + ActivePort.dataBits() +  ActivePort.parity() + ActivePort.stopBits();

}

/*!
 * \brief SerialManager::close_port
 *
 *
 *
 * zamykanie portu i wyswietlanie komunikatu
 */
void SerialManager::close_port()
{
    if(ActivePort.isOpen())
    {
        ActivePort.close();
        qDebug()<< "port is closed";
    }
}

/*!
 * \brief SerialManager::send_msg
 * \param msg tekst wiadomosci do przeslania
 *
 *
 *
 * wysylanie danych po UART'cie w postacji tablicy char'ow
 */
void SerialManager::send_msg(QString msg)
{
    if(ActivePort.isOpen())
    {
        ActivePort.write(msg.toStdString().c_str());
        ActivePort.waitForBytesWritten(1000);
    }
    else
    {
        //emit ErrorMsg("port hasn't been opened");
    }
}

/*!
 * \brief SerialManager::~SerialManager
 *
 *
 *
 * destruktor klasy, zamykamy port
 */
SerialManager::~SerialManager()
{
    close_port();
}

/*!
 * \brief SerialManager::handleReadyRead
 *
 *
 * jezeli bedzie odpowiedni sygnal do dodajemy odczytane dane do tablicy 'm_readData'
 */
void SerialManager::handleReadyRead()
{
    m_readData.append(ActivePort.readAll());
    qDebug() << m_readData;
}


/*!
 * \brief SerialManager::handleTimeout
 *
 *
 *
 * po przepelnieniu timera,
 * jezeli zgromadzilismy jakies odczytane dane,
 * to wysylamy je do PlainText'u aby moc je podejrzec
 * emitujemy odpowiedni sygnal, ktory zostanie obsluzony w innej klasie
 */
void SerialManager::handleTimeout()
{
    if(!m_readData.isEmpty())
    {
        emit NewData(m_readData);
        m_readData.clear();
    }
}

/*!
 * \brief SerialManager::handleError
 * \param serialPortError komunikat bledu
 *
 *
 *
 * obsluga bledow komunikacji,
 * emitujemy odpowiedni sygnal, ktory zostanie obsluzony w innej klasie
 */
void SerialManager::handleError(QSerialPort::SerialPortError serialPortError)
{
  emit ErrorMsg(ActivePort.portName() + " " + ActivePort.errorString());
}
