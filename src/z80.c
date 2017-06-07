typedef struct {
	short bytes_to_load;
	short m_ticks;
	void (*operation)();
} instruction_t;

// Processor Registers
uint16_t pc;
uint16_t sp;

uint8_t a, b, c, d, e, h, l, f;

#define BC (((uint16_t)b << 8)|c)
#define DE (((uint16_t)d << 8)|e)
#define HL (((uint16_t)h << 8)|l)

uint8_t imm[2]; // Loaded immediate values 

#define IMM_8  imm[0];
#define IMM_16 (((uint8_t)imm[1] << 8)|imm[0])

// Flag definitions

#define Z_FLAG  0x80
#define N_FLAG  0x40
#define H_FLAG  0x20
#define CY_FLAG 0x10

#define CLEAR_FLAGS() f = 0
#define SET_Z_IF(COND)  f |= (COND) ? Z_FLAG : 0
#define SET_N_IF(COND)  f |= (COND) ? N_FLAG : 0
#define SET_H_IF(COND)  f |= (COND) ? H_FLAG : 0
#define SET_CY_IF(COND) f |= (COND) ? CY_FLAG : 0

#define SET_Z_IF_ZERO(VAL) f |= (COND) ? 0 : Z_FLAG;

extern uint8_t read_byte(uint16_t address);
extern uint8_t write_byte(uint16_t address, uint8_t value); 

// ===== Operations =====

// -- Load operations --

// register to register loads

void ld_b_c() { b = c; }
void ld_b_d() { b = d; }
void ld_b_e() { b = e; }
void ld_b_h() { b = h; }
void ld_b_l() { b = l; }
void ld_b_a() { b = a; }

void ld_c_b() { c = b; }
void ld_c_d() { c = d; }
void ld_c_e() { c = e; }
void ld_c_h() { c = h; }
void ld_c_l() { c = l; }
void ld_c_a() { c = a; }

void ld_d_b() { d = b; }
void ld_d_c() { d = c; }
void ld_d_e() { d = e; }
void ld_d_h() { d = h; }
void ld_d_l() { d = l; }
void ld_d_a() { d = a; }

void ld_e_b() { e = b; }
void ld_e_c() { e = c; }
void ld_e_d() { e = d; }
void ld_e_h() { e = h; }
void ld_e_l() { e = l; }
void ld_e_a() { e = a; }

void ld_h_b() { h = b; }
void ld_h_c() { h = c; }
void ld_h_d() { h = d; }
void ld_h_e() { h = e; }
void ld_h_l() { h = l; }
void ld_h_a() { h = a; }

void ld_l_b() { l = b; }
void ld_l_c() { l = c; }
void ld_l_d() { l = d; }
void ld_l_e() { l = e; }
void ld_l_h() { l = h; }
void ld_l_a() { l = a; }

void ld_a_b() { a = b; }
void ld_a_c() { a = c; }
void ld_a_d() { a = d; }
void ld_a_e() { a = e; }
void ld_a_h() { a = h; }
void ld_a_l() { a = l; }

// imm0ediate loads
void ld_b_n() { b = IMM_8; }
void ld_c_n() { c = IMM_8; }
void ld_d_n() { d = IMM_8; }
void ld_e_n() { e = IMM_8; }
void ld_h_n() { h = IMM_8; }
void ld_l_n() { l = IMM_8; }
void ld_a_n() { a = IMM_8; }

// Load into memory at HL
void ld_hl_b() { write_byte(HL, b); }
void ld_hl_c() { write_byte(HL, c); }
void ld_hl_d() { write_byte(HL, d); }
void ld_hl_e() { write_byte(HL, e); }
void ld_hl_h() { write_byte(HL, h); }
void ld_hl_l() { write_byte(HL, l); }
void ld_hl_a() { write_byte(HL, a); }
void ld_hl_n() { write_byte(HL, IMM_8) }

