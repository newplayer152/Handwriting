#include"writemanager.h"


WriteManager::WriteManager(WriteWidget* writeWidget ,QWidget* parent) :QObject(parent),
m_movePx(0), m_minOverlopTime(800), m_filePath("userLexicon.txt"), m_divideTimer(), m_writeWidget(writeWidget) {
        //初始化词库
        initializeWordMap(m_filePath);
        // 连接upMouseRelease鼠标离开信号到存储墨迹并识别槽函数
        connect(writeWidget, &WriteWidget::upMouseRelease, this, [=](const bool& upPath) {
            if (upPath) {
                upCurrentPathToInkManagerAndRecognize();
            }
        });
    }
WriteManager::~WriteManager() {
        saveUpdatedWordsToFile("userLexicon.txt");
    };

// 异步获取联想词方法
void WriteManager::associateAsync(const QString& currentWord) {
    // 启动异步任务
    QtConcurrent::run([=]() {
        QStringList recognizedWords;

        //  从内存中的 wordMap 查询匹配字段
        for (const auto& [frequency, word] : m_wordMap) {
            if (word.startsWith(currentWord)) {
                recognizedWords.append(word.mid(currentWord.size()));//显示时排除已选字
                if (recognizedWords.size() >= 8) break; // 限制最多 8 个结果
            }
        }

        //  发射信号
        emit recognitionFinished(true, recognizedWords);
        });
}

//对词库更新
void WriteManager::updateWordFrequency(const QString& word) {
    // 查找是否已有词条
    for (auto it = m_wordMap.begin(); it != m_wordMap.end(); ++it) {
        if (it->second == word) {
            int newFrequency = it->first + 1; // 增加频率
            m_wordMap.erase(it); // 删除旧词条
            m_wordMap.emplace(newFrequency, word); // 插入更新后的词条
            qDebug() << "Succeed update WordMap !";
            return;
        }
    }
    // 如果词不存在，则新增
    m_wordMap.emplace(1, word); // 初始频率为 1

    qDebug() << "Succeed add WordMap !";
}

void WriteManager::recognizeInkAsync() {
    if (m_inkManager.GetStrokes().Size() == 0) {
        emit recognitionFinished(false, { u8"你", u8"我", u8"他", u8"是", u8"了", u8"就", u8"在",
            u8"的", u8"都", u8"什么", u8"呵呵", u8"我们", u8"好的" });
        return;
    }
    try {
        // 检查是否有可用的识别器
        InkRecognizerContainer recognizerContainer;
        auto recognizers = recognizerContainer.GetRecognizers();
        //for (const auto& recognizer : recognizers) {//查看识别器种类
        //    qDebug() << u8"Recognizer Name: " << QString::fromStdWString(recognizer.Name().c_str());
        //}
        /*if (recognizerContainer.GetRecognizers().Size() == 0) {
            emit recognitionFinished({ "No recognizers available." });
            return;
        }*/

        // 使用 QtConcurrent 异步运行 RecognizeAsync
        QFuture<QStringList> future = QtConcurrent::run([=]() -> QStringList {
            QStringList recognizedWords;
            try {
                auto results = m_inkManager.RecognizeAsync(InkRecognitionTarget::All).get();
                if (results.Size() == 0) { return recognizedWords;
                    /*recognizedWords<< "No handwriting recognized.";*/
                }
                for (const auto& result : results) {
                    auto candidates = result.GetTextCandidates();
                    for (const auto& candidate : candidates) {
                        recognizedWords << QString::fromStdWString(candidate.c_str());
                    }
                }
                return recognizedWords;
            }
            catch (const winrt::hresult_error& e) {
                recognizedWords << "Recognition failed: " + QString::fromStdWString(e.message().c_str());
                return recognizedWords;
            }
            });

        // 使用 QFutureWatcher 监听异步结果
        QFutureWatcher<QStringList>* watcher = new QFutureWatcher<QStringList>(this);
        connect(watcher, &QFutureWatcher<QStringList>::finished, this, [=]() {
            QStringList result = watcher->result();
            emit recognitionFinished(false, result);  // 通过信号通知结果
            watcher->deleteLater();
            });

        watcher->setFuture(future);

    }
    catch (const winrt::hresult_error& e) {
        emit recognitionFinished(false, { "Recognition failed: " + QString::fromStdWString(e.message().c_str()) });
    }
}

