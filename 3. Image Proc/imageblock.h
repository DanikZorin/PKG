#ifndef IMAGEBLOCK_H
#define IMAGEBLOCK_H

#include <QWidget>
#include <QtWidgets>
class ImageBlock : public QWidget
{
    Q_OBJECT

    QImage image;
public:
    ImageBlock(QWidget *parent = nullptr);

    void setImage(const QImage&);
    QImage getImage() const;
    void load(const QString& fileName);
    void reset();

    void paintEvent(QPaintEvent*);
};

#endif // IMAGEBLOCK_H
