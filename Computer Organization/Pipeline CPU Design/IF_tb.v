`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: ZJU
// Engineer: tangyi
//
// Create Date:   10:54:16 10/29/2011
// Design Name:   IF
// Module Name:   IF_tb.v
// Project Name:  MipsPipelineCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: IF
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module IF_tb_v;

	// Inputs
	reg clk;
	reg reset;
	reg Z;
	reg J;
	reg JR;
	reg PC_IFWrite;
	reg [31:0] JumpAddr;
	reg [31:0] JrAddr;
	reg [31:0] BranchAddr;

	// Outputs
	wire [31:0] Instruction_if;
	wire [31:0] PC;
	wire [31:0] NextPC_if;

	// Instantiate the Unit Under Test (UUT)
	IF uut (
		.clk(clk), 
		.reset(reset), 
		.Z(Z), 
		.J(J), 
		.JR(JR), 
		.PC_IFWrite(PC_IFWrite), 
		.JumpAddr(JumpAddr), 
		.JrAddr(JrAddr), 
		.BranchAddr(BranchAddr), 
		.Instruction_if(Instruction_if), 
		.PC(PC), 
		.NextPC_if(NextPC_if)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		reset = 1;
		JR = 0; J = 0;	Z = 0;
		PC_IFWrite = 1;
		JumpAddr = 32'd44;
		JrAddr = 32'd52;
		BranchAddr = 32'd4;

		// Wait 100 ns for global reset to finish
		#100 reset=0;
        
		// Add stimulus here
		#800 JR = 1; J = 0;	Z = 0;
		#200 JR = 0; J = 1;	Z = 0;
		#200 JR = 0; J = 0;	Z = 1;
		#200 JR = 0; J = 0;	Z = 0; PC_IFWrite = 0;
		#100 $stop;	

	end
	
	always #50 clk=~clk;

      
endmodule

