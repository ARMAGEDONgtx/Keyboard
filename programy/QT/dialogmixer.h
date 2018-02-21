/*!
    @author Michal Wróblewski
    @file dialogmixer.h
    @date 22-12-2017
    @brief SM PROJECT

*/

#ifndef DIALOGMIXER_H
#define DIALOGMIXER_H

#include <QDialog>

namespace Ui {
class DialogMixer;
}


/*!
 * \brief The DialogMixer class
 *
 *  klasa typu dialog,
 *  do parametryzowania czestotliwosci
 *  dla danych klawiszy
 */
class DialogMixer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMixer(QWidget *parent = 0); //!< konstrukor klasy
    ~DialogMixer(); //!< dekonstruktor klasy
    void set_value_to_change(int *data); //!< funckja ustawiajaca, ktora wartosc bedzie zmieniana w dialogu
private slots:
    void on_horizontalSlider_valueChanged(int value); //!< slot to obslugi przesuniecia suwaka
private:
    int m_value; //!< zmienna przechowujaca wartosc dialogu - czestotliwosc
    int *value_to_change; //!< wskaznik do zmiennej, ktora dialog zmienia
    Ui::DialogMixer *ui; //!< ui
};

#endif // DIALOGMIXER_H
