/*
 * rvv-hl.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: ge29noy
 */

#include "rvv-hl.h"
#include "rvv-hl-base.hpp"
#include "rvv-hl-lsu.hpp"
#include "rvv-hl-arithmetic.hpp"

#ifdef ETISS_BUILD
#include "etiss/jit/CPU.h"
#include "etiss/jit/System.h"
#ifndef ETISSCPU_SPEC
#include "<PLACE_HOLD_VCPUARCH>.h"
#define ETISSCPU_SPEC <PLACE_HOLD_VCPUARCH>
#else
#include "RV64IMACV.h"
#define ETISSCPU_SPEC RV64IMACV
#endif
#endif

extern "C"
{

int8_t vtype_decode(uint16_t vtype, uint8_t* ta, uint8_t* ma, uint32_t* sew, uint8_t* z_lmul, uint8_t* n_lmul){
	return (VTYPE::decode(vtype, ta , ma, sew, z_lmul, n_lmul));
}

uint16_t vtype_encode(uint16_t sew, uint8_t z_lmul, uint8_t n_lmul, uint8_t ta, uint8_t ma){
	return VTYPE::encode(sew, z_lmul, n_lmul, ta, ma);
}

uint8_t vtype_extractSEW(uint16_t pVTYPE)
{
	return VTYPE::extractSEW(pVTYPE);
}

uint8_t vtype_extractLMUL(uint16_t pVTYPE)
{
	return VTYPE::extractLMUL(pVTYPE);
}

uint8_t vtype_extractTA(uint16_t pVTYPE)
{
	return VTYPE::extractTA(pVTYPE);
}

uint8_t vtype_extractMA(uint16_t pVTYPE)
{
	return VTYPE::extractMA(pVTYPE);
}


uint8_t vcfg_concatEEW(uint8_t mew, uint8_t width){
	return (VTYPE::concatEEW(mew, width));
}

uint8_t vload_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*8) || (_z_emul > _n_emul*8)) return 1;

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)buff[i] = pM[addr];
    };
#endif
    VLSU::load_eew(f_readMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd, pMSTART, pVSTART, pVm, 0);
    return (0);
}

uint8_t vload_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t  pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*8) || (_z_emul > _n_emul*8)){
        return 1;
    }

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)buff[i] = pM[addr];
    };
#endif

    VLSU::load_eew(f_readMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd, pMSTART, pVSTART, pVm, pSTRIDE);

    return (0);
}

uint8_t vload_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*pNF*8) || (_z_emul*pNF > _n_emul*8)) return 1;
    if ((pVd + pNF*_z_emul/_n_emul) > 32) return 1;
    if (pVSTART >= pVL) return (0);

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)buff[i] = pM[addr];
    };
#endif
    uint16_t _vstart = pVSTART;
    uint64_t _moffset = pMSTART;
    for(int i = 0; i< pNF; ++i){
        VLSU::load_eew(f_readMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd + (i*_z_emul/_n_emul), _moffset, _vstart, pVm, 0);
        _moffset+= (pVL-_vstart)*pEEW/8;
        _vstart = 0;
    }
    return (0);
}

uint8_t vload_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pSTRIDE)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*pNF*8) || (_z_emul*pNF > _n_emul*8)) return 1;
    if ((pVd + pNF*_z_emul/_n_emul) > 32) return 1;
    if (pVSTART >= pVL) return (0);

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_readMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)buff[i] = pM[addr];
    };
#endif
    uint16_t _vstart = pVSTART;
    uint64_t _moffset = pMSTART;
    for(int i = 0; i< pNF; ++i){
        _moffset = pMSTART + i*pEEW/8;
        VLSU::load_eew(f_readMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd + (i*_z_emul/_n_emul), _moffset, _vstart, pVm, pSTRIDE);
        _vstart = 0;
    }
    return (0);
}

uint8_t vstore_encoded_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*8) || (_z_emul > _n_emul*8)) return 1;

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)pM[addr] = buff[i];
    };
#endif
    VLSU::store_eew(f_writeMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd, pMSTART, pVSTART, pVm, 0);

    return (0);
}

uint8_t vstore_encoded_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint16_t pEEW, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*8) || (_z_emul > _n_emul*8)) return 1;

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)pM[addr] = buff[i];
    };
#endif
    VLSU::store_eew(f_writeMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd, pMSTART, pVSTART, pVm, pStride);

    return (0);
}

uint8_t vstore_segment_unitstride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*pNF*8) || (_z_emul*pNF > _n_emul*8)) return 1;
    if ((pVd + pNF*_z_emul/_n_emul) > 32) return 1;
    if (pVSTART >= pVL) return (0);

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)pM[addr] = buff[i];
    };
