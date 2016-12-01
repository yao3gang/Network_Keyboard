#include <QMutex>
#include "page_manager.h"

static QWidget * page[PAGE_MAX] = {0};
static QMutex mutex;

int registerPage(PAGE_TYPE page_type, QWidget *ppage)
{
    QMutexLocker locker(&mutex);

    if (page_type <= PAGE_NONE || page_type >= PAGE_MAX)
    {
        return -EPARAM;
    }

    if (page[page_type])
    {
        ERR_PRINT("page(%d) != NULL\n", page_type);
        return -FAILURE;
    }

    page[page_type] = ppage;
    return SUCCESS;
}

QWidget * getPage(PAGE_TYPE page_type)
{
    QMutexLocker locker(&mutex);

    if (page_type <= PAGE_NONE || page_type >= PAGE_MAX)
    {
        return NULL;
    }

    return page[page_type];
}

