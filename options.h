#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

constexpr auto DEFAULT_OSLOADER_PAGE = 1408;
constexpr auto DEFAULT_SYSTEM_PAGE = 1984;

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

    void setPage(const int &page_OSLoader, const int &page_System);

signals:
    void returnPage(int page_OSLoader, int page_System);

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::Options *ui;
};

#endif // OPTIONS_H
