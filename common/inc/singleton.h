

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

/////////////////////////////////////////////////
////////////////单件模式
#define PATTERN_SINGLETON_DECLARE(classname)	\
static classname * instance();

#define PATTERN_SINGLETON_IMPLEMENT(classname)	\
classname * classname::instance()		\
{												\
	static classname * _instance = NULL;		\
	if( NULL == _instance)						\
	{											\
		_instance = new classname;				\
	}											\
	return _instance;							\
}

#endif

