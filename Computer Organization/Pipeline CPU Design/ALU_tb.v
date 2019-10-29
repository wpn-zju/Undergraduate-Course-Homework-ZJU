`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: ZJU
// Engineer: tangyi
//
// Create Date:   15:49:50 10/29/2011
// Design Name:   ALU
// Module Name:   ALU_tb.v
// Project Name:  MipsPipelineCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: ALU
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module ALU_tb;

	// Inputs
	reg [4:0] ALUCode;
	reg [31:0] A;
	reg [31:0] B;

	// Outputs
	wire [31:0] Result;

	// Instantiate the Unit Under Test (UUT)
	ALU uut (
		.Result(Result), 
		.ALUCode(ALUCode), 
		.A(A), 
		.B(B)
	);

	initial begin
		// Initialize Inputs
		ALUCode = 5'd0; A = 32'h00004012; B = 32'h1000200F;//add
		      
		// Add stimulus here
		#100 ALUCode = 5'd0;	A = 32'h40000000;	B = 32'h40000000;//add
		#100 ALUCode = 5'd1;	A = 32'hFF0C0E10;	B = 32'h10DF30FF;//and
		#100 ALUCode = 5'd2;	A = 32'hFF0C0E10;	B = 32'h10DF30FF;//xor
		#100 ALUCode = 5'd3;	A = 32'hFF0C0E10;	B = 32'h10DF30FF;//or
		#100 ALUCode = 5'd4;	A = 32'hFF0C0E10;	B = 32'h10DF30FF;//nor
		#100 ALUCode = 5'd5;	A = 32'h70F0C0E0;	B = 32'h10003054;//sub
		#100 ALUCode = 5'd6;	A = 32'hFF0C0E10;	B = 32'hFFFFE0FF;//andi
		#100 ALUCode = 5'd7;	A = 32'hFF0C0E10;	B = 32'hFFFFE0FF;//xori
		#100 ALUCode = 5'd8;	A = 32'hFF0C0E10;	B = 32'hFFFFE0FF;//ori
				
		#100 ALUCode = 5'd16;A = 32'h00000004;	B = 32'hFFFFE0FF;//sll
		#100 ALUCode = 5'd17;A = 32'h00000004;	B = 32'hFFFFE0FF;//srl
		#100 ALUCode = 5'd18;A = 32'h00000004;	B = 32'hFFFFE0FF;//sra
		#100 ALUCode = 5'd19;A = 32'hFF000004;	B = 32'h700000FF;//slt
		#100 ALUCode = 5'd20;A = 32'hFF000004;	B = 32'h700000FF;//sltu
		#100 $stop;

	end
      
endmodule