#endif
    uint16_t _vstart = pVSTART;
    uint64_t _moffset = pMSTART;
    for(int i = 0; i< pNF; ++i){
        VLSU::store_eew(f_writeMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd + (i*_z_emul/_n_emul), _moffset, _vstart, pVm, 0);
        _moffset+= (pVL-_vstart)*pEEW/8;
        _vstart = 0;
    }
    return (0);
}

uint8_t vstore_segment_stride(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pM,
#endif
    uint16_t pVTYPE, uint8_t  pVm, uint16_t pEEW, uint8_t pNF, uint8_t pVd, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint64_t pMSTART, int16_t pStride)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint64_t _z_emul = pEEW*_vt._z_lmul;
    uint64_t _n_emul = _vt._sew*_vt._n_lmul;

    if ((_n_emul > _z_emul*pNF*8) || (_z_emul*pNF > _n_emul*8)) return 1;
    if ((pVd + pNF*_z_emul/_n_emul) > 32) return 1;
    if (pVSTART >= pVL) return (0);

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pSystem, pCpu](size_t addr, uint8_t* buff, size_t len) {
    	(*(pSystem->dread))(pSystem->handle, pCpu, addr, buff, len);
    };
#else
    uint8_t* VectorRegField = pV;
    std::function<void(size_t, uint8_t*, size_t)> f_writeMem = [pM](size_t addr, uint8_t* buff, size_t len) {
    	for (size_t i = 0; i<len; ++i)pM[addr] = buff[i];
    };
#endif
    uint16_t _vstart = pVSTART;
    uint64_t _moffset = pMSTART;
    for(int i = 0; i< pNF; ++i){
    	_moffset = pMSTART + i*pEEW/8;
        VLSU::store_eew(f_writeMem, VectorRegField, _z_emul, _n_emul, pEEW/8, pVL, pVLEN/8, pVd + (i*_z_emul/_n_emul), _moffset, _vstart, pVm, 0);
        _moffset+= (pVL-_vstart)*pEEW/8;
       _vstart = 0;
    }
    return (0);
}

/* Vector Arithmetic functions*/

uint8_t vadd_vv(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs1, uint8_t pVs2, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL)
{
    VTYPE::VTYPE _vt(pVTYPE);

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
#else
    uint8_t* VectorRegField = pV;
#endif

    VARITH::add_vv(VectorRegField, _vt._z_lmul, _vt._n_lmul, _vt._sew / 8, pVL, pVLEN / 8, pVd, pVs1, pVs2, pVSTART, pVm);

    return (0);
}

uint8_t vadd_vi(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pVimm, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL)
{
    VTYPE::VTYPE _vt(pVTYPE);

#ifdef ETISS_BUILD
    uint8_t* VectorRegField = ((ETISSCPU_SPEC*)pCpu)->V;
#else
    uint8_t* VectorRegField = pV;
#endif

    VARITH::add_vi(VectorRegField, _vt._z_lmul, _vt._n_lmul, _vt._sew / 8, pVL, pVLEN / 8, pVd, pVs2, pVimm, pVSTART, pVm);

    return (0);
}

uint8_t vadd_vx(
#ifdef ETISS_BUILD
	ETISS_CPU* const pCpu,
	ETISS_System* const pSystem,
#else
    uint8_t* pV,
	uint8_t* pR,
#endif
    uint16_t pVTYPE, uint8_t pVm, uint8_t pVd, uint8_t pVs2, uint8_t pRs1, uint16_t pVSTART, uint16_t pVLEN, uint16_t pVL, uint8_t pXLEN)
{
    VTYPE::VTYPE _vt(pVTYPE);
    uint8_t* ScalarReg;
    uint8_t* VectorRegField;

#ifdef ETISS_BUILD
    VectorRegField  = ((ETISSCPU_SPEC*)pCpu)->V;
    ScalarReg 		= (uint8_t*)((ETISSCPU_SPEC*)pCpu)->X[pRs1];
#else
    VectorRegField = pV;
    if(pXLEN <= 32) ScalarReg = &(pR[pRs1*4]);
    else ScalarReg = &(pR[pRs1*8]);
#endif

    VARITH::add_vx(VectorRegField, _vt._z_lmul, _vt._n_lmul, _vt._sew / 8, pVL, pVLEN / 8, pVd, pVs2, ScalarReg, pVSTART, pVm, pXLEN/8);

    return (0);
}

} // extern "C"
