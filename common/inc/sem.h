#ifndef __SEMAPHORE_API_H__
#define __SEMAPHORE_API_H__

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup SemaphoreAPI API Semaphore
/// D?o?¨¢?API¡迡¤a¡a㨢?2?¨ª?2¨´¡a?¦̨ª3¦ʿD?o?¨¢????¨®?¡舯//	\n ¦̡¨®?¨¢¡3¨¬¨ª?:
/// \code
///    ================================
///                |
///         SemaphoreCreate
///                |------------------+
///    SemaphorePost,SemaphorePend    |
///                |------------------+
///        SemaphoreDestory
///                |
///    ================================
/// \endcode
/// @{


/// ¡䢤?¡§D?o?¨¢??¡舯// 
/// \param [out] phSemaphore D?o?¨¢?¡䢤?¡§3¨¦1|¨º¡!迡⽳?¦ʿD?o?¨¢???¡(²?¡舯// \param [in] dwInitialCount D?o?¨¢???¨ºy¦ʿ3?¨º??¦ʿ¡舯// \retval 0  ¡䢤?¡§3¨¦1|
/// \retval <0  ¡䢤?¡§¨º¡좣¨¹
int SemaphoreCreate(VD_HANDLE *phSemaphore, uint dwInitialCount);


/// ?¨²?¨´D?o?¨¢??¡舯// 
/// \param [in] hSemaphore D?o?¨¢???¡(²?¡舯// \retval 0  ?¨²?¨´3¨¦1|
/// \retval <0 ?¨²?¨´¨º¡좣¨¹
int SemaphoreDestory(VD_HANDLE hSemaphore);


/// ??¨¦¨´D?o?¨¢???¨ºy?¡舯// 
/// \param [in] hSemaphore D?o?¨¢???¡(²?¡舯// \retval 0  2¨´¡a3¨¦1|
/// \retval <0  2¨´¡a¨º¡좣¨¹
int SemaphorePend(VD_HANDLE hSemaphore);
int SemaphoreTryPend(HANDLE semId);
int SemaphoreTimedPend(HANDLE semId, u32 seconds);


/// ???¨®D?o?¨¢???¨ºy?¡舯// 
/// \param [in] hSemaphore D?o?¨¢???¡(²?¡舯// \retval 0  2¨´¡a3¨¦1|
/// \retval <0  2¨´¡a¨º¡좣¨¹
int SemaphorePost(VD_HANDLE hSemaphore);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__SEMAPHORE_API_H__
