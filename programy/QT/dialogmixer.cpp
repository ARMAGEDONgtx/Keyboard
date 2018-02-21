/*!
    @author Michal Wróblewski
    @file dialogmixer.cpp
    @date 22-12-2017
    @brief SM PROJECT

*/


#include "dialogmixer.h"
#include "ui_dialogmixer.h"

/*!
 * \brief DialogMixer::DialogMixer
 * \param parent klasa bazowa
 *
 *
 * konstruktor klasy
 * ustawiamy ui
 */
DialogMixer::DialogMixer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMixer)
{
    ui->setupUi(this);
}

/*!
 * \brief DialogMixer::~DialogMixer
 *
 *
 * dekonstruktor klasy
 * przy konczeniu dialogu przekierwujemy
 * wskaznik aby na nic nie wskazywal
 */
DialogMixer::~DialogMixer()
{
    value_to_change = NULL;
    delete ui;
}

/*!
 * \brief DialogMixer::set_value_to_change
 * \param data wskaźnik do zmiennej, która będzie zmieniana
 *
 *
 * ustaiwamy wyswieltacz i suwak
 * na zadana wartosc
 */
void DialogMixer::set_value_to_change(int *data)
{
    value_to_change = data;
    ui->lcdNumber->display(*data);
    ui->horizontalSlider->setValue(*data);
}

/*!
 * \brief DialogMixer::on_horizontalSlider_valueChanged
 * \param value pozycja suwaka
 *
 *
 * jezeli zmieni sie pozycja suwaka zmieniamy wartosc zmiennej,
 * oraz aktualizujemy wyswieltacz
 */
void DialogMixer::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber->display(value);
    *value_to_change = value;
}
