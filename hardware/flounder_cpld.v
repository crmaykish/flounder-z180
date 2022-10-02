module flounder_cpld(
    input CLK,
    input RST,
    input MREQ,
    input IOREQ,
    input R,
    input W,
    input [19:13] A,
    // input [1:0] AL,
    // input KB_CLK,
    // input KB_DATA,
    output [7:0] D, // TODO: Data bus might need to be inout eventually
    output ROMEN,
    output RAMEN,
    output RAMWR
);

// 32 KB at 0x0000
assign ROMEN = ~(~A[19] * ~A[18] * ~A[17] * ~A[16] * ~A[15] * ~MREQ * ~R);

// 16 KB at 0x8000
assign RAMEN = ~(~A[19] * ~A[18] * ~A[17] * ~A[16] * A[15] * ~A[14] * ~MREQ);

// active high, 0xC000
assign CPLDEN = ~A[19] * ~A[18] * ~A[17] * ~A[16] * A[15] * A[14] * ~MREQ * ~R;

assign RAMWR = W;

// If keyboard enabled (I/O 0x8000), output value to data bus
assign D = (CPLDEN) ? 8'b11110000 : 8'bZ;

endmodule
