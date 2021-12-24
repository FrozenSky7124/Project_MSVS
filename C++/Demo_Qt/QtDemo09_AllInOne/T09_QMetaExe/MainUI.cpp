#include "MainUI.h"
#include "ui_MainUI.h"

MainUI::MainUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUI)
{
    ui->setupUi(this);

    //Init Data
    ui->spinBoxA->setProperty("isPlayerA", true);
    ui->spinBoxB->setProperty("isPlayerA", false);

    playerA = new QPlayer("旅行者·空");
    playerA->setProperty("hp", 12000);
    playerA->setProperty("level", 70);
    connect(playerA, &QPlayer::levelChanged, this, &MainUI::on_playerLevelChanged);

    playerB = new QPlayer("旅行者·荧");
    playerB->setProperty("hp", 13000);
    playerB->setProperty("level", 90);
    connect(playerB, &QPlayer::levelChanged, this, &MainUI::on_playerLevelChanged);

    ui->spinBoxA->setValue(playerA->level());
    ui->spinBoxB->setValue(playerB->level());
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::on_spinBox_valueChanged(unsigned int argv1)
{
    Q_UNUSED(argv1)
    QSpinBox *spinBox = qobject_cast<QSpinBox*>(sender());
    if (spinBox->property("isPlayerA").toBool())
        playerA->setLevel(spinBox->value());
    else
        playerB->setLevel(spinBox->value());
}

void MainUI::on_playerLevelChanged(unsigned int level)
{
    Q_UNUSED(level)
    QPlayer *player = qobject_cast<QPlayer*>(sender());
    QString p_name = player->property("name").toString();
    unsigned int p_level = player->property("level").toUInt();
    unsigned int p_hp = player->property("hp").toUInt();

    if (p_name == "旅行者·空")
        ui->spinBoxA->setValue(player->level());
    else
        ui->spinBoxB->setValue(player->level());

    ui->plainTextEdit->appendPlainText(p_name + "  Lv " + QString::asprintf("%3d", p_level) + "  HP " + QString::asprintf("%d", p_hp));
}

void MainUI::on_btnAUp_clicked()
{
    ui->spinBoxA->stepUp();
}

void MainUI::on_btnBUp_clicked()
{
    ui->spinBoxB->stepUp();
}

void MainUI::on_btnMInfo_clicked()
{

}

void MainUI::on_spinBoxA_valueChanged(int value)
{
    playerA->setLevel(value);
}