void ld_a_bc() { a = read_byte(BC); }
void ld_a_de() { a = read_byte(DE); }
void ld_bc_a() { write_byte(BC, a); }
void ld_de_a() { write_byte(DE, a); }

void ld_a_ffc() { a = read_byte(0xFF00 + c) }
void ld_ffc_a() { write_byte(0xFF00 + c, a); }

void ld_a_ffn() { a = read_byte(0xFF00 + IMM_8); }
void ld_ffn_a() { write_byte(0xFF00 + IMM_8, a); }

void ld_a_nn() { a = read_byte(IMM_16); }
void ld_nn_a() { write_byte(IMM_16, a); }

void ld_a_hli() { 
	uint16_t hl = HL;
	a = read_byte(hl++);
	h = (uint8_t)(hl >> 8);
	l = (uint8_t)(hl & 0xFF);
}
void ld_a_hld() { 
	uint16_t hl = HL;
	a = read_byte(hl--);
	h = (uint8_t)(hl >> 8);
	l = (uint8_t)(hl & 0xFF);
}
void ld_hli_a() {
	uint16_t hl = HL;
	write_byte(hl++, a);
	h = (uint8_t)(hl >> 8);
	l = (uint8_t)(hl & 0xFF);
}
void ld_hld_a() {
	uint16_t hl = HL;
	write_byte(hl--, a);
	h = (uint8_t)(hl >> 8);
	l = (uint8_t)(hl & 0xFF);
}

// 16 bit transfers
void ld_bc_nn() { b = imm[1]; c = imm[0]; }
void ld_de_nn() { d = imm[1]; e = imm[0]; }
void ld_hl_nn() { h = imm[1]; l = imm[0]; }
void ld_sp_nn() { sp = IMM_16; }

void ld_sp_hl() { sp = HL; }

// -- Stack operations --
void push_bc() { write_byte(--sp, b); write_byte(--sp, c); }
void push_de() { write_byte(--sp, d); write_byte(--sp, e); }
void push_hl() { write_byte(--sp, h); write_byte(--sp, l); }
void push_af() { write_byte(--sp, a); write_byte(--sp, f); }

void pop_bc() { c = read_byte(sp--); b = read_byte(sp--); }
void pop_de() { e = read_byte(sp--); d = read_byte(sp--); }
void pop_hl() { l = read_byte(sp--); h = read_byte(sp--); }
void pop_af() { f = read_byte(sp--); a = read_byte(sp--); }

void ldhl_sp() { 
	CLEAR_FLAGS();
	uint16_t spe = IMM_8;
	SET_H_IF(((sp & 0xFFF) + spe) & 0x1000);
	SET_CY_IF(((int32_t)sp + spe) & 0x10000);
	spe += sp;
	h = (uint8_t)(spe >> 8); l = spe & 0xFF;
}

void ld_nn_sp() { write_byte(IMM_16, sp & 0xFF); write_byte(IMM_16 + 1, sp >> 8); }

// -- Arithmetic and Logical Instructions --

#define ADD_A(R) CLEAR_FLAGS(); \
				 SET_H_IF(((a & 15) + (R & 15)) & 0x10); \
				 SET_CY_IF(((uint16_t)a + R) & 0x100); \
				 a += R; \
				 SET_Z_IF_ZERO(a)

void add_a_b() { ADD_A(b); }
void add_a_c() { ADD_A(c); }
void add_a_d() { ADD_A(d); }
void add_a_e() { ADD_A(e); }
void add_a_h() { ADD_A(h); }
void add_a_l() { ADD_A(l); }
void add_a_a() { ADD_A(a); }
void add_a_n() { ADD_A(IMM_8); }
void add_a_hl() {
	uint8_t rb = read_byte(HL);
	ADD_A(rb);
}

