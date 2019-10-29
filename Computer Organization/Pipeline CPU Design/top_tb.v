`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: ZJU
// Engineer: tangyi
//
// Create Date:   16:00:23 10/29/2011
// Design Name:   MipsPipelineCPU
// Module Name:   top_tb.v
// Project Name:  MipsPipelineCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: MipsPipelineCPU
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module top_tb_v;

	// Inputs
	reg clk;
	reg reset;

	// Outputs
	wire [2:0] JumpFlag;
	wire [31:0] Instruction_id;
	wire [31:0] ALU_A;
	wire [31:0] ALU_B;
	wire [31:0] ALUResult;
	wire [31:0] PC;
	wire [31:0] MemDout_wb;
	wire Stall;
	

	// Instantiate the Unit Under Test (UUT)
	MipsPipelineCPU uut (
		.clk(clk), 
		.reset(reset), 
		.JumpFlag(JumpFlag), 
		.Instruction_id(Instruction_id), 
		.ALU_A(ALU_A), 
		.ALU_B(ALU_B), 
		.ALUResult(ALUResult), 
		.PC(PC), 
		.MemDout_wb(MemDout_wb), 
		.Stall(Stall) 
		
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		// Wait 100 ns for global reset to finish
		#100 reset=0;
     	#1950 $stop;

	end
	
	always #50 clk=~clk;
      
endmodule

