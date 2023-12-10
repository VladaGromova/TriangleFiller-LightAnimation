#pragma once
#include <QQuickImageProvider>
class ImageSource : public QQuickImageProvider {
  Q_OBJECT
 public:
  QSharedPointer<QImage> image;
  explicit ImageSource(QSharedPointer<QImage> image = nullptr);
  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize) override;
};