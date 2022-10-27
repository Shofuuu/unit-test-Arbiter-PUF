`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/13/2022 10:40:02 PM
// Design Name: 
// Module Name: delay_path
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`include "defined.vh"

module delay_path(
        input Clk,
        input [C_LENGTH - 1 : 0] Challenge,
        output y0,
        output y1
    );

    (* dont_touch = "yes" *) wire [2 * C_LENGTH + 1 : 0] path;
    
    assign path[0] = Clk;
    assign path[1] = Clk;
    
    generate
        genvar x;

        for(x = 1; x <= C_LENGTH; x = x + 1)
        begin
            mux2x1 M0(
                .a(path[x * 2 - 2]),
                .b(path[x * 2 - 1]),
                .s(Challenge[x - 1]),
                .y(path[x * 2])
            );

            mux2x1 M1(
                .a(path[x * 2 - 1]),
                .b(path[x * 2 - 2]),
                .s(Challenge[x - 1]),
                .y(path[x * 2 + 1])
            );
        end
    endgenerate
    
    assign y0 = path[C_LENGTH * 2];
    assign y1 = path[C_LENGTH * 2 + 1];
    
endmodule
