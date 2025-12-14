module main  (G0,G1,G2,G3,G17);

input G0;
input G1;
input G2;
input G3;

output G17;

wire    G5,G6,G7,G14,G8,G12,G15,G16
        ,G13,G9,G11,G10;

        dff     XG1     (G5,G10);
        dff     XG2     (G6,G11);
        dff     XG3     (G7,G13);
        not     XG4     (G14,G0);
        and     XG5     (G8,G6,G14);
        nor     XG6     (G12,G7,G1);
        or      XG7     (G15,G8,G12);
        or      XG8     (G16,G8,G3);
        nor     XG9     (G13,G12,G2);
        nand    XG10    (G9,G15,G16);
        nor     XG11    (G11,G9,G5);
        nor     XG12    (G10,G11,G14);
        not     XG13    (G17,G11);

endmodule


