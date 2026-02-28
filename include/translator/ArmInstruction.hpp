#ifndef ARMINSTRUCTION_HPP
#define ARMINSTRUCTION_HPP

namespace Translator {
class ArmInstruction {
   public:
    virtual const std::string& toString() { return this->printOut; };

    std::string printOut;
};  // TODO implement
}  // namespace Translator

#endif