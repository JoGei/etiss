//////////////////////////////////////////////////////////////////////////////////////
/// \file rvv-hl.h
/// \brief C/C++ Header for ETISS JIT libary or independent C application
/// \author J. Geier (johannes.geier@tum.de)
/// \date 06/23/2020
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
/// \brief Decode a VTYPE bitfield and store retrieved fields to Output parameter set
/// \return If field valid 1, else -1 (e.g. reserved LMUL code)
int8_t vtype_decode(uint16_t vtype, 	//!<[in] vtype bitfield
					uint8_t* ta, 		//!<[out] tail agnostic flag
					uint8_t* ma, 		//!<[out] mask agnostic flag
					uint32_t* sew, 		//!<[out] SEW (decoded) [bits]
					uint8_t* z_lmul, 	//!<[out] LMUL nominator
					uint8_t* n_lmul		//!<[out] LMUL denominator
				   );

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Encode Input parameter set of bitfields to a VTYPE bitfield
/// \return Encoded VTYPE bitfield
uint16_t vtype_encode(uint16_t sew, 	//!<[in] SEW (decoded) [bits]
					  uint8_t z_lmul, 	//!<[in] LMUL nominator
					  uint8_t n_lmul, 	//!<[in] LMUL denominator
					  uint8_t ta, 		//!<[in] tail agnostic flag
					  uint8_t ma		//!<[in] tail mask flag
					 );

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Extract SEW bitfield from VTYPE bitfield
/// \return Encoded SEW bitfield
uint8_t vtype_extractSEW(uint16_t pVTYPE //!<[in] vtype bitfield
						);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Extract LMUL bitfield from VTYPE bitfield
/// \return Encoded LMUL bitfield
uint8_t vtype_extractLMUL(uint16_t pVTYPE //!<[in] vtype bitfield
						);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Extract TA bitfield from VTYPE bitfield
/// \return Encoded TA bitfield
uint8_t vtype_extractTA(uint16_t pVTYPE //!<[in] vtype bitfield
						);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Extract MA bitfield from VTYPE bitfield
/// \return Encoded MA bitfield
uint8_t vtype_extractMA(uint16_t pVTYPE //!<[in] vtype bitfield
						);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Concatenate MEW and WIDTH to EEW and return number of bits for EEW
/// \return Decoded EEW [bits]
uint16_t vcfg_concatEEW(uint8_t mew, 	//!<[in] MEW bit
						uint8_t width);	//!<[in] WIDTH bits

/* Vector Loads/Stores Helpers*/
//////////////////////////////////////////////////////////////////////////////////////
/// \brief Load encoded (unitstride) from memory to target vector (-group)
/// \return 0 if no exception triggered, else 1
uint8_t vload_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Load encoded (strided) from memory to target vector (-group)
/// \return 0 if no exception triggered, else 1
uint8_t vload_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Load seqgmented (unitstride) from memory to target vector (-group)
/// \return 0 if no exception triggered, else 1
uint8_t vload_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Load encoded (strided) from memory to target vector (-group)
/// \return 0 if no exception triggered, else 1
uint8_t vload_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Store encoded (unitstride) source vector (-group) to memory
/// \return 0 if no exception triggered, else 1
uint8_t vstore_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Store encoded (strided) source vector (-group) to memory
/// \return 0 if no exception triggered, else 1
uint8_t vstore_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Store segmented (unitstride) source vector (-group) to memory
/// \return 0 if no exception triggered, else 1
uint8_t vstore_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
    uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief Store segmented (strided) source vector (-group) to memory
/// \return 0 if no exception triggered, else 1
uint8_t vstore_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pM,						//!<[inout] Local memory
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride);

/* Vector Arithmetic Helpers*/
//////////////////////////////////////////////////////////////////////////////////////
/// \brief ADD vector-vector
/// \return 0 if no exception triggered, else 1
uint8_t vadd_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief ADD vector-immediate
/// \return 0 if no exception triggered, else 1
uint8_t vadd_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief ADD vector-scalar
/// \return 0 if no exception triggered, else 1
uint8_t vadd_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pR,						//!<[in] Integer/General Purpose register field
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief AND vector-vector
/// \return 0 if no exception triggered, else 1
uint8_t vand_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief AND vector-immediate
/// \return 0 if no exception triggered, else 1
uint8_t vand_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief AND vector-scalar
/// \return 0 if no exception triggered, else 1
uint8_t vand_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pR,						//!<[in] Integer/General Purpose register field
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief OR vector-vector
/// \return 0 if no exception triggered, else 1
uint8_t vor_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief OR vector-immediate
/// \return 0 if no exception triggered, else 1
uint8_t vor_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief OR vector-scalar
/// \return 0 if no exception triggered, else 1
uint8_t vor_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pR,						//!<[in] Integer/General Purpose register field
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);


//////////////////////////////////////////////////////////////////////////////////////
/// \brief XOR vector-vector
/// \return 0 if no exception triggered, else 1
uint8_t vxor_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief XOR vector-immediate
/// \return 0 if no exception triggered, else 1
uint8_t vxor_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL);

//////////////////////////////////////////////////////////////////////////////////////
/// \brief XOR vector-scalar
/// \return 0 if no exception triggered, else 1
uint8_t vxor_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,				//!<[inout] Pointer to ETISS_CPU structure. Supports Vector register field
	ETISS_System* const pSystem,		//!<[inout] Pointer to ETISS_System structure. Supports databus/memory acesses
#else
	uint8_t* pV,						//!<[inout] Vector register field as local memory
	uint8_t* pR,						//!<[in] Integer/General Purpose register field
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __RVVHL_H__ */
