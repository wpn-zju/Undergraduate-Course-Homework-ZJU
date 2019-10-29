`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: ZJU
// Engineer: tangyi
//
// Create Date:   11:07:48 10/29/2011
// Design Name:   Decode
// Module Name:   Decode_tb.v
// Project Name:  MipsPipelineCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Decode
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module Decode_tb_v;

	// Inputs
	reg [31:0] Instruction;

	// Outputs
	wire MemtoReg;
	wire RegWrite;
	wire MemWrite;
	wire MemRead;
	wire [4:0] ALUCode;
	wire ALUSrcA;
	wire ALUSrcB;
	wire RegDst;
	wire J;
	wire JR;

	// Instantiate the Unit Under Test (UUT)
	Decode uut (
		.MemtoReg(MemtoReg), 
		.RegWrite(RegWrite), 
		.MemWrite(MemWrite), 
		.MemRead(MemRead), 
		.ALUCode(ALUCode), 
		.ALUSrcA(ALUSrcA), 
		.ALUSrcB(ALUSrcB), 
		.RegDst(RegDst), 
		.J(J), 
		.JR(JR), 
		.Instruction(Instruction)
	);

	initial begin
		// Initialize Inputs
		Instruction = 32'h0800000b;//j later(later address is 2Ch)		
        
		// Add stimulus here
		#100 Instruction = 32'h20080042;//addi $t0,$0,42
		#100 Instruction = 32'h01095022;//sub $t2,$t0,$t1
		#100 Instruction = 32'h01485825;//or $t3,$t2,$t0
		#100 Instruction = 32'hac0b000c;//sw $t3,0C($0)
		#100 Instruction = 32'h8d2c0008;//lw $t4,08($t1)
		#100 Instruction = 32'h000c4080;//sll $t0,$t4,2		
		#100 Instruction = 32'h012a582b ;//sltu $t3,$t1,$t2		
		#100 Instruction = 32'h14000001 ;//bne $0,$0,end(end address is 34h)
		#100 Instruction = 32'h1000fff4 ;//beq $0,$0,earlier(earlier address is 4h)			
             	#100 $stop;
	end
      
endmodule

