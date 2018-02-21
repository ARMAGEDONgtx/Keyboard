/*!
    @author Michal Wróblewski
    @file serialmanager.h
    @date 22-12-2017
    @brief SM PROJECT
*/


#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QList>
#include <QDebug>
#include <QByteArrayData>
#include <QTimer>


/*!
 * \brief The SerialManager class
 *
 *
 * klasa menadżera komunikacji szeregowej
 *
 */
class SerialManager : public QObject
{
    Q_OBJECT
public:
    SerialManager(); //!< konstruktor
    QStringList get_ports(); //!< funkcja - pobieranie portow widzianych przez system
    void set_port(QString name, QString baud, QString data, QString parity, QString stop); //!< funckja - ustaiwanie parametrow polaczenia
    void close_port(); //!< funkcja - zamykanie polaczenia
    void send_msg(QString msg); //!< funkcja - wysylanie danych
    ~SerialManager(); //!< dekonstrukor
private:
    QStringList PortsList; //!< lista widzanych portow przez system
    QSerialPort ActivePort; //!< aktywny , wybrany port
    QTimer m_timer; //!< timer do obslugi komunikacji
    QByteArray m_readData; //!< przechowywane dane z portu
signals:
    void ErrorMsg(QString msg); //!< sygnal z bledem przekierowaywany do klasy glownego okna
    void NewData(QString msg); //!< sygnal z nowymi danymi przekierowywany do klasy glownego okna
private slots:
    void handleReadyRead(); //!< slot - obsluga pojawiajacych sie nowych danych
    void handleTimeout(); //!< slot - obsluga przepelnienia timera
    void handleError(QSerialPort::SerialPortError serialPortError); //!< slot - obsluga bledow
};

#endif // SERIALMANAGER_H
