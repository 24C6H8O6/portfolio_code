#include "client.h"
#include "ui_client.h"
#include <QtNetwork>
#include <QDebug>
#include <QString>

//추가
#include <QSqlQuery>
//그래프
#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);//0으로 바꾸기
    // drawStarOnLabel(150, 150, 30, Qt::white);

    //사진넣는 경로
    QPixmap ID(":/new/prefix1/resources/remove_id.png");
    QPixmap PW(":/new/prefix1/resources/remove_password.png");
    QPixmap signId(":/new/prefix1/resources/remove_id.png");
    QPixmap signPw(":/new/prefix1/resources/remove_password2.png");
    QPixmap signPwChk(":/new/prefix1/resources/remove_password2.png");
    QPixmap signBirth(":/new/prefix1/resources/remove_birth.png");
    QPixmap signEmail(":/new/prefix1/resources/remove_email.png");
    QPixmap signSex(":/new/prefix1/resources/remove_gender.png");
    QPixmap signName(":/new/prefix1/resources/remove_name.png");
    QPixmap signPhone(":/new/prefix1/resources/remove_phone.png");
    QPixmap findidName(":/new/prefix1/resources/remove_name.png");
    QPixmap findidPhone(":/new/prefix1/resources/remove_phone.png");
    QPixmap findpwId(":/new/prefix1/resources/remove_id.png");
    QPixmap findpwName(":/new/prefix1/resources/remove_name.png");
    QPixmap findpwPhone(":/new/prefix1/resources/remove_phone.png");
    //사진 사이즈 맞추는 함수 : 넓이
    int w_ID = ui->label_loginId->width();
    int w_PW = ui->label_loginPw->width();
    int w_signId = ui->label_signId->width();
    int w_signPw = ui->label_signPw->width();
    int w_signPwChk = ui->label_signPwChk->width();
    int w_signBirth = ui->label_signBirth->width();
    int w_signSex = ui->label_signGender->width();
    int w_signName = ui->label_signName->width();
    int w_signPhone = ui->label_signPhone->width();
    int w_findidName = ui->label_findIdName->width();
    int w_findidPhone = ui->label_findIdPhone->width();
    int w_findpwId = ui->label_findPwId->width();
    int w_findpwName = ui->label_findPwName->width();
    int w_findpwPhone = ui->label_findPwPhone->width();
    //사진 사이즈 맞추는 함수 : 높이
    int h_ID = ui->label_loginId->height();
    int h_PW = ui->label_loginPw->height();
    int h_signId = ui->label_signId->height();
    int h_signPw = ui->label_signPw->height();
    int h_signPwChk = ui->label_signPwChk->height();
    int h_signBirth = ui->label_signBirth->height();
    int h_signSex = ui->label_signGender->height();
    int h_signName = ui->label_signName->height();
    int h_signPhone = ui->label_signPhone->height();
    int h_findidName = ui->label_findIdName->height();
    int h_findidPhone = ui->label_findIdPhone->height();
    int h_findpwId = ui->label_findPwId->height();
    int h_findpwName = ui->label_findPwName->height();
    int h_findpwPhone = ui->label_findPwPhone->height();
    // 사진 아이콘 넓이, 높이를 전부 라벨 크기에 맞추기
    ui->label_loginId->setPixmap(ID.scaled(w_ID,h_ID,Qt::KeepAspectRatio));
    ui->label_loginPw->setPixmap(PW.scaled(w_PW,h_PW,Qt::KeepAspectRatio));
    ui->label_signId->setPixmap(signId.scaled(w_signId,h_signId,Qt::KeepAspectRatio));
    ui->label_signPw->setPixmap(signPw.scaled(w_signPw,h_signPw,Qt::KeepAspectRatio));
    ui->label_signPwChk->setPixmap(signPwChk.scaled(w_signPwChk,h_signPwChk,Qt::KeepAspectRatio));
    ui->label_signBirth->setPixmap(signBirth.scaled(w_signBirth,h_signBirth,Qt::KeepAspectRatio));
    ui->label_signGender->setPixmap(signSex.scaled(w_signSex,h_signSex,Qt::KeepAspectRatio));
    ui->label_signName->setPixmap(signName.scaled(w_signName,h_signName,Qt::KeepAspectRatio));
    ui->label_signPhone->setPixmap(signPhone.scaled(w_signPhone,h_signPhone,Qt::KeepAspectRatio));
    ui->label_findIdName->setPixmap(findidName.scaled(w_findidName,h_findidName,Qt::KeepAspectRatio));
    ui->label_findIdPhone->setPixmap(findidPhone.scaled(w_findidPhone,h_findidPhone,Qt::KeepAspectRatio));
    ui->label_findPwId->setPixmap(findpwId.scaled(w_findpwId,h_findpwId,Qt::KeepAspectRatio));
    ui->label_findPwName->setPixmap(findpwName.scaled(w_findpwName,h_findpwName,Qt::KeepAspectRatio));
    ui->label_findPwPhone->setPixmap(findpwPhone.scaled(w_findpwPhone,h_findpwPhone,Qt::KeepAspectRatio));

    // back_login 버튼을 클릭할 때 실행될 슬롯
    connect(ui->back_login, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // 첫 번째 페이지로 이동
    });

    // back_login_2 버튼을 클릭할 때 실행될 슬롯
    connect(ui->back_login_2, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // 첫 번째 페이지로 이동
    });

    // back_login_3 버튼을 클릭할 때 실행될 슬롯
    connect(ui->back_login_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // 첫 번째 페이지로 이동
    });
    connect(ui->StartButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(5);  // 다섯 번째 페이지로 이동
    });

    // Gender 라디오 버튼 그룹에 남성 및 여성 라디오 버튼을 추가
    genderRadioGroup->addButton(ui->radioButton_signMale);
    genderRadioGroup->addButton(ui->radioButton_signFemale);

    // 비밀번호 입력란 및 비밀번호 확인 입력란의 텍스트가 변경될 때 CheckPw 함수를 호출하는 연결을 설정
    connect(ui->lineEdit_signPw, SIGNAL(textChanged(const QString&)), this, SLOT(CheckPw(const QString&)));
    connect(ui->lineEdit_signPwChk, SIGNAL(textChanged(const QString&)), this, SLOT(CheckPw(const QString&)));

    // "로그인" 버튼이 클릭될 때 Login 함수를 호출하는 연결을 설정
    connect(ui->pushButton_login, SIGNAL(clicked()), this, SLOT(Login()));
    // "가입" 버튼이 클릭될 때 Sign 함수를 호출하는 연결을 설정
    connect(ui->pushButton_sign, SIGNAL(clicked()), this, SLOT(Sign()));
    // "회원가입" 버튼이 클릭될 때 Signup 함수를 호출하는 연결을 설정
    connect(ui->pushButton_signUp, SIGNAL(clicked()), this, SLOT(Signup()));

    // back_login 버튼을 클릭할 때 실행될 슬롯
    connect(ui->back_login, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // 첫 번째 페이지로 이동
    });
    // back_login_2 버튼을 클릭할 때 실행될 슬롯
    connect(ui->back_login_2, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // 첫 번째 페이지로 이동
    });
    // back_login_3 버튼을 클릭할 때 실행될 슬롯
    connect(ui->back_login_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // 첫 번째 페이지로 이동
    });

    //이걸누르면 find 페이지로 이동
    connect(ui->pushButton_find, SIGNAL(clicked()), this, SLOT(FindPage()));
    //아이디 비밀번호 찾기
    connect(ui->pushButton_findId, SIGNAL(clicked()), this, SLOT(FindId()));
    connect(ui->pushButton_findPw, SIGNAL(clicked()), this, SLOT(FindPw()));
    //아이디 중복체크
    connect(ui->Chk_id, SIGNAL(clicked()), this, SLOT(CheckId()));
    connect(ui->Chk_Phone, SIGNAL(clicked()), this, SLOT(CheckPhone()));

    connect(ui->lineEdit_signName, SIGNAL(textChanged(const QString&)), this, SLOT(CheckName(const QString&)));
    connect(ui->lineEdit_signPhone, SIGNAL(textChanged(const QString&)), this, SLOT(CheckPhone(const QString&)));

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectButton()));
    // 선택한 이미지로 검사 시작
    connect(ui->examinationStart, SIGNAL(clicked()), this, SLOT(testStart()));
    // 검사에서 뒤로가기
    connect(ui->testStart_backBtn, SIGNAL(clicked()), this, SLOT(testBack()));

    // 다음 버튼
    // 검사 첫 페이지에서 두 번째 페이지로
    connect(ui->btn_next1, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentIndex(7);});
    // 검사 두번째 페이지에서 세 번째 페이지로
    connect(ui->btn_next2, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentIndex(8);});
    // 검사 세번쨰 페이지에서 네 번째 페이지로
    connect(ui->btn_next3, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentIndex(9);});

    // 이전 버튼
    // 검사 두번째 페이지에서 첫 번째 페이지로
    connect(ui->btn_front2, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentIndex(6);});
    // 검사 세번째 페이지에서 두 번째 페이지로
    connect(ui->btn_front3, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentIndex(7);});
    // 검사 네번째 페이지에서 세 번째 페이지로
    connect(ui->btn_front4, &QPushButton::clicked, [=](){ui->stackedWidget->setCurrentIndex(8);});
    // 설문 제출하기 버튼을 누를 시 설문 결과를 서버에 보내고 검토중 화면으로 이동
    connect(ui->btn_submit, SIGNAL(clicked()), this, SLOT(send_testData()));
    // 날짜별 설문 결과 조회 페이지 이동
    connect(ui->ResultButton, SIGNAL(clicked()), this, SLOT(viewResult()));
    connect(ui->backTest, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentIndex(4);
        // 클리어 필요
        pictureList = {}; // pictureList 초기화
        // 버튼 13개 border 초기화
        ui->pushButton_index1->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index2->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index3->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index4->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index5->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index6->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index7->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index8->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index9->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index10->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index11->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index12->setStyleSheet("border: 1px solid white;");
        ui->pushButton_index13->setStyleSheet("border: 1px solid white;");
        // textEdit 초기화
        ui->textEdit_image1->clear();
        ui->textEdit_image2->clear();
        ui->textEdit_image3->clear();
        ui->textEdit_image4->clear();
    });
    connect(ui->backtoMain, SIGNAL(clicked()), this, SLOT(BacktoMain()));
    connect(ui->backToMain_2, SIGNAL(clicked()), this, SLOT(BacktoMain2()));


    // 이미지 선택 시 테두리 생성 및 리스트 추가
    image_Btns = new QButtonGroup;
    image_Btns->addButton(ui->pushButton_index1);
    image_Btns->addButton(ui->pushButton_index2);
    image_Btns->addButton(ui->pushButton_index3);
    image_Btns->addButton(ui->pushButton_index4);
    image_Btns->addButton(ui->pushButton_index5);
    image_Btns->addButton(ui->pushButton_index6);
    image_Btns->addButton(ui->pushButton_index7);
    image_Btns->addButton(ui->pushButton_index8);
    image_Btns->addButton(ui->pushButton_index9);
    image_Btns->addButton(ui->pushButton_index10);
    image_Btns->addButton(ui->pushButton_index11);
    image_Btns->addButton(ui->pushButton_index12);
    image_Btns->addButton(ui->pushButton_index13);
    connect(image_Btns, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(picturePick(QAbstractButton*)));
    initialize();
    QSqlDatabase userdb;

    // 리스트에 모든 이미지 경로 넣기
    picturePathList = {":/new/prefix1/resources/neu1.png",
                       ":/new/prefix1/resources/neu2.png",
                       ":/new/prefix1/resources/neu3.jpg",
                       ":/new/prefix1/resources/neu4.png",
                       ":/new/prefix1/resources/pos1.jpg",
                       ":/new/prefix1/resources/pos2.jpg",
                       ":/new/prefix1/resources/pos3.png",
                       ":/new/prefix1/resources/pos4.png",
                       ":/new/prefix1/resources/neg1.png",
                       ":/new/prefix1/resources/neg2.png",
                       ":/new/prefix1/resources/neg3.jpg",
                       ":/new/prefix1/resources/neg4.jpg",
                       ":/new/prefix1/resources/neg5.png"};

    btnStyleSheet();  // 버튼 스타일시트

    // gif 넣기 : 조사결과 로딩중
    QMovie *Movie=new QMovie(":/new/prefix1/resources/movie1.gif");
    // QMovie의 크기를 QLabel의 크기에 맞게 조정
    Movie->setScaledSize(ui->label_loading->size());
    // QLabel에 QMovie를 설정하고 애니메이션을 시작
    ui->label_loading->setMovie(Movie);
    Movie->start();

    // 추가 : 데이터 베이스 시작하고 열기
    //    if (!userdb.isOpen() && !userdbOpen())
    //    {
    //        qDebug() << "데이터베이스를 열지 못했습니다.";
    //    }
}

