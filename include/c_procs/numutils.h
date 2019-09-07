#ifndef _NUMUTILS_H_
#define _NUMUTILS_H_

#include <stdint.h>

#define IS_INT8(_n_) (((_n_) >= INT8_MIN && (_n_) <= INT8_MAX))
#define IS_UINT8(_n_) (((_n_) >= 0 && (_n_) <= UINT8_MAX))

#define IS_INT16(_n_) (((_n_) >= INT16_MIN && (_n_) <= INT16_MAX))
#define IS_UINT16(_n_) (((_n_) >= 0 && (_n_) <= UINT16_MAX))

#define IS_INT32(_n_) (((_n_) >= INT32_MIN && (_n_) <= INT32_MAX))
#define IS_UINT32(_n_) (((_n_) >= 0 && (_n_) <= UINT32_MAX))

#endif /* _NUMUTILS_H_ */
