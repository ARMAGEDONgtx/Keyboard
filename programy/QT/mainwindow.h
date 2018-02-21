/*!
    @author Michal Wróblewski
    @file mainwindow.h
    @date 22-12-2017
    @brief SM PROJECT
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "serialmanager.h"
#include "dialogmixer.h"
#include <QSignalMapper>
#include <QPair>

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 *
 * klasa głównego okna
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    void updateLCD(); //!< funckja - aktualizowanie wyswietlaczy
private slots:
    void on_ConnectButton_clicked(); //!< slot od przycisku "connect"
    void CommunicationExcetion(QString msg); //!< slot - obsluga bledow komunikacji

    void on_DisconnectButton_clicked(); //!< slot od przycisku "disconnect"

    void on_SendButton_clicked(); //!< slot od przecisku "send"
    void DataRecieved(QString data); //!< slot do przetwarzania otrzymanych danych po UART

    void on_ClearButton_clicked(); //!< slot od przycisku "clear"

    void PageUart(); //!< funkcja - zmiana strony na konfiguracje UART'a
    void PageMixer(); //!< funckja - zmiana strony na ustawienia czestotliwosci
    void PageSongs(); //!< funckja - zmiana strony na piosneki - odtwarzania piosneke nie zreazliowane
    void on_pushButton_clicked(); //!< slot od przycisku wysylanai konfiguracji

    void handleklawClick(int numb); //!< wywolywanie dialogu to konfiguracji czestotliwosci
    void on_songsBox_activated(const QString &arg1); //!< slot - zmienianie pisoenki - nie zaimplementowane do konca

signals:
    void klawClicked(int numb); //!< sygnal generowany przez klikniecie przycisku, powiazane z maperem sygnalow

private:
    QSignalMapper signalmapp; //!< mapper sygnalow do przyciskow
    QList<QPushButton*> buttons; //!< tablica przechowujaca wskazniki do przyciskow
    QList<int> values; //!< tablica z wartosciami czestotliwosci odpowiadajacych przyciska
    QList<QPair<QString, QString>> songs; //!< tytuly + teksty piosenek - nie zaimplelemntowane
    SerialManager m_serialmng; //!< menedzer UART'a
    QString actual_file_name; //!< nazwa pliku, do ktorego zapisywac pisoneke - nie zaimplementowane
    QStringList baudlist; //!< lista baudrate'ow
    QStringList stoplist; //!< lista bit'ow stopu
    QStringList paritylist; //!< lista bit'ow parzystosci
    QStringList datalist; //!< lista dlugosci danych
    DialogMixer dialog; //!< dialog od zmieniania czestotliwosci
    Ui::MainWindow *ui; //!< ui
};

#endif // MAINWINDOW_H
