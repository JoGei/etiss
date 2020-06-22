/*
 * rvv-hl-lsu.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: ge29noy
 */

#include "rvv-hl-arithmetic.hpp"
#include "rvv-hl-base.hpp"

void VARITH::add_vv(
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
	)
{
	int _iReg = -1; //_vd - 1;
	int _nElementsPerReg = (_z_mul >= _n_mul) ? (_VLENB/_sewB) : (((uint64_t)_VLENB)*_z_mul/_sewB/_n_mul);
	for(int _iElement = _vstart; _iElement < _VL; ++_iElement){
		int _iRegEl = _iElement % _nElementsPerReg;
		if(_iRegEl == 0){
			++_iReg;
		}
		if(_Vm || (_V[_iElement/8]&(1<<_iElement%8))){
			uint8_t cin = 0;
			for (int _iElementByte = 0; _iElementByte < _sewB; ++_iElementByte){
				uint8_t x1 = _V[(_vs1 + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte];
				uint8_t x2 = _V[(_vs2 + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte];

				uint16_t x = x1 + x2 + cin;
				cin = (x & (0x1 << 8)) ? 1 : 0;
				_V[(_vd + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte] = (uint8_t)(x&0xff);
			}
		}
	}
}

void VARITH::add_vi(
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
	)
{
	int _iReg = -1; //_vd - 1;
	int _nElementsPerReg = (_z_mul >= _n_mul) ? (_VLENB/_sewB) : (((uint64_t)_VLENB)*_z_mul/_sewB/_n_mul);
	for(int _iElement = _vstart; _iElement < _VL; ++_iElement){
		int _iRegEl = _iElement % _nElementsPerReg;
		if(_iRegEl == 0){
			++_iReg;
		}
		if(_Vm || (_V[_iElement/8]&(1<<_iElement%8))){
			uint8_t cin = 0;
			for (int _iElementByte = 0; _iElementByte < _sewB; ++_iElementByte){
				uint8_t x1 = _V[(_vs1 + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte];
				uint8_t x2;
				if(_iElementByte == 0){
					x2 = _vimm & 0x1f;
				}else{
					if(_vimm & (0x1 << 4)){
						x2 = 0xff;
					}else{
						x2 = 0;
					}
				}

				uint16_t x = x1 + x2 + cin;
				cin = (x & (0x1 << 8)) ? 1 : 0;
				_V[(_vd + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte] = (uint8_t)(x&0xff);
			}
		}
	}
}

void VARITH::add_vx(
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
)
{
	int _iReg = -1; //_vd - 1;
	int _nElementsPerReg = (_z_mul >= _n_mul) ? (_VLENB/_sewB) : (((uint64_t)_VLENB)*_z_mul/_sewB/_n_mul);
	for(int _iElement = _vstart; _iElement < _VL; ++_iElement){
		int _iRegEl = _iElement % _nElementsPerReg;
		if(_iRegEl == 0){
			++_iReg;
		}
		if(_Vm || (_V[_iElement/8]&(1<<_iElement%8))){
			uint8_t cin = 0;
			for (int _iElementByte = 0; _iElementByte < _sewB; ++_iElementByte){
				uint8_t x1 = _V[(_vs1 + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte];
				uint8_t x2; // = (_xlenB );
				if(_iElementByte > (_xlenB-1)){
					x2 = (_rs[_xlenB-1] & 0x8) ? 0xff : 0x0;
				}else{
					x2 = _rs[_iElementByte];
				}

				uint16_t x = x1 + x2 + cin;
				cin = (x & (0x1 << 8)) ? 1 : 0;
				_V[(_vd + _iReg)*_VLENB + _iRegEl*_sewB + _iElementByte] = (uint8_t)(x&0xff);
			}
		}
	}
}

