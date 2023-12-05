/*
Tung Nguyen
Alina Enikeeva
*/

#ifndef __PROJECT1_CPP__
#define __PROJECT1_CPP__

#include "project1.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc < 4) // Checks that at least 3 arguments are given in command line
    {
        std::cerr << "Expected Usage:\n ./assemble infile1.asm infile2.asm ... infilek.asm staticmem_outfile.bin instructions_outfile.bin\n" << std::endl;
        exit(1);
    }
    //Prepare output files
    std::ofstream inst_outfile, static_outfile;
    static_outfile.open(argv[argc - 2], std::ios::binary);
    inst_outfile.open(argv[argc - 1], std::ios::binary);
    std::vector<std::string> instructions;                              //instructions only
    std::vector<std::string> instructionsLabels;                        //instructions and labels
    std::vector<std::string> raw_instructions;                          //the whole file

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
            raw_instructions.push_back(str); // TODO This will need to change for labels
            std::vector<std::string> termsth = split(str, WHITESPACE+",()");
            if (std::find(valid_instructions.begin(), valid_instructions.end(), termsth[0]) != valid_instructions.end()) {
                instructions.push_back(str);
            }
            if (std::find(valid_instructions.begin(), valid_instructions.end(), termsth[0]) != valid_instructions.end() || (termsth.size() == 1 && termsth[0].back() == ':')) {
                instructionsLabels.push_back(str);
            }
        }
    }
