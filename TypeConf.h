#ifndef __TYPE_CONF_H__
#define __TYPE_CONF_H__


#if (defined(WIN32) || defined(_WIN64))
#include <windows.h>
#define WP_WINAPI __stdcall
#else
#define WP_WINAPI    
#endif

#if (defined(WIN32) || defined(_WIN64))

#ifdef WIN32
#define WP_LLONG LONG
#else //WIN64 
#define WP_LLONG INT64
#endif

#ifdef WIN32
#define WP_LPDWORD DWORD
#else //WIN64 
#define WP_LPDWORD INT64
#endif

#define WP_HMODULE   HMODULE
#define WP_LONG	     LONG
#define WP_WORD	     WORD
#define WP_DWORD     DWORD
#define WP_INT64     LLONG
#define WP_LDWORD	 LDWORD 
#define WP_BOOL	     BOOL
#define WP_TRUE	     TRUE
#define WP_FALSE     FALSE
#define WP_NULL	     NULL
#define WP_HDC		 HDC
#define WP_BYTE	     BYTE
#define WP_UINT	     UNIT
#define WP_HWND	     HWND
#define WP_LPVOID	 LPVOID
#define WP_HANDLE    HANDLE
#define WP_MAX_PATH  MAX_PATH 
typedef signed char            wp_Int8;
typedef unsigned char          wp_UInt8;
typedef signed short           wp_Int16;
typedef unsigned short         wp_UInt16;
typedef signed int             wp_Int32;
typedef unsigned int           wp_UInt32;
typedef signed __int64         wp_Int64;
typedef unsigned __int64       wp_UInt64;
#if defined(_WIN64)
typedef signed __int64     wp_IntPtr;
typedef unsigned __int64   wp_UIntPtr;
#else
typedef signed long        wp_IntPtr;
typedef unsigned long      wp_UIntPtr;
#endif
#else
typedef WP_HMODULE   void *
#define WP_DWORD     unsigned int 
#define WP_LONG	     long
#define WP_WORD	     unsigned short
#define WP_LPDWORD	 WP_DWORD*
#define WP_BOOL	     int
#define WP_TRUE	     1
#define WP_FALSE     0
#define WP_NULL	     0
#define WP_BYTE	     unsigned char
#define WP_UINT	     unsigned int
#define WP_HDC		 void*
#define WP_HWND	     void*
#define WP_LPVOID	 void*
#define WP_LLONG	 long
#define WP_INT64     long long
#define WP_LDWORD	 long 
#define WP_HANDLE    void *
#define WP_MAX_PATH  260       
#endif
#endif//__TYPE_CONF_H__