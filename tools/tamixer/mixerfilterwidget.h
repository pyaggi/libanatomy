#ifndef MIXERFILTERWIDGET_H
#define MIXERFILTERWIDGET_H

#include <QWidget>

class MixerFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MixerFilterWidget(QWidget *parent = nullptr);

    QString idFilter() const;
    QString contentFilter() const;
    bool unmatchedFilter() const;


signals:
    void filterUpdated();
    void expand();
    void collapse();
private:
    class QLineEdit *m_id,*m_text;
    class QPushButton *m_unmatched;
};

#endif // MIXERFILTERWIDGET_H