Client::~Client()
{
    delete ui;
}

void Client::initialize()
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()),
            this,      SLOT(readMessage()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this,      SLOT(disconnected()));
}

void Client::connectButton()
{
    // 서버의 IP 주소와 포트를 설정
    serverIP = "10.10.21.103"; // 여기를 변경해라 : 서버의 IP 주소로 변경
    serverPort = "8888";     // 서버의 포트로 변경

    QHostAddress serverAddress(serverIP);
    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());

    if (!tcpSocket->waitForConnected(5000))
    {
        // 서버에 연결에 실패한 경우
        QMessageBox msgbox0;
        msgbox0.setWindowTitle("감정분석");
        msgbox0.setIcon(QMessageBox::Warning);
        msgbox0.setText("서버에 연결할 수 없습니다.");
        msgbox0.setStandardButtons(QMessageBox::Ok);

        if (msgbox0.exec() == QMessageBox::Ok)
        {           
            // 연결에 실패한 경우, 아무것도 하지 않고 메시지 박스만 표시
        }
    }

    else
    {
        // 서버에 연결이 성공한 경우
        ui->textEdit->append("<- 서버에게 연결 요청");

        // 서버에 연결이 성공한 경우에만 메시지 박스 표시
        QMessageBox msgbox1;
        msgbox1.setWindowTitle("감정분석");
        msgbox1.setIcon(QMessageBox::Information);
        msgbox1.setText("서버에 접속되었습니다.");
        msgbox1.setStandardButtons(QMessageBox::Ok);

        if (msgbox1.exec() == QMessageBox::Ok)
        {
            // 다른 페이지로 전환, 예를 들어, stackedwidget1로 전환 (필요에 따라 인덱스를 변경)
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
}

QHostAddress Client::getMyIP()
{
    QHostAddress myAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // localhost(127.0.0.1) 가 아닌 것을 사용
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            myAddress = ipAddressesList.at(i);
            break;
        }
    }
    if (myAddress.toString().isEmpty())
        myAddress = QHostAddress(QHostAddress::LocalHost);
    return myAddress;
}

