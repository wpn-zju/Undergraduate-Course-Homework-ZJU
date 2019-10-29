module InstructionROM(addr,dout);
	input [5 : 0] addr;
	output [31 : 0] dout;
	//
	reg [31 : 0] dout;
	always @(*)
		case (addr)
			6'd0:   dout=32'h0800000b ;
			6'd1:   dout=32'h20080042 ;
			6'd2:   dout=32'h20090004 ;
			6'd3:   dout=32'h01095022 ;
			6'd4:   dout=32'h01485825 ;
			6'd5:   dout=32'hac0b000c ;
			6'd6:   dout=32'h8d2c0008 ;
			6'd7:   dout=32'h000c4080 ;
			6'd8:   dout=32'h8d2b0008 ;
			6'd9:   dout=32'h012a582b ;
			6'd10:  dout=32'h0800000a ;
			6'd11:  dout=32'h14000001 ;
			6'd12:  dout=32'h1000fff4 ;
			6'd13:  dout=32'h00000000 ;
			default:dout=32'h00000000 ;
		endcase	
endmodule