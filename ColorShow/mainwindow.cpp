#include "mainwindow.h"

#include <QBoxLayout>
#include <QPalette>
#include "panel.h"
#include <QColorDialog>
#include <QPushButton>
#include <QApplication>
class ColorReview: public QWidget{

    QColor clr;
public:
    ColorReview(QWidget* parent = nullptr);

    void setColor(const RGB& rgb);
    RGB getColor() const;
};
ColorReview::ColorReview(QWidget* parent): QWidget(parent){
    setAutoFillBackground(true);
}
void ColorReview::setColor(const RGB& rgb){
    clr = rgb.getQColor();
    QPalette pal(clr);
    setPalette(pal);
}
RGB ColorReview::getColor() const{
    return clr;
}
#define regular "123456"
#define light "203050"
#define lighter "304070"
#define dark "152535"
#define darker "051525"
#define grey "606080"
#define greyer "202050"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    qApp->setStyleSheet(QString(R"(
        /* === Общий фон === */
        QMainWindow {
            background: #%1;
        }
        /* === Общие настройки === */
        QWidget{
            font-family: "Arial";
            font-size: 15px;
            font-weight: bold;
        }

        /* === Панель === */
        Panel{
            background-color: #%5;
            border-radius: 16px;
            border: 5px solid #%4;
        }
        /* === Сладер === */
        QSlider {
            background: #%2;
            border-radius: 6px;
            padding: -1px 5px;
            border: 3px solid #%3;
        }
        QSlider:hover {
            background: #%4;
        }
        QSlider:pressed {
            background: #%1;
            color: white;
        }
        QSlider:handle {
            background: #%4;
            border: 3px solid #%6;
            border-radius: 6px;
        }
        QSlider:handle:hover{
            background: #%5;
        }

        /* === Поле с цифрами === */
        QDoubleSpinBox {
            background: #%2;
            border-radius: 6px;
            border: 3px solid #%3;
        }

        /* === Канал цвета (буква) === */
        QLabel#channelName{
            margin-left: 10px;
        }


        /* === Кнопки === */
        QPushButton{
            background: #%7;
        }

                        )")
                            .arg(regular)
                            .arg(dark)
                            .arg(darker)
                            .arg(light)
                            .arg(lighter)
                            .arg(grey)
                            .arg(greyer));

    setWindowTitle("Color Display");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *vBox = new QVBoxLayout(central);

    QHBoxLayout *hBox = new QHBoxLayout();
    vBox->addLayout(hBox);
    hBox->setSpacing(10);

    ColorModel *colorModels[3] = {new RGB(), new CMYK(), new HLS()};
    Panel *panels[3];

    ColorReview *colorReview = new ColorReview();
    vBox->addWidget(colorReview);
    colorReview->setMinimumHeight(50);

    for (int i = 0; i < 3; i++) {
        panels[i] = new Panel(colorModels[i]);
        hBox->addWidget(panels[i]);
        connect(panels[i], &Panel::colorChange, colorReview,
                &ColorReview::setColor);
    }
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (i==j){
                continue;
            }
            connect(panels[i], &Panel::colorChange, panels[j], &Panel::setColor);
        }
    }
    panels[0]->emitNewColor();

    QPushButton* palleteBtn = new QPushButton("Палитра");
    vBox->addWidget(palleteBtn);
    connect(palleteBtn, &QPushButton::clicked, [=](){
        QColor clr = QColorDialog::getColor(colorReview->getColor().getQColor());
        if (clr.isValid()){
            for (int i=0;i<3;i++){
                panels[i]->colorChange(RGB(clr.red(), clr.green(), clr.blue()));
            }
        }
    });





}

MainWindow::~MainWindow() {}