//추가 페이지 이동하게 하는 함수
void Client::LoginPage() // 로그인 페이지
{
    ui->lineEdit_loginId->clear();    
    ui->lineEdit_loginPw->clear();    
    ui->stackedWidget->setCurrentIndex(1);
}

void Client::SignPage() // 회원가입 페이지
{
    ui->lineEdit_signId->clear();
    ui->lineEdit_signPw->clear();
    ui->lineEdit_signPwChk->clear();
    ui->lineEdit_signName->clear();
    ui->lineEdit_signPhone->clear();
    ui->dateEdit_signBirth->clear();
    ui->label_signIdChk->clear();
    ui->label_signPhone2->clear();

    ui->radioButton_signMale->setChecked(false);
    ui->radioButton_signFemale->setChecked(false);
}


void Client::FindPage() // 아이디/비번찾기 페이지
{
    ui->lineEdit_findIdName->clear();
    ui->lineEdit_findPwId->clear();
    ui->lineEdit_findIdPhone->clear();
    ui->lineEdit_findPwName->clear();
    ui->lineEdit_findPwPhone->clear();
    ui->stackedWidget->setCurrentIndex(3);
}

//추가 : 회원가입 함수인데 DB연결되면 주석풀어라.
void Client::Sign()
{
    ui->stackedWidget->setCurrentIndex(2);

    // 회원가입 페이지로 이동할 때 모든 입력 필드 및 레이블 초기화
    ui->lineEdit_signId->clear();
    ui->lineEdit_signPw->clear();
    ui->lineEdit_signPwChk->clear();
    ui->lineEdit_signName->clear();
    ui->lineEdit_signPhone->clear();
    ui->dateEdit_signBirth->clear();
    ui->label_signIdChk->clear();
    ui->label_signPhone2->clear();
}


void Client::Signup()
{
    // 회원가입에서 입력했던 정보를 가져옴
    QString id = ui->lineEdit_signId->text();
    QString pw = ui->lineEdit_signPw->text();
    QString pwchk = ui->lineEdit_signPwChk->text();
    QString name = ui->lineEdit_signName->text();
    QString gender_choice;
    QString birth = ui->dateEdit_signBirth->date().toString("yyyyMMdd");
    QString phonenumber = ui->lineEdit_signPhone->text();

    // 남녀 고르는 것 설정
    if (ui->radioButton_signMale->isChecked())
    {
        gender_choice = "남";
    }
    else if (ui->radioButton_signFemale->isChecked())
    {
        gender_choice = "여";
    }

    // 회원가입할 때 정보 하나라도 빠뜨렸을 시에 메세지박스 띄움
    if (id.isEmpty() || pw.isEmpty() || pwchk.isEmpty() || name.isEmpty()
        || gender_choice.isEmpty() || birth.isEmpty() || phonenumber.isEmpty())
    {
        // 회원가입 부분이 미완료 됐을 때
        QMessageBox msgbox1;
        msgbox1.setWindowTitle("감정분석");
        msgbox1.setIcon(QMessageBox::Warning);
        msgbox1.setText("빈 칸을 모두 입력해 주세요.");
        msgbox1.setStandardButtons(QMessageBox::Ok);

        // Ok 누르면 창 닫힘
        if (msgbox1.exec() == QMessageBox::Ok)
        {
            msgbox1.close();
        }
    }

    else if (pw != pwchk)
    {
        // 비밀번호와 비밀번호 확인이 일치하지 않는 경우
        QMessageBox msgbox1;
        msgbox1.setWindowTitle("감정분석");
        msgbox1.setIcon(QMessageBox::Warning);
        msgbox1.setText("비밀번호와 비밀번호 확인이 일치하지 않습니다.");
        msgbox1.setStandardButtons(QMessageBox::Ok);

        // Ok 누르면 창 닫힘
        if (msgbox1.exec() == QMessageBox::Ok)
        {
            msgbox1.close();
        }
    }

    else if (ui->dateEdit_signBirth->date().isNull())
    {
        // 생년월일을 선택하지 않았을 경우
        msgbox1.setWindowTitle("감정분석");
        msgbox1.setIcon(QMessageBox::Warning);
        msgbox1.setText("생년월일을 선택하세요.");
        msgbox1.setStandardButtons(QMessageBox::Ok);

        if (msgbox1.exec() == QMessageBox::Ok)
        {
            msgbox1.close();
        }
    }

    else if (pw.length() < 8 || !pw.contains(QRegularExpression("[!@#$%^&*()]")))
    {
        // 비밀번호가 8자리 이상이고 특수문자를 포함하지 않는 경우
        msgbox1.setWindowTitle("감정분석");
        msgbox1.setIcon(QMessageBox::Warning);
        msgbox1.setText("비밀번호는 8자리 이상이며, 특수문자를 포함해야 합니다.");
        msgbox1.setStandardButtons(QMessageBox::Ok);

        // Ok 누르면 창 닫힘
        if (msgbox1.exec() == QMessageBox::Ok)
        {
            msgbox1.close();
        }
    }

    else if (phonenumber.length() != 13 || !phonenumber.contains(QRegularExpression("^\\d{3}-\\d{4}-\\d{4}$")))
    {
        // 휴대폰 번호가 13자리이고 정해진 형식이 아닌 경우
        QMessageBox msgbox1;
        msgbox1.setWindowTitle("감정분석");
        msgbox1.setIcon(QMessageBox::Warning);
        msgbox1.setText("휴대폰 번호는 XXX-XXXX-XXXX 형식으로 입력해야 합니다.");
        msgbox1.setStandardButtons(QMessageBox::Ok);

        // Ok 누르면 창 닫힘
        if (msgbox1.exec() == QMessageBox::Ok)
        {
            msgbox1.close();
        }
    }

    else
    {
        // 모든 조건을 통과한 경우에만 회원가입 진행됨
        message = "SIGN_REQUEST●" + id + "●" + pw + "●" + name + "●" + gender_choice + "●" + birth + "●" + phonenumber+ "●";
                      QByteArray SignData = message.toUtf8();
        tcpSocket->write(SignData);
    }
}

// 추가: 아이디 비번찾기
void Client::FindId()
{
    QString findName = ui->lineEdit_findIdName->text();
    QString findPhone = ui->lineEdit_findIdPhone->text();

    if (findName.isEmpty() || findPhone.isEmpty())
    {
        msgbox3.warning(this, "감정분석", "이름과 전화번호를 입력하세요.");
        return;
    }

    // 검색 조건을 구분자로 생성
    QString message = "FIND_ID_REQUEST●" + findName + "●" + findPhone + "●";
                      QByteArray FoundId = message.toUtf8();
    tcpSocket->write(FoundId);
}

void Client::FindPw()
{
    QString findID = ui->lineEdit_findPwId->text();
    QString findName = ui->lineEdit_findPwName->text();
    QString findPhone = ui->lineEdit_findPwPhone->text();

    if (findID.isEmpty() || findName.isEmpty() || findPhone.isEmpty())
    {
        msgbox4.warning(this, "감정분석", "아이디, 이름, 전화번호를 입력하세요.");
        return;
    }

    // 서버에 비밀번호 찾기 요청을 보냄
    QString message = "FIND_PW_REQUEST●" + findID + "●" + findName + "●" + findPhone + "●";
                          QByteArray FoundPw = message.toUtf8();
    tcpSocket->write(FoundPw);
}