void WriteManager::backSpacing() {
    if (!m_writeWidget->m_paths.empty()) {
        m_writeWidget->m_paths.pop_back();
        m_writeWidget->update();
        undoLastStroke(m_inkManager);
        recognizeInkAsync();
    }

}

void WriteManager::clear() {
    if (!m_writeWidget->m_paths.empty()) {
        m_movePx = 0;
        m_divideTimer = QElapsedTimer();// 模拟“关闭”

        m_writeWidget->m_paths.clear();
        m_writeWidget->update();
        resetInkManager(m_inkManager);
        recognizeInkAsync();
    }
}


//保存墨迹并识别
void WriteManager::upCurrentPathToInkManagerAndRecognize() {
    if (m_writeWidget->m_overlapFlag && m_divideTimer.isValid()) {
        if (m_divideTimer.elapsed() > m_minOverlopTime) { m_movePx = min(m_movePx + 200, 500); }//最多5个字

    }
    // Create InkPoints from currentPath
    std::vector<InkPoint> inkPoints;
    for (const auto& qPoint : m_writeWidget->m_currentPath) {
        InkPoint inkPoint(
            Point(static_cast<float>(qPoint.x()) + m_movePx, static_cast<float>(qPoint.y())),
            0.5f // default pressure
        );
        inkPoints.push_back(inkPoint);
    }

    // Create an IVector<InkPoint>
    auto inkPointVector = winrt::single_threaded_vector<InkPoint>();
    for (const auto& inkPoint : inkPoints) {
        inkPointVector.Append(inkPoint);
    }

    // Create InkStroke from inkPoints
    InkStrokeBuilder inkStrokeBuilder;
    InkStroke stroke = inkStrokeBuilder.CreateStrokeFromInkPoints(
        inkPointVector,
        float3x2::identity()
    );

    // Add the stroke to the InkManager
    m_inkManager.AddStroke(stroke);

    recognizeInkAsync();
    if (m_writeWidget->m_overlapFlag)m_divideTimer.restart();

}

//删除最后一笔
void WriteManager::undoLastStroke(InkManager& inkManager) {
    auto strokes = inkManager.GetStrokes();
    if (strokes.Size() > 0) {
        auto lastStroke = strokes.GetAt(strokes.Size() - 1);
        lastStroke.Selected(true); // 选中最后一笔
        inkManager.DeleteSelected(); // 删除选中的笔迹
    }
}

// 重置所有笔迹
void WriteManager::resetInkManager(InkManager& inkManager) {
    size_t cout = inkManager.GetStrokes().Size();
    while (cout--) {
        undoLastStroke(inkManager);
    }
}

//初始化词库
void WriteManager::initializeWordMap(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        // 设置编码为 UTF-8
        stream.setCodec("UTF-8");
        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            QStringList parts = line.split(':');
            if (parts.size() == 2) {
                QString word = parts[0];
                int frequency = parts[1].toInt();
                m_wordMap.emplace(frequency, word);
            }
        }
        file.close();
        qDebug() << "WordMap initialized with" << m_wordMap.size() << "entries.";
    }
    else {
        qDebug() << "Failed to open file for initialization:" << filePath;
    }
}

//保存更新内容
void WriteManager::saveUpdatedWordsToFile(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8"); // 设置输出为 UTF-8 编码

        size_t count = 0; // 计数器
        for (const auto& word : m_wordMap) {
            if (count >= 50000) break; // 超过 50000 次时退出循环
            stream << word.second << ":" << word.first << "\n";
            ++count;
        }
        file.close();
        qDebug() << "Updated words saved to file.";
    }
    else {
        qDebug() << "Failed to open file for saving:" << filePath;
    }
}

