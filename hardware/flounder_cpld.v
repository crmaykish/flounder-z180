module flounder_cpld(
    input CLK,
    input RST,
    input MREQ,
    input IOREQ,
    input R,
    input W,
    input [19:13] A,
    input KB_CLK,
    input KB_DATA,
    output [7:0] D, // TODO: Data bus might need to be inout eventually
    output ROMEN,
    output RAMEN,
    output PIOEN,
    output reg U0,
    output reg U1,
    output LCDEN
);

// 32 KB ROM at 0x0000
assign ROMEN = ~(~A[19] * ~A[18] * ~A[17] * ~A[16] * ~A[15] * ~MREQ * ~R);

// 32 KB SRAM at 0x8000
assign RAMEN = ~(~A[19] * ~A[18] * ~A[17] * ~A[16] * A[15] * ~MREQ);

// I/O 0x2000, active low
assign PIOEN = ~(~A[15] * ~A[14] * A[13] * ~IOREQ);

// I/O 0x4000, active low
assign CPLDEN = ~(~A[15] * A[14] * ~A[13] * ~IOREQ);

// I/O 0x6000, active high
assign LCDEN = ~A[15] * A[14] * A[13] * ~IOREQ;

// PS/2 keyboard handler

reg [3:0] kb_index = 0;
reg [7:0] kb_val = 0;
reg [7:0] temp_val = 0;
reg kb_clk_read = 0;
reg [3:0] sample_delay = 0;

always @(posedge CLK) begin
    if (~RST) begin
        kb_index <= 0;
        kb_val <= 0;
        temp_val <= 0;
		  U0 <= 0;
		  U1 <= 0;
    end
    else begin
        // PS/2 state machine

        if (~KB_CLK) begin
            // PS/2 clock line is active-low

            if (~kb_clk_read)
                sample_delay <= sample_delay + 1;

            // When the PS/2 clock line goes low, wait 8 CPU cycles and then sample the data line

            if (sample_delay == 8) begin
                case (kb_index)
                        0:begin
                            // Stop bit
                            U0 <= 1;    // LED goes high when a scan code starts
                        end
                        1: temp_val[0] <= KB_DATA;
                        2: temp_val[1] <= KB_DATA;
                        3: temp_val[2] <= KB_DATA;
                        4: temp_val[3] <= KB_DATA;
                        5: temp_val[4] <= KB_DATA;
                        6: temp_val[5] <= KB_DATA;
                        7: temp_val[6] <= KB_DATA;
                        8: temp_val[7] <= KB_DATA;
                        9: begin
                            // Stop bit
                            U0 <= 0;    // LED goes low when the scan code is completely read
                        end
                        10: kb_val <= temp_val;    // Parity bit, latch the complete scan code into the storage register
                    endcase

                    if (kb_index < 10)
                        kb_index <= kb_index + 1;
                    else
                        kb_index <= 0;

                    kb_clk_read <= 1;
            end
        end
        else begin
            // PS/2 clock line is inactive-high
            kb_clk_read <= 0;
            sample_delay <= 0;
        end

    end
end

// If the CPLD is selected on the address bus, output the last keyboard value on the data bus, else high-impedance
assign D = (~CPLDEN) ? kb_val : 8'bZ;

endmodule
