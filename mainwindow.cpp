#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QStringList>
#include <QFileInfoList>
#include <QWebSettings>
#include <QtWebKitWidgets/QWebView>

// ============================================================
// Constructor
// ============================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      webView(new QWebView(this)),
      showButton(new QPushButton(
          QStringLiteral("نمایش گزارش / Show Report"), this))
{
    QWidget *central = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);

    showButton->setMinimumHeight(48);

    layout->addWidget(showButton);
    layout->addWidget(webView);

    setCentralWidget(central);

    connect(showButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadReport);

    showMaximized();
}

// ============================================================
// Destructor
// ============================================================

MainWindow::~MainWindow()
{
}

// ============================================================
// Load report
// ============================================================

void MainWindow::loadReport()
{
    const QString txtFilePath =
        QStringLiteral("/home/root/sample9.txt");

    const QString imagesDirectory =
        QStringLiteral("/home/root/DtcDocs/EBUS_SHETAB/1811/");

    // ========================================================
    // Read text file
    // ========================================================

    QFile txtFile(txtFilePath);

    if (!txtFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open text file:" << txtFilePath;

        webView->setHtml(
            QStringLiteral(
                "<html lang='fa' dir='rtl'>"
                "<head><meta charset='UTF-8'></head>"
                "<body style='font-family:Tahoma; padding:30px;'>"
                "<h3>خطا در باز کردن فایل متنی</h3>"
                "<p>%1</p>"
                "</body></html>"
            ).arg(txtFilePath.toHtmlEscaped())
        );

        return;
    }

    const QString text = QString::fromUtf8(txtFile.readAll());
    txtFile.close();

    // ========================================================
    // Find image files
    // ========================================================

    QDir dir(imagesDirectory);

    const QStringList filters = {
        "*.jpg",  "*.JPG",
        "*.jpeg", "*.JPEG",
        "*.png",  "*.PNG",
        "*.bmp",  "*.BMP",
        "*.gif",  "*.GIF",
        "*.svg",  "*.SVG"
    };

    const QFileInfoList imageFiles =
        dir.entryInfoList(filters, QDir::Files, QDir::Name);

    // ========================================================
    // Build image HTML
    // ========================================================

    QString imagesHtml;

    for (const QFileInfo &imageInfo : imageFiles)
    {
        const QString imageName =
            imageInfo.completeBaseName();

        const QString imagePath =
            QUrl::fromLocalFile(
                imageInfo.absoluteFilePath()
            ).toString();

        imagesHtml += QString(
            R"HTML(
                <div class="image-card">

                    <div class="image-title">
                        <div class="image-zoom-controls">
                            <button type="button" class="zoom-button zoom-minus"
                                    onclick="changeImageZoom(this, -25)">−</button>
                            <button type="button" class="zoom-button zoom-plus"
                                    onclick="changeImageZoom(this, 25)">+</button>
                        </div>
                        %1
                    </div>

                    <div class="image-container">
                        <img
                            src="%2"
                            alt="%1">
                    </div>

                </div>
            )HTML"
        ).arg(
            imageName.toHtmlEscaped(),
            imagePath.toHtmlEscaped()
        );
    }

    if (imageFiles.isEmpty())
    {
        imagesHtml =
            QStringLiteral(
                "<div class='no-images'>"
                "هیچ تصویری در مسیر مشخص‌شده پیدا نشد."
                "</div>"
            );

        qWarning() << "No image files found in:" << imagesDirectory;
    }

    // ========================================================
    // Prepare text
    // ========================================================

    QString safeText = text.toHtmlEscaped();

    safeText.replace(QStringLiteral("\r\n"),
                     QStringLiteral("\n"));

    safeText.replace(QStringLiteral("\r"),
                     QStringLiteral("\n"));

    safeText.replace(QStringLiteral("\n"),
                     QStringLiteral("<br>"));

    // ========================================================
    // HTML and CSS
    // ========================================================

    const QString html = QString(
        R"HTML(
<!DOCTYPE html>
<html lang="fa" dir="rtl">

<head>
    <meta charset="UTF-8">

    <meta name="viewport"
          content="width=device-width, initial-scale=1.0">

    <title>گزارش عیب ‌یابی</title>

    <style>

        /* ====================================================
           GLOBAL SETTINGS
           ==================================================== */

        * {
            box-sizing: border-box;
        }

        html {
            direction: rtl;
            width: 100%;
            margin: 0;
            padding: 0;
        }

        body {
            width: 100%;
            margin: 0;
            padding: 0;

            background: #f3f5f7;
            color: #263238;

            font-family: "Segoe UI", Tahoma, Arial, sans-serif;
            font-size: 16px;
            line-height: 1.8;
        }

        /* ====================================================
           MAIN PAGE
           ==================================================== */

        .page {
            display: block;

            width: 100%;
            max-width: none;

            margin: 0 auto;
            padding: 15px 5px 30px;

            box-sizing: border-box;
        }

        /* ====================================================
           HEADER
           ==================================================== */

        .header {
            width: 100%;
            margin: 0 0 22px;
            padding: 0;

            background: #ffffff;

            border: 1px solid #dfe3e6;
            border-radius: 12px;

            overflow: hidden;

            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
        }

        .header-top {
            padding: 24px 28px;

            background: #263238;
            color: #ffffff;

            font-size: 23px;
            font-weight: 600;

            text-align: right;
        }

        .header-description {
            padding: 20px 28px;
            background: #ffffff;
        }

        .header-description p {
            margin: 0;

            color: #546e7a;
            font-size: 16px;

            line-height: 1.8;
        }

        /* ====================================================
           REPORT SECTIONS
           ==================================================== */

        .section {
            display: block;

            width: 100%;
            max-width: none;

            margin: 0 0 22px;
            padding: 0;

            background: #ffffff;

            border: 1px solid #dfe3e6;
            border-radius: 12px;

            overflow: hidden;

            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.04);
        }

        .section-title {
            padding: 14px 22px;

            background: #f8fafb;

            border-bottom: 1px solid #e3e7ea;

            color: #263238;
            font-size: 18px;
            font-weight: 700;

            text-align: right;
        }

        .section-title::before {
            content: "";

            display: inline-block;

            width: 4px;
            height: 20px;

            margin-left: 10px;

            background: #1976d2;

            vertical-align: middle;

            border-radius: 2px;
        }

        .section-content {
            display: block;

            width: 100%;
            max-width: none;

            margin: 0;
            padding: 15px;

            box-sizing: border-box;
        }

        /* ====================================================
           TEXT CONTAINER
           ==================================================== */

        .action-box {
            display: block;

            width: 100%;
            max-width: none;

            margin: 0;
            padding: 20px 80px;

            background: #f7fbff;

            border-right: 4px solid #1976d2;
            border-radius: 6px;

            font-size: 16px;
            line-height: 2.2;

            overflow: visible;

            box-sizing: border-box;
        }

        .report-text {
            display: block;

            width: 100%;
            max-width: 100%;

            margin: 0;
            padding: 0;

            direction: rtl;
            text-align: right;

            font-family: "Segoe UI", Tahoma, Arial, sans-serif;
            font-size: 16px;
            line-height: 2.2;

            white-space: pre-wrap;

            overflow-wrap: break-word;
            word-wrap: break-word;

            border: none;

            box-sizing: border-box;
        }

        .report-text br {
            display: block;
            content: "";
        }

        /* ====================================================
           IMAGE SECTION
           ==================================================== */

        .images-grid {
            display: block;

            width: 100%;
            max-width: none;

            margin: 0;
            padding: 10px;

            text-align: center;

            box-sizing: border-box;
        }

        /* ====================================================
           IMAGE CARDS
           ==================================================== */

        .image-card {
            display: block;

            width: 100%;
            max-width: none;

            margin: 0 auto 30px;
            padding: 8px;

            background: #ffffff;

            border: 2px solid #d6dfe7;
            border-radius: 10px;

            text-align: center;

            box-shadow: 0 3px 12px rgba(0, 0, 0, 0.08);

            box-sizing: border-box;

            page-break-inside: avoid;
        }

        /* ====================================================
           IMAGE TITLE
           ==================================================== */

        .image-title {
            display: block;
            position: relative;

            width: 100%;

            margin: 0 0 8px;
            padding: 14px 55px;

            color: #263238;

            background: #edf4fa;

            border: 1px solid #d6e2ed;
            border-radius: 6px;

            font-family: "Segoe UI", Tahoma, Arial, sans-serif;
            font-size: 20px;
            font-weight: 700;

            text-align: center;

            overflow-wrap: break-word;
            word-wrap: break-word;

            box-sizing: border-box;
        }

        .image-zoom-controls {
            position: absolute;
            left: 8px;
            right: 8px;
            top: 50%;
            transform: translateY(-50%);
            display: flex;
            align-items: center;
            justify-content: space-between;
            direction: ltr;
            pointer-events: none;
        }

        .image-zoom-controls .zoom-button {
            pointer-events: auto;
        }

        .zoom-button {
            min-width: 32px;
            min-height: 28px;
            margin: 0 2px;
            padding: 3px 8px;
            font-size: 20px;
            font-weight: 700;
            line-height: 1;
            color: #263238;
            background: #edf4fa;
            border: 1px solid #d6e2ed;
            border-radius: 6px;
            cursor: pointer;
        }

        .zoom-button:disabled {
            opacity: 0.45;
            cursor: default;
        }

        /* ====================================================
           IMAGE CONTAINER
           Normal mode: image fits the available width.
           Zoom mode: independent horizontal and vertical
           scrollbars are available.
           ==================================================== */

        .image-container {
            display: block;

            position: relative;

            width: 100%;
            max-width: none;

            height: auto;
            min-height: 0;
            max-height: none;

            margin: 0;
            padding: 0;

            background: #ffffff;

            border: 1px solid #e0e5e9;
            border-radius: 6px;

            overflow: hidden;

            text-align: left;

            box-sizing: border-box;

            -webkit-overflow-scrolling: touch;
        }

        /* ====================================================
           ZOOM MODE CONTAINER
           Both scrollbars are independent and accessible.
           ==================================================== */

        .image-container.zoomed-container {
            height: 70vh;
            min-height: 250px;
            max-height: 700px;

            overflow-x: scroll;
            overflow-y: scroll;

            scrollbar-width: auto;

            -webkit-overflow-scrolling: touch;
        }




        /* ====================================================
           VERTICAL AND HORIZONTAL SCROLLBARS
           ==================================================== */

        .image-container.zoomed-container::-webkit-scrollbar {
            width: 17px;
            height:17px;
        }

        .image-container.zoomed-container::-webkit-scrollbar-track {
            background: #e3eaf0;
            border-radius: 6px;
        }

        .image-container.zoomed-container::-webkit-scrollbar-thumb {
            background: #78909c;
            border: 2px solid #e3eaf0;
            border-radius: 6px;
        }

        /* ====================================================
           NORMAL IMAGE
           ==================================================== */

        .image-container img {
            display: block;

            width: 100%;
            max-width: 100%;

            height: auto;
            max-height: none;

            margin: 0;
            padding: 0;

            object-fit: contain;
            object-position: center center;

            border: none;
            border-radius: 4px;

            cursor: default;

            box-sizing: border-box;
        }

        /* ====================================================
           ZOOMED IMAGE
           Width is controlled by the + and - buttons.
           ==================================================== */

        .image-container img.zoomed {
            width: 200%;
            max-width: none;

            height: auto;
            max-height: none;

            margin: 0;

            cursor: default;
        }

        /* ====================================================
           NO IMAGES MESSAGE
           ==================================================== */

        .no-images {
            width: 100%;
            padding: 25px;

            color: #607d8b;
            background: #f8fafb;

            border: 1px solid #dfe3e6;
            border-radius: 8px;

            font-size: 17px;
            text-align: center;
        }

        /* ====================================================
           FOOTER
           ==================================================== */

        .footer {
            width: 100%;

            padding: 15px 0 0;
            margin: 0;

            color: #90a4ae;
            font-size: 12px;

            text-align: center;
        }

        /* ====================================================
           RESPONSIVE DESIGN
           ==================================================== */

        @media (max-width: 700px) {

            .page {
                width: 100%;
                max-width: none;

                margin: 0 auto;
                padding: 8px 3px 20px;
            }

            .header-top {
                padding: 18px 15px;
                font-size: 20px;
            }

            .header-description {
                padding: 15px;
            }

            .section-content {
                padding: 8px;
            }

            .section-title {
                padding: 12px 15px;
                font-size: 17px;
            }

            .action-box {
                padding: 15px;
            }

            .report-text {
                padding-right: 0;
                padding-left: 0;

                font-size: 17px;
                line-height: 2.1;
            }

            .images-grid {
                display: block;

                width: 100%;
                max-width: none;

                margin: 0;
                padding: 4px;
            }

            .image-card {
                width: 100%;
                max-width: none;

                margin: 0 auto 20px;
                padding: 5px;

                border-radius: 8px;
            }

            .image-title {
                margin-bottom: 6px;
                padding: 10px 45px;

                font-size: 17px;
            }

            .image-container {
                width: 100%;

                height: auto;
                min-height: 0;
                max-height: none;

                padding: 0;
                margin: 0;

                overflow: hidden;
            }

            .image-container.zoomed-container {
                height: 60vh;
                min-height: 220px;
                max-height: 500px;

                overflow-x: scroll;
                overflow-y: scroll;
            }

            .image-container img {
                width: 100%;
                max-width: 100%;

                height: auto;
                max-height: none;
            }

            .image-container img.zoomed {
                width: 200%;
                max-width: none;
            }
        }

        /* ====================================================
           LARGE SCREENS
           ==================================================== */

        @media (min-width: 701px) {

            .page {
                width: 100%;
                max-width: none;

                padding-right: 5px;
                padding-left: 5px;
            }

            .images-grid,
            .image-card,
            .image-container {
                width: 100%;
                max-width: none;
            }

            .image-container img {
                width: 100%;
                max-width: 100%;

                height: auto;
                max-height: none;
            }

            .image-container img.zoomed {
                width: 200%;
                max-width: none;
            }
        }

    </style>

    <script>
        // Zoom each image independently using the + and - buttons.
        // Zoom is limited to 100% through 250% in 25% increments.
        function changeImageZoom(button, change)
        {
            var controls = button.parentNode;
            var title = controls.parentNode;
            var card = title.parentNode;
            var container = card.querySelector(".image-container");
            var img = container.querySelector("img");

            var zoom = parseInt(img.getAttribute("data-zoom") || "100", 10);
            zoom = Math.max(100, Math.min(250, zoom + change));
            img.setAttribute("data-zoom", zoom.toString());

            if (zoom > 100)
            {
                img.classList.add("zoomed");
                container.classList.add("zoomed-container");
                img.style.width = zoom + "%";
                img.style.maxWidth = "none";
            }
            else
            {
                img.classList.remove("zoomed");
                container.classList.remove("zoomed-container");
                img.style.width = "";
                img.style.maxWidth = "";
                container.scrollLeft = 0;
                container.scrollTop = 0;
            }

            var buttons = controls.querySelectorAll(".zoom-button");
            buttons[0].disabled = (zoom <= 100);
            buttons[1].disabled = (zoom >= 250);
        }
    </script>

</head>

<body>

    <div class="page">

        <!-- Header -->

        <div class="header">

            <div class="header-top">
                گزارش عیب‌یابی
            </div>

            <div class="header-description">
                <p>
                    دستورالعمل‌ها و اطلاعات فنی مربوط به عیب‌یابی
                </p>
            </div>

        </div>

        <!-- Report text -->

        <div class="section">

            <div class="section-title">
                اقدام اجرایی
            </div>

            <div class="section-content">

                <div class="action-box">
                    <div class="report-text">%1</div>
                </div>

            </div>

        </div>

        <!-- Images -->

        <div class="section">

            <div class="section-title">
                نقشه‌ها و مستندات فنی
            </div>

            <div class="section-content">

                <div class="images-grid">
                    %2
                </div>

            </div>

        </div>

    </div>

</body>
</html>
)HTML"
    ).arg(safeText, imagesHtml);

    // ========================================================
    // Display HTML in QWebView
    // ========================================================

    webView->settings()->setAttribute(
        QWebSettings::LocalContentCanAccessFileUrls,
        true
    );

    webView->setHtml(
        html,
        QUrl::fromLocalFile(imagesDirectory + "/")
    );

}