//로그인함수
void Client::Login()
{
    QString loginID = ui->lineEdit_loginId->text();
    QString loginPW = ui->lineEdit_loginPw->text();

    if (loginID.isEmpty() || loginPW.isEmpty())
    {
        msgbox5.warning(this, "로그인 오류", "아이디와 비밀번호를 입력하세요.");
        return;
    }

    // 아이디와 비밀번호를 구분자를 사용해서 서버로 보냄!
    QString message = "LOGIN_REQUEST●" + loginID + "●" + loginPW+ "●";
                          QByteArray requestData = message.toUtf8();
    tcpSocket->write(requestData);

}

bool idChecked = false;     // 아이디 중복 확인 여부를 저장하는 변수
bool phoneChecked = false;  // 휴대폰 중복 확인 여부를 저장하는 변수

// 아이디 중복체크
void Client::CheckId()
{
    QRegularExpression englishNumberRegExp("^[A-Za-z0-9]*$");    // 영어 대소문자와 숫자만 입력 가능하게 구현

    // 영어와 숫자 입력 필터를 적용한 validator 생성
    QValidator* englishNumberValidator = new QRegularExpressionValidator(englishNumberRegExp, this);

    // lineEdit_signId에 validator 설정
    ui->lineEdit_signId->setValidator(englishNumberValidator);


    QString text = ui->lineEdit_signId->text();

    if (!text.isEmpty())
    {
        // 사용자가 입력한 ID를 가져옴
        QString findID = text;

        // 구분자를 사용하여 서버로 메시지를 보냄
        QString message = "CHECK_ID_REQUEST●" + findID + "●";
        QByteArray checkcheckID = message.toUtf8();
        tcpSocket->write(checkcheckID);

        //아이디 중복확인
        idChecked = true;
    }
    else
    {
        ui->label_signIdChk->clear();
        idChecked = false;
    }
}

void Client::CheckPw(const QString& text)
{
    QString signPw = ui->lineEdit_signPw->text();
    QString signPwChk = ui->lineEdit_signPwChk->text();

    if (!signPw.isEmpty() && !signPwChk.isEmpty())
    {
        if (signPw == signPwChk)
        {
            // 비밀번호와 비밀번호 확인이 일치하는 경우
            QRegularExpression specialCharPattern("[!@#$%^&*(),.?:{}|<>]+");
            if (!signPwChk.contains(specialCharPattern) || signPwChk.length() < 8)
            {
                ui->label_signPwChk2->setText("비밀번호는 8자리 이상, 특수문자를 포함해야 합니다.");
            }
            else
            {
                ui->label_signPwChk2->setText("비밀번호를 확인하였습니다.");

            }
        }
        else
        {
            // 비밀번호와 비밀번호 확인이 일치하지 않는 경우
            ui->label_signPwChk2->setText("비밀번호를 다시 확인해주세요.");
        }
    }
    else
    {
        ui->label_signPwChk2->clear();
    }
}

void Client::CheckBirth()
{
    QDate birthDate = ui->dateEdit_signBirth->date();        // dateEdit_signBirth 위젯에서 선택한 날짜 가져오기
    QString birthString = birthDate.toString("yyyyMMdd");    // 날짜를 원하는 형식으로 변환 (yyyyMMdd)

    if (birthString.length() != 8)
    {
        ui->label_signBirth2->setText("생년월일은 8자리로 입력하세요.");
    }
    else
    {
        ui->label_signBirth2->setText("생년월일을 확인하였습니다.");
    }
}

void Client::CheckName()
{
    QRegularExpression koreanRegExp("^[가-힣]*$");             // 한글만 입력 가능하게 구현

    // 한글 입력 필터를 적용한 validator 생성
    QValidator* koreanValidator = new QRegularExpressionValidator(koreanRegExp, this);

    // lineEdit_signName에 validator 설정
    ui->lineEdit_signName->setValidator(koreanValidator);
}

//휴대폰 중복체크
void Client::CheckPhone()
{
    QString signPhone = ui->lineEdit_signPhone->text();

    // 입력된 숫자를 하이픈(-)으로 구분하여 11자리로.
    QString ChkPhone = signPhone;
    ChkPhone.remove(QRegularExpression("[^0-9]"));

    if (ChkPhone.length() == 11)
    {
        ChkPhone = ChkPhone.left(3) + "-" + ChkPhone.mid(3, 4) + "-" + ChkPhone.right(4);
        ui->lineEdit_signPhone->setText(ChkPhone);

        // 구분자를 사용하여 서버로 메시지를 보냄
        QString message = "CHECK_PHONE_REQUEST●" + ChkPhone + "●";
        QByteArray checkcheckphone = message.toUtf8();
        tcpSocket->write(checkcheckphone);

        phoneChecked = true;
    }
    else
    {
        ui->label_signPhone2->setText("휴대폰 번호는 11자리 숫자로 입력하세요.");

        phoneChecked = false;
    }
}


