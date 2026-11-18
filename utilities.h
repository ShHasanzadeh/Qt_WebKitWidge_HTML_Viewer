#ifndef UTILITIES_H
#define UTILITIES_H
#include <QString>
#include <QMap>
namespace utilities {
QString getDtcDocsReportLayout();
QString buildDtcDocsReportHtml(const QString &dtcCode,
                               const QString &dtcTitle,
                               const QString &description,
                               const QString &txtFilePath,
                               const QMap<QString, QString> &images);
}
#endif
