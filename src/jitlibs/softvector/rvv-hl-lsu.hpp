#ifndef __RVVHL_VLSU_H__
#define __RVVHL_VLSU_H__

#include "stdint.h"
#include <functional>

namespace VLSU {

	void load_eew(
			std::function<void(size_t, uint8_t*, size_t)> readMem,
			uint8_t* _V,
		    uint64_t _z_emul,
		    uint64_t _n_emul,
		    uint16_t _eewB,
		    uint16_t _VL,
		    uint16_t _VLENB,
		    uint16_t _vd,
		    uint64_t _memStart,
		    uint16_t _vstart,
		    uint8_t  _Vm,
			int16_t  _stride
		);

	void store_eew(
			std::function<void(size_t, uint8_t*, size_t)> writeMem,
			uint8_t* _V,
		    uint64_t _z_emul,
		    uint64_t _n_emul,
		    uint16_t _eewB,
		    uint16_t _VL,
		    uint16_t _VLENB,
		    uint16_t _vd,
		    uint64_t _memStart,
		    uint16_t _vstart,
		    uint8_t  _Vm,
			int16_t  _stride
		);

}


#endif /* __RVVHL_VLSU_H__ */
