#include "widget.h"
#include "ui_widget.h"
#include <QIcon>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("NoMi音乐");
    this->setWindowIcon(QIcon(":/icon/cat2.png"));
    //this->setWindowOpacity(0.5);//半透明
    //去除标题栏
    this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    Init();

    //信号与槽关联
    connect(ui->listWidget, &QListWidget::itemClicked, this, &Widget::Music_Action);//单击歌单实现弹出功能选项卡

    connect(this->player,&QMediaPlayer::positionChanged, this, &Widget::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &Widget::updateDuration);
    connect(this->playLst,  &QMediaPlaylist::currentIndexChanged, this, &Widget::updateList);//播放歌曲变了
    connect(ui->horizontalSlider, &QAbstractSlider::sliderMoved, this, &Widget::Process);//设置进度
    connect(ui->vSlider, &QAbstractSlider::valueChanged,  this, &Widget::Voice);//设置音量

}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()
{
    this->musicLst = new QFileInfoList;
    this->pixmap = new QPixmap(":/backImg/12.jpg");
    this->current_music = -1;
    this->player = new QMediaPlayer(this);
    this->playLst = new QMediaPlaylist(this);
    player->setPlaylist(playLst);
    this->playLst->setPlaybackMode(QMediaPlaylist::Loop);

    //音量键隐藏
    ui->vSlider->hide();

    //搜索框控件调整
    this->searchAct = new QAction(this);
    searchAct->setIcon(QIcon(":/icon/cat2.png"));
    ui->lineEdit->addAction(searchAct, QLineEdit::TrailingPosition);
    connect(searchAct, SIGNAL(triggered(bool)), this, SLOT(onSearch(bool)));
    ui->lineEdit->setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");


    //左边栏设计

    this->like = new QListWidget(this);
    ui->tabWidget->addTab(like, QIcon(":/icon/love.png"), "喜欢");

    //控件透明
    ui->label->setStyleSheet("background-color:transparent");
    ui->exitButton->setStyleSheet("background-color:transparent");
    //ui->lineEdit->setStyleSheet("background-color:transparent");
    ui->themeButton->setStyleSheet("background-color:transparent");
    ui->controlButton->setStyleSheet("background-color:transparent");
    ui->roundButton->setStyleSheet("background-color:transparent");
    ui->priorButton->setStyleSheet("background-color:transparent");
    ui->playButton->setStyleSheet("background-color:transparent");
    ui->nextButton->setStyleSheet("background-color:transparent");
    ui->soundButton->setStyleSheet("background-color:transparent");
    ui->sizeButton->setStyleSheet("background-color:transparent");
    ui->pinglunButton->setStyleSheet("background-color:transparent");
    ui->deleButton->setStyleSheet("background-color:transparent");
    ui->listWidget->setStyleSheet("background-color:transparent");
    this->like->setStyleSheet("background-color:transparent");
    ui->lineEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color:transparent");
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),*this->pixmap);
}

void Widget::colorSet()
{
    QString pixpath = QFileDialog::getOpenFileName(this, "选择背景图片", "./");
    if(pixpath.isEmpty()){
        QMessageBox::information(this, "提示", "自定义背景图片失败！");
        return;
    }
    this->pixmap->load(pixpath);
    //this->paintEvent(NULL);
}

void Widget::onSearch(bool)
{
    qDebug()<<"searching";
}


//重写鼠标事件
void Widget::mousePressEvent(QMouseEvent *e)

{
    last = e->globalPos();
}
void Widget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


//窗口事件
void Widget::on_exitButton_clicked()
{
    this->close();
    this->deleteLater();
}

void Widget::on_deleButton_clicked()
{
    this->showMinimized();
}


//加载本地歌曲
void Widget::on_localButton_clicked(){

    //获取本地歌曲列表
    QDir dir = QFileDialog::getExistingDirectory(this, "选择文件目录", "./");
    QStringList filters;
    filters <<"*.mp3" << "*.wav";
    *this->musicLst= dir.entryInfoList(filters);
    if(this->musicLst->size() == 0){
        QMessageBox::information(this, "提示", "当前列表为空");
    }

    this->current_music = 0;

    //加载到Qliestwidget
    for(int i = 0; i < musicLst->size(); i ++){
        ui->listWidget->addItem("🎵 " + musicLst->at(i).fileName());
        this->playLst->addMedia(QUrl::fromLocalFile(musicLst->at(i).filePath()));
    }

}


//设置背景图片
void Widget::on_themeButton_clicked(){
    qDebug()<<"theme";
    QMenu *menu = new QMenu(this);
    QAction *act1 = new QAction(QIcon(":/icon/1106822.png"),"自定义背景",this);
    QAction *act2 = new QAction(QIcon(":/icon/1106822.png"),"版本详情", this);
    QAction *act3 = new QAction(QIcon(":/icon/1106822.png"),"CSDN支持", this);
    menu->addAction(act1);
    menu->addAction(act3);
    menu->addAction(act2);
    menu->show();
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    connect(act1, SIGNAL(triggered(bool)), this, SLOT(colorSet()));
    connect(act3, SIGNAL(triggered(bool)), this, SLOT(GO_Csdn()));
    connect(act2, SIGNAL(triggered(bool)), this, SLOT(About()));
}


//播放、暂停音乐
void Widget::Play_Music()
{
    qDebug()<<"播放";
    player->play();
}
void Widget::Play_Lst_Music()
{
    qDebug()<<"列表播放";
    this->current_music = ui->listWidget->currentRow();
    this->playLst->setCurrentIndex(this->current_music);
    player->play();
    ui->music_Name_label->setText(this->musicLst->at(this->playLst->currentIndex()).fileName());//设置显示当前歌曲
}
void Widget::Stop_Music()
{
    qDebug()<<"暂停歌曲";
    player->pause();
}

