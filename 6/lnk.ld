SECTIONS {
	. = 0x7e00;
	.text : {*(.text);
	. = ALIGN(512);}
}