#include <QtCore/QPointer>
#include <QtTest>

#include "eventstorage.h"

namespace Tests {

class UtEventStorage : public QObject
{
    Q_OBJECT

public:
    UtEventStorage();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void addItem1();
    void testItem1Properties_data();
    void testItem1Properties();
    void addItem2();
    void testItem2Properties_data();
    void testItem2Properties();
    void updateItem2();
    void testItem2UpdatedProperties_data();
    void testItem2UpdatedProperties();
    void removeItem1();
    void removeItemsBySourceName();
    void purgeOutdatedItems();

private:
    void populateTestData(const QVariantMap &properties);
    static QVariant readProperty(const Event &event, const QString &property);

private:
    QPointer<EventStorage> m_storage;
    QVariantMap m_parameters1;
    QVariantMap m_parameters2;
    QVariantMap m_parameters2Updated;
    qlonglong m_id1;
    qlonglong m_id2;
};

} // namespace Tests

using namespace Tests;

UtEventStorage::UtEventStorage()
    : m_id1(0),
      m_id2(0)
{
}

void UtEventStorage::initTestCase()
{
    m_storage = new EventStorage(this);
    m_storage->open();
    QVERIFY(m_storage->getAllItems().isEmpty());
}

void UtEventStorage::cleanupTestCase()
{
    delete m_storage;
}

void UtEventStorage::addItem1()
{
    m_parameters1["title"] = "title1";
    m_parameters1["body"] = "body1";
    m_parameters1["timestamp"] = "2013-01-01 01:23:45";
    m_parameters1["footer"] = "footer1";
    m_parameters1["url"] = "http://foo1.example.com";
    m_parameters1["sourceName"] = "source1";
    m_parameters1["sourceDisplayName"] = "Source #1";
    m_parameters1["icon"] = "icon1";
    m_parameters1["imageList"] = QStringList() << "image11" << "image12";
    m_parameters1["testKey"] = "testValue";

    m_id1 = m_storage->addItem(m_parameters1);

    QVERIFY(m_id1 != -1);

    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(events.count(), 1);
    QCOMPARE(events.at(0)->id(), m_id1);
    qDeleteAll(events);
}

void UtEventStorage::testItem1Properties_data()
{
    populateTestData(m_parameters1);
}

void UtEventStorage::testItem1Properties()
{
    QFETCH(QVariant, expected);
    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(readProperty(*events.at(0), QTest::currentDataTag()), expected);
    qDeleteAll(events);
}

void UtEventStorage::addItem2()
{
    m_parameters2["title"] = "title2";
    m_parameters2["body"] = "body2";
    m_parameters2["timestamp"] = "2013-02-02 02:34:56";
    m_parameters2["footer"] = "footer2";
    m_parameters2["url"] = "http://foo2.example.com";
    m_parameters2["sourceName"] = "source2";
    m_parameters2["sourceDisplayName"] = "Source #2";
    m_parameters2["icon"] = "icon2";
    m_parameters2["imageList"] = QStringList() << "image21" << "image22";
    m_parameters2["video"] = true;
    m_parameters2["testKey"] = "testValue";

    m_id2 = m_storage->addItem(m_parameters2);

    QVERIFY(m_id2 != -1);

    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(events.count(), 2);
    QCOMPARE(events.at(0)->id(), m_id2);
    qDeleteAll(events);
}

void UtEventStorage::testItem2Properties_data()
{
    populateTestData(m_parameters2);
}

void UtEventStorage::testItem2Properties()
{
    QFETCH(QVariant, expected);
    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(readProperty(*events.at(0), QTest::currentDataTag()), expected);
    qDeleteAll(events);
}