#define ADC_A(R) uint16_t cyf = (f >> 4) & 1; \
				 CLEAR_FLAGS(); \
				 SET_H_IF(((a & 15) + (R & 15) + cyf) & 0x10); \
				 SET_CY_IF((cyf + a + R) & 0x100); \
				 a += R; \
				 SET_Z_IF_ZERO(a)

void adc_a_b() { ADC_A(b); }
void adc_a_c() { ADC_A(c); }
void adc_a_d() { ADC_A(d); }
void adc_a_e() { ADC_A(e); }
void adc_a_h() { ADC_A(h); }
void adc_a_l() { ADC_A(l); }
void adc_a_a() { ADC_A(a); }
void adc_a_n() { ADC_A(IMM_8); }
void adc_a_hl() {
	uint8_t rb = read_byte(HL);
	ADC_A(rb);
}

#define SUB_A(R) f = N_FLAG; \
				 SET_H_IF((R & 15) > (a & 15)); \
				 SET_CY_IF(R > a); \
				 a -= R; \
				 SET_Z_IF_ZERO(a)

void sub_b() { SUB_A(b); }
void sub_c() { SUB_A(c); }
void sub_d() { SUB_A(d); }
void sub_e() { SUB_A(e); }
void sub_h() { SUB_A(h); }
void sub_l() { SUB_A(l); }
void sub_a() { SUB_A(a); }
void sub_n() { SUB_A(IMM_8); }
void sub_hl() {
	uint8_t rb = read_byte(HL);
	SUB_A(rb);
}

#define SBC_A(R) uint16_t cyf = (f >> 4) & 1; \
				 f |= N_FLAG; \
				 SET_H_IF(((R & 15) + cyf) > (a & 15)); \
				 SET_CY_IF(R + cyf > a); \
				 a -= R; \
				 SET_Z_IF_ZERO(a)

void sbc_b() { SBC_A(b); }
void sbc_c() { SBC_A(c); }
void sbc_d() { SBC_A(d); }
void sbc_e() { SBC_A(e); }
void sbc_h() { SBC_A(h); }
void sbc_l() { SBC_A(l); }
void sbc_a() { SBC_A(a); }
void sbc_n() { SBC_A(IMM_8); }
void sbc_hl() {
	uint8_t rb = read_byte(HL);
	SBC_A(rb);
}


void and_b() { f = N_FLAG; a &= b; SET_Z_IF_ZERO(a); } 
void and_c() { f = N_FLAG; a &= c; SET_Z_IF_ZERO(a); }
void and_d() { f = N_FLAG; a &= d; SET_Z_IF_ZERO(a); }
void and_e() { f = N_FLAG; a &= e; SET_Z_IF_ZERO(a); }
void and_h() { f = N_FLAG; a &= h; SET_Z_IF_ZERO(a); }
void and_l() { f = N_FLAG; a &= l; SET_Z_IF_ZERO(a); }
void and_a() { f = N_FLAG; a &= a; SET_Z_IF_ZERO(a); }
void and_n() { f = N_FLAG; a &= IMM_8; SET_Z_IF_ZERO(a); }
void and_hl() { f = N_FLAG; a &= read_byte(HL); SET_Z_IF_ZERO(a); }

void or_b() { f = 0; a |= b; SET_Z_IF_ZERO(a); }
void or_c() { f = 0; a |= c; SET_Z_IF_ZERO(a); }
void or_d() { f = 0; a |= d; SET_Z_IF_ZERO(a); }
void or_e() { f = 0; a |= e; SET_Z_IF_ZERO(a); }
void or_h() { f = 0; a |= h; SET_Z_IF_ZERO(a); }
void or_l() { f = 0; a |= l; SET_Z_IF_ZERO(a); }
void or_a() { f = 0; a |= a; SET_Z_IF_ZERO(a); }
void or_n() { f = 0; a |= IMM_8; SET_Z_IF_ZERO(a); }
void or_hl() { f = 0; a |= read_byte(HL); SET_Z_IF_ZERO(a); }

