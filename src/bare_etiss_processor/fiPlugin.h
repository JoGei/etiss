

#pragma once

#include <assert.h>

#include <random>

#include "etiss/ETISS.h"

class fiPlugin : public etiss::CoroutinePlugin
{
  public:
    fiPlugin(etiss::VirtualStruct *cpu_struct) : vstruct_{ cpu_struct }, etiss::CoroutinePlugin{} {}

    ~fiPlugin() override {
        //std::cout << "#" << instr_cnt_ << "\n";
    }

    std::string _getPluginName() const override { return "fi"; }

    etiss::int32 execute() override
    {
        static uint64_t do_fi_at_instr_cnt{ fi_time_dist_(gen_) };

        //    do_fi_at_instr_cnt = 602372;

        if (instr_cnt_ == do_fi_at_instr_cnt)
        {
            std::cout << "\nFI_INFO: Injecting bitflip at " << instr_cnt_ << "\n";

            std::vector<std::string> reg_names{};
            for (auto &f : vstruct_->listFields())
            {
                reg_names.emplace_back(f);
            }

            auto fi_reg{ reg_names[fi_space_dist_.first(gen_)] };
            while (1)
            {
                if (fi_reg.size())
                {
                    // avoiding FI in zero register as this is wires
                    if (fi_reg.compare("R0") == 0)
                    {
                        continue;
                    }

                    if (std::string{ fi_reg[0] }.compare("F") && std::string{ fi_reg[0] }.compare("C"))
                    {
                        break;
                    }
                }

                fi_reg = reg_names[fi_space_dist_.first(gen_)];
            }

            auto fi_bit{ fi_space_dist_.second(gen_) };

            //       fi_reg = "R7";
            //       fi_bit = 31;

            std::cout << "FI_INFO: Injecting in " << fi_reg << "[" << fi_bit << "]"
                      << "\n";

            vstruct_->foreachField(
                [&fi_reg, &fi_bit](auto f)
                {
                    if (!f->name_.compare(fi_reg))
                    {
                        if (!etiss::cfg().get<bool>("disable_fi", false))
                        {
                            f->write(f->read() ^ (1 << fi_bit));
                        }
                        else
                        {
                            std::cout << "FI Disabled\n";
                        }
                    }
                });
        }

        // if (vstruct_->findName("instructionPointer").get()->read() == 0x88c) {
        //   vstruct_->foreachField([](auto f) {
        //     std::cout << f->name_ << " = " << std::hex << f->read() << std::dec
        //               << "\n";
        //   });
        //
        //   exit(1);
        // }

        if (instr_cnt_ > etiss::cfg().get<int>("etiss.fi_max_instr_cnt_term", 0))
        {
            std::cout << "Terminating from FIPlugin \n";
            //return etiss::RETURNCODE::CPUTERMINATED;
            return etiss::RETURNCODE::CPUFINISHED;
        }

        instr_cnt_++;
        return etiss::RETURNCODE::NOERROR;
    }

    uint64_t getInstrCnt() { return instr_cnt_; }

  protected:
    etiss::VirtualStruct *vstruct_{ nullptr };
    uint64_t instr_cnt_{ 0 };
    std::random_device rd_{};
    std::default_random_engine gen_{ rd_() };

    std::uniform_int_distribution<uint64_t> fi_time_dist_{ 0, static_cast<uint64_t>(
                                                                  etiss::cfg().get<int>("etiss.fi_max_instr_cnt", 0)) };

    //  std::uniform_int_distribution<uint64_t> fi_time_dist_{1105578, 1116358};

    std::pair<std::uniform_int_distribution<uint64_t>, std::uniform_int_distribution<uint64_t>> fi_space_dist_{
        std::uniform_int_distribution<uint64_t>{ 0, vstruct_->listFields().size() },
        std::uniform_int_distribution<uint64_t>{ 0, 31 }
    };
};

