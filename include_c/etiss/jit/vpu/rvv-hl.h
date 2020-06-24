//////////////////////////////////////////////////////////////////////////////////////
// \file rvv-hl.h
// \brief C/C++ Header for ETISS JIT libary or independent C application
// \author J. Geier (johannes.geier@tum.de)
// \date 06/23/2020
//////////////////////////////////////////////////////////////////////////////////////

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
//////////////////////////////////////////////////////////////////////////////////////
//\brief Decode a VTYPE bitfield and store retrieved fields to Output parameter set
int8_t vtype_decode(uint16_t vtype, uint8_t* ta, uint8_t* ma, uint32_t* sew, uint8_t* z_lmul, uint8_t* n_lmul);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Encode Input parameter set of bitfields to a VTYPE bitfield
uint16_t vtype_encode(uint16_t sew, uint8_t z_lmul, uint8_t n_lmul, uint8_t ta, uint8_t ma);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Extract SEW bitfield from VTYPE bitfield
uint8_t vtype_extractSEW(uint16_t pVTYPE);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Extract LMUL bitfield from VTYPE bitfield
uint8_t vtype_extractLMUL(uint16_t pVTYPE);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Extract TA bitfield from VTYPE bitfield
uint8_t vtype_extractTA(uint16_t pVTYPE);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Extract MA bitfield from VTYPE bitfield
uint8_t vtype_extractMA(uint16_t pVTYPE);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Concatenate MEW and WIDTH to EEW and return number of bits for EEW
uint16_t vcfg_concatEEW(uint8_t mew, uint8_t width);

/* Vector Loads/Stores Helpers*/
//////////////////////////////////////////////////////////////////////////////////////
//\brief Load encoded (unitstride) from memory to target vector (-group)
uint8_t vload_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Load encoded (strided) from memory to target vector (-group)
uint8_t vload_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Load seqgmented (unitstride) from memory to target vector (-group)
uint8_t vload_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Load encoded (strided) from memory to target vector (-group)
uint8_t vload_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Store encoded (unitstride) source vector (-group) to memory
uint8_t vstore_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Store encoded (strided) source vector (-group) to memory
uint8_t vstore_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Store segmented (unitstride) source vector (-group) to memory
uint8_t vstore_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
//\brief Store segmented (strided) source vector (-group) to memory
uint8_t vstore_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride);

/* Vector Arithmetic Helpers*/
//////////////////////////////////////////////////////////////////////////////////////
//\brief ADD vector-vector
uint8_t vadd_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief ADD vector-immediate
uint8_t vadd_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief ADD vector-scalar
uint8_t vadd_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pR,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);

//////////////////////////////////////////////////////////////////////////////////////
//\brief AND vector-vector
uint8_t vand_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief AND vector-immediate
uint8_t vand_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief AND vector-scalar
uint8_t vand_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pR,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);

//////////////////////////////////////////////////////////////////////////////////////
//\brief OR vector-vector
uint8_t vor_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief OR vector-immediate
uint8_t vor_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief OR vector-scalar
uint8_t vor_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pR,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);


//////////////////////////////////////////////////////////////////////////////////////
//\brief XOR vector-vector
uint8_t vxor_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief XOR vector-immediate
uint8_t vxor_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
//\brief XOR vector-scalar
uint8_t vxor_vx(
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
