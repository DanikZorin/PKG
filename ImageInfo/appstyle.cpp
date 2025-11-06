#include "appstyle.h"

void AppStyle::apply(QApplication &app) {
    app.setStyleSheet(styleSheet());
}

QString AppStyle::styleSheet() {
    return R"(
        /* === Общий фон === */
        QWidget {
            background-color: #123456;
            color: #DDDDDD;
            font-family: "Arial";
            font-size: 12pt;
        }

        /* === Кнопки === */
        QPushButton {
            background: #052535;
            border-radius: 8px;
            padding: 10px 16px;
            font-size: 13pt;
            font-weight: bold;
            color: #EEEEEE;
        }
        QPushButton:hover {
            background: #234567;
        }
        QPushButton:pressed {
            background: #041525;
            color: white;
        }


        /* === Таблица: заголовки === */
        QHeaderView::section {
            background: #052535;
            font-size: 12pt;
            font-weight: bold;
            padding: 4px;
            border-top: 1px solid #041525;
            border-bottom: 2px solid #041525;
            border-right: 1px solid #041525;
            border-left: none;
        }
        QHeaderView::section:first {
            border-left: 1px solid #041525;
        }

                /* === Таблица: строки === */
                QTableView {
                    gridline-color: #041525;
                    selection-background-color: #041525;
                    selection-color: white;
                    font-size: 12pt;
                }

                /* === Статусбар === */
                QStatusBar {
                    background: #041525;
                    font-size: 13pt;
                }

        /* === Всплывающее меню (File) === */
        QMenu {
            background-color: #052535;
            border: 2px solid #041525;
            padding: 6px;
            font-size: 15pt;
        }

        QMenu::item {
            background-color: transparent;
            padding: 8px 20px;
            color: #EEEEEE;
        }

        QMenu::item:selected {
            background-color: #041525;
            color: #EEEEEE;
            border-radius: 6px;
        }

        QMenu::separator {
            height: 1px;
            background: #041525;
            margin: 4px 10px;
        }

        /* === Всплывающие подсказки QToolTip === */
        QToolTip {
            background-color: #052535;
            color: #EEEEEE;
            border: 2px solid #041525;
            padding: 6px 10px;
            border-radius: 6px;
            font-family: "Arial";
            font-weight: bold;
            font-size: 12pt;
        }
          )";


}
