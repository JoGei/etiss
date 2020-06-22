#ifndef __RVVHL_H__
#define __RVVHL_H__

#include "stdint.h"

#ifndef RVVHL_TESTBUILD
#define ETISS_BUILD
#endif
#ifdef ETISS_BUILD
struct ETISS_System;
struct ETISS_CPU;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* Vector Configuration Helpers*/
int8_t vtype_decode(uint16_t vtype, uint8_t* ta, uint8_t* ma, uint32_t* sew, uint8_t* z_lmul, uint8_t* n_lmul);
uint16_t vtype_encode(uint16_t sew, uint8_t z_lmul, uint8_t n_lmul, uint8_t ta, uint8_t ma);
uint8_t vtype_extractSEW(uint16_t pVTYPE);
uint8_t vtype_extractLMUL(uint16_t pVTYPE);
uint8_t vtype_extractTA(uint16_t pVTYPE);
uint8_t vtype_extractMA(uint16_t pVTYPE);
uint8_t vcfg_concatEEW(uint8_t mew, uint8_t width);
/* Vector Loads/Stores Helpers*/
uint8_t vload_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

uint8_t vload_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE);

uint8_t vload_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

uint8_t vload_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE);

uint8_t vstore_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

uint8_t vstore_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride);

uint8_t vstore_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

uint8_t vstore_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride);

uint8_t vadd_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

uint8_t vadd_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

uint8_t vadd_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pR,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __RVVHL_H__ */
