#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>
#include <QListWidget>
#include <QPalette>
#include <QPaintEvent>
#include <QPainter>
#include <QFont>
#include <QAction>
#include <QDebug>
#include <QMouseEvent>
#include <QDir>
#include <QFileDialog>
#include <QStringList>
#include <QFileInfoList>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPixmap>
#include <QSlider>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDesktopServices>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void Init();


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent* e);

private slots:
    void paintEvent(QPaintEvent *);
    void onSearch(bool);
    void colorSet();
    void About();
    void GO_Csdn();

    void Play_Music();
    void Play_Lst_Music();
    void Stop_Music();
    void Music_Action();

    //播放次序
    void Play_onece();
    void Play_onecyc();
    void Play_order();
    void Play_cycle();
    void Play_rand();

    //进度条设置
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);

    //歌曲列表动态
    void updateList(int value);

    //进度、音量控制
    void Voice(int value);
    void Process(int value);

    void on_exitButton_clicked();
    void on_deleButton_clicked();
    void on_localButton_clicked();
    void on_themeButton_clicked();
    void on_soundButton_clicked();

    void on_roundButton_clicked();

    void on_playButton_clicked();

    void on_nextButton_clicked();

    void on_priorButton_clicked();

private:
    Ui::Widget *ui;
    QAction *searchAct;
    QPoint last;
    QPixmap *pixmap;
    QFileInfoList *musicLst;
    int current_music;
    QMediaPlayer *player;
    QMediaPlaylist *playLst;
    QListWidget *like;

};

#endif // WIDGET_H
