/*
 * rvv-hl-lsu.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: ge29noy
 */

#include "rvv-hl-lsu.hpp"
#include "rvv-hl-base.hpp"

void VLSU::load_eew(
		//function for mem access readMem(address, buffer, length)
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
){
	int _iMemOffset = 0;
	int _iReg = _vd - 1;
	int _nElementsPerReg = (_z_emul >= _n_emul) ? (_VLENB/_eewB) : (((uint64_t)_VLENB)*_z_emul/_eewB/_n_emul);
	for(int _iElement = _vstart; _iElement < _VL; ++_iElement){
		int _iRegEl = _iElement % _nElementsPerReg;
		if(_iRegEl == 0){
			++_iReg;
		}
		if(_Vm or (_V[_iElement/8]&(1<<_iElement%8))){
			for (int _iElementByte = 0; _iElementByte < _eewB; ++_iElementByte){
				uint8_t x;
				readMem(_memStart + _iMemOffset, &x, 1);
				_V[_iReg*_VLENB + _iRegEl*_eewB + _iElementByte] = x;
				++_iMemOffset;
			}
		}else{
			_iMemOffset+=_eewB;
		}
		_iMemOffset += _stride;
	}
}

void VLSU::store_eew(
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
){
	int _iMemOffset = 0;
	int _iReg = _vd - 1;
	int _nElementsPerReg = (_z_emul >= _n_emul) ? (_VLENB/_eewB) : (((uint64_t)_VLENB)*_z_emul/_eewB/_n_emul);
	for(int _iElement = _vstart; _iElement < _VL; ++_iElement){
		int _iRegEl = _iElement % _nElementsPerReg;
		if(_iRegEl == 0){
			++_iReg;
		}
		if(_Vm || (_V[_iElement/8]&(1<<_iElement%8))){
			for (int _iElementByte = 0; _iElementByte < _eewB; ++_iElementByte){
				uint8_t x = _V[_iReg*_VLENB + _iRegEl*_eewB + _iElementByte];
				writeMem(_memStart + _iMemOffset, &x, 1);
				++_iMemOffset;
			}
		}else{
			_iMemOffset+=_eewB;
		}
		if(_stride){
			_memStart += _stride;
			_iMemOffset = 0;
		}
	}
}
