#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <QWidget>

typedef enum
{
    PAGE_ALM,
    PAGE_CONFIG,
    PAGE_DEV_MGT,
    PAGE_MAIN,
    PAGE_PLAYBACK,
    PAGE_PREVIEW,
    PAGE_TVWALL,
    PAGE_MAX
} PAGE_TYPE;


//function  declaration
#ifdef __cplusplus
extern "C" {
#endif

int registerPage(PAGE_TYPE page_type, QWidget *ppage);
QWidget * getPage(PAGE_TYPE page_type);


#ifdef __cplusplus
}
#endif

#endif // PAGE_MANAGER_H
