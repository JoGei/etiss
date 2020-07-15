/*

        @copyright

        <pre>

        Copyright 2018 Chair of Electronic Design Automation, TUM

        This file is part of ETISS tool, see <https://gitlab.lrz.de/de-tum-ei-eda-open/etiss>.

        The initial version of this software has been created with the funding support by the German Federal
        Ministry of Education and Research (BMBF) in the project EffektiV under grant 01IS13022.

        Redistribution and use in source and binary forms, with or without modification, are permitted
        provided that the following conditions are met:

        1. Redistributions of source code must retain the above copyright notice, this list of conditions and
        the following disclaimer.

        2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
        and the following disclaimer in the documentation and/or other materials provided with the distribution.

        3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
        or promote products derived from this software without specific prior written permission.

        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
        WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
        PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
        DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
        HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
        POSSIBILITY OF SUCH DAMAGE.

        </pre>

        @author Aote Jin <aote.jin@tum.de>, Chair of Electronic Design Automation, TUM

        @version 0.1

*/

// This file was generated on Thu Jul 12 13:49:55 CEST 2018
// If necessary please modify this file according to the instructions
// Contact: eda@tum

// Modified to add architecture specific functionalities on July 12 2018

#ifndef ETISS_RV64GCVArch_RV64GCVARCHSPECIFICIMP_H_
#define ETISS_RV64GCVArch_RV64GCVARCHSPECIFICIMP_H_

#include "Encoding.h"
#include "RV64GCV.h"
#include "RV64GCVArch.h"
#include "etiss/CPUCore.h"

etiss::instr::InstructionGroup ISA64_RV64GCV("ISA64_RV64GCV", 64);
etiss::instr::InstructionGroup ISA16_RV64GCV("ISA16_RV64GCV", 16);
etiss::instr::InstructionGroup ISA32_RV64GCV("ISA32_RV64GCV", 32);
etiss::instr::InstructionClass ISA16_RV64GCVClass(1, "ISA16_RV64GCV", 16, ISA16_RV64GCV);
etiss::instr::InstructionClass ISA32_RV64GCVClass(1, "ISA32_RV64GCV", 32, ISA32_RV64GCV);
etiss::instr::InstructionClass ISA64_RV64GCVClass(1, "ISA64_RV64GCV", 64, ISA64_RV64GCV);

etiss::instr::InstructionCollection RV64GCVISA("RV64GCVISA", ISA16_RV64GCVClass, ISA32_RV64GCVClass, ISA64_RV64GCVClass);

