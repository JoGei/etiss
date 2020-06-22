// This file was generated on Mon Jun 22 18:08:07 CEST 2020
// If necessary please modify this file according to the instructions
// Contact: eda@tum

#ifndef ETISS_RV64IMACVArch_RV64IMACVArch_H_
#define ETISS_RV64IMACVArch_RV64IMACVArch_H_

#include "etiss/CPUArch.h"
#include "etiss/Instruction.h"
#include "etiss/InterruptVector.h"
#include "RV64IMACV.h"
#include "RV64IMACVGDBCore.h"

#include <map>

class RV64IMACVArch : public etiss::CPUArch {

public:

	RV64IMACVArch();

	virtual const std::set<std::string> & getListenerSupportedRegisters();


	virtual ETISS_CPU * newCPU();
	virtual void resetCPU(ETISS_CPU * cpu,etiss::uint64 * startpointer);
	virtual void deleteCPU(ETISS_CPU *);
	
	/**
		@brief get the VirtualStruct of the core to mitigate register access
		
		@see RV64IMACVArchSpecificImp.h
	*/
	virtual std::shared_ptr<etiss::VirtualStruct> getVirtualStruct(ETISS_CPU * cpu);

	/**
		@return 8 (jump instruction + instruction of delay slot)
	*/
	virtual unsigned getMaximumInstructionSizeInBytes();

	/**
		@return 2
	*/
	virtual unsigned getInstructionSizeInBytes();

	/**
		@brief required headers (RV64IMACV.h)
	*/
	virtual const std::set<std::string> & getHeaders() const;

	/**
		@brief This function will be called automatically in order to handling architecure dependent exceptions such 
		       as interrupt, system call, illegal instructions

		@see RV64IMACVArchSpecificImp.h
	*/
	virtual etiss::int32 handleException(etiss::int32 code,ETISS_CPU * cpu);

	/**
		@brief This function is called during CPUArch initialization

		@see RV64IMACVArchSpecificImp.h
	*/
	virtual void initInstrSet(etiss::instr::ModedInstructionSet & ) const;
	virtual void initCodeBlock(etiss::CodeBlock & cb) const;

	/**
		@brief Target architecture may have inconsistent endianess. Data read from memory is buffered, and this function 			       
			   is called to alter sequence of buffered data so that the inconsistent endianess is compensated.

		@see RV64IMACVArchSpecificImp.h
	*/
	virtual void compensateEndianess(ETISS_CPU * cpu,etiss::instr::BitArray & ba) const ;
	
	/**
		@brief If interrupt handling is expected, vector table could be provided to support interrupt triggering

		@see RV64IMACVArchSpecificImp.h
	*/
	virtual etiss::InterruptVector * createInterruptVector(ETISS_CPU * cpu);
	virtual void deleteInterruptVector(etiss::InterruptVector * vec,ETISS_CPU * cpu);
	
	/**
		@brief get the GDBcore for RV64IMACV architecture
		
		@see RV64IMACVGDBCore.h for implementation of GDBcore
	*/
	virtual etiss::plugin::gdb::GDBCore & getGDBCore();

private:

	std::set<std::string> listenerSupportedRegisters_;
	std::set<std::string> headers_;
	RV64IMACVGDBCore gdbcore_;


};
#endif
