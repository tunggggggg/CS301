#ifndef __PROJECT1_CPP__
#define __PROJECT1_CPP__

#include "project1.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 4) // Checks that at least 3 arguments are given in command line
    {
        std::cerr << "Expected Usage:\n ./assemble infile1.asm infile2.asm ... infilek.asm staticmem_outfile.bin instructions_outfile.bin\n" << std::endl;
        exit(1);
    }
    //Prepare output files
    std::ofstream inst_outfile, static_outfile;
    static_outfile.open(argv[argc - 1], std::ios::binary);
    inst_outfile.open(argv[argc - 1], std::ios::binary);
    std::vector<std::string> instructions;

    /**
     * Phase 1:
     * Read all instructions, clean them of comments and whitespace DONE
     * TODO: Determine the numbers for all static memory labels
     * (measured in bytes starting at 0)
     * TODO: Determine the line numbers of all instruction line labels
     * (measured in instructions) starting at 0
    */


    //For each input file:
    for (int i = 1; i < argc - 2; i++) {
        std::ifstream infile(argv[i]); //  open the input file for reading
        if (!infile) { // if file can't be opened, need to let the user know
            std::cerr << "Error: could not open file: " << argv[i] << std::endl;
            exit(1);
        }

        std::string str;
        while (getline(infile, str)){ //Read a line from the file
            str = clean(str); // remove comments, leading and trailing whitespace
            if (str == "") { //Ignore empty lines
                continue;
            }
            instructions.push_back(str); // TODO This will need to change for labels
        }
        infile.close();
    }

    // determine numbers for static members
    std::unordered_map<std::string, int> static_member;
    int i = 0;
    while (i<instructions.size()){
        if (instructions[i] == ".data"){
            break;
        }
        i++;
    }

        
    int label_num = 0;
    while(instructions[i+1]!=".text"){
            std::vector<std::string> terms = split(instructions[i+1], WHITESPACE+",()");
            std::string label = terms[0];
            label.pop_back();
            static_member[label] = label_num;
            label_num += (terms.size() - 2) * 4; //how many static fields a member has
            i++;
    }


    std::unordered_map<std::string, int> instruction_labels;
    i = 0;
    while (i<instructions.size()){
        if (instructions[i] == ".main"){
            break;
        }
        i++;
    }

    int label_num = 0;
    while(i<instructions.size()){
        //insturctions.zise()-i * 4

            std::vector<std::string> terms = split(instructions[i+1], WHITESPACE+",()");
            std::string label = terms[0];
            label.pop_back();
            static_member[label] = label_num;
            label_num += (terms.size() - 2) * 4; //how many static fields a member has
            i++;
     }

    /** Phase 2
     * Process all static memory, output to static memory file
     * TODO: All of this
    */

    /** Phase 3
     * Process all instructions, output to instruction memory file
     * TODO: Almost all of this, it only works for adds
     */
    for(std::string inst : instructions) {
        std::vector<std::string> terms = split(inst, WHITESPACE+",()");
        std::string inst_type = terms[0];
        if (inst_type == "add") {
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 32),inst_outfile);
        }
        else if (inst_type == "sub") {
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 34),inst_outfile);
        }
        /// begin question encode_Itype function correct? it's in project1.h
        /// the thing is I convert to binary but I do not know how many bits to allocate. 
        /// How to know? And how to do?
        else if (inst_type == "addi") {
            write_binary(encode_Itype(8, registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
        }
        /// end question
        else if (inst_type == "mult"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 24), inst_outfile);
        }
        else if (inst_type == "div"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 27), inst_outfile);
        }
        else if (inst_type == "mflo"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 19), inst_outfile);
        }
        else if (inst_type == "mfhi"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 16), inst_outfile);
        }

        /// begin question: since it's Rtype, I am using encode_Rtype(). Is it correct to have 0 in place
        /// of the second source register?
        else if (inst_type == "sll"){
            write_binary(encode_Rtype(0, registers[terms[2]], 0, registers[terms[1]], stoi(terms[3]), 0), inst_outfile);
        }
        else if (inst_type == "slr"){
            write_binary(encode_Rtype(0, registers[terms[2]], 0, registers[terms[1]], stoi(terms[3]), 2), inst_outfile);
        }
        /// end question

        else if (inst_type == "lw") {
            write_binary(encode_Itype(35,registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "sw") {
            write_binary(encode_Itype(43,registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "slt"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 42), inst_outfile);
        }
        else if (inst_type == "beq") {
            write_binary(encode_Itype(4,registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "bne") {
            write_binary(encode_Itype(5,registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "jalr"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 9), inst_outfile);
        }
        else if (inst_type == "jr"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 8), inst_outfile);
        }

        /// begin question. For jType instructions, how do I know the address of label?
        else if (inst_type == "j"){
            write_binary(encode_Jtype(2, ), inst_outfile);
        }
        else if (inst_type == "jal"){
            write_binary(encode_Jtype(3, ), inst_outfile);
        }
        /// end question
        else if (inst_type == "la"){
            //write_binary(, inst_outfile);
        }

        // do I just write 000000 00000 00000 11010 00000 001100 to file?
        else if (inst_type == "syscall"){
            //write_binary(, inst_outfile);
        }



       
    }
}

#endif
