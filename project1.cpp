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
    static_outfile.open(argv[argc - 2], std::ios::binary);
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
            std::vector<std::string> terms1 = split(instructions[i+1], WHITESPACE+",()");
            std::string label = terms1[0];
            label.pop_back();
            static_member[label] = label_num;
            label_num += (terms1.size() - 2) * 4; //how many static fields a member has
            i++;
    }


    std::unordered_map<std::string, int> instruction_labels;
    i = 0;
    while (i<instructions.size()){
        if (instructions[i] == "main:"){
            break;
        }
        i++;
    }
    std::vector<std::string> label_names;
    for(int j = i; j < instructions.size(); j++) {
        std::vector<std::string> terms2 = split(instructions[j], WHITESPACE+",()");
        if (terms2.size() == 1 && terms2[0] != "syscall"){ //if label
            terms2[0].pop_back();
            label_names.push_back(terms2[0]);
        }
    }
    
    int count_labels=0;
    
    
    for(int j = i; j < instructions.size(); j++) {
        std::vector<std::string> terms3 = split(instructions[j], WHITESPACE+",()");
        if (terms3.size() == 1 && terms3[0] != "syscall"){ //if label
            count_labels += 1;
            instruction_labels[label_names[count_labels-1]] = (j - i + 1 - count_labels);
        }
    }    


  for (auto el: instruction_labels){
    std::cout<<el.first<< " " << el.second << std::endl;
  }
    

    //Phase 2
    i = 0;
    std::vector<int> staticToWrite;
    while (i<instructions.size()){
        if (instructions[i] == ".data"){
            i++;
            break;
        }
        i++;
    }
    while (i<instructions.size()){
        if (instructions[i] == ".text"){
            break;
        }
        std::vector<std::string> terms_1 = split(instructions[i], WHITESPACE+",()");
        for (int j = 2; j < terms_1.size(); j++) {
            if (instruction_labels.find(terms_1[j]) != instruction_labels.end()) {    //Find the label in the instruction labels map
                staticToWrite.push_back(instruction_labels.at(terms_1[j])*4);           //Add the value associated with that key to toWrite
            } else {
                staticToWrite.push_back(stoi(terms_1[j]));                                  //Else just add it directly
            }
        }
        i++;
    }                                                                                 //Completed toWrite

    for (int j = 0; j < staticToWrite.size(); j++) {
        write_binary(staticToWrite[j],static_outfile);
    }

 //for (auto el: instructions){
   // std::cout<<el<<std::endl;
 // }

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
        else if (inst_type == "addi") {
            write_binary(encode_Itype(8, registers[terms[2]], registers[terms[1]], stoi(terms[3])), inst_outfile);
        }
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
        }
        else if (inst_type == "sw") {
            write_binary(encode_Itype(43,registers[terms[3]], registers[terms[1]], stoi(terms[2])), inst_outfile);
        }
        else if (inst_type == "slt"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 42), inst_outfile);
        }


        else if (inst_type == "beq") {
            int branchTo = instruction_labels.at(terms[3]);
            //write_binary(line_Num, inst_outfile);
            //int branchTo = instruction_labels.at(terms[3]);
            //write_binary(currentLine, inst_outfile);
        }
        else if (inst_type == "bne") {
            int currentLine = std::distance(instructions.begin(),find(instructions.begin(), instructions.end(),inst));
            int branchTo = instruction_labels.at(terms[3]);
            write_binary(encode_Itype(5,registers[terms[1]], registers[terms[2]], branchTo - currentLine - 1), inst_outfile);
        }
        

        else if (inst_type == "jalr"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 9), inst_outfile);
        }
        else if (inst_type == "jr"){
            write_binary(encode_Rtype(0, registers[terms[2]], registers[terms[3]], registers[terms[1]], 0, 8), inst_outfile);
        }
        /*
        else if (inst_type == "j"){
            //write_binary(encode_Jtype(2, ), inst_outfile);
        }
        else if (inst_type == "jal"){
            //write_binary(encode_Jtype(3, ), inst_outfile);
        }
        /// end question
        else if (inst_type == "la"){
            //write_binary(, inst_outfile);
        }

        // do I just write 000000 00000 00000 11010 00000 001100 to file?
        else if (inst_type == "syscall"){
            //write_binary(, inst_outfile);
        }*/
       

    }

    static_outfile.close();
    inst_outfile.close();  
    }


#endif