/**
        @brief This function will be called automatically in order to handling
   exceptions such as interrupt, system call, illegal instructions

        @details Exception handling mechanism is implementation dependent for
   each cpu variant. Please add it to the following block if exception handling
   is demanded. Pesudo example: switch(cause){ case
   etiss::RETURNCODE::INTERRUPT:
                                                .
                                                .
                                                .
                                        break;

*/
etiss::int32 RV64GCVArch::handleException(etiss::int32 cause, ETISS_CPU *cpu)
{
    etiss_uint32 handledCause = cause;

    std::function<void()> disableItr = [cpu]() {
        if (likely((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MIE))
        {
            // Push MIE, SIE, UIE to MPIE, SPIE, UPIE
            etiss_uint32 irq_enable = ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MIE) |
                                      ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_UIE) |
                                      ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_SIE);
            (((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) = (irq_enable << 4) | ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & 0xffffff00);
        }
    };

    std::function<etiss_uint32(etiss_uint32, etiss_uint32)> handle = [cpu, cause](etiss_uint32 causeCode,
                                                                                  etiss_uint32 addr) {
        std::stringstream msg;

        msg << "Exception is captured with cause code: 0x" << std::hex << causeCode;
        msg << "  Exception message: " << etiss::RETURNCODE::getErrorMessages()[cause] << std::endl;
        msg << "Exception occurs at instruction address: 0x" << std::hex << cpu->instructionPointer << std::endl;
        msg << "--------Dump the CPU state--------" << std::endl;
        for (uint32_t i = 0; i < 32; ++i)
        {
            auto core_ = (etiss::CPUCore *)cpu->_etiss_private_handle_;
            std::stringstream ss;
            ss << "R" << i;
            msg << ss.str() << " = 0x" << std::hex << core_->getStruct()->findName(ss.str().c_str())->read()
                << std::endl;
        }

        msg << "--------Dump the VPU state--------" << std::endl;
        auto VLEN = ((RV64GCV *)cpu)->CSR[3106]*8;

        if ( unlikely( reinterpret_cast<etiss_uint8*>(((RV64GCV *)cpu)->V) == nullptr)) {
			/* Get the Vector Units implementation platform config*/
			int vlen = etiss::cfg().get<int>("RVV::VLEN", 1024);
			int elen = etiss::cfg().get<int>("RVV::ELEN", 1024);
			int slen = etiss::cfg().get<int>("RVV::SLEN", 1024);
			((RV64GCV *)cpu)->CSR[3106] = vlen/8;

			// Allocate vector register space.
			((RV64GCV *)cpu)->V = reinterpret_cast<etiss_uint64>(new etiss_uint8[32*vlen/8]);
		}

        for (uint32_t i = 0; i < 32; ++i)
        {
           	std::stringstream ss;
           	ss << "V" << i << "\t";
           	for(uint32_t k = 0; k < VLEN/8; ++k){
           		ss << '|' << std::setfill('0') << std::setw(2) << std::hex << (int) ((etiss_uint8*)((RV64GCV *)cpu)->V)[(i+1)*VLEN/8 - (k+1)] ;
           	}
          	ss << '|' << std::dec;
            msg << ss.str() << std::endl;
        }

        switch (causeCode & 0x80000000)
        {
        // Exception
        case 0x0:
            // Check exception delegation
            if (((RV64GCV *)cpu)->CSR[CSR_MEDELEG] & (static_cast<etiss_uint64>(1) << (causeCode & 0x1f)))
            {
                etiss::log(etiss::VERBOSE, "Exception is delegated to supervisor mode");
                // Pop MPIE to MIE
                (((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) ^=
                    (((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MPIE) >> 4) ^ ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MIE);
                ((RV64GCV *)cpu)->CSR[CSR_SCAUSE] = causeCode;
                // Redo the instruction encoutered exception after handling
                ((RV64GCV *)cpu)->CSR[CSR_SEPC] = cpu->instructionPointer - 4;
                ((RV64GCV *)cpu)->CSR[CSR_SSTATUS] ^=
                    (((RV64GCV *)cpu)->CSR[3088] << 8) ^ (((RV64GCV *)cpu)->CSR[CSR_SSTATUS] & MSTATUS_SPP);
                ((RV64GCV *)cpu)->CSR[3088] = PRV_S;
                etiss::log(etiss::VERBOSE, "Privilege mode is changed to supervisor mdoe:" + etiss::toString(PRV_S));
                cpu->instructionPointer = ((RV64GCV *)cpu)->CSR[CSR_STVEC] & ~0x3;
            }
            else
            {
                ((RV64GCV *)cpu)->CSR[CSR_MCAUSE] = causeCode;
                // Redo the instruction encoutered exception after handling
                ((RV64GCV *)cpu)->CSR[CSR_MEPC] = cpu->instructionPointer - 4;
                (((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) ^=
                    (((RV64GCV *)cpu)->CSR[3088] << 11) ^ ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MPP);
                ((RV64GCV *)cpu)->CSR[3088] = PRV_M;
                etiss::log(etiss::VERBOSE, "Privilege mode is changed to machine mdoe: " + etiss::toString(PRV_M));
                // Customized handler address other than specified in RISC-V ISA
                // manual
                if (addr)
                {
                    cpu->instructionPointer = addr;
                    break;
                }
                cpu->instructionPointer = ((RV64GCV *)cpu)->CSR[CSR_MTVEC] & ~0x3;
            }
            break;

        // Interrupt
        case 0x80000000:
            // Check exception delegation
            if (((RV64GCV *)cpu)->CSR[CSR_MIDELEG] & (static_cast<etiss_uint64>(1) << (causeCode & 0x1f)))
            {
                etiss::log(etiss::VERBOSE, "Interrupt is delegated to supervisor mode");
                // Pop MPIE to MIE
                (((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) ^=
                    (((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MPIE) >> 4) ^ ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MIE);
                ((RV64GCV *)cpu)->CSR[CSR_SCAUSE] = causeCode;
                // Return to instruction next interrupted one
                ((RV64GCV *)cpu)->CSR[CSR_SEPC] = cpu->instructionPointer;
                ((RV64GCV *)cpu)->CSR[CSR_SSTATUS] ^=
                    (((RV64GCV *)cpu)->CSR[3088] << 8) ^ (((RV64GCV *)cpu)->CSR[CSR_SSTATUS] & MSTATUS_SPP);
                ((RV64GCV *)cpu)->CSR[3088] = PRV_S;
                etiss::log(etiss::VERBOSE, "Privilege mode is changed to supervisor mdoe:" + etiss::toString(PRV_S));
                if (((RV64GCV *)cpu)->CSR[CSR_STVEC] & 0x1)
                    cpu->instructionPointer = (((RV64GCV *)cpu)->CSR[CSR_STVEC] & ~0x3) + causeCode * 4;
                else
                    cpu->instructionPointer = ((RV64GCV *)cpu)->CSR[CSR_STVEC] & ~0x3;
            }
            else
            {
                ((RV64GCV *)cpu)->CSR[CSR_MCAUSE] = causeCode;
                // Return to instruction next interrupted one
                ((RV64GCV *)cpu)->CSR[CSR_MEPC] = cpu->instructionPointer;
                (((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) ^=
                    (((RV64GCV *)cpu)->CSR[3088] << 11) ^ ((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MPP);
                ((RV64GCV *)cpu)->CSR[3088] = PRV_M;
                etiss::log(etiss::VERBOSE, "Privilege mode is changed to machine mdoe: " + etiss::toString(PRV_M));
                // Customized handler address other than specified in RISC-V ISA
                // manual
                if (addr)
                {
                    cpu->instructionPointer = addr;
                    break;
                }
                if (((RV64GCV *)cpu)->CSR[CSR_MTVEC] & 0x1)
                    cpu->instructionPointer = (((RV64GCV *)cpu)->CSR[CSR_MTVEC] & ~0x3) + causeCode * 4;
                else
                    cpu->instructionPointer = ((RV64GCV *)cpu)->CSR[CSR_MTVEC] & ~0x3;
            }
            break;
        }

        msg << "Program is redirected to address: 0x" << std::hex << cpu->instructionPointer << std::endl;
        etiss::log(etiss::VERBOSE, msg.str());
        return etiss::RETURNCODE::NOERROR;
    };

    switch (cause)
    {
    case etiss::RETURNCODE::INTERRUPT:
        if (!((((RV64GCV *)cpu)->CSR[CSR_MSTATUS]) & MSTATUS_MIE))
        {
            std::stringstream msg;
            msg << "Interrupt handling is globally disabled. Interrupt line is "
                   "still pending."
                << std::endl;
            etiss::log(etiss::INFO, msg.str());
            handledCause = etiss::RETURNCODE::NOERROR;
            break;
        }
        {
            etiss_uint64 mip_tmp = ((((RV64GCV *)cpu))->CSR[CSR_MIP]);
            if (0 == mip_tmp)
            {
                handledCause = etiss::RETURNCODE::NOERROR;
                break;
            }
            etiss_uint32 irqLine = 0;
            for (size_t i = 0; i < sizeof(mip_tmp) * 8; ++i)
            {
                // Highest interrupt line with highest priority
                if (unlikely((mip_tmp >> i) & 0x1))
                    irqLine = i;
            }

            if (!(((((RV64GCV *)cpu))->CSR[CSR_MIE]) & (static_cast<etiss_uint64>(1) << irqLine)))
            {
                std::stringstream msg;
                handledCause = etiss::RETURNCODE::NOERROR;
                msg << "Interrupt line: " << irqLine << " is disabled. Interrupt is still pending." << std::endl;
                etiss::log(etiss::INFO, msg.str());
                break;
            }

            disableItr();

            handledCause = handle(irqLine | 0x80000000, 0);
        }
        break;

    case etiss::RETURNCODE::RESET:
        handledCause = handle(0, 0x80);
        break;

    case etiss::RETURNCODE::INSTR_PAGEFAULT:
        disableItr();
        handledCause = handle(CAUSE_FETCH_PAGE_FAULT, 0);
        break;

    case etiss::RETURNCODE::LOAD_PAGEFAULT:

        disableItr();
        handledCause = handle(CAUSE_LOAD_PAGE_FAULT, 0);
        break;

    case etiss::RETURNCODE::STORE_PAGEFAULT:

        disableItr();
        handledCause = handle(CAUSE_STORE_PAGE_FAULT, 0);
        break;

    case etiss::RETURNCODE::ILLEGALINSTRUCTION:
    {
        disableItr();
        std::stringstream msg;
        msg << "Illegal instruction at address: 0x" << std::hex << cpu->instructionPointer << std::endl;
        ((RV64GCV *)cpu)->CSR[CSR_MTVAL] = cpu->instructionPointer;
        // Point to next instruction
        cpu->instructionPointer += 4;
        etiss::log(etiss::WARNING, msg.str());
        handledCause = handle(CAUSE_ILLEGAL_INSTRUCTION, 0);
        break;
    }

    case etiss::RETURNCODE::DBUS_READ_ERROR:

        disableItr();
        handledCause = handle(CAUSE_LOAD_ACCESS, 0);
        break;

    case etiss::RETURNCODE::DBUS_WRITE_ERROR:

        disableItr();
        handledCause = handle(CAUSE_STORE_ACCESS, 0);
        break;

    case etiss::RETURNCODE::IBUS_READ_ERROR:

        disableItr();
        handledCause = handle(CAUSE_FETCH_ACCESS, 0);
        break;

    case etiss::RETURNCODE::IBUS_WRITE_ERROR:

        disableItr();
        handledCause = handle(CAUSE_STORE_ACCESS, 0);
        break;

    case etiss::RETURNCODE::BREAKPOINT:

        disableItr();
        handledCause = handle(CAUSE_BREAKPOINT, 0);
        break;

    case etiss::RETURNCODE::SYSCALL:

        disableItr();
        switch (((RV64GCV *)cpu)->CSR[3088])
        {
        case PRV_U:
            handledCause = handle(CAUSE_USER_ECALL, 0);
            break;
        case PRV_S:
            handledCause = handle(CAUSE_SUPERVISOR_ECALL, 0);
            break;
        case PRV_M:
            handledCause = handle(CAUSE_MACHINE_ECALL, 0);
            break;
        default:
            etiss::log(etiss::ERROR, "System call type not supported for current architecture.");
        }

        break;

    case etiss::RETURNCODE::ILLEGALJUMP:
    {
        disableItr();
        std::stringstream msg;
        msg << "Illegal instruction access at address: 0x" << std::hex << cpu->instructionPointer << std::endl;
        ((RV64GCV *)cpu)->CSR[CSR_MTVAL] = cpu->instructionPointer;
        // Point to next instruction
        cpu->instructionPointer += 4;
        etiss::log(etiss::WARNING, msg.str());
        handledCause = handle(CAUSE_FETCH_ACCESS, 0);
        break;
    }

    default:
    {
        std::stringstream msg;
        msg << "Exception is not handled by architecture. Exception message: ";
        msg << etiss::RETURNCODE::getErrorMessages()[cause] << std::endl;
        etiss::log(etiss::INFO, msg.str());
    }
        handledCause = cause;
        break;
    }

    return handledCause;
}

/**
        @brief This function is called during CPUArch initialization

        @details Function pointer length_updater_ has to be replaced if multiple
   length instruction execution is supported. This function enables dynamic
   instruction length update in order to guarantee correct binary translation
                         Pesudo example:
                           vis->length_updater_ = [](VariableInstructionSet &
   ,InstructionContext & ic, BitArray & ba)
                            {
                                switch(ba.byteCount()){
                                        case 4:
                                                if (
   INSTRUCTION_LENTH_NOT_EQUAL(4)){ updateInstrLength(ic, ba);
                                                        ic.is_not_default_width_
   = true;
                                                                .
                                                                .
                                                                .
                                                }
                                                break;
                                }
                                };

*/
void RV64GCVArch::initInstrSet(etiss::instr::ModedInstructionSet &mis) const
{
    {
     /* Set default JIT Extensions. Read Parameters set from ETISS configuration and append with architecturally needed */
     std::string cfgPar = "";
     cfgPar = etiss::cfg().get<std::string>("JIT-External::Headers", ";");
     etiss::cfg().set<std::string>("JIT-External::Headers", cfgPar + "etiss/jit/fpu/softfloat_orig.h;etiss/jit/fpu/libdbtrise_fp_funcs.h;etiss/jit/vpu/softvector.h"); 

     cfgPar = etiss::cfg().get<std::string>("JIT-External::Libs", ";");
     etiss::cfg().set<std::string>("JIT-External::Libs", cfgPar + "softfloat;dbtrise_fp_funcs;softvector");   
     
     cfgPar = etiss::cfg().get<std::string>("JIT-External::HeaderPaths", ";");
     etiss::cfg().set<std::string>("JIT-External::HeaderPaths", cfgPar + "/etiss/jit/fpu;/etiss/jit/vpu");
     
     cfgPar = etiss::cfg().get<std::string>("JIT-External::LibPaths", ";");
     etiss::cfg().set<std::string>("JIT-External::LibPaths", cfgPar + "/etiss/jit/fpu;/etiss/jit/vpu");
    }     
    
    {
        // Pre-compilation of instruction set to view instruction tree. Could be
        // disabled.
        /*etiss::instr::ModedInstructionSet iset("RV64GCVISA");
        bool ok = true;
        RV64GCVISA.addTo(iset, ok);

        iset.compile();

        std::cout << iset.print() << std::endl;*/
    }

    bool ok = true;
    RV64GCVISA.addTo(mis, ok);
    if (!ok)
        etiss::log(etiss::FATALERROR, "Failed to add instructions for RV64GCVISA");

    etiss::instr::VariableInstructionSet *vis = mis.get(1);

    using namespace etiss;
    using namespace etiss::instr;
    vis->length_updater_ = [](VariableInstructionSet &, InstructionContext &ic, BitArray &ba) {
        std::function<void(InstructionContext & ic, etiss_uint32 opRd)> updateRiscvInstrLength =
            [](InstructionContext &ic, etiss_uint32 opRd) {
                ic.instr_width_fully_evaluated_ = true;
                ic.is_not_default_width_ = true;
                if (opRd == 0x3f)
                    ic.instr_width_ = 64;
                else if ((opRd & 0x3f) == 0x1f)
                    ic.instr_width_ = 48;
                else if (((opRd & 0x1f) >= 0x3) && ((opRd & 0x1f) < 0x1f))
                    ic.instr_width_ = 32;
                else if ((opRd & 0x3) != 0x3)
                    ic.instr_width_ = 16;
                else
                    // This might happen when code is followed by data.
                    ic.is_not_default_width_ = false;
            };

        BitArrayRange op(6, 0);
        etiss_uint32 opRd = op.read(ba);

        /*BitArrayRange fullOp(ba.byteCount()*8-1,0);
        etiss_uint32 fullOpRd = fullOp.read(ba);

        std::stringstream ss;
        ss << "Byte count: " << ba.byteCount()<< std::endl;
        ss << "opcode: 0x" <<std::hex<< fullOpRd << std::endl;
        ss << "Current PC: 0x" <<std::hex<< ic.current_address_ << std::endl;
        std::cout << ss.str() << std::endl;*/

        switch (ba.byteCount())
        {
        case 2:
            if (((opRd & 0x3) != 0x3) || (opRd == 0))
            {
                ic.is_not_default_width_ = false;
                break;
            }
            else
            {
                updateRiscvInstrLength(ic, opRd);
                break;
            }
        case 4:
            if ((((opRd & 0x1f) >= 0x3) || ((opRd & 0x1f) < 0x1f)) || (opRd == 0))
            {
                ic.is_not_default_width_ = false;
                break;
            }
            else
            {
                updateRiscvInstrLength(ic, opRd);
                break;
            }
        case 6:
            if (((opRd & 0x3f) == 0x1f) || (opRd == 0))
            {
                ic.is_not_default_width_ = false;
                break;
            }
            else
            {
                updateRiscvInstrLength(ic, opRd);
                break;
            }
        case 8:
            if ((opRd == 0x3f) || (opRd == 0))
            {
                ic.is_not_default_width_ = false;
                break;
            }
            else
            {
                updateRiscvInstrLength(ic, opRd);
                break;
            }
        default:
            // This might happen when code is followed by data.
            ic.is_not_default_width_ = false;
        }
    };
}

/**
        @brief This function is called whenever a data is read from memory

        @details Target architecture may have inconsistent endianess. Data read
   from memory is buffered, and this function is called to alter sequence of
   buffered data so that the inconsistent endianess is compensated. Example for
   ARMv6M: void * ptr = ba.internalBuffer(); if (ba.byteCount() == 2)
                            {
                                *((uint32_t*)ptr) =
   ((uint16_t)(*((uint8_t*)ptr))) | ((uint16_t)(*(((uint8_t*)ptr)+1)) << 8);
                            }
                            else if (ba.byteCount() == 4)
                            {
                                *((uint32_t*)ptr) =
   ((((uint32_t)(*((uint8_t*)ptr))) | ((uint32_t)(*(((uint8_t*)ptr)+1)) << 8))
   << 16) | ((uint32_t)(*(((uint8_t*)ptr)+2)) ) |
   ((uint32_t)(*(((uint8_t*)ptr)+3)) << 8);
                            }
                            else
                            {
                                etiss::log(etiss::FATALERROR,"Endianess cannot
   be handled",ba.byteCount());
                            }

        @attention Default endianess: little-endian

*/
void RV64GCVArch::compensateEndianess(ETISS_CPU *cpu, etiss::instr::BitArray &ba) const
{
    /**************************************************************************
     *		                Endianess compensation *
     ***************************************************************************/
}

/**
        @brief VirtualStruct for RV64GCV architecture to faciliate register
   acess

        @details VirtualStruct enables user to access certain register via their
   name without knowning ETISS hierarchy of a core. Further fiels might be
   needed to enable gdbserver etc.

*/
class RegField : public etiss::VirtualStruct::Field
{
  private:
    const unsigned gprid_;

  public:
    RegField(etiss::VirtualStruct &parent, unsigned gprid)
        : Field(parent, std::string("R") + etiss::toString(gprid), std::string("R") + etiss::toString(gprid), R | W, 8)
        , gprid_(gprid)
    {
    }

    RegField(etiss::VirtualStruct &parent, std::string name, unsigned gprid)
        : Field(parent, name, name, R | W, 8), gprid_(gprid)
    {
    }

    virtual ~RegField() {}

  protected:
    virtual uint64_t _read() const { return (uint64_t) * ((RV64GCV *)parent_.structure_)->X[gprid_]; }

    virtual void _write(uint64_t val)
    {
        etiss::log(etiss::VERBOSE, "write to ETISS cpu state", name_, val);
        *((RV64GCV *)parent_.structure_)->X[gprid_] = (etiss_uint64)val;
    }
};

class pcField : public etiss::VirtualStruct::Field
{
  public:
    pcField(etiss::VirtualStruct &parent) : Field(parent, "instructionPointer", "instructionPointer", R | W, 8) {}

    virtual ~pcField() {}

  protected:
    virtual uint64_t _read() const { return (uint64_t)((ETISS_CPU *)parent_.structure_)->instructionPointer; }

    virtual void _write(uint64_t val)
    {
        etiss::log(etiss::VERBOSE, "write to ETISS cpu state", name_, val);
        ((ETISS_CPU *)parent_.structure_)->instructionPointer = (etiss_uint64)val;
    }
};

template <typename T>
class CSRField : public etiss::VirtualStruct::Field
{
  private:
    T *const csr_;
    const unsigned csrid_;

  public:
    CSRField(etiss::VirtualStruct &parent, T *csr, unsigned csrid)
        : Field(parent, std::string("CSR") + etiss::toString(csrid), std::string("CSR") + etiss::toString(csrid), R | W,
                sizeof(T))
        , csr_(csr)
        , csrid_(csrid)
    {
    }

    virtual ~CSRField() {}

  protected:
    virtual uint64_t _read() const { return (uint64_t)*csr_; }

    virtual void _write(uint64_t val)
    {
        etiss::log(etiss::VERBOSE, "write to ETISS cpu state", name_, val);
        *csr_ = (T)val;
    }
};

std::shared_ptr<etiss::VirtualStruct> RV64GCVArch::getVirtualStruct(ETISS_CPU *cpu)
{
    auto ret = etiss::VirtualStruct::allocate(cpu, [](etiss::VirtualStruct::Field *f) { delete f; });

    for (uint32_t i = 0; i < 32; ++i)
    {
        ret->addField(new RegField(*ret, i));
    }
    ret->addField(new pcField(*ret));
    return ret;
}

/**
        @brief If interrupt handling is expected, vector table could be provided
   to support interrupt triggering

        @details Interrupt vector table is used to inform the core whenever an
   edge/level triggered interrupt incoming. The content of interrupt vector
   could be a special register or standalone interrupt lines.
*/
etiss::InterruptVector *RV64GCVArch::createInterruptVector(ETISS_CPU *cpu)
{
    if (cpu == 0)
        return 0;
    RV64GCV *riscvcpu = (RV64GCV *)cpu;
    std::vector<etiss::uint64 *> vec;
    vec.push_back(&riscvcpu->CSR[CSR_MIP]);
    std::vector<etiss::uint64 *> mask;
    mask.push_back(&riscvcpu->CSR[CSR_MIE]);
    return new etiss::MappedInterruptVector<etiss::uint64>(vec, mask);
}
void RV64GCVArch::deleteInterruptVector(etiss::InterruptVector *vec, ETISS_CPU *cpu)
{
    delete vec;
}

#endif