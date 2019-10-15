#ifndef _MEMUTILS_H_
#define _MEMUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAKEPTR(type,base,offs) ((type)((uintptr_t)(base)+(uintptr_t)(offs)))

#ifdef __cplusplus
}
#endif

#endif /* _MEMUTILS_H_ */