class printInstr : public fiPlugin
{
  public:
    printInstr(etiss::VirtualStruct *cpu_struct) : fiPlugin{ cpu_struct }
    {
        pc_ = cpu_struct->findName("instructionPointer").get();
        if (!pc_)
        {
            std::cout << "PC member not setup properly in printInstr()\n";
            exit(1);
        }
    }

    etiss::int32 execute() override
    {
        if (std::find(interesting_instr_cnts_.begin(), interesting_instr_cnts_.end(), instr_cnt_) !=
            interesting_instr_cnts_.end())
        {
            std::cout << "instr_cnt: " << instr_cnt_ << ", pc: " << std::hex << pc_->read() << std::dec << "\n";
        }

        /*    if (instr_cnt_ > 5000000) {
              std::cout << "instr_cnt: " << instr_cnt_ << ", pc: " << std::hex
                        << pc_->read() << std::dec << "\n";
            }*/

        /*    if (pc_->read() == 0x5ac) {
              std::cout << "out_checksum update\n";
            } else if (pc_->read() == 0x8ae) {
              exit(1);
            } else if (pc_->read() == 0x7d6) {
              std::cout << "filter_data_for_checksum\n";
            } else if (pc_->read() == 0x720) {
              std::cout << "input_data_for_checksum\n";
            } else if (pc_->read() == 0x79e) {
              std::cout << "input_checksum\n";
            }

        */

        instr_cnt_++;
        return etiss::RETURNCODE::NOERROR;
    }

  private:
    etiss::VirtualStruct::Field *pc_{ nullptr };
    std::vector<uint64_t> interesting_instr_cnts_{
        2507,  3710,  4339,  4392,   4865,   5132,   5869,    8626,    8786,    11165,
        17232, 27577, 33334, 557590, 700589, 966078, 1944188, 1946570, 1946890, 1947035,

    };
};

class hamartiaFIPlugin : public fiPlugin
{
  public:
    hamartiaFIPlugin(etiss::CPUCore *cpu, etiss::SimpleMemSystem *mem)
        : fiPlugin{ cpu->getStruct().get() }, cpu_{ cpu }, mem_{ mem }
    {
        pc_ = cpu->getStruct()->findName("instructionPointer").get();
        assert(pc_);
    }

    ~hamartiaFIPlugin() override
    {
        // for (auto k : dyn_instr_cnt_) {
        //   if (isALInstr(k.first)) {
        //     continue;
        //   }
        //
        //   if (Instrs2String_.find(k.first) == Instrs2String_.end()) {
        //     std::cerr << "ERROR: key not found in Instrs2String_ in "
        //                  "~hamartiaFIPlugin()\n";
        //     exit(EXIT_FAILURE);
        //   }
        //
        //   std::cout << Instrs2String_.at(k.first) << " : " << k.second << "\n";
        // }
    }

    std::string _getPluginName() const override { return "hamartia-fi"; }

