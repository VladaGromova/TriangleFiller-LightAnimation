#include "ImageSource.h"

ImageSource::ImageSource(QSharedPointer<QImage> image)
    : QQuickImageProvider(QQuickImageProvider::Image), image(image) {
}

QImage ImageSource::requestImage(const QString& id, QSize* size,
                                   const QSize& requestedSize) {
  return *image.get();
}
