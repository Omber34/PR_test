//
// Created by Herman on 4/16/2022.
//

#ifndef PR_TEST_QOBJECTTESTHELPER_H
#define PR_TEST_QOBJECTTESTHELPER_H

#include <QObject>

class QObjectTestHelper : public QObject {
    Q_OBJECT
public slots:
    void testSlot(QString name) {
        counters[name]++;
    }

    std::map<QString, uint32_t> counters;

};


#endif //PR_TEST_QOBJECTTESTHELPER_H
