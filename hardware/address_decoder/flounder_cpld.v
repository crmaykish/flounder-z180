module flounder_cpld(
    input CLK,
    input RST,
    input MREQ,
    input IOREQ,
    input R,
    input W,
    input [19:13] A,
    output ROMEN,
    output RAMEN,
    output RAMRW
);

// 32 KB at 0x0000
assign ROMEN = ~(~A[19] * ~A[18] * ~A[17] * ~A[16] * ~A[15] * ~MREQ * ~R);

// 32 KB at 0x8000
assign RAMEN = ~(~A[19] * ~A[18] * ~A[17] * ~A[16] * A[15] * ~MREQ);

assign RAMWR = ~(CLK * ~W);

endmodule
