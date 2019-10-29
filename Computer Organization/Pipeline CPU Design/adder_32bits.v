module adder_32bits(a,b,s,ci,co);
  input [31:0] a;
  input [31:0] b;
  input ci;
  output co;
  output [31:0] s;
  wire [31:0] s0;
  wire [31:0] s1;
  wire c07,c011,c015,c019,c023,c027,c031;
  wire c17,c111,c115,c119,c123,c127,c131;
  wire c3,c7,c11,c15,c19,c23,c27;
  
  assign c7 = (c3 & c17) | c07;
  assign c11 = (c7 & c111) | c011;
  assign c15 = (c11 & c115) | c015;
  assign c19 = (c15 & c119) | c019;
  assign c23 = (c19 & c123) | c023;
  assign c27 = (c23 & c127) | c027;
  assign co = (c27 & c131) | c031;
  
  assign s[7:4] = c3?s1[7:4]:s0[7:4];
  assign s[11:8] = c7?s1[11:8]:s0[11:8];
  assign s[15:12] = c11?s1[15:12]:s0[15:12];
  assign s[19:16] = c15?s1[19:16]:s0[19:16];
  assign s[23:20] = c19?s1[23:20]:s0[23:20];
  assign s[27:24] = c23?s1[27:24]:s0[27:24];
  assign s[31:28] = c27?s1[31:28]:s0[31:28];
  
  adder_4bits adder_4bits01(.a(a[3:0]), .b(b[3:0]), .s(s[3:0]), .ci(ci), .co(c3));
  adder_4bits adder_4bits02(.a(a[7:4]), .b(b[7:4]), .s(s1[7:4]), .ci(1), .co(c17));
  adder_4bits adder_4bits03(.a(a[7:4]), .b(b[7:4]), .s(s0[7:4]), .ci(0), .co(c07));
  adder_4bits adder_4bits04(.a(a[11:8]), .b(b[11:8]), .s(s1[11:8]), .ci(1), .co(c111));
  adder_4bits adder_4bits05(.a(a[11:8]), .b(b[11:8]), .s(s0[11:8]), .ci(0), .co(c011));
  adder_4bits adder_4bits06(.a(a[15:12]), .b(b[15:12]), .s(s1[15:12]), .ci(1), .co(c115));
  adder_4bits adder_4bits07(.a(a[15:12]), .b(b[15:12]), .s(s0[15:12]), .ci(0), .co(c015));
  adder_4bits adder_4bits08(.a(a[19:16]), .b(b[19:16]), .s(s1[19:16]), .ci(1), .co(c119));
  adder_4bits adder_4bits09(.a(a[19:16]), .b(b[19:16]), .s(s0[19:16]), .ci(0), .co(c019));
  adder_4bits adder_4bits10(.a(a[23:20]), .b(b[23:20]), .s(s1[23:20]), .ci(1), .co(c123));
  adder_4bits adder_4bits11(.a(a[23:20]), .b(b[23:20]), .s(s0[23:20]), .ci(0), .co(c023));
  adder_4bits adder_4bits12(.a(a[27:24]), .b(b[27:24]), .s(s1[27:24]), .ci(1), .co(c127));
  adder_4bits adder_4bits13(.a(a[27:24]), .b(b[27:24]), .s(s0[27:24]), .ci(0), .co(c027));
  adder_4bits adder_4bits14(.a(a[31:28]), .b(b[31:28]), .s(s1[31:28]), .ci(1), .co(c131));
  adder_4bits adder_4bits15(.a(a[31:28]), .b(b[31:28]), .s(s0[31:28]), .ci(0), .co(c031));

endmodule