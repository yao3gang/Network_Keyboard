#include <QMutex>
#include "page_manager.h"

static QWidget * page[PAGE_MAX] = {0};
static QMutex mutex;

int registerPage(PAGE_TYPE page_type, QWidget *ppage)
{
    QMutexLocker locker(&mutex);

    if (page_type < 0 || page_type >= PAGE_MAX)
    {
        return -1;
    }

    page[page_type] = ppage;

    return 0;
}

QWidget * getPage(PAGE_TYPE page_type)
{
    QMutexLocker locker(&mutex);

    if (page_type < 0 || page_type >= PAGE_MAX)
    {
        return NULL;
    }

    return page[page_type];
}