void UtEventStorage::updateItem2()
{
    m_parameters2Updated["title"] = "title2u";
    m_parameters2Updated["body"] = "body2u";
    m_parameters2Updated["timestamp"] = "2013-03-03 03:43:21";
    m_parameters2Updated["footer"] = "footer2u";
    m_parameters2Updated["url"] = "http://foo2u.example.com";
    m_parameters2Updated["sourceName"] = "source2u";
    m_parameters2Updated["sourceDisplayName"] = "Source #2u";
    m_parameters2Updated["icon"] = "icon2u";
    // intentionally lower the number of associated images!
    m_parameters2Updated["imageList"] = QStringList() << "image21u" /*<< "image22u"*/;
    m_parameters2Updated["video"] = false;
    m_parameters2Updated["testKey"] = "testValueu";

    m_storage->updateItem(m_id2, m_parameters2Updated);

    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(events.count(), 2);
    QCOMPARE(events.at(0)->id(), m_id2);
    qDeleteAll(events);
}

void UtEventStorage::testItem2UpdatedProperties_data()
{
    populateTestData(m_parameters2Updated);
}

void UtEventStorage::testItem2UpdatedProperties()
{
    QFETCH(QVariant, expected);
    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(readProperty(*events.at(0), QTest::currentDataTag()), expected);
    qDeleteAll(events);
}

void UtEventStorage::removeItem1()
{
    QVERIFY(m_storage->removeItem(m_id1));
    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(events.count(), 1);
    QCOMPARE(events.at(0)->id(), m_id2);
    QCOMPARE(events.at(0)->title(), m_parameters2Updated["title"].toString());
    qDeleteAll(events);
}

void UtEventStorage::removeItemsBySourceName()
{
    const QList<qlonglong> removed =
        m_storage->removeItemsBySourceName(m_parameters2Updated["sourceName"].toString());
    QCOMPARE(removed.count(), 1);
    QCOMPARE(removed.at(0), m_id2);
    const QList<Event *> events = m_storage->getAllItems();
    QCOMPARE(events.count(), 0);
    qDeleteAll(events);
}

Q_DECLARE_METATYPE(QList<qlonglong>)

void UtEventStorage::purgeOutdatedItems()
{
    QVariantMap parameters;
    parameters["title"] = "title";
    parameters["body"] = "body";
    parameters["timestamp"] = "2013-01-01 01:23:45";
    parameters["footer"] = "footer";
    parameters["url"] = "http://foo.example.com";
    parameters["sourceName"] = "source";
    parameters["sourceDisplayName"] = "Source";
    parameters["icon"] = "icon";
    parameters["imageList"] = QStringList() << "image1" << "image2";
    parameters["video"] = false;
    parameters["testKey"] = "testValue";

    qRegisterMetaType<QList<qlonglong> >();
    QSignalSpy spy(m_storage, SIGNAL(itemsOutdated(QList<qlonglong>)));

    for (int i = 0; i < MAX_EVENT_ITEMS + EVENT_COUNT_HYSTERESIS * 2; ++i) {
        parameters["title"] = QString("title %1").arg(i);
        m_storage->addItem(parameters);
    }

    const QList<Event *> events = m_storage->getAllItems();
    QVERIFY(events.count() <= MAX_EVENT_ITEMS);
    qDeleteAll(events);

    QVERIFY(spy.count() > 0);
}

void UtEventStorage::populateTestData(const QVariantMap &properties)
{
    QTest::addColumn<QVariant>("expected");

    QMapIterator<QString, QVariant> it(properties);
    while (it.hasNext()) {
        it.next();
        QTest::newRow(qPrintable(it.key())) << it.value();
    }
}

QVariant UtEventStorage::readProperty(const Event &event, const QString &property)
{
    if (property == "title") {
        return event.title();
    } else if (property == "body") {
        return event.body();
    } else if (property == "timestamp") {
        return event.timestamp();
    } else if (property == "footer") {
        return event.footer();
    } else if (property == "url") {
        return event.url();
    } else if (property == "sourceName") {
        return event.sourceName();
    } else if (property == "sourceDisplayName") {
        return event.sourceDisplayName();
    } else if (property == "icon") {
        return event.icon();
    } else if (property == "imageList") {
        return event.imageList();
    } else if (property == "video") {
        return event.isVideo();
    } else {
        return event.metaData().value(property);
    }
}

QTEST_MAIN(Tests::UtEventStorage)

#include "ut_eventstorage.moc"