void xor_b() { f = 0; a ^= b; SET_Z_IF_ZERO(a); }
void xor_c() { f = 0; a ^= c; SET_Z_IF_ZERO(a); }
void xor_d() { f = 0; a ^= d; SET_Z_IF_ZERO(a); }
void xor_e() { f = 0; a ^= e; SET_Z_IF_ZERO(a); }
void xor_h() { f = 0; a ^= h; SET_Z_IF_ZERO(a); }
void xor_l() { f = 0; a ^= l; SET_Z_IF_ZERO(a); }
void xor_a() { f = 0; a ^= a; SET_Z_IF_ZERO(a); }
void xor_n() { f = 0; a ^= IMM_8; SET_Z_IF_ZERO(a); }
void xor_hl() { f = 0; a ^= read_byte(HL); SET_Z_IF_ZERO(a); }

#define CP_A(R) f = N_FLAG; \
				SET_H_IF((R & 15) > (a & 15)); \
				SET_CY_IF(R > a); \
				SET_Z_IF_ZERO(a - R)

void cp_b() { CP_A(b); }
void cp_c() { CP_A(c); }
void cp_d() { CP_A(d); }
void cp_e() { CP_A(e); }
void cp_h() { CP_A(h); }
void cp_l() { CP_A(l); }
void cp_a() { CP_A(a); }
void cp_n() { CP_A(IMM_8); }
void cp_hl() { 
	uint16_t rb = read_byte(HL);
	CP_A(rb); 
}

#define INCR(R) f &= CY_FLAG; \
				SET_H_IF((R & 15) == 15); \
				R++; \
				SET_Z_IF_ZERO(R);

void inc_b() { INCR(b); }
void inc_c() { INCR(c); }
void inc_d() { INCR(d); }
void inc_e() { INCR(e); }
void inc_h() { INCR(h); }
void inc_l() { INCR(l); }
void inc_a() { INCR(a); }
void inc_at_hl() {
	uint8_t hl = read_byte(HL);
	INCR(hl);
	write_byte(hl);
}

#define DECR(R) f = (f & CY_FLAG) | N_FLAG; \
				SET_H_IF((R & 15) == 0); \
				R--; \
				SET_Z_IF_ZERO(R);

void dec_b() { DECR(b); }
void dec_c() { DECR(c); }
void dec_d() { DECR(d); }
void dec_e() { DECR(e); }
void dec_h() { DECR(h); }
void dec_l() { DECR(l); }
void dec_a() { DECR(a); }
void dec_at_hl() {
	uint8_t hl = read_byte(HL);
	DECR(hl);
	write_byte(hl);
}

// 16-Bit Arithmetic

#define ADD_HL(RP) uint16_t hl = HL, rp = RP; \
	f &= Z_FLAG; \
	SET_H_IF(((hl & 0xFFF) + (rp & 0xFFF)) & 0x1000); \
	SET_CY_IF(((int32_t)hl + rp) & 0x10000); \
	hl += rp; \
	h = (uint8_t)(hl >> 8); l = hl & 0xFF \

void add_hl_bc() { ADD_HL(BC); }
void add_hl_de() { ADD_HL(DE); }
void add_hl_hl() { ADD_HL(HL); }
void add_hl_sp() { ADD_HL(sp); }

void add_sp_n() {
	CLEAR_FLAGS();
	SET_H_IF(((hl & 0xFFF) + IMM_8) & 0x1000);
	SET_CY_IF(((int32_t)hl + IMM_8) & 0x10000);
	sp += IMM_8;
}

void inc_bc() { c++; if (c == 0) b++; }
void inc_de() { e++; if (e == 0) d++; }
void inc_hl() { l++; if (l == 0) h++; }
void inc_sp() { sp++; }

void dec_bc() { c--; if (c == 0xFF) b--; }
void dec_de() { e--; if (e == 0xFF) d--; }
void dec_hl() { l--; if (l == 0xFF) h--; }
void dec_sp() { sp--; }