    etiss::int32 execute() override
    {
        auto readWordFromMem{ [this](uint64_t addr) -> uint32_t
                              {
                                  uint32_t ret{ 0 };
                                  etiss::uint8 buf[4];
                                  mem_->dread(cpu_->getState(), addr, buf, 4);
                                  ret = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
                                  return ret;
                              } };

        // if (std::find(interesting_instr_cnts_.begin(),
        //               interesting_instr_cnts_.end(),
        //               instr_cnt_) != interesting_instr_cnts_.end()) {
        //   std::cout << "instr_cnt: " << instr_cnt_ << ", pc: " << std::hex
        //             << pc_->read() << std::dec << "\n";
        //
        //   if (instr_cnt_ == 43898024 && 0) {
        //     vstruct_->foreachField([](auto f) {
        //       if (std::string{f->name_[0]}.compare("R") == 0) {
        //         std::cout << f->name_ << " = 0x" << std::hex << f->read()
        //                   << std::dec << "\n";
        //       }
        //     });
        //
        //     return etiss::RETURNCODE::CPUTERMINATED;
        //   }
        // }

        // if (instr_cnt_ != 0) {
        // auto last_exec_instr_tag{getInstrTag(last_exec_instr_word_)};
        // if (dyn_instr_cnt_.find(last_exec_instr_tag) != dyn_instr_cnt_.end()) {
        //   dyn_instr_cnt_[last_exec_instr_tag]++;
        // } else {
        //   dyn_instr_cnt_[last_exec_instr_tag] = 1;
        // }
        // }

        static uint64_t do_fi_at_instr_cnt{ fi_time_dist_(gen_) };
        if (do_fi_at_instr_cnt == instr_cnt_)
        {
            look_for_next_al_instr = true;
        }

        if (look_for_next_al_instr)
        {
            if (isALInstr(getInstrTag(last_exec_instr_word_)))
            {
                auto rd{ (last_exec_instr_word_ & 0xf80) >> 7 };
                auto fi_reg{ std::string{ "R" } + std::to_string(rd) };
                auto fi_bit{ fi_space_dist_.second(gen_) };

                std::cout << "FI_INFO: Injecting in " << fi_reg << "[" << fi_bit << "]"
                          << "\n";

                vstruct_->foreachField(
                    [&fi_reg, &fi_bit](auto f)
                    {
                        if (!f->name_.compare(fi_reg))
                        {
                            f->write(f->read() ^ (1 << fi_bit));
                        }
                    });

                look_for_next_al_instr = false;
            }
        }

        instr_cnt_++;
        last_exec_instr_word_ = readWordFromMem(pc_->read());

        return etiss::RETURNCODE::NOERROR;
    }

  private:
    // std::vector<uint64_t> interesting_instr_cnts_{
    //     93548,    42341280, 42342523, 42343626, 42349009, 42350218, 42364882,
    //     42376389, 42385586, 42398124, 42404169, 42407073, 42409528, 42414412,
    //     42414859, 42418696, 66494005, 66498327, 66501810, 66502448, 66510153,
    //     66519885, 66540353, 66549862, 66555285, 66571954, 66576511, 66579732};
    etiss::VirtualStruct::Field *pc_{ nullptr };
    etiss::CPUCore *cpu_{ nullptr };
    etiss::SimpleMemSystem *mem_{ nullptr };
    uint32_t last_exec_instr_word_{ 0 };
    bool look_for_next_al_instr{ false };

