module adder_4bits(a,b,s,ci,co);
input [3:0] a;
input [3:0] b;
input ci;
output [3:0] s;
output co;
wire c1,c2,c3;
wire [3:0] g;
wire [3:0] p;

assign g[0] = a[0] & b[0];
assign p[0] = a[0] | b[0];
assign s[0] = g[0] ^ p[0] ^ ci;
assign c1 = g[0] | (p[0] & ci);

assign g[1] = a[1] & b[1];
assign p[1] = a[1] | b[1];
assign s[1] = g[1] ^ p[1] ^ c1;
assign c2 = g[1] | (p[1] & c1);

assign g[2] = a[2] & b[2];
assign p[2] = a[2] | b[2];
assign s[2] = g[2] ^ p[2] ^ c2;
assign c3 = g[2] | (p[2] & c2);

assign g[3] = a[3] & b[3];
assign p[3] = a[3] | b[3];
assign s[3] = g[3] ^ p[3] ^ c3;
assign co = g[3] | (p[3] & c3);

endmodule