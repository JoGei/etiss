#ifndef __RVVHL_ARITH_H__
#define __RVVHL_ARITH_H__

#include "stdint.h"
#include <functional>

namespace VARITH {

	void add_vv(
		uint8_t* _V,
		uint64_t _z_mul,
		uint64_t _n_mul,
		uint16_t _sewB,
		uint16_t _VL,
		uint16_t _VLENB,
		uint16_t _vd,
		uint16_t _vs1,
		uint16_t _vs2,
		uint16_t _vstart,
		uint8_t  _Vm
	);

	void add_vi(
		uint8_t* _V,
		uint64_t _z_mul,
		uint64_t _n_mul,
		uint16_t _sewB,
		uint16_t _VL,
		uint16_t _VLENB,
		uint16_t _vd,
		uint16_t _vs1,
		uint8_t  _vimm,
		uint16_t _vstart,
		uint8_t  _Vm
	);

	void add_vx(
		uint8_t* _V,
		uint64_t _z_mul,
		uint64_t _n_mul,
		uint16_t _sewB,
		uint16_t _VL,
		uint16_t _VLENB,
		uint16_t _vd,
		uint16_t _vs1,
		uint8_t*  _rs,
		uint16_t _vstart,
		uint8_t  _Vm,
		uint8_t  _xlenB
	);

}


#endif /* __RVVHL_ARITH_H__ */
