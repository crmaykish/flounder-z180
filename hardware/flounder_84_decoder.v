module flounder_84_decoder(
	input CLK,
	input CLK2,
	input RST,
	input [19:0] ADDR,
	output [7:0] DATA,
	
	output WAIT,
	
	input R,
	input W,
	input MREQ,
	input IOREQ,
	input M1,
	
	output NMI,
	output [2:0] INT,
	output RAMEN,
	output ROMEN,
	output USBEN,
	output PIOEN,
	output LCDEN0,
	output LCDEN1,
	
	input USBINT,
	
	output CLK_ASCI,
	
	input KB_CLK,
	input KB_DATA,
	
	output [2:0] LED,
	output [7:0] USER
);

wire CPLDEN;

// 32 KB ROM at 0x0000
assign ROMEN = ~(~ADDR[19] * ~ADDR[18] * ~ADDR[17] * ~ADDR[16] * ~ADDR[15] * ~MREQ * ~R);

// 32 KB SRAM at 0x8000
assign RAMEN = ~(~ADDR[19] * ~ADDR[18] * ~ADDR[17] * ~ADDR[16] * ADDR[15] * ~MREQ);

// I/O 0x2000, active low
assign PIOEN = ~(~ADDR[15] * ~ADDR[14] * ADDR[13] * ~IOREQ);

// I/O 0x4000, active low
//assign CPLDEN = ~(~ADDR[15] * ADDR[14] * ~ADDR[13] * ~IOREQ);

// I/O 0x6000, active high
assign LCDEN0 = ~ADDR[15] * ADDR[14] * ADDR[13] * ~IOREQ;

// I/O 0x8000, active high
assign LCDEN1 = ADDR[15] * ~ADDR[14] * ~ADDR[13] * ~IOREQ;

// I/O 0xA000, active low
assign USBEN = ~(ADDR[15] * ~ADDR[14] * ADDR[13] * ~IOREQ);


assign NMI = 1'bZ;
assign INT = 3'bZ;

assign WAIT = 1'bZ;



assign USER = 8'b0;

assign DATA = 8'bZ;

reg [19:0] counter = 0;

always @(posedge CLK2) begin
	if (~RST) counter <= 1'b0;
	else counter <= counter + 1'b1;
end

assign LED = counter[19:17];
assign CLK_ASCI = counter[0];	// 1.8432 MHz / 2

/*
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
    end
    else begin
        // PS/2 state machine

        if (~KB_CLK) begin
            // PS/2 clock line is active-low

            if (~kb_clk_read)
                sample_delay <= sample_delay + 1'b1;

            // When the PS/2 clock line goes low, wait 8 CPU cycles and then sample the data line

            if (sample_delay == 8) begin
                case (kb_index)
                        0:;
                        1: temp_val[0] <= KB_DATA;
                        2: temp_val[1] <= KB_DATA;
                        3: temp_val[2] <= KB_DATA;
                        4: temp_val[3] <= KB_DATA;
                        5: temp_val[4] <= KB_DATA;
                        6: temp_val[5] <= KB_DATA;
                        7: temp_val[6] <= KB_DATA;
                        8: temp_val[7] <= KB_DATA;
                        9:;
                        10: kb_val <= temp_val;    // Parity bit, latch the complete scan code into the storage register
                    endcase

                    if (kb_index < 10)
                        kb_index <= kb_index + 1'b1;
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
assign DATA = (~CPLDEN) ? kb_val : 8'bZ;
*/
endmodule
