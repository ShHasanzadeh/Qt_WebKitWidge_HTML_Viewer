# Qt_WebKitWidge_HTML_Viewer
A Qt 5.15.2-based diagnostic report viewer using Qt WebKit (QWebView) to display DTC fault information, troubleshooting instructions, and related images with touch-enabled navigation, scrolling, and pinch-to-zoom functionality.

A Qt-based diagnostic report viewer designed to display automotive Diagnostic Trouble Code (DTC) information through an interactive HTML interface powered by **Qt WebKit (QWebView)**.

The application provides a structured and user-friendly presentation of fault information, troubleshooting instructions, and associated diagnostic images, with support for touch-based navigation and page zooming.

## Features

- **Qt WebKit Integration:** HTML rendering using `QWebView`.
- **DTC Information Display:** Structured presentation of diagnostic trouble codes.
- **HTML-Based Reports:** Dynamically generated diagnostic report layouts.
- **Persian RTL Support:** Right-to-left text rendering for Persian content.
- **Image Visualization:** Display of diagnostic images associated with fault reports.
- **Touch Navigation:** Touch-based page scrolling.
- **Pinch-to-Zoom:** Two-finger zooming for improved readability.
- **Custom Scrollbars:** Styled horizontal and vertical scrollbars.
- **Responsive Layout:** Organized report presentation for embedded displays and tablet interfaces.

## Technology Stack

| Component | Technology |
|---|---|
| Programming Language | C++ |
| Framework | Qt 5.15.2 |
| Web Rendering Engine | Qt WebKit |
| Web View | QWebView |
| UI Design | Qt Widgets |
| Report Rendering | HTML / CSS / JavaScript |
| Build System | qmake |

## Project Structure

```text
Qt-WebKit-DTC-Report-Viewer/
│
├── frmFaultDetails.h
├── frmFaultDetails.cpp
├── frmFaultDetails.ui
│
├── utilities.h
├── utilities.cpp
│
├── resources/
│   └── images/
│
├── README.md
└── *.pro
```

*Note: The project structure may vary depending on the integration into the main application.*

## Requirements

- Qt 5.15.2
- C++ compiler compatible with Qt 5.15.2
- Qt WebKit module
- Qt Widgets module
- qmake build system

## Qt Modules

The following modules are required in the `.pro` file:

```qmake
QT += core gui widgets webkitwidgets
```

## Build Instructions

1. Clone the repository:

   ```bash
   git clone <repository-url>
   ```

2. Navigate to the project directory:

   ```bash
   cd Qt-WebKit-DTC-Report-Viewer
   ```

3. Open the project using Qt Creator or generate a Makefile:

   ```bash
   qmake
   make
   ```

4. Run the application after a successful build.

## Core Components

### frmFaultDetails
Main dialog responsible for displaying diagnostic reports through the Qt WebKit interface.

### utilities
Provides utility functions, including dynamic HTML report generation and diagnostic report layout management.

### QWebView
Qt WebKit-based widget responsible for rendering HTML, CSS, and JavaScript content within the Qt application.

## Application Purpose

This project provides an embedded HTML-based interface for automotive diagnostic information, enabling users and technicians to review DTC fault details, troubleshooting procedures, and supporting images through an interactive touch-friendly interface.

## License

Specify the appropriate license before distributing or publishing this project.

---

**Developed with C++, Qt 5.15.2, and Qt WebKit.**