/*
    for (std::string inst : instructionsLabels){
        std::cout << inst << std::endl;
    }
*/
    // determine numbers for static members
    std::unordered_map<std::string, int> static_member;
    int i = 1;
    int static_end = 0;
    while (i<raw_instructions.size()){
        if (raw_instructions[i-1] == ".data"){
            int label_num = 0;
            while(raw_instructions[i]!=".text"){
                std::vector<std::string> terms1 = split(raw_instructions[i], WHITESPACE+",()");
                std::string label = terms1[0];
                label.pop_back();
                if (terms1[1]==".asciiz") {
                    static_member[label] = label_num;
                    std::cout << label_num << std::endl;
                    std::vector<std::string> staticString = split(raw_instructions[i], "\"");
                    std::string word = staticString[1];
                    label_num += (word.size() + 1) * 4;
                } else {
                    static_member[label] = label_num;
                    label_num += (terms1.size() - 2) * 4;
                } //how many static fields a member has
                i++;
            }
        static_end = label_num;
        }
        i++;
    }
    static_member["_END_OF_STATIC_MEMORY_"]= static_end;
    
    for(auto el: static_member){
        std::cout << el.first <<el.second<< std::endl;
    }

    std::unordered_map<std::string, int> instruction_labels;
    std::vector<std::string> label_names;
    i = 0;
    while (i<raw_instructions.size()){
        std::vector<std::string> terms2 = split(raw_instructions[i], WHITESPACE+",()");
        if (terms2.size() == 1 && terms2[0].back() == ':'){ //if label
            terms2[0].pop_back();
            label_names.push_back(terms2[0]);
        }
            i++;
    }
    
    
    i = 0;
    int count_labels=0;    
    while(i<instructionsLabels.size()) {
        std::vector<std::string> terms3 = split(instructionsLabels[i], WHITESPACE+",()");
        if (terms3.size() == 1 && terms3[0] != "syscall"){ //if label
            count_labels += 1;      
            
            // find out which line each instruction is on (it is the next line after 
            // the label (i+1) minus the number of lines that contain only labels and
            // no instructions)
            instruction_labels[label_names[count_labels-1]] = (i + 1 - count_labels);
            
        }
        i++;
    }    
    
    /*
    for (auto el : instruction_labels){
        std::cout << el.first << el.second << std::endl;
    }
    */
    
    
    //Phase 2
    std::vector<int> staticToWrite;
    i = 1;

    //assign bytes to static memory members
    while (i<raw_instructions.size()){
        
        if (raw_instructions[i-1] == ".data"){
            
            while(raw_instructions[i]!=".text"){
                std::vector<std::string> terms_1 = split(raw_instructions[i], WHITESPACE+",()");
                if(terms_1[1]==".asciiz"){
                    std::vector<std::string> staticString = split(raw_instructions[i], "\"");
                    std::cout << staticString[1] << std::endl;
                    std::string word = staticString[1];
                    for (int x = 0; x < word.size(); x++) {
                        //std::cout << word[x] << std::endl;
                        staticToWrite.push_back(int(word[x]));
                    }
                    staticToWrite.push_back(0);
                } else {
                    for (int j = 2; j < terms_1.size(); j++) {
                        if (instruction_labels.find(terms_1[j]) != instruction_labels.end()) { 
                            //Find the label in the instruction labels map
                            staticToWrite.push_back(instruction_labels.at(terms_1[j])*4);           //Add the value associated with that key to toWrite
                        } else {                    
                            staticToWrite.push_back(stoi(terms_1[j]));                                  //Else just add it directly
                        }
                    }
                }
                i++;
            }
        }
        i++;
    }
    
    for (int j = 0; j < staticToWrite.size(); j++) {
        write_binary(staticToWrite[j],static_outfile);
    }
    

    /** Phase 3
     * Process all instructions, output to instruction memory file
     * TODO: Almost all of this, it only works for adds
     */
    int line_number = 0;
    for(std::string inst : instructions) {
        std::vector<std::string> terms = split(inst, WHITESPACE+",()");
        std::string inst_type = terms[0];
        if (inst_type == "add") {
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 32),inst_outfile);
        }
        else if (inst_type == "sub") {
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 34),inst_outfile);
        }
        else if (inst_type == "addi") {
            //bonus handling immediates that are larger than 16 bits, but smaller than 32 bits
            int imm = stoi(terms[3]);
            
            if (imm > 32767){
                int x = imm&32767;
                imm = imm >> 15;
                write_binary(encode_Itype(8, registers[terms[2]], registers[terms[1]], x), inst_outfile);
                write_binary(encode_Itype(8, registers.at("$0"), registers.at("$at"), imm), inst_outfile);
                write_binary(encode_Rtype(0, 0, registers.at("$at"), registers.at("$at"), 15, 0), inst_outfile);
                write_binary(encode_Rtype(0, registers[terms[1]], registers.at("$at"), registers[terms[1]], 0, 32),inst_outfile);
            }
            
            else {
                write_binary(encode_Itype(8, registers[terms[2]], registers[terms[1]], stoi(terms[3])), inst_outfile);
            
            }
            
        }
        else if (inst_type == "mult"){
           write_binary(encode_Rtype(0, registers[terms[1]], registers[terms[2]], 0, 0, 24), inst_outfile);
        }
        else if (inst_type == "div"){
            write_binary(encode_Rtype(0, registers[terms[1]], registers[terms[2]],0, 0, 26), inst_outfile);
        }
        else if (inst_type == "mflo"){
           write_binary(encode_Rtype(0, 0, 0, registers[terms[1]], 0, 18), inst_outfile);
        }
        else if (inst_type == "mfhi"){
            write_binary(encode_Rtype(0, 0, 0, registers[terms[1]], 0, 16), inst_outfile);
        }
        
        else if (inst_type == "sll"){
            write_binary(encode_Rtype(0, 0, registers[terms[2]], registers[terms[1]], stoi(terms[3]), 0), inst_outfile);
        }
        else if (inst_type == "srl"){
            write_binary(encode_Rtype(0, 0, registers[terms[2]], registers[terms[1]], stoi(terms[3]), 2), inst_outfile);
        }
   
        //lw $t0, 4($s0): rt - $t0 (term 1), rs - $s0 (term3), offset - 4 (term2)
        else if (inst_type == "lw") {
            //std::cout << "" << std::endl; registers[terms[3]]<< registers[terms[1]]<< registers[terms[2]]<< std::endl;
            write_binary(encode_Itype(35,registers[terms[3]], registers[terms[1]], stoi(terms[2])), inst_outfile);
            std::cout << 10 << std::endl;
        }
        else if (inst_type == "sw") {
            write_binary(encode_Itype(43,registers[terms[3]], registers[terms[1]], stoi(terms[2])), inst_outfile);
            std::cout << 11 << std::endl;
        }
        else if (inst_type == "slt"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 42), inst_outfile);
            std::cout << 12 << std::endl;
        }

        else if (inst_type == "beq") {
            if(instruction_labels.find(terms[3]) != instruction_labels.end()) {
                int branchTo = instruction_labels.at(terms[3]);
                write_binary(encode_Itype(4,registers[terms[1]], registers[terms[2]], branchTo - line_number - 1), inst_outfile);
                std::cout << 14 << std::endl;
            } else {
                write_binary(encode_Itype(4,registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
            }
        }
        else if (inst_type == "bne") {
            if(instruction_labels.find(terms[3]) != instruction_labels.end()) {
                int branchTo = instruction_labels.at(terms[3]);
                write_binary(encode_Itype(5,registers[terms[1]], registers[terms[2]], branchTo - line_number - 1), inst_outfile);
                std::cout << 14 << std::endl;
            } else {
                write_binary(encode_Itype(5,registers[terms[1]], registers[terms[2]], stoi(terms[3])), inst_outfile);
            }
            
        }

        else if (inst_type == "jalr"){
            write_binary(encode_Rtype(0, registers[terms[1]], 0, registers.at("$ra"), 0, 9), inst_outfile);
            std::cout << 15 << std::endl;
        }
        else if (inst_type == "jr"){
            write_binary(encode_Rtype(0, registers[terms[1]], 0, 0, 0, 8), inst_outfile);
            std::cout << 16 << std::endl;
        }
        
        else if (inst_type == "j"){       
            int branchTo = instruction_labels.at(terms[1]);
            write_binary(encode_Jtype(2, branchTo), inst_outfile);
            std::cout << 17 << std::endl;
        }
        else if (inst_type == "jal"){       
            int branchTo = instruction_labels.at(terms[1]);
            write_binary(encode_Jtype(3, branchTo), inst_outfile);
            std::cout << 18 << std::endl;
        }
        
        else if (inst_type == "la"){  
            int branchTo = static_member.at(terms[2]);
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], branchTo), inst_outfile);
            std::cout << 19 << std::endl;
            }

        else if (inst_type == "syscall"){
            write_binary(encode_Rtype(0, 0, 0, 26, 0, 12), inst_outfile);
            std::cout << 20 << std::endl;
        }


        // bonus instructions
        else if (inst_type == "mov"){
            write_binary(encode_Rtype(0,registers[terms[2]], registers.at("$0"), registers[terms[1]], 0, 32),inst_outfile);
            std::cout << 1 << std::endl;
        }

        else if (inst_type == "li") {
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], stoi(terms[2])), inst_outfile);
            std::cout << 3 << std::endl;
        }
        else if (inst_type == "sgt"){
            write_binary(encode_Rtype(0, registers[terms[3]], registers[terms[2]], registers[terms[1]], 0, 42), inst_outfile);
            
        }
        else if (inst_type == "sge"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers.at("$at"), 0, 42), inst_outfile);
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], 1), inst_outfile);
            write_binary(encode_Itype(4,registers.at("$at"), registers.at("$0"), 1), inst_outfile);
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], 0), inst_outfile);
        }
        else if (inst_type == "sle"){
            write_binary(encode_Rtype(0, registers[terms[3]], registers[terms[2]], registers.at("$at"), 0, 42), inst_outfile);
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], 1), inst_outfile);
            write_binary(encode_Itype(4,registers.at("$at"), registers.at("$0"), 1), inst_outfile);
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], 0), inst_outfile);
        }
        else if (inst_type == "seq"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 42), inst_outfile);
            write_binary(encode_Rtype(0, registers[terms[3]], registers[terms[2]], registers[terms[1]], 0, 42), inst_outfile);
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[2]], 1), inst_outfile);
            write_binary(encode_Rtype(0, registers[terms[1]], registers[terms[2]], registers[terms[1]], 0, 42), inst_outfile);
        }
         else if (inst_type == "sne"){
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], 1), inst_outfile);
            write_binary(encode_Itype(5,registers[terms[2]], registers[terms[3]], 1), inst_outfile);     
            write_binary(encode_Itype(8, registers.at("$0"), registers[terms[1]], 0), inst_outfile);
        }

        else if (inst_type == "bge") {
            int branchTo = instruction_labels.at(terms[3]);
            write_binary(encode_Rtype(0, registers[terms[1]], registers[terms[2]], registers.at("$at"), 0, 42), inst_outfile);
            write_binary(encode_Itype(4,registers.at("$at"),registers.at("$0"), branchTo - line_number - 1), inst_outfile);
            
        }
        else if (inst_type == "bgt"){
            int branchTo = instruction_labels.at(terms[3]);
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[1]], registers.at("$at"), 0, 42), inst_outfile);
            write_binary(encode_Itype(5,registers.at("$at"),registers.at("$0"), branchTo - line_number - 1), inst_outfile);
            
        }

         else if (inst_type == "ble"){
            int branchTo = instruction_labels.at(terms[3]);
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[1]], registers.at("$at"), 0, 42), inst_outfile);
            write_binary(encode_Itype(4, registers.at("$at"), registers.at("$0"), branchTo - line_number - 1), inst_outfile);
        }
        else if (inst_type == "blt"){
            int branchTo = instruction_labels.at(terms[3]);
            write_binary(encode_Rtype(0, registers[terms[1]], registers[terms[2]], registers.at("$at"), 0, 42), inst_outfile);
            write_binary(encode_Itype(5,registers.at("$at"),registers.at("$0"), branchTo - line_number - 1), inst_outfile);
    
        }
        else if (inst_type == "abs"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers.at("$0"), registers[terms[1]], 0, 42), inst_outfile);
            write_binary(encode_Itype(5,registers[terms[1]], registers.at("$0"), 1), inst_outfile);
            write_binary(encode_Rtype(0,registers[terms[2]], registers.at("$0"), registers[terms[1]], 0, 32),inst_outfile);
            write_binary(encode_Rtype(0,registers[terms[2]], registers.at("$0"), registers[terms[2]], 0, 34),inst_outfile);
            write_binary(encode_Rtype(0,registers[terms[2]], registers.at("$0"), registers[terms[1]], 0, 32),inst_outfile);
        }

        else if (inst_type == "and"){
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 36),inst_outfile);
        }

        else if (inst_type == "or"){
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 37),inst_outfile);
        
        }
        else if (inst_type == "nor"){
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 39),inst_outfile);
        
        }
        else if (inst_type == "xor"){
            write_binary(encode_Rtype(0,registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 38),inst_outfile);
        
        }
        else if (inst_type == "andi"){
            write_binary(encode_Itype(12, registers[terms[2]], registers[terms[1]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "ori"){
            write_binary(encode_Itype(13, registers[terms[2]], registers[terms[1]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "xori"){
            write_binary(encode_Itype(14, registers[terms[2]], registers[terms[1]], stoi(terms[3])), inst_outfile);
        }
        else if (inst_type == "lui"){
            write_binary(encode_Itype(15, registers[terms[2]], registers[terms[1]], stoi(terms[3])), inst_outfile);
        }
   
        line_number++;
    }
    
    static_outfile.close();
    inst_outfile.close();  
}


#endif