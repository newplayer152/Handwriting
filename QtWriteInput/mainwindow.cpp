#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    this->setFixedSize(800, 550);
    setStyleSheet(" background-color: #FFFFFF; "); // 白色背景

    //设置无标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 设置扩展样式，防止窗口激活
    HWND hwnd = reinterpret_cast<HWND>(winId());
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_TOPMOST);
    
    QFont font("微软雅黑", 12, QFont::Bold);
    m_ui->m_singleWidgetButton->setFont(font);
    m_ui->m_multiWidgetButton->setFont(font);
     // 自定义标题栏
    QWidget* titleBar = new QWidget(this);
    titleBar->setGeometry(QRect(0, 0, 800, 40));
    titleBar->setStyleSheet("background-color: #F78F4F;");


    // 自定义标题栏相关控件与样式
    m_btnClose = new QPushButton( titleBar);
    m_btnSettings = new QPushButton( titleBar);
    QLabel* titleLabel = new QLabel(u8"手写输入法", titleBar);
    QLabel* btnLabel = new QLabel(u8"|", titleBar);

    // 样式
    m_btnClose->setFlat(true);
    m_btnSettings->setFlat(true);
    titleLabel->setGeometry(QRect(325, 0, 100, 40));
    titleLabel->setStyleSheet("font-family: '微软雅黑';  color: #FFFFFF; font: 75 15pt; ");
    btnLabel->setFixedSize(30, 30);

    m_btnClose->setIcon(QIcon(":/images/close.png"));
    m_btnClose->setIconSize(QSize(40, 40));
    
    m_btnSettings->setIcon(QIcon(":/images/set.png"));
    m_btnSettings->setIconSize(QSize(30, 30));

    QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
    titleLayout->addWidget(m_btnSettings);
    titleLayout->addWidget(btnLabel);
    titleLayout->addWidget(m_btnClose);
    titleLayout->setContentsMargins(10, 0, 10, 0);

    // 设置布局靠右对齐
    titleLayout->setAlignment(Qt::AlignRight);
    titleBar->setLayout(titleLayout);


    //创建单字识别页面
    m_singleWordPage = new SingleWordWidget();
    m_ui->stackedWidget->addWidget(m_singleWordPage);

    //创建多字识别页面
    m_multiWordPage = new MultiWordWidget();
    m_ui->stackedWidget->addWidget(m_multiWordPage);

    // 创建手写窗口类 writeWidget 的实例并默认加入singleWordPage中，同时设置位置大小
    m_writeWidget = new WriteWidget(m_singleWordPage);
    m_writeWidget->setGeometry(0, 0, 440, 425); // (x, y, width, height)
    m_writeManager = new WriteManager(m_writeWidget,this);//交给后台管理者


    //初始化小键盘容器窗口 默认给单字输入窗口绑定
    m_smallKeyboardStackedWidget = new QStackedWidget(m_singleWordPage);
    m_smallKeyboardStackedWidget->setGeometry(35, 106, 370, 220);

    //初始化小键盘并添加入容器中
    m_symbolKeyboardWidget = new SmallKeyboardWidget({
        u8"，",u8"。",u8"、",u8"；",u8"：",u8"？",u8"！",u8"·",u8" …",
        u8"—",u8"～",u8"‘’",u8"“”",u8"/",u8"\\",u8"ˉ",u8"¨",u8"―",
        u8"ˇ", u8"∶",u8"﹕",u8"﹗",u8"｀",u8"＇",u8"〃",u8"＂",u8"々",
        u8"｜",u8"‖",u8"<>",u8"＜＞",u8"〈〉",u8"《》",u8"≮≯",u8"()",u8"（）",
        u8"〔〕",u8"［］",u8"〖〗",u8"【】",u8"「」",u8"『』",u8"﹝﹞",u8"{}",u8"｛｝",
        u8"︿﹀",u8"︽︾",u8"︹︺",u8"︵︶",u8"︻︼",u8"﹁﹂",u8"﹃﹄",u8"︷︸",u8""}); 
    m_numberKeyboardWidget = new SmallKeyboardWidget({
        u8"0",u8"1",u8"2",u8"3",u8"4",u8"5",u8"6",u8"7",u8"8",
        u8"9",u8"",u8"",u8"",u8"",u8"",u8"",u8"",u8"",
        u8"一", u8"二",u8"三",u8"四",u8"五",u8"六",u8"七",u8"八",u8"九",
        u8"十",u8"零",u8"",u8"",u8"",u8"",u8"",u8"",u8"",
        u8"壹",u8"贰",u8"叁",u8"肆",u8"伍",u8"陆",u8"柒",u8"捌",u8"玖",
        u8"拾",u8"佰",u8"仟",u8"万",u8"",u8"",u8"",u8"",u8"" });
    m_letterKeyboardWidget = new SmallKeyboardWidget({
        u8"a",u8"b",u8"c",u8"d",u8"e",u8"f",u8"g",u8"h",u8"i",
        u8"j",u8"k",u8"l",u8"m",u8"n",u8"o",u8"p",u8"q",u8"r",
        u8"s", u8"t",u8"u",u8"v",u8"w",u8"x",u8"y",u8"z",u8"",
        u8"A",u8"B",u8"C",u8"D",u8"E",u8"F",u8"G",u8"H",u8"I",
        u8"J",u8"K",u8"L",u8"M",u8"N",u8"O",u8"P",u8"Q",u8"R",
        u8"S",u8"T",u8"U",u8"V",u8"W",u8"X",u8"Y",u8"Z",u8"" });
    m_smallKeyboardStackedWidget->addWidget(m_symbolKeyboardWidget);
    m_smallKeyboardStackedWidget->addWidget(m_numberKeyboardWidget);
    m_smallKeyboardStackedWidget->addWidget(m_letterKeyboardWidget);
    m_smallKeyboardStackedWidget->hide();//默认隐藏
    
    //初始化设置界面
    m_settingWidget = new SettingWidget(this);

    //初始化后台信号连接控制器
    SignalController* signalController = new SignalController(this);
    //连接后台各个信号到处理函数
    signalController->connectControll(m_ui, m_singleWordPage, m_multiWordPage,
        m_writeWidget, m_writeManager, m_smallKeyboardStackedWidget,
        m_symbolKeyboardWidget, m_numberKeyboardWidget,
        m_letterKeyboardWidget, m_settingWidget, m_btnSettings
    );

    //初始化当前界面内连接信号
    currentWidgetConnect();
}

//初始化当前界面内连接信号
void MainWindow::currentWidgetConnect() {

    
    // 映射键盘类型到相应的 QWidget 简化代码
    static const QMap<QString, SmallKeyboardWidget*> keyboardMap = {
        {"symbol", m_symbolKeyboardWidget},
        {"number", m_numberKeyboardWidget},
        {"letter", m_letterKeyboardWidget}
    };

    //singleWordPage(单字界面) 连接切换键盘信号到结果显示小键盘槽函数
    connect(m_singleWordPage, &SingleWordWidget::keyboardSelected, [=](const QString& keyboardType) {

        if (keyboardType == "noKeyboard") {
            m_writeManager->clear();
            m_writeWidget->show();//当按下按钮时候出现
            m_smallKeyboardStackedWidget->hide();
            return;
        }

        m_writeManager->clear();
        m_smallKeyboardStackedWidget->setCurrentWidget(keyboardMap[keyboardType]);
        m_writeWidget->hide();//当按下按钮时候出现
        m_smallKeyboardStackedWidget->show();
        });

    // multiWordPage(多字界面)连接切换键盘信号到结果显示小键盘槽函数
    connect(m_multiWordPage, &MultiWordWidget::keyboardSelected, [=](const QString& keyboardType) {
        if (keyboardType == "noKeyboard") {
            m_writeManager->clear();
            m_writeWidget->show();//当按下按钮时候出现
            m_smallKeyboardStackedWidget->hide();
            return;
        }
        m_writeManager->clear();
        m_smallKeyboardStackedWidget->setCurrentWidget(keyboardMap[keyboardType]);
        m_writeWidget->hide();//当按下按钮时候出现
        m_smallKeyboardStackedWidget->show();
        });

    //切换单字输入与多字输入
    connect(m_ui->m_singleWidgetButton, &QPushButton::clicked, [=]() {
        m_writeManager->clear();
        m_writeWidget->setParent(m_singleWordPage);//把手写窗口传入新页面
        m_writeWidget->setGeometry(0, 0, 440, 425); //设置位置大小 (x, y, width, height)
        m_ui->stackedWidget->setCurrentWidget(m_singleWordPage); // 切换到单字手写区域
        //给单字输入窗口绑定
        m_smallKeyboardStackedWidget->setParent(m_singleWordPage);
        m_smallKeyboardStackedWidget->setGeometry(35, 106, 370, 220);
        m_smallKeyboardStackedWidget->hide();//默认隐藏
        //改变按钮样式
        m_ui->m_singleWidgetButton->setStyleSheet(" color: #FFFFFF;background-color: #F78F4F;");
        m_ui->m_multiWidgetButton->setStyleSheet("color: #000000; background-color: rgb(255, 255, 255)");
        });
    connect(m_ui->m_multiWidgetButton, &QPushButton::clicked, [=]() {
        m_writeManager->clear();
        m_writeWidget->setParent(m_multiWordPage);//把手写窗口传入新页面
        m_writeWidget->setGeometry(0, 55, 680, 370); //设置位置大小 (x, y, width, height)
        m_ui->stackedWidget->setCurrentWidget(m_multiWordPage); // 切换到多字手写区域
        //给多字输入窗口绑定
        m_smallKeyboardStackedWidget->setParent(m_multiWordPage);
        m_smallKeyboardStackedWidget->setGeometry(112, 106, 370, 220);
        m_smallKeyboardStackedWidget->hide();//默认隐藏
        //改变按钮样式
        m_ui->m_multiWidgetButton->setStyleSheet(" color: #FFFFFF;background-color: #F78F4F;");
        m_ui->m_singleWidgetButton->setStyleSheet("color: #000000; background-color: rgb(255, 255, 255);");
        });

    //连接关闭按钮关闭界面
    connect(m_btnClose, &QPushButton::clicked, this, &QWidget::close);

    //连接设置按钮到显示设置界面
    connect(m_btnSettings, &QPushButton::clicked, this, [=]() {//从当前各个类中获取最新状态并显示
        m_settingWidget->updataCurrentStatus();
        });
}

//浮动在上层
void MainWindow::showAt(const QPoint& pos) {
    move(pos);
    show();

    // 确保窗口浮在最上层
    HWND hwnd = reinterpret_cast<HWND>(winId());
    SetWindowPos(hwnd, HWND_TOPMOST, pos.x(), pos.y(), width(), height(),
        SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

// 实现拖动窗口功能
void MainWindow::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_drag = true;
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
    if (m_drag && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    m_drag = false;
}



MainWindow::~MainWindow()
{

}
