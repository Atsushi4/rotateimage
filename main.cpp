#include <QWidget>

class ImageWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    QImage image() const;
    double angle() const;

public slots:
    void setImage(QImage image);
    void setAngle(double angle);

signals:
    void imageChanged(QImage image);
    void angleChanged(double angle);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QImage m_image;
    double m_angle = 0;
};

#include <QApplication>
#include <QPropertyAnimation>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageWidget w;
    QImage image;
    image.load(":/images/rotate.png");
    w.setImage(image);
    QPropertyAnimation anim(&w, "angle");
    anim.setLoopCount(-1);
    anim.setStartValue(0);
    anim.setEndValue(-360);
    anim.setDuration(4000);
    anim.start();
    w.show();
    return app.exec();
}

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{

}

QImage ImageWidget::image() const
{
    return m_image;
}

double ImageWidget::angle() const
{
    return m_angle;
}

void ImageWidget::setImage(QImage image)
{
    if (m_image == image)
        return;

    m_image = image;
    emit imageChanged(m_image);
    update();
}

void ImageWidget::setAngle(double angle)
{
    if (m_angle == angle)
        return;

    m_angle = angle;
    emit angleChanged(m_angle);
    update();
}

#include <QPainter>
void ImageWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if (m_image.isNull()) return;
    QPainter p(this);
    const auto &rect = QRectF(m_image.rect());
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (width() != rect.width() || height() != rect.height()) {
        p.scale(width() / rect.width(), height() / rect.height());
    }
    if (m_angle != 0) {
        const auto &center = rect.center();
        p.translate(center);
        p.rotate(m_angle);
        p.translate(-center);

        p.setFont(QFont("meiryo", 16));
        // refs https://stackoverrun.com/ja/q/12725550
        QPainterPath path;
        path.addText(center, p.font(), "______________________");
        path.addText(center.x(), center.y() + 20, p.font(), QString::number(m_angle));
        p.fillPath(path, p.pen().color());
//        p.drawText(center, "______________________");
//        p.drawText(center.x(), center.y() + 20, QString::number(m_angle));
    }
    p.drawImage(rect, m_image);
}

#include "main.moc"
