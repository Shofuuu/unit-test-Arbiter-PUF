`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/13/2022 11:10:07 PM
// Design Name: 
// Module Name: puf
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

module puf(
        input Clk,
        input [C_LENGTH - 1:0] Challenge,
        output [C_LENGTH - 1:0] Response
    );

    generate
        genvar x;
        
        for (x=0;x<C_LENGTH;x=x+1) begin
            (* dont_touch = "yes" *) arbiter A0(
                .Clk(Clk),
                .Challenge(Challenge),
                .Response(Response[x])
            );
        end
    endgenerate 

endmodule
