#ifndef  _CONFIG_H
#define  _CONFIG_H

#define  ENABLE_AUTO_FILE_LOG
#include "z_log.h"



// # ifdef	_DEBUG
// 	#	define	PROTECT(s)	{	static	TCHAR *__FUNC_NAME__	= TEXT(#s);
// 	#	define	UNPROTECT	}
// # else
	#	define	PROTECT(s)	{		\
		                        try	\
								{	\
	                               static	TCHAR *__FUNC_NAME__= TEXT(#s);

	#	define	UNPROTECT		}			\
		catch(...)	\
								{			\
								    START_LOG;  \
								    LOG <<  "\r\n  *********�쳣:"  << __FUNC_NAME__; \
									END_LOG;  \
								}			\
							}
// # endif

#endif