`timescale 1ns / 1ps

`include "defined.vh"

module arbiter(
        input Clk,
        input [C_LENGTH - 1:0] Challenge,
        output Response
    );

    wire [1:0] out;

    delay_path DP0(
        .Clk(Clk),
        .Challenge(Challenge),
        .y0(out[0]),
        .y1(out[1])
    );

    dff D0(
        .d(out[0]),
        .Clk(out[1]),
        .q(Response)
    );

endmodule