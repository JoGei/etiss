#ifndef __RVVHL_BASE_H__
#define __RVVHL_BASE_H__

#include "stdint.h"

namespace VTYPE {

typedef enum MASK{
	MSKSEW   = 0x1C,
	MSKLMUL  = 0x03,
	MSKFLMUL = 0x20,
	MSKTA 	 = 0x40,
	MSKMA 	 = 0x80,
}mask_t;

typedef enum OFFSETS{
	OFFSEW   = 2,
	OFFFLMUL = 3
}offsets_t;

typedef enum BITS_SEW{
	E8 		= 0x0,
	E16 	= 0x1,
	E32 	= 0x2,
	E64 	= 0x3,
	E128 	= 0x4,
	E256 	= 0x5,
	E512 	= 0x6,
	E1024 	= 0x7
}bits_sew_t;
typedef enum BITS_LMUL{
	RES		= 0x4,
	MF8		= 0x5,
	MF4		= 0x6,
	MF2		= 0x7,
	M1 		= 0x0,
	M2	 	= 0x1,
	M4	 	= 0x2,
	M8	 	= 0x3
}bits_LMUL_t;
typedef enum BITS_TA{
	TAU		= 0x0,
	TAGN	= 0x1
}bits_TA_t;
typedef enum BITS_MA{
	MAU		= 0x0,
	MAGN	= 0x1
}bits_MA_t;

typedef enum BITS_EEW{
	EEW_8 	= 0x0,
	EEW_16	= 0x5,
	EEW_32  = 0x6,
	EEW_64  = 0x7,
	EEW_128 = 0x8,
	EEW_256 = 0xd,
	EEW_512 = 0xe,
	EEW_1024= 0xf
} bits_eew_t;

	int8_t decode(uint16_t vtype, uint8_t* ta, uint8_t* ma, uint32_t* sew, uint8_t* z_lmul, uint8_t* n_lmul);

	uint16_t encode(uint16_t sew, uint8_t z_lmul, uint8_t n_lmul, uint8_t ta, uint8_t ma);

	uint8_t extractSEW(uint16_t pVTYPE);

	uint8_t extractLMUL(uint16_t pVTYPE);

	uint8_t extractTA(uint16_t pVTYPE);

	uint8_t extractMA(uint16_t pVTYPE);

	uint16_t concatEEW(uint8_t mew, uint8_t width);

	class VTYPE{
	public:
		uint16_t _bitfield{};
	    uint8_t _z_lmul{}, _n_lmul{}, _ta{}, _ma{};
	    uint32_t _sew{};
	    VTYPE(uint16_t _vtype_bitfield): _bitfield(_vtype_bitfield){
	    	decode(_bitfield, &_ta, &_ma, &_sew, &_z_lmul, &_n_lmul);
	    }
	    VTYPE(uint16_t sew, uint8_t z_lmul, uint8_t n_lmul, uint8_t ta, uint8_t ma) :
	    _z_lmul(z_lmul), _n_lmul(n_lmul), _ta(ta), _ma(ma), _sew(sew){
	    	_bitfield = encode(_sew, _z_lmul, _n_lmul, _ta, _ma);
	    }
	};
}


#endif /* __RVVHL_BASE_H__ */
