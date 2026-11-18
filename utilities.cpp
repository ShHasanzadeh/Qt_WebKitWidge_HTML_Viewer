#include "utilities.h"
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QStringList>
#include <QDebug>

namespace utilities {
QString getDtcDocsReportLayout()
{
    return QStringLiteral(R"HTML(<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>{{DTC_CODE}}</title>
<style>
* { box-sizing:border-box; }
html,body { width:100%; min-height:100%; }
body { margin:0; padding:0; background:#f3f5f7; color:#263238;
       font-family:"Segoe UI",Tahoma,Arial,sans-serif; line-height:1.8; overflow-x:hidden; }
pre { font-family:inherit; white-space:pre-wrap; overflow-wrap:anywhere;
      word-wrap:break-word; line-height:inherit; margin:0; }
.page { width:100%; max-width:1250px; margin:20px auto; padding:0 16px 35px; }
.header { background:#fff; border-radius:12px; border:1px solid #dfe3e6;
          overflow:hidden; box-shadow:0 2px 8px rgba(0,0,0,.05); margin-bottom:22px; }
.header-top { padding:24px 28px; background:#263238; color:#fff; }
.dtc-code { display:inline-block; direction:ltr; font-family:Consolas,monospace;
            font-size:24px; font-weight:bold; background:#fff; color:#263238;
            padding:5px 14px; border-radius:6px; margin-left:12px; }
.dtc-title { font-size:23px; font-weight:600; }
.header-description { padding:20px 28px; background:#fff; }
.header-description p { margin:0; color:#546e7a; font-size:16px; }
.section { background:#fff; border:1px solid #dfe3e6; border-radius:12px;
           margin-bottom:22px; overflow:hidden; box-shadow:0 2px 8px rgba(0,0,0,.04); }
.section-title { padding:14px 22px; background:#f8fafb; border-bottom:1px solid #e3e7ea;
                 font-size:18px; font-weight:700; color:#263238; }
.section-title:before { content:""; display:inline-block; width:4px; height:20px;
                        background:#1976d2; vertical-align:middle; margin-left:10px; border-radius:2px; }
.section-content { padding:24px; }
.action-box { background:#f7fbff; border-right:4px solid #1976d2; padding:20px 22px;
              border-radius:6px; font-size:16px; direction:rtl; text-align:right; overflow-wrap:anywhere; }
.action-box pre { font-size:16px; line-height:2; direction:rtl; text-align:right;
                  white-space:pre-wrap; overflow-wrap:anywhere; word-wrap:break-word; }
.images-grid { display:grid; grid-template-columns:repeat(auto-fit,minmax(300px,1fr)); gap:20px; }
.image-card { min-width:0; border:1px solid #dfe3e6; border-radius:10px; overflow:hidden; background:#fff; }
.image-title { padding:12px 16px; background:#f8fafb; border-bottom:1px solid #e3e7ea;
               font-weight:600; color:#455a64; overflow-wrap:anywhere; }
.image-container { height:350px; padding:12px; background:#fafafa; text-align:center; }
.image-container img { width:100%; height:100%; object-fit:contain; display:block; }
.empty-images { color:#78909c; padding:20px; text-align:center; }
.footer { text-align:center; color:#90a4ae; font-size:12px; padding-top:15px; }
@media(max-width:700px) {
 .page { margin:12px auto; padding:0 8px 25px; }
 .header-top { padding:20px; }
 .dtc-code { font-size:20px; display:block; width:fit-content; margin-bottom:10px; }
 .dtc-title { font-size:18px; }
 .section-content { padding:14px; }
 .images-grid { grid-template-columns:1fr; }
 .image-container { height:280px; }
}
</style>
</head>
<body>
<div class="page">
 <div class="header">
  <div class="header-top"><span class="dtc-code">{{DTC_CODE}}</span><span class="dtc-title">{{DTC_TITLE}}</span></div>
  <div class="header-description"><p>{{DTC_DESCRIPTION}}</p></div>
 </div>
 <div class="section">
  <div class="section-title">اقدام اجرایی</div>
  <div class="section-content"><div class="action-box"><pre>{{ACTION_TEXT}}</pre></div></div>
 </div>
 <div class="section">
  <div class="section-title">نقشه‌ها و مستندات فنی</div>
  <div class="section-content"><div class="images-grid">{{IMAGES_HTML}}</div></div>
 </div>
 <div class="footer">DTC Technical Report</div>
</div>
</body>
</html>)HTML");
}

QString buildDtcDocsReportHtml(const QString &dtcCode,
                               const QString &dtcTitle,
                               const QString &description,
                               const QString &txtFilePath,
                               const QMap<QString, QString> &images)
{
    QFile file(txtFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open TXT file:" << txtFilePath << file.errorString();
        return QString();
    }

    QTextStream in(&file);
    in.setCodec("UTF-8"); // Qt 5
    QString actionText = in.readAll();
    file.close();

    actionText.replace("\r\n", "\n");
    actionText.replace('\r', '\n');
    if (!actionText.isEmpty() && actionText.at(0) == QChar(0xFEFF))
        actionText.remove(0, 1);
    actionText = actionText.toHtmlEscaped();

    QString imagesHtml;
    for (auto it = images.constBegin(); it != images.constEnd(); ++it) {
        const QString key = it.key();
        const QStringList parts = key.split('_');
        const QString title = parts.size() > 1 ? parts.at(1) : key;
        QString imageUrl = it.value();

        if (!imageUrl.startsWith("data:", Qt::CaseInsensitive) &&
            !imageUrl.startsWith("http://", Qt::CaseInsensitive) &&
            !imageUrl.startsWith("https://", Qt::CaseInsensitive) &&
            !imageUrl.startsWith("file://", Qt::CaseInsensitive)) {
            imageUrl = QUrl::fromLocalFile(imageUrl).toString();
        }

        imagesHtml += QStringLiteral(
            "<div class=\"image-card\"><div class=\"image-title\">%0</div>"
            "<div class=\"image-container\"><img src=\"%1\" alt=\"%0\"></div></div>")
            .arg(title.toHtmlEscaped(), imageUrl.toHtmlEscaped());
    }

    if (imagesHtml.isEmpty())
        imagesHtml = QStringLiteral("<div class=\"empty-images\">تصویری برای نمایش وجود ندارد.</div>");

    QString html = getDtcDocsReportLayout();
    html.replace("{{DTC_CODE}}", dtcCode.toHtmlEscaped());
    html.replace("{{DTC_TITLE}}", dtcTitle.toHtmlEscaped());
    html.replace("{{DTC_DESCRIPTION}}", description.toHtmlEscaped());
    html.replace("{{ACTION_TEXT}}", actionText);
    html.replace("{{IMAGES_HTML}}", imagesHtml);
    return html;
}
} // namespace utilities