    // TODO: WIP
    enum class Instrs
    {
        LUI,
        AUIPC,
        ADDI,
        SLTI,
        SLTIU,
        XORI,
        ORI,
        ANDI,
        SLLI,
        SRLI,
        SRAI,
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND,
        FENCE,
        FENCE_I,
        CSRRW,
        CSRRS,
        CSRRC,
        CSRRWI,
        CSRRSI,
        CSRRCI,
        ECALL,
        EBREAK,
        URET,
        SRET,
        MRET,
        WFI,
        SFENCE_VMA,
        LB,
        LH,
        LW,
        LBU,
        LHU,
        SB,
        SH,
        SW,
        JAL,
        JALR,
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,
        MUL,
        MULH,
        MULHSU,
        MULHU,
        DIV,
        DIVU,
        REM,
        REMU,
        C_ADDI4SPN,
        C_FLD,
        C_LW,
        C_FLW,
        C_LD,
        C_FSD,
        C_SW,
        C_FSW,
        C_SD,
        C_NOP,
        C_ADDI,
        C_JAL,
        C_ADDIW,
        C_LI,
        C_ADDI16SP,
        C_LUI,
        C_SRLI,
        C_SRAI,
        C_ANDI,
        C_SUB,
        C_XOR,
        C_OR,
        C_AND,
        C_SUBW,
        C_ADDW,
        C_J,
        C_BEQZ,
        C_BNEZ,
        C_SLLI,
        C_FLDSP,
        C_LWSP,
        C_FLWSP,
        C_LDSP,
        C_JR,
        C_MV,
        C_EBREAK,
        C_JALR,
        C_ADD,
        C_FSDSP,
        C_SWSP,
        C_FSWSP,
        C_SDSP
    };
    // TODO: WIP
    const std::map<Instrs, std::string> Instrs2String_{ { Instrs::LUI, "LUI" },
                                                        { Instrs::AUIPC, "AUIPC" },
                                                        { Instrs::ADDI, "ADDI" },
                                                        { Instrs::SLTI, "SLTI" },
                                                        { Instrs::SLTIU, "SLTIU" },
                                                        { Instrs::XORI, "XORI" },
                                                        { Instrs::ORI, "ORI" },
                                                        { Instrs::ANDI, "ANDI" },
                                                        { Instrs::SLLI, "SLLI" },
                                                        { Instrs::SRLI, "SRLI" },
                                                        { Instrs::SRAI, "SRAI" },
                                                        { Instrs::ADD, "ADD" },
                                                        { Instrs::SUB, "SUB" },
                                                        { Instrs::SLL, "SLL" },
                                                        { Instrs::SLT, "SLT" },
                                                        { Instrs::SLTU, "SLTU" },
                                                        { Instrs::XOR, "XOR" },
                                                        { Instrs::SRL, "SRL" },
                                                        { Instrs::SRA, "SRA" },
                                                        { Instrs::OR, "OR" },
                                                        { Instrs::AND, "AND" },
                                                        { Instrs::FENCE, "FENCE" },
                                                        { Instrs::FENCE_I, "FENCE_I" },
                                                        { Instrs::CSRRW, "CSRRW" },
                                                        { Instrs::CSRRS, "CSRRS" },
                                                        { Instrs::CSRRC, "CSRRC" },
                                                        { Instrs::CSRRWI, "CSRRWI" },
                                                        { Instrs::CSRRSI, "CSRRSI" },
                                                        { Instrs::CSRRCI, "CSRRCI" },
                                                        { Instrs::ECALL, "ECALL" },
                                                        { Instrs::EBREAK, "EBREAK" },
                                                        { Instrs::URET, "URET" },
                                                        { Instrs::SRET, "SRET" },
                                                        { Instrs::MRET, "MRET" },
                                                        { Instrs::WFI, "WFI" },
                                                        { Instrs::SFENCE_VMA, "SFENCE_VMA" },
                                                        { Instrs::LB, "LB" },
                                                        { Instrs::LH, "LH" },
                                                        { Instrs::LW, "LW" },
                                                        { Instrs::LBU, "LBU" },
                                                        { Instrs::LHU, "LHU" },
                                                        { Instrs::SB, "SB" },
                                                        { Instrs::SH, "SH" },
                                                        { Instrs::SW, "SW" },
                                                        { Instrs::JAL, "JAL" },
                                                        { Instrs::JALR, "JALR" },
                                                        { Instrs::BEQ, "BEQ" },
                                                        { Instrs::BNE, "BNE" },
                                                        { Instrs::BLT, "BLT" },
                                                        { Instrs::BGE, "BGE" },
                                                        { Instrs::BLTU, "BLTU" },
                                                        { Instrs::BGEU, "BGEU" },
                                                        { Instrs::MUL, "MUL" },
                                                        { Instrs::MULH, "MULH" },
                                                        { Instrs::MULHSU, "MULHSU" },
                                                        { Instrs::MULHU, "MULHU" },
                                                        { Instrs::DIV, "DIV" },
                                                        { Instrs::DIVU, "DIVU" },
                                                        { Instrs::REM, "REM" },
                                                        { Instrs::REMU, "REMU" },
                                                        { Instrs::C_ADDI4SPN, "C_ADDI4SPN" },
                                                        { Instrs::C_FLD, "C_FLD" },
                                                        { Instrs::C_LW, "C_LW" },
                                                        { Instrs::C_FLW, "C_FLW" },
                                                        { Instrs::C_LD, "C_LD" },
                                                        { Instrs::C_FSD, "C_FSD" },
                                                        { Instrs::C_SW, "C_SW" },
                                                        { Instrs::C_FSW, "C_FSW" },
                                                        { Instrs::C_SD, "C_SD" },
                                                        { Instrs::C_NOP, "C_NOP" },
                                                        { Instrs::C_ADDI, "C_ADDI" },
                                                        { Instrs::C_JAL, "C_JAL" },
                                                        { Instrs::C_ADDIW, "C_ADDIW" },
                                                        { Instrs::C_LI, "C_LI" },
                                                        { Instrs::C_ADDI16SP, "C_ADDI16SP" },
                                                        { Instrs::C_LUI, "C_LUI" },
                                                        { Instrs::C_SRLI, "C_SRLI" },
                                                        { Instrs::C_SRAI, "C_SRAI" },
                                                        { Instrs::C_ANDI, "C_ANDI" },
                                                        { Instrs::C_SUB, "C_SUB" },
                                                        { Instrs::C_XOR, "C_XOR" },
                                                        { Instrs::C_OR, "C_OR" },
                                                        { Instrs::C_AND, "C_AND" },
                                                        { Instrs::C_SUBW, "C_SUBW" },
                                                        { Instrs::C_ADDW, "C_ADDW" },
                                                        { Instrs::C_J, "C_J" },
                                                        { Instrs::C_BEQZ, "C_BEQZ" },
                                                        { Instrs::C_BNEZ, "C_BNEZ" },
                                                        { Instrs::C_SLLI, "C_SLLI" },
                                                        { Instrs::C_FLDSP, "C_FLDSP" },
                                                        { Instrs::C_LWSP, "C_LWSP" },
                                                        { Instrs::C_FLWSP, "C_FLWSP" },
                                                        { Instrs::C_LDSP, "C_LDSP" },
                                                        { Instrs::C_JR, "C_JR" },
                                                        { Instrs::C_MV, "C_MV" },
                                                        { Instrs::C_EBREAK, "C_EBREAK" },
                                                        { Instrs::C_JALR, "C_JALR" },
                                                        { Instrs::C_ADD, "C_ADD" },
                                                        { Instrs::C_FSDSP, "C_FSDSP" },
                                                        { Instrs::C_SWSP, "C_SWSP" },
                                                        { Instrs::C_FSWSP, "C_FSWSP" },
                                                        { Instrs::C_SDSP, "C_SDSP" } };