// 서버에서 클라이언트로 보내는 함수
void Client::readMessage()
{
    if (tcpSocket->bytesAvailable() > 0)
    {
        QByteArray readData = tcpSocket->readAll();
        ui->textEdit->append("-> " + readData);

        // 메시지를 처리하는 부분
        QList<QString> responseList = QString::fromUtf8(readData).split("●");

        if (responseList[0] == "LOGIN_SUCCESS")
        {
            // 로그인 성공 메시지를 받은 경우
            // 사용자 리스트에 아이디 추가 및 다른 동작 수행
            msgbox7.setWindowTitle("로그인 성공");
            msgbox7.setIcon(QMessageBox::Information);
            msgbox7.setText("로그인이 성공적으로 완료되었습니다!");
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();

            ui->stackedWidget->setCurrentIndex(4); // 로그인 성공 후 이동할 페이지 인덱스
            login_id = responseList[1];
            ui->lineEdit_loginId->clear();
            ui->lineEdit_loginPw->clear();
        }

        else if (responseList[0] == "LOGIN_FAILURE")
        {
            // 로그인 실패 메시지를 받은 경우
            msgbox7.setWindowTitle("로그인 실패");
            msgbox7.setIcon(QMessageBox::Critical);
            msgbox7.setText("로그인에 실패했습니다. 아이디 또는 비밀번호를 다시 확인해주세요.");
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();
        }

        else if (responseList[0] == "SIGN_SUCCESS")
        {
            // 회원가입 성공 메시지를 받은 경우
            msgbox7.setWindowTitle("회원가입 성공");
            msgbox7.setIcon(QMessageBox::Information);
            msgbox7.setText("회원가입이 성공적으로 완료되었습니다!");
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();

            genderRadioGroup->setExclusive(false);
            ui->radioButton_signMale->setChecked(false);
            ui->radioButton_signFemale->setChecked(false);
            genderRadioGroup->setExclusive(true);

            ui->stackedWidget->setCurrentIndex(1);      // 로그인 성공 후 이동할 페이지 인덱스

        }
        else if (responseList[0] == "SIGN_FAILURE")
        {
            // 회원가입 실패 메시지를 받은 경우
            msgbox7.setWindowTitle("회원가입 실패");
            msgbox7.setIcon(QMessageBox::Critical);
            msgbox7.setText("회원가입 중 오류가 발생했습니다.");
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();
            // 원하는 동작 수행 (가령, 실패 시 다시 회원가입 페이지로 돌아가는 코드 등)
        }

        else if (responseList[0] == "FOUND_ID")
        {
            // 아이디를 찾은 경우
            QString foundID = responseList[1];           // 아이디는 responseList의 두 번째 항목에 있을 것이라 가정

            msgbox7.setWindowTitle("아이디 찾기");
            msgbox7.setIcon(QMessageBox::Information);
            msgbox7.setText("아이디를 찾았습니다: " + foundID);
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();

            ui->stackedWidget->setCurrentIndex(1);
        }

        else if (responseList[0] == "ID_NOT_FOUND")
        {
            // 일치하는 아이디가 없는 경우
            msgbox7.setWindowTitle("아이디 찾기");
            msgbox7.setIcon(QMessageBox::Warning);
            msgbox7.setText("일치하는 아이디를 찾을 수 없습니다.");
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();

            ui->stackedWidget->setCurrentIndex(1);
        }

        else if (responseList[0] == "FOUND_PW")
        {
            // 비밀번호를 찾은 경우
            QString foundPW = responseList[1];             // 비밀번호는 responseList의 두 번째 항목에 있을 것이라 가정

            msgbox7.setWindowTitle("비밀번호 찾기");
            msgbox7.setIcon(QMessageBox::Information);
            msgbox7.setText("비밀번호를 찾았습니다: " + foundPW);
            msgbox7.setStandardButtons(QMessageBox::Ok);
            msgbox7.exec();

            ui->stackedWidget->setCurrentIndex(1);         // 로그인 페이지 (1번 페이지)로 이동
        }

        else if (responseList[0] == "PW_NOT_FOUND")
        {
            // 비밀번호를 찾지 못한 경우
            msgbox7.warning(this, "감정분석", "일치하는 정보를 찾을 수 없습니다.");
            ui->stackedWidget->setCurrentIndex(1);
        }


        else if (responseList[0] == "CHECK_ID_RESPONSE")
        {
            // 아이디 중복 확인 응답 메시지를 받은 경우
            QString result = responseList[1];               // 응답 메시지의 두 번째 항목은 결과

            if (result == "UNAVAILABLE")
            {
                ui->label_signIdChk->setText("이미 사용 중인 ID입니다.");
            }

            else if (result == "AVAILABLE")
            {
                ui->label_signIdChk->setText("사용 가능한 ID입니다.");
            }
            else
            {
                ui->label_signIdChk->setText("서버에서 알 수 없는 응답을 받았습니다.");
            }
        }

        else if (responseList[0] == "CHECK_PHONE_RESPONSE")
        {
            // 휴대폰 번호 확인 응답 메시지를 받은 경우
            QString result = responseList[1];                 // 응답 메시지의 두 번째 항목은 결과

            if (result == "VALID")
            {
                ui->label_signPhone2->setText("휴대폰 번호가 사용가능 합니다.");
            }
            else if (result == "INVALID")
            {
                ui->label_signPhone2->setText("중복된 휴대폰 번호입니다.");
            }
            else
            {
                ui->label_signPhone2->setText("서버에서 알 수 없는 응답을 받았습니다.");
            }
        }

        // 파이썬에서 검사 완료한 것을 qt서버에서 받아왔을 때 11번 페이지로 넘어가도록 구현
        else if (responseList[0] == "TEST_RESULT")
        {
            ui->stackedWidget->setCurrentIndex(11);

            // 받아온 각각의 인덱스를 각각의 리스트에 넣어서 각 사진 레이블에 넣고
            // 텍스트 레이블에 결과 값들을 하나씩 넣을 것
            QList<QString> res_idx_list = responseList[1].split(',');
            QList<QString> res_x_list = responseList[2].split(',');
            QList<QString> res_y_list = responseList[3].split(',');
            QList<QString> res_emo_list = responseList[4].split(',');
            QList<QString> res_final_list = responseList[5].split(',');

            // 사진 라벨사이즈에 맞춰 넣기
            QPixmap pixmap_img1(picturePathList[res_idx_list[0].toInt()-1]);
            QSize labelSize = ui->label_result_img1->size();
            QPixmap scaledPixmap1 = pixmap_img1.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img1->setPixmap(scaledPixmap1);
            ui->label_result_img1->setAlignment(Qt::AlignCenter);

            QPixmap pixmap_img2(picturePathList[res_idx_list[1].toInt()-1]);
            QPixmap scaledPixmap2 = pixmap_img2.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img2->setPixmap(scaledPixmap2);
            ui->label_result_img2->setAlignment(Qt::AlignCenter);

            QPixmap pixmap_img3(picturePathList[res_idx_list[2].toInt()-1]);
            QPixmap scaledPixmap3 = pixmap_img3.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img3->setPixmap(scaledPixmap3);
            ui->label_result_img3->setAlignment(Qt::AlignCenter);

            QPixmap pixmap_img4(picturePathList[res_idx_list[3].toInt()-1]);
            QPixmap scaledPixmap4 = pixmap_img4.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img4->setPixmap(scaledPixmap4);
            ui->label_result_img4->setAlignment(Qt::AlignCenter);

            // 결과 값 넣기
            ui->label_resText1->setText(QString("<첫 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[0]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[0]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[0]));
            ui->label_resText2->setText(QString("<두 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[1]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[1]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[1]));
            ui->label_resText3->setText(QString("<세 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[2]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[2]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[2]));
            ui->label_resText4->setText(QString("<네 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[3]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[3]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[3]));
            ui->label_finalText->setText(QString("<최종 감정 분석 결과>\n")+
                                         "쾌/불쾌 수치 : " + QString(res_final_list[0]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_final_list[1]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_final_list[2]));

            // 그래프에 점 찍기
            double final_x = res_final_list[0].toDouble();
            double final_y = res_final_list[1].toDouble();
            moveStar(final_x, final_y);
        }

        // 서버로 부터 DB의 시간 값들을 받아왔을 때
        else if (responseList[0] == "RESULT_DATES")
        {
            if (responseList[1] == "0")
            {
                // 서버 응답에 저장된 감정 분석 결과가 없을 때 오류 메시지를 표시하고 함수 종료
                QMessageBox::critical(this, "감정분석", "저장되어 있는 감정 분석 결과들이 없습니다!");
                return;
            }
            else
            {
                ui->stackedWidget->setCurrentIndex(12);
                // 서버 응답에서 날짜 및 시간 목록을 추출
                qDebug() << responseList[2];
                QList<QString> timeList = responseList[2].split(',');

                // 각 날짜와 시간을 순회하며 화면에 표시
                for (int i = 0; i < timeList.size(); i++)
                {
                    QHBoxLayout *hbl = new QHBoxLayout();                                      // 수평 레이아웃 생성
                    QLabel *label = new QLabel(timeList[i].replace('T', ' ').split(".")[0]);   // 레이블 생성 및 날짜 및 시간 형식 수정
                    label->setStyleSheet("QLabel {"
                                         "    color: white;"

                                         "}");
                    QPushButton *btn = new QPushButton("결과 조회");                             // 버튼 생성
                    btn->setStyleSheet("QPushButton {"
                                       "    color: white;"
                                       "    background-color: rgba(255, 255, 255, 0);"          // 완전히 투명
                                       "    border: 1px solid white;"
                                       "    border-radius: 7px;"
                                       "    padding: 5px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "    background-color: #faa5f6;"
                                       "}");

                    label->setFixedSize(160, 40);                      // 레이블 크기 설정
                    btn->setFixedSize(140, 40);                        // 버튼 크기 설정
                    hbl->addWidget(label);                             // 수평 레이아웃에 레이블 추가
                    hbl->addWidget(btn);                               // 수평 레이아웃에 버튼 추가
                    ui->verticalLayout_4->addLayout(hbl);              // 수직 레이아웃에 수평 레이아웃 추가

                    // 결과 조회 버튼 클릭 시 화면 전환
                    connect(btn, &QPushButton::clicked, this, [=]()
                        {
                            QString sendData = "VIEWTIMERESULT●" + login_id + "●" + timeList[i] + "●";
                            tcpSocket->write(sendData.toUtf8());
                            ui->backtoMain->setText("뒤로가기");
                        });
                }

                // 수직 레이아웃에 빈 공간 추가로 위젯이 균일하게 표시
                ui->verticalLayout_4->addStretch();
            }
        }

        else if (responseList[0] == "RESULT_LIST")
        {
            ui->stackedWidget->setCurrentIndex(11);

            // 받아온 각각의 인덱스를 각각의 리스트에 넣어서 각 사진 레이블에 넣고
            // 텍스트 레이블에 결과 값들을 하나씩 넣을 것
            QList<QString> res_idx_list = responseList[1].split(',');
            QList<QString> res_x_list = responseList[2].split(',');
            QList<QString> res_y_list = responseList[3].split(',');
            QList<QString> res_emo_list = responseList[4].split(',');
            QList<QString> res_final_list = responseList[5].split(',');

            // 사진 라벨사이즈에 맞춰 넣기
            QPixmap pixmap_img1(picturePathList[res_idx_list[0].toInt()-1]);
            QSize labelSize = ui->label_result_img1->size();
            QPixmap scaledPixmap1 = pixmap_img1.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img1->setPixmap(scaledPixmap1);
            ui->label_result_img1->setAlignment(Qt::AlignCenter);

            QPixmap pixmap_img2(picturePathList[res_idx_list[1].toInt()-1]);
            QPixmap scaledPixmap2 = pixmap_img2.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img2->setPixmap(scaledPixmap2);
            ui->label_result_img2->setAlignment(Qt::AlignCenter);

            QPixmap pixmap_img3(picturePathList[res_idx_list[2].toInt()-1]);
            QPixmap scaledPixmap3 = pixmap_img3.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img3->setPixmap(scaledPixmap3);
            ui->label_result_img3->setAlignment(Qt::AlignCenter);

            QPixmap pixmap_img4(picturePathList[res_idx_list[3].toInt()-1]);
            QPixmap scaledPixmap4 = pixmap_img4.scaled(labelSize, Qt::KeepAspectRatio);
            ui->label_result_img4->setPixmap(scaledPixmap4);
            ui->label_result_img4->setAlignment(Qt::AlignCenter);

            // 결과 값 넣기
            ui->label_resText1->setText(QString("<첫 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[0]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[0]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[0]));
            ui->label_resText2->setText(QString("<두 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[1]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[1]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[1]));
            ui->label_resText3->setText(QString("<세 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[2]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[2]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[2]));
            ui->label_resText4->setText(QString("<네 번째 이미지 감정 분석 결과>\n")+
                                        "쾌/불쾌 수치 : " + QString(res_x_list[3]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_y_list[3]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_emo_list[3]));
            ui->label_finalText->setText(QString("<최종 감정 분석 결과>\n")+
                                         "쾌/불쾌 수치 : " + QString(res_final_list[0]) + "\n" +
                                        "긍정/부정 수치 : " + QString(res_final_list[1]) + "\n" +
                                        "감정 분석 결과 : " + QString(res_final_list[2]));

            // 그래프에 점 찍기
            double final_x = res_final_list[0].toDouble();
            double final_y = res_final_list[1].toDouble();
            moveStar(final_x, final_y);

        }

    }
}

// 버튼을 클릭하면 실행되는 함수로, 사진 선택을 처리
void Client::picturePick(QAbstractButton *btn)
{
    int btn_index = -1;

    // 클릭한 버튼이 어떤 버튼인지 확인하고 해당 버튼의 인덱스를 할당
    if (btn == ui->pushButton_index1) btn_index = 1;
    else if (btn == ui->pushButton_index2) btn_index = 2;
    else if (btn == ui->pushButton_index3) btn_index = 3;
    else if (btn == ui->pushButton_index4) btn_index = 4;
    else if (btn == ui->pushButton_index5) btn_index = 5;
    else if (btn == ui->pushButton_index6) btn_index = 6;
    else if (btn == ui->pushButton_index7) btn_index = 7;
    else if (btn == ui->pushButton_index8) btn_index = 8;
    else if (btn == ui->pushButton_index9) btn_index = 9;
    else if (btn == ui->pushButton_index10) btn_index = 10;
    else if (btn == ui->pushButton_index11) btn_index = 11;
    else if (btn == ui->pushButton_index12) btn_index = 12;
    else if (btn == ui->pushButton_index13) btn_index = 13;

    if (btn_index != -1)
    {
         // 이미 선택된 이미지인지 확인하고, 선택한 경우 테두리를 표시하고 리스트에 추가
        if (btn->styleSheet() == "border: 5px solid red;")
        {
            pictureList.removeOne(QString::number(btn_index)); // 리스트에서 삭제
            btn->setStyleSheet(""); // 선택 해제
        }
        else // 처음으로 선택한 이미지라면
        {
            // 이미 4장 이상의 이미지가 선택된 경우 경고 메시지를 표시
            if(pictureList.size() == 4)
            {
                msgbox8.setWindowTitle("감정검사");
                msgbox8.setIcon(QMessageBox::Information);
                msgbox8.setText("사진을 4장이상 선택 하셨습니다.");
                msgbox8.setStandardButtons(QMessageBox::Ok);

                if (msgbox8.exec() == QMessageBox::Ok)
                {
                     msgbox8.close();
                }
            }

            // 선택한 이미지를 리스트에 추가하고 테두리를 추가
            else
            {
                pictureList.append(QString::number(btn_index)); // 리스트에 추가
                btn->setStyleSheet("border: 5px solid red;"); // 선택됨(테두리 추가)
            }
        }
    }
}

// 검사 시작 버튼을 클릭했을 때 실행되는 함수
void Client::testStart()
{
    // 선택된 이미지가 4장이 아닌 경우 경고 메시지를 표시하고 반환
    if (pictureList.size() != 4)
    {
        QMessageBox::critical(this, "감정 분석", "사진을 4개 선택해주세요!");
        return;
    }

    // 선택된 이미지가 4장인 경우
    else
    {
        // 화면 이동
        ui->stackedWidget->setCurrentIndex(6);

        // 화면을 변경하고 선택된 이미지를 라벨에 표시
        QPixmap pixmap_img1(picturePathList[pictureList[0].toInt()-1]);
        QSize labelSize = ui->label_image1->size();
        QPixmap scaledPixmap1 = pixmap_img1.scaled(labelSize, Qt::KeepAspectRatio);
        ui->label_image1->setPixmap(scaledPixmap1);
        ui->label_image1->setAlignment(Qt::AlignCenter);

        QPixmap pixmap_img2(picturePathList[pictureList[1].toInt()-1]);
        QPixmap scaledPixmap2 = pixmap_img2.scaled(labelSize, Qt::KeepAspectRatio);
        ui->label_image2->setPixmap(scaledPixmap2);
        ui->label_image2->setAlignment(Qt::AlignCenter);

        QPixmap pixmap_img3(picturePathList[pictureList[2].toInt()-1]);
        QPixmap scaledPixmap3 = pixmap_img3.scaled(labelSize, Qt::KeepAspectRatio);
        ui->label_image3->setPixmap(scaledPixmap3);
        ui->label_image3->setAlignment(Qt::AlignCenter);

        QPixmap pixmap_img4(picturePathList[pictureList[3].toInt()-1]);
        QPixmap scaledPixmap4 = pixmap_img4.scaled(labelSize, Qt::KeepAspectRatio);
        ui->label_image4->setPixmap(scaledPixmap4);
        ui->label_image4->setAlignment(Qt::AlignCenter);
    }
}

// 검사 중 뒤로가기
void Client::testBack()
{
    ui->stackedWidget->setCurrentIndex(5);
}

// 제출하기 버튼을 누를 시 감정분석 결과를 서버에 보내고 검토중 화면으로 이동
void Client::send_testData()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->backtoMain->setText("처음으로");

    // 서버에 감정검사 결과 데이터 전송
    QString data = "TESTDATA●" + login_id + "●"
                   + pictureList[0] + "●" + ui->textEdit_image1->toPlainText() + "●"
                   + pictureList[1] + "●" + ui->textEdit_image2->toPlainText() + "●"
                   + pictureList[2] + "●" + ui->textEdit_image3->toPlainText() + "●"
                   + pictureList[3] + "●" + ui->textEdit_image4->toPlainText() + "●";

    tcpSocket->write(data.toUtf8());

}

// 날짜별 결과 조회 페이지
void Client::viewResult()
{
    QString view_result = "VIEWRESULT●";
    tcpSocket->write(view_result.toUtf8());
}

// 그래프에 있는 별을 이동시키는 함수
void Client::moveStar(double x, double y)
{
    double move_x = 150+(x*150);
    double move_y = 1510-(y*150);
    ui->label_star->move(static_cast<int>(move_x),static_cast<int>(move_y));
}


void Client::BacktoMain()
{
    if (ui->backtoMain->text()=="처음으로")
    {
        ui->stackedWidget->setCurrentIndex(4);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(12);
    }

    pictureList = {};       // pictureList 초기화

    // 버튼 13개 border 초기화
    ui->pushButton_index1->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index2->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index3->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index4->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index5->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index6->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index7->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index8->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index9->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index10->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index11->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index12->setStyleSheet("border: 1px solid white;");
    ui->pushButton_index13->setStyleSheet("border: 1px solid white;");
    // textEdit 초기화
    ui->textEdit_image1->clear();
    ui->textEdit_image2->clear();
    ui->textEdit_image3->clear();
    ui->textEdit_image4->clear();
    // layout_4 초기화
    QLayoutItem* item;

    while ((item = ui->verticalLayout_4->takeAt(0)) != nullptr)
    {
        // 위젯을 삭제
        if (item->widget())
        {
            delete item->widget();
        }
        // 아이템을 삭제
        delete item;
    }

}

void Client::BacktoMain2()
{
    ui->stackedWidget->setCurrentIndex(4);

    // layout_4 초기화
    QLayoutItem* item;

    while ((item = ui->verticalLayout_4->takeAt(0)) != nullptr)
    {
        // 위젯을 삭제
        if (item->widget())
        {
            delete item->widget();
        }
        // 아이템을 삭제.
        delete item;
    }
}

//UI에 보여지는 버튼 모양 변경하는 함수
void Client::btnStyleSheet()
{
    // 첫 페이지 버튼
    ui->connectButton->setStyleSheet(        "QPushButton {"
                                     "    color: white;"
                                     "    border-radius: 20px;"
                                     "    border-style: solid;"
                                     "    border-color: white;"
                                     "    border-width: 2px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: #faa5f6;"
                                     "}");

    // 로그인 버튼
    ui->pushButton_login->setStyleSheet(        "QPushButton {"
                                        "    color: white;"
                                        "    border-radius: 2px;"
                                        "    border-style: solid;"
                                        "    border-color: white;"
                                        "    border-width: 1px;"
                                        "}"
                                        "QPushButton:hover {"
                                        "    background-color: #faa5f6;"
                                        "}");

    // 로그인페이지 내 회원가입, 회원정보 찾기 버튼
    ui->pushButton_sign->setStyleSheet(        "QPushButton {"
                                       "    color: white;"
                                       "    border-radius: 2px;"
                                       "    border-style: solid;"
                                       "    border-color: white;"
                                       "    border-width: 1px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "    background-color: #faa5f6;"
                                       "}");
    ui->pushButton_find->setStyleSheet(        "QPushButton {"
                                       "    color: white;"
                                       "    border-radius: 2px;"
                                       "    border-style: solid;"
                                       "    border-color: white;"
                                       "    border-width: 1px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "    background-color: #faa5f6;"
                                       "}");
    ui->lineEdit_loginId->setStyleSheet(        "QLineEdit {"
                                        "    color: grey;"
                                        "    border-radius: 7px;"
                                        "    border-style: solid;"
                                        "    border-color: white;"
                                        "    border-width: 1px;"
                                        "}");
    ui->lineEdit_loginPw->setStyleSheet(        "QLineEdit {"
                                        "    color: grey;"
                                        "    border-radius: 7px;"
                                        "    border-style: solid;"
                                        "    border-color: white;"
                                        "    border-width: 1px;"
                                        "}");

    // 회원가입페이지
    ui->back_login->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->Chk_id->setStyleSheet(        "QPushButton {"
                              "    color: white;"
                              "    border-radius: 5px;"
                              "    border-style: solid;"
                              "    border-color: white;"
                              "    border-width: 1px;"
                              "}"
                              "QPushButton:hover {"
                              "    background-color: #faa5f6;"
                              "}");
    ui->Chk_Phone->setStyleSheet(        "QPushButton {"
                                 "    color: white;"
                                 "    border-radius: 5px;"
                                 "    border-style: solid;"
                                 "    border-color: white;"
                                 "    border-width: 1px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: #faa5f6;"
                                 "}");
    ui->pushButton_signUp->setStyleSheet(        "QPushButton {"
                                         "    color: black;"
                                         "    border-radius: 7px;"
                                         "    border-style: solid;"
                                         "    border-color: white;"
                                         "    border-width: 2px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background-color: #faa5f6;"
                                         "}");

    // 회원정보찾기 페이지
    ui->back_login_2->setStyleSheet(        "QPushButton {"
                                    "    color: white;"
                                    "    border-radius: 7px;"
                                    "    border-style: solid;"
                                    "    border-color: white;"
                                    "    border-width: 1px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #faa5f6;"
                                    "}");
    ui->pushButton_findId->setStyleSheet(        "QPushButton {"
                                         "    color: white;"
                                         "    border-radius: 7px;"
                                         "    border-style: solid;"
                                         "    border-color: white;"
                                         "    border-width: 1px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background-color: #faa5f6;"
                                         "}");
    ui->pushButton_findPw->setStyleSheet(        "QPushButton {"
                                         "    color: white;"
                                         "    border-radius: 7px;"
                                         "    border-style: solid;"
                                         "    border-color: white;"
                                         "    border-width: 1px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background-color: #faa5f6;"
                                         "}");

    // 메인화면 페이지
    ui->back_login_3->setStyleSheet(        "QPushButton {"
                                    "    color: white;"
                                    "    border-radius: 7px;"
                                    "    border-style: solid;"
                                    "    border-color: white;"
                                    "    border-width: 1px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #faa5f6;"
                                    "}");
    ui->StartButton->setStyleSheet(        "QPushButton {"
                                   "    color: white;"
                                   "    border-radius: 7px;"
                                   "    border-style: solid;"
                                   "    border-color: white;"
                                   "    border-width: 1px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "    background-color: #faa5f6;"
                                   "}");
    ui->ResultButton->setStyleSheet(        "QPushButton {"
                                    "    color: white;"
                                    "    border-radius: 7px;"
                                    "    border-style: solid;"
                                    "    border-color: white;"
                                    "    border-width: 1px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #faa5f6;"
                                    "}");

    //뒤로가기 버튼들
    ui->back_login->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->back_login_2->setStyleSheet(        "QPushButton {"
                                    "    color: white;"
                                    "    border-radius: 7px;"
                                    "    border-style: solid;"
                                    "    border-color: white;"
                                    "    border-width: 1px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #faa5f6;"
                                    "}");
    ui->back_login_3->setStyleSheet(        "QPushButton {"
                                    "    color: white;"
                                    "    border-radius: 7px;"
                                    "    border-style: solid;"
                                    "    border-color: white;"
                                    "    border-width: 1px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #faa5f6;"
                                    "}");
    ui->backTest->setStyleSheet(        "QPushButton {"
                                "    color: white;"
                                "    border-radius: 7px;"
                                "    border-style: solid;"
                                "    border-color: white;"
                                "    border-width: 1px;"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: #faa5f6;"
                                "}");
    ui->backtoMain->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->testStart_backBtn->setStyleSheet(        "QPushButton {"
                                         "    color: white;"
                                         "    border-radius: 7px;"
                                         "    border-style: solid;"
                                         "    border-color: white;"
                                         "    border-width: 1px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background-color: #faa5f6;"
                                         "}");

    //이전, 다음, 제출하기 버튼 모양변경
    ui->btn_front2->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->btn_front3->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->btn_front4->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->btn_next1->setStyleSheet(        "QPushButton {"
                                 "    color: white;"
                                 "    border-radius: 7px;"
                                 "    border-style: solid;"
                                 "    border-color: white;"
                                 "    border-width: 1px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: #faa5f6;"
                                 "}");
    ui->btn_next2->setStyleSheet(        "QPushButton {"
                                 "    color: white;"
                                 "    border-radius: 7px;"
                                 "    border-style: solid;"
                                 "    border-color: white;"
                                 "    border-width: 1px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: #faa5f6;"
                                 "}");
    ui->btn_next3->setStyleSheet(        "QPushButton {"
                                 "    color: white;"
                                 "    border-radius: 7px;"
                                 "    border-style: solid;"
                                 "    border-color: white;"
                                 "    border-width: 1px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: #faa5f6;"
                                 "}");
    ui->btn_submit->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
    ui->testStart_backBtn->setStyleSheet(        "QPushButton {"
                                         "    color: white;"
                                         "    border-radius: 7px;"
                                         "    border-style: solid;"
                                         "    border-color: white;"
                                         "    border-width: 1px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background-color: #faa5f6;"
                                         "}");
    ui->backToMain_2->setStyleSheet(        "QPushButton {"
                                  "    color: white;"
                                  "    border-radius: 7px;"
                                  "    border-style: solid;"
                                  "    border-color: white;"
                                  "    border-width: 1px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: #faa5f6;"
                                  "}");
}


//빌드시 UI가 가운데로 띄워짐
void Client::centerOnScreen()
{
    // 현재 화면의 크기 정보 가져오기
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 창의 크기 정보 가져오기
    int windowWidth = width();
    int windowHeight = height();

    // 화면 중앙 위치 계산
    int x = (screenGeometry.width() - windowWidth) / 2;
    int y = (screenGeometry.height() - windowHeight) / 2;

    // 창의 위치 설정
    move(x, y);
}
void Client::disconnected()
{
    //    ui->textEdit->append("-> 서버로부터 연결 해제");
    qDebug() << Q_FUNC_INFO << "서버로부터 접속 종료.";
}
void Client::drawStarOnLabel(int x, int y, int size, const QColor& color) {

    QPixmap pixmap = ui->label_graph->pixmap(); // 현재 QLabel의 이미지를 가져옵니다.

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(color);
    painter.setBrush(color);

    int halfSize = size / 2;

    QPolygon starPolygon;
    for (int i = 0; i < 5; ++i) {
        int x1 = x + halfSize * cos(2 * M_PI * i / 5);
        int y1 = y + halfSize * sin(2 * M_PI * i / 5);
        int x2 = x + halfSize / 2 * cos(2 * M_PI * (i + 2) / 5);
        int y2 = y + halfSize / 2 * sin(2 * M_PI * (i + 2) / 5);
        starPolygon << QPoint(x1, y1) << QPoint(x2, y2);
    }

    painter.drawPolygon(starPolygon, Qt::WindingFill);
    ui->label_graph->setPixmap(pixmap); // QLabel에 이미지를 설정합니다.
}
void Client::findPath()
{

}


void Client::sendFile()
{
    // TCP 소켓이 유효한지 확인
    if (tcpSocket)
    {
        // TCP 소켓이 열려 있는지 확인
        if (tcpSocket->isOpen())
        {
            // 파일 선택을 위한 파일 대화상자 열기
            QString filePath = QFileDialog::getOpenFileName(this, ("첨부 파일 선택"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), ("파일 (*.json *.txt *.png *.jpg *.jpeg)"));

            // 사용자가 파일 선택을 취소했는지 확인
            if (filePath.isEmpty())
            {
                QMessageBox::critical(this, "QTCPClient", "첨부 파일을 선택하지 않았습니다!");
                return;
            }

            // 선택한 파일 열기
            QFile m_file(filePath);
            if (m_file.open(QIODevice::ReadOnly))
            {
                // 파일 정보 가져오기
                QFileInfo fileInfo(m_file.fileName());
                QString fileName(fileInfo.fileName());

                // 소켓을 통해 데이터를 전송하기 위한 데이터 스트림 생성
                QDataStream socketStream(tcpSocket);
                socketStream.setVersion(QDataStream::Qt_5_15);

                // 헤더 정보 생성
                QByteArray header;
                header.prepend(QString("fileType:attachment,fileName:%1,fileSize:%2;").arg(fileName).arg(m_file.size()).toUtf8());
                header.resize(128);

                // 파일의 모든 내용을 읽어와서 헤더와 함께 데이터 배열에 추가
                QByteArray byteArray = m_file.readAll();
                byteArray.prepend(header);

                // 데이터 스트림을 사용하여 데이터 배열을 소켓을 통해 전송
                socketStream << byteArray;
            }
            else
                QMessageBox::critical(this, "QTCPClient", "첨부 파일을 읽을 수 없습니다!");
        }
        else
            QMessageBox::critical(this, "QTCPClient", "소켓이 열려 있지 않아 보입니다.");
    }

    else
        QMessageBox::critical(this, "QTCPClient", "연결되어 있지 않습니다.");

}