void Widget::Music_Action()
{
    qDebug()<<"弹出歌曲按钮";
    this->current_music = ui->listWidget->currentRow();
    QMenu *menu = new QMenu(this);
    QAction *act1 = new QAction(QIcon(":/icon/1106822.png"),"播放",this);
    QAction *act2 = new QAction(QIcon(":/icon/1106822.png"),"暂停", this);
    menu->addAction(act1);
    menu->addAction(act2);
    connect(act1, SIGNAL(triggered(bool)), this, SLOT(Play_Lst_Music()));
    connect(act2, SIGNAL(triggered(bool)), this, SLOT(Stop_Music()));
    menu->show();
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
}

void Widget::on_soundButton_clicked()
{
    qDebug()<<"改变音量";
    static int flag = 0;
    if(0 == flag){
        ui->vSlider->show();
        flag = 1;
    }
    else{
        ui->vSlider->hide();
        flag = 0;
    }

}

void Widget::on_roundButton_clicked()
{
    qDebug()<<"点击了循环方式按钮";
    QMenu *menu = new QMenu(this);
    QAction *onece = new QAction(QIcon(":/icon/cat2.png"),"单次播放", this);
    QAction *onecyc = new QAction(QIcon(":/icon/cat2.png"),"单曲循环", this);
    QAction *order = new QAction(QIcon(":/icon/cat2.png"),"顺序播放", this);
    QAction *cycle = new QAction(QIcon(":/icon/cat2.png"),"循环播放", this);
    QAction *rand = new QAction(QIcon(":/icon/cat2.png"),"随机播放", this);
    menu->addAction(onece);
    menu->addAction(onecyc);
    menu->addAction(order);
    menu->addAction(cycle);
    menu->addAction(rand);
    connect(onece, SIGNAL(triggered(bool)), this, SLOT(Play_onece()));
    connect(onecyc, SIGNAL(triggered(bool)), this, SLOT(Play_onecyc()));
    connect(order, SIGNAL(triggered(bool)), this, SLOT(Play_order()));
    connect(cycle, SIGNAL(triggered(bool)), this, SLOT(Play_cycle()));
    connect(rand, SIGNAL(triggered(bool)), this, SLOT(Play_rand()));
    menu->show();
    menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
}

//播放次序
void Widget::Play_onece()
{
    qDebug()<<"单次";
    this->playLst->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
}
void Widget::Play_onecyc()
{
    qDebug()<<"单曲循环";
    this->playLst->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}
void Widget::Play_order()
{
    qDebug()<<"顺序播放";
    this->playLst->setPlaybackMode(QMediaPlaylist::Sequential);
}
void Widget::Play_cycle()
{
    qDebug()<<"循环播放";
    this->playLst->setPlaybackMode(QMediaPlaylist::Loop);
}
void Widget::Play_rand()
{
    qDebug()<<"随机播放";
    this->playLst->setPlaybackMode(QMediaPlaylist::Random);
}

//播放
void Widget::on_playButton_clicked()
{
    qDebug()<<"点击了播放按钮";
    qDebug()<<"点击了播放";
    if(QMediaPlayer::StoppedState == this->player->state()){
        player->play();
        ui->music_Name_label->setText(this->musicLst->at(this->playLst->currentIndex()).fileName());//设置显示当前歌曲
    }
    else if(QMediaPlayer::PausedState == this->player->state()){
        player->play();
        ui->music_Name_label->setText(this->musicLst->at(this->playLst->currentIndex()).fileName());//设置显示当前歌曲
    }
    else{
        Stop_Music();
    }
}

//进度条滑动槽
void Widget::updatePosition(qint64 position)
{
    ui->horizontalSlider->setValue(position);
}
void Widget::updateDuration(qint64 duration)//更新播放歌曲的时间
{
    ui->horizontalSlider->setRange(0,duration);//根据播放时长来设置滑块的范围
    //ui->horizontalSlider->setEnabled(duration>0);
    ui->horizontalSlider->setPageStep(duration/10);//以及每一步的步数
}


void Widget::updateList(int value)
{
    if(value >= musicLst->size()){
        this->playLst->setCurrentIndex(0);
        value = 0;
    }
    if(this->playLst->currentIndex() == -1){
        this->playLst->setCurrentIndex(0);
    }
    ui->listWidget->setCurrentRow(value);
    ui->listWidget->item(value)->setSelected(true);
    this->current_music = value;
    ui->music_Name_label->setText(this->musicLst->at(this->playLst->currentIndex()).fileName());//设置显示当前歌曲
}

//音量、进度控制
void Widget::Process(int value)
{
    this->player->setPosition(value);
}
void Widget::Voice(int value)
{
    this->player->setVolume(value);
}

//下一曲
void Widget::on_nextButton_clicked()
{
    qDebug()<<"点击了下一曲";
    this->current_music++;
    if(this->current_music>= this->musicLst->size()){
        this->current_music = 0;
    }
    playLst->setCurrentIndex(this->current_music);
    Play_Music();
}

void Widget::on_priorButton_clicked()
{
    qDebug()<<"点击了上一曲";
    this->current_music--;
    if(this->current_music<0){
        this->current_music = this->musicLst->size()-1;
    }
    playLst->setCurrentIndex(this->current_music);
    Play_Music();
}

void Widget::About()
{
    QMessageBox::about(this, "关于NoMi播放器", "软件名：NoMi播放器\n版本号:2.1.0版本\n技术支持：Qt、CSDN个人博客\n版权所有:地球村科技有限公司\n版权所有者：NoMi(刘讯)\n NoMi音乐，感谢有你！");
}
void Widget::GO_Csdn()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://blog.csdn.net/qq_38853493")));
    return;
}
