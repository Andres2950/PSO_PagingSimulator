#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

class Instruction {
    public:
        virtual void execute() = 0;
};

class New_Instruction: public Instruction {};

#endif //INSTRUCTIONS_H
