#ifndef RECORDVIEW_H
#define RECORDVIEW_H

#include <QDialog>

namespace Ui {
class RecordView;
}

class RecordView : public QDialog
{
    Q_OBJECT

public:
    explicit RecordView(int type,QWidget *parent = 0);
    ~RecordView();

private:
    Ui::RecordView *ui;
};

#endif // RECORDVIEW_H
