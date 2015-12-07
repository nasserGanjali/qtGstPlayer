#include "playergui.h"
#include "ui_mainwindow.h"
#include "player.h"
#include <QBoxLayout>
#include <QFileDialog>
#include <QToolButton>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>

playerGui::playerGui(QWidget *parent)
    : QWidget(parent)
{
    //create the player
    m_player = new Player(this);
    connect(m_player, SIGNAL(positionChanged()), this, SLOT(onPositionChanged()));
    connect(m_player, SIGNAL(stateChanged()), this, SLOT(onStateChanged()));

    //m_baseDir is used to remember the last directory that was used.
    //defaults to the current working directory
    m_baseDir = QLatin1String(".");

    //this timer (re-)hides the controls after a few seconds when we are in fullscreen mode
    m_fullScreenTimer.setSingleShot(true);
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(hideControls()));

    //create the UI
    QVBoxLayout *appLayout = new QVBoxLayout;
    appLayout->setContentsMargins(0, 0, 0, 0);
    createUI(appLayout);
    setLayout(appLayout);

    onStateChanged(); //set the controls to their default state

    setWindowTitle(tr("QtGStreamer example player"));
    resize(400, 400);

    // m_player->setUri("");
}

playerGui::~playerGui()
{
    delete m_player;
}

void playerGui::openFile(const QString & fileName)
{
    m_baseDir = QFileInfo(fileName).path();
    m_player->stop();
    m_player->setUri(fileName);
    m_player->play();
}

void playerGui::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a Movie"), m_baseDir);

    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void playerGui::camera()
{
    const QString fileName = "http://admin:abc123@192.168.1.119:/video.cgi";
    m_baseDir = QFileInfo(fileName).path();
    m_player->stop();
    m_player->setUri(fileName);
    m_player->play();

}

void playerGui::toggleFullScreen()
{
    if (isFullScreen()) {
        setMouseTracking(false);
        m_player->setMouseTracking(false);
        m_fullScreenTimer.stop();
        showControls();
        showNormal();
    } else {
        setMouseTracking(true);
        m_player->setMouseTracking(true);
        hideControls();
        showFullScreen();
    }
}

void playerGui::onStateChanged()
{
    QGst::State newState = m_player->state();
    m_playButton->setEnabled(newState != QGst::StatePlaying);
    m_pauseButton->setEnabled(newState == QGst::StatePlaying);
    m_stopButton->setEnabled(newState != QGst::StateNull);
    m_positionSlider->setEnabled(newState != QGst::StateNull);
    m_volumeSlider->setEnabled(newState != QGst::StateNull);
    m_volumeLabel->setEnabled(newState != QGst::StateNull);
    m_volumeSlider->setValue(m_player->volume());

    //if we are in Null state, call onPositionChanged() to restore
    //the position of the slider and the text on the label
    if (newState == QGst::StateNull) {
        onPositionChanged();
    }
}

/* Called when the positionChanged() is received from the player */
void playerGui::onPositionChanged()
{
    QTime length(0,0);
    QTime curpos(0,0);

    if (m_player->state() != QGst::StateReady &&
            m_player->state() != QGst::StateNull)
    {
        length = m_player->length();
        curpos = m_player->position();
    }

    m_positionLabel->setText(curpos.toString("hh:mm:ss.zzz")
                             + "/" +
                             length.toString("hh:mm:ss.zzz"));

    if (length != QTime(0,0)) {
        m_positionSlider->setValue(curpos.msecsTo(QTime(0,0)) * 1000 / length.msecsTo(QTime(0,0)));
    } else {
        m_positionSlider->setValue(0);
    }

    if (curpos != QTime(0,0)) {
        m_positionLabel->setEnabled(true);
        m_positionSlider->setEnabled(true);
    }
}

/* Called when the user changes the slider's position */
void playerGui::setPosition(int value)
{
    uint length = -m_player->length().msecsTo(QTime(0,0));
    if (length != 0 && value > 0) {
        QTime pos(0,0);
        pos = pos.addMSecs(length * (value / 1000.0));
        m_player->setPosition(pos);
    }
}

void playerGui::showControls(bool show)
{
    m_openButton->setVisible(show);
    m_playButton->setVisible(show);
    m_pauseButton->setVisible(show);
    m_stopButton->setVisible(show);
    m_fullScreenButton->setVisible(show);
    m_positionSlider->setVisible(show);
    m_volumeSlider->setVisible(show);
    m_volumeLabel->setVisible(show);
    m_positionLabel->setVisible(show);
}

void playerGui::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (isFullScreen()) {
        showControls();
        m_fullScreenTimer.start(3000); //re-hide controls after 3s
    }
}

QToolButton *playerGui::initButton(QStyle::StandardPixmap icon, const QString & tip,
                                    QObject *dstobj, const char *slot_method, QLayout *layout)
{
    QToolButton *button = new QToolButton;
    button->setIcon(style()->standardIcon(icon));
    button->setIconSize(QSize(36, 36));
    button->setToolTip(tip);
    connect(button, SIGNAL(clicked()), dstobj, slot_method);
    layout->addWidget(button);

    return button;
}

void playerGui::createUI(QBoxLayout *appLayout)
{
    appLayout->addWidget(m_player);

    m_positionLabel = new QLabel();

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setTickPosition(QSlider::TicksBelow);
    m_positionSlider->setTickInterval(10);
    m_positionSlider->setMaximum(1000);

    connect(m_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPosition(int)));

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setTickPosition(QSlider::TicksLeft);
    m_volumeSlider->setTickInterval(2);
    m_volumeSlider->setMaximum(10);
    m_volumeSlider->setMaximumSize(64,32);

    connect(m_volumeSlider, SIGNAL(sliderMoved(int)), m_player, SLOT(setVolume(int)));

    QGridLayout *posLayout = new QGridLayout;
    posLayout->addWidget(m_positionLabel, 1, 0);
    posLayout->addWidget(m_positionSlider, 1, 1, 1, 2);
    appLayout->addLayout(posLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

    m_openButton = initButton(QStyle::SP_DialogOpenButton, tr("Open File"),
                              this, SLOT(open()), btnLayout);

    m_playButton = initButton(QStyle::SP_MediaPlay, tr("Play"),
                              m_player, SLOT(play()), btnLayout);

    m_pauseButton = initButton(QStyle::SP_MediaPause, tr("Pause"),
                               m_player, SLOT(pause()), btnLayout);

    m_stopButton = initButton(QStyle::SP_MediaStop, tr("Stop"),
                              m_player, SLOT(stop()), btnLayout);

    m_cameraButton = initButton(QStyle::SP_ComputerIcon,tr("Camera"),this,SLOT(camera()),btnLayout);
    m_fullScreenButton = initButton(QStyle::SP_TitleBarMaxButton, tr("Fullscreen"),
                                    this, SLOT(toggleFullScreen()), btnLayout);
    btnLayout->addStretch();

    m_volumeLabel = new QLabel();
    m_volumeLabel->setPixmap(
                style()->standardIcon(QStyle::SP_MediaVolume).pixmap(QSize(32, 32),
                                                                     QIcon::Normal, QIcon::On));

    btnLayout->addWidget(m_volumeLabel);
    btnLayout->addWidget(m_volumeSlider);
    appLayout->addLayout(btnLayout);
}