    std::map<Instrs, uint64_t> dyn_instr_cnt_{};

    // TODO: WIP
    Instrs getInstrTag(unsigned instr_word)
    {
        auto opcode{ instr_word & 0x7f };
        auto funct3{ (instr_word & 0x7000) >> 12 };
        auto funct7{ (instr_word & 0xfe000000) >> 25 };

        if (opcode == 0x37)
        {
            return Instrs::LUI;
        }
        else if (opcode == 0x17)
        {
            return Instrs::AUIPC;
        }
        else if (opcode == 0x6f)
        {
            return Instrs::JAL;
        }
        else if (opcode == 0x67)
        {
            return Instrs::JALR;
        }
        else if (opcode == 0x63)
        {
            if (funct3 == 0)
            {
                return Instrs::BEQ;
            }
            else if (funct3 == 1)
            {
                return Instrs::BNE;
            }
            else if (funct3 == 4)
            {
                return Instrs::BLT;
            }
            else if (funct3 == 5)
            {
                return Instrs::BGE;
            }
            else if (funct3 == 6)
            {
                return Instrs::BLTU;
            }
            else if (funct3 == 7)
            {
                return Instrs::BGEU;
            }
        }
        else if (opcode == 0x3)
        {
            if (funct3 == 0)
            {
                return Instrs::LB;
            }
            else if (funct3 == 1)
            {
                return Instrs::LH;
            }
            else if (funct3 == 2)
            {
                return Instrs::LW;
            }
            else if (funct3 == 4)
            {
                return Instrs::LBU;
            }
            else if (funct3 == 5)
            {
                return Instrs::LHU;
            }
        }
        else if (opcode == 0x23)
        {
            if (funct3 == 0)
            {
                return Instrs::SB;
            }
            else if (funct3 == 1)
            {
                return Instrs::SH;
            }
            else if (funct3 == 2)
            {
                return Instrs::SW;
            }
        }
        else if (opcode == 0x13)
        {
            if (funct3 == 0)
            {
                return Instrs::ADDI;
            }
            else if (funct3 == 2)
            {
                return Instrs::SLTI;
            }
            else if (funct3 == 3)
            {
                return Instrs::SLTIU;
            }
            else if (funct3 == 4)
            {
                return Instrs::XORI;
            }
            else if (funct3 == 6)
            {
                return Instrs::ORI;
            }
            else if (funct3 == 7)
            {
                return Instrs::ANDI;
            }
            else if (funct3 == 1)
            {
                return Instrs::SLLI;
            }
            else if (funct3 == 5)
            {
                if (funct7 == 0)
                {
                    return Instrs::SRLI;
                }
                else
                {
                    return Instrs::SRAI;
                }
            }
        }
        else if (opcode == 0x33)
        {
            if (funct3 == 0)
            {
                if (funct7 == 0)
                {
                    return Instrs::ADD;
                }
                else
                {
                    return Instrs::SUB;
                }
            }
            else if (funct3 == 1)
            {
                return Instrs::SLL;
            }
            else if (funct3 == 2)
            {
                return Instrs::SLT;
            }
            else if (funct3 == 3)
            {
                return Instrs::SLTU;
            }
            else if (funct3 == 4)
            {
                return Instrs::XOR;
            }
            else if (funct3 == 5)
            {
                if (funct7 == 0)
                {
                    return Instrs::SRL;
                }
                else
                {
                    return Instrs::SRA;
                }
            }
            else if (funct3 == 6)
            {
                return Instrs::OR;
            }
            else if (funct3 == 7)
            {
                return Instrs::AND;
            }
        }
        else if (opcode == 0xf)
        {
            if (funct3 == 0)
            {
                return Instrs::FENCE;
            }
            else
            {
                return Instrs::FENCE_I;
            }
        }
        else if (opcode == 0x73)
        {
            if (funct3 == 0)
            {
                auto funct12{ (instr_word & 0xfff00000) >> 20 };

                if (funct12 == 0)
                {
                    return Instrs::ECALL;
                }
                else
                {
                    return Instrs::EBREAK;
                }
            }
            else if (funct3 == 1)
            {
                return Instrs::CSRRW;
            }
            else if (funct3 == 2)
            {
                return Instrs::CSRRS;
            }
            else if (funct3 == 3)
            {
                return Instrs::CSRRC;
            }
            else if (funct3 == 5)
            {
                return Instrs::CSRRWI;
            }
            else if (funct3 == 6)
            {
                return Instrs::CSRRSI;
            }
            else if (funct3 == 7)
            {
                return Instrs::CSRRCI;
            }
        }

        std::cerr << "ERROR: unrecognized opcode: 0x" << std::hex << opcode << " funct3: 0x" << funct3 << std::dec
                  << "\n";
        std::cerr << "instr_word: 0x" << std::hex << instr_word << std::dec << "\n";
        exit(EXIT_FAILURE);
    }

    // TODO: WIP
    bool isALInstr(Instrs i)
    {
        if (i == Instrs::LUI || i == Instrs::AUIPC || i == Instrs::ADDI || i == Instrs::ORI || i == Instrs::ANDI ||
            i == Instrs::SLLI || i == Instrs::SRLI || i == Instrs::SRAI || i == Instrs::ADD || i == Instrs::SUB ||
            i == Instrs::XOR || i == Instrs::OR)
        {
            return true;
        }
        return false;
    }
};
