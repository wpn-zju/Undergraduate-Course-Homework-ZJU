//******************************************************************************
//******************************************************************************

module MultiRegisters (
	// Outputs
	RsData, RtData,  

	// Inputs
	clk, reset, WriteData, WriteAddr, RegWrite, RsAddr, RtAddr
);

	input		clk;
	input   reset;
	// Info for register write port
	input [31:0]	 WriteData;
	input [4:0]	   WriteAddr;
	input		RegWrite;

	input [4:0]	RsAddr, RtAddr;

	// Data from register read ports
	output [31:0]	RsData;		// data output for read port A
	output [31:0]	RtData;		// data output for read port B

	// 32-register memory declaration
	reg [31:0]regs [31:0];
	


//******************************************************************************
// get data from read registers
//******************************************************************************

  assign RsData = (RsAddr == 5'b0) ? 32'b0 : regs[RsAddr];
	assign RtData = (RtAddr == 5'b0) ? 32'b0 : regs[RtAddr];

//******************************************************************************
// write to register if necessary 
//******************************************************************************

	always @ (posedge clk) begin
		if (RegWrite)
			regs[WriteAddr] <= WriteData;
	end

endmodule