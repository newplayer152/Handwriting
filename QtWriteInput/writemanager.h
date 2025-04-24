#ifndef WRITEMANAGER_H
#define WRITEMANAGER_H

#include"writewidget.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>


// UWP includes
#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.Input.Inking.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <unordered_set>


//using namespace winrt;
//using namespace Windows::Foundation;
//using namespace Windows::UI::Input::Inking;
//using namespace Windows::Foundation::Numerics;

using  InkManager = winrt::Windows::UI::Input::Inking::InkManager;
using  InkRecognizerContainer=winrt::Windows::UI::Input::Inking::InkRecognizerContainer;
using  Point=winrt::Windows::Foundation::Point;
using  InkRecognitionTarget= winrt::Windows::UI::Input::Inking::InkRecognitionTarget;
using  InkPoint= winrt::Windows::UI::Input::Inking::InkPoint;
using  InkStrokeBuilder = winrt::Windows::UI::Input::Inking::InkStrokeBuilder;
using  InkStroke = winrt::Windows::UI::Input::Inking::InkStroke;
using  float3x2=winrt::Windows::Foundation::Numerics::float3x2;

class WriteManager : public QObject {
    Q_OBJECT
signals:
    void recognitionFinished(bool associate,const QStringList& result);
 
public:

    //初始化管理书写界面
    WriteManager(WriteWidget* writeWidget, QWidget* parent = nullptr);
    ~WriteManager();

    // 异步获取联想词方法
    void associateAsync(const QString& currentWord);

    //对词库更新
    void updateWordFrequency(const QString& word);

    //异步识别
    void recognizeInkAsync();

    //退一笔
    void backSpacing();

    //重置
    void clear();

protected:

    //保存墨迹并识别
    void upCurrentPathToInkManagerAndRecognize();

    //删除最后一笔
    void undoLastStroke(InkManager& inkManager);

    // 重置所有笔迹
    void resetInkManager(InkManager& inkManager);
    
    //初始化词库
    void initializeWordMap(const QString& filePath);

    //保存更新内容
    void saveUpdatedWordsToFile(const QString& filePath);

public:
    size_t m_minOverlopTime;
private:
    InkManager m_inkManager;//墨迹管理
    QString m_filePath;
    size_t m_movePx;//重叠输入墨迹偏移量
    QElapsedTimer m_divideTimer;//重叠输入定时器
    std::multimap<int, QString, std::greater<int>> m_wordMap; // 全局词库
    WriteWidget* m_writeWidget;//手写窗口
};

#endif // WRITEMANAGER_H
