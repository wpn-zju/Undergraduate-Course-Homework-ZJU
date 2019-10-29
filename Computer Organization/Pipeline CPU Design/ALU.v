//******************************************************************************
// MIPS verilog model
//
// ALU.v
//
// The ALU performs all the arithmetic/logical integer operations 
// specified by the ALUsel from the decoder. 
// 
// verilog written QMJ
// modified by 
// modified by 
//
//******************************************************************************

module ALU (
	// Outputs
	Result,
	// Inputs
	ALUCode, A, B
);

	input [4:0]	ALUCode;				// Operation select
	input [31:0]	A, B;

	output [31:0]	Result;

//******************************************************************************
// Shift operation: ">>>" will perform an arithmetic shift, but the operand
// must be reg signed
//******************************************************************************
	reg signed [31:0] B_reg;
	
	always @(B) begin
		B_reg = B;
	end

	
// Decoded ALU operation select (ALUsel) signals
  parameter	 alu_add = 5'b00000;
  parameter	 alu_and = 5'b00001;
  parameter	 alu_xor = 5'b00010;
  parameter	 alu_or  = 5'b00011;
  parameter	 alu_nor = 5'b00100;
  parameter	 alu_sub = 5'b00101;
  parameter	 alu_andi= 5'b00110;
	parameter	 alu_xori= 5'b00111;
	parameter	 alu_ori = 5'b01000;
	parameter  alu_jr  = 5'b01001;
	parameter	 alu_beq = 5'b01010;
  parameter	 alu_bne = 5'b01011;
	parameter	 alu_bgez= 5'b01100;
  parameter	 alu_bgtz= 5'b01101;
  parameter	 alu_blez= 5'b01110;
  parameter	 alu_bltz= 5'b01111;
	parameter  alu_sll = 5'b10000;
	parameter	 alu_srl = 5'b10001;
	parameter	 alu_sra = 5'b10010;	
	parameter	 alu_slt = 5'b10011;
  parameter	 alu_sltu= 5'b10100;
   
  wire Binvert;
  wire [31:0] sum_alu;
  wire [31:0] and_alu;
  wire [31:0] xor_alu;
  wire [31:0] or_alu;
  wire [31:0] nor_alu;
  wire [31:0] andi_alu;
  wire [31:0] xori_alu;
  wire [31:0] jr_alu;
  wire [31:0] ori_alu;
  wire [31:0] sll_alu;
  wire [31:0] srl_alu;
  wire [31:0] sra_alu;
  wire [31:0] slt_alu;
  wire [31:0] sltu_alu;
  
  assign Binvert = ~(ALUCode == alu_add);
  assign and_alu [31:0] = A & B;
  assign xor_alu [31:0] = A ^ B;
  assign or_alu  [31:0] = A | B;
  assign nor_alu [31:0] = ~(A | B);
  assign andi_alu[31:0] = A & {16'd0,B[15:0]};
  assign xori_alu[31:0] = A ^ {16'd0,B[15:0]};
  assign jr_alu  [31:0] = A;
  assign ori_alu [31:0] = A | {16'd0,B[15:0]};
  assign sll_alu [31:0] = B << A;
  assign srl_alu [31:0] = B >> A;
  assign sra_alu [31:0] = B_reg >>> A;
  assign slt_alu [31:0] = (A[31] && (~B[31])) || ((A[31]~^B[31]) && sum_alu[31]);
  assign sltu_alu[31:0] = A[31:0] < B[31:0] ? 1 : 0;

	
//******************************************************************************
// ALU Result datapath
//******************************************************************************

  reg [31:0] Result;
  always@(*)
  begin
    case(ALUCode)
      alu_add:  Result <= sum_alu;
      alu_and:  Result <= and_alu;
      alu_xor:  Result <= xor_alu;
      alu_or :  Result <= or_alu;
      alu_nor:  Result <= nor_alu;
      alu_sub:  Result <= sum_alu;
      alu_andi: Result <= andi_alu;
    	alu_xori: Result <= xori_alu;
    	alu_jr:   Result <= jr_alu;
	    alu_ori:  Result <= ori_alu;
    	alu_sll:  Result <= sll_alu;
    	alu_srl:  Result <= srl_alu;
    	alu_sra:  Result <= sra_alu;	
      alu_slt:  Result <= slt_alu;
      alu_sltu: Result <= sltu_alu;
      default:  Result <= 32'b0;
    endcase
  end
  
  adder_32bits adder(.a(A[31:0]), .b(B[31:0] ^ {32{Binvert}}), .s(sum_alu[31:0]), .ci(Binvert), .co());
  
endmodule