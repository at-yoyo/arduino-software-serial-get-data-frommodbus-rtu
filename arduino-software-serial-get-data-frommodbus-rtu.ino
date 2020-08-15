#include <SoftwareSerial.h>

/*----------------FUNCTION VARIABLE FOR CALCULATOR----------------*/
String _FINAL_23_BITS,_BIN2DEC_RIGHT_E11_7,_BIN2DEC_F11,_BIN2DEC_G11;
long NEXT8BITS,BIN2DEC_RIGHT_E11_7,BIN2DEC_F11,BIN2DEC_G11;
uint8_t BUFF[256];
int CMD_LENGTH,DATA_LENGTH,SOFT_WARESERIAL_AVAILABLE;
/*----------------FUNCTION VARIABLE FOR CALCULATOR----------------*/
//{SLAVE ADDRESS , FUNCTION CODE , 0x00 , START ADDRESS , 0x00 , NUMBER OF COILS}
/*
 * FUNCTION CODE
 * 1 READ COILS = 0x01
 * 2 READ DISCRETE INPUT = 0x02
 * 3 READ HOLDING REGISTERS = 0x03
 * 4 READ INPUT REGISTERS = 0x04
 * 15 WRITE MULTIPLR COILS = 0x0f
 * 16 WRITE MULTIPLR REGISTERS = 0x10
 */
 /*
  * BASE 16 AND FUNCTION READ DESCRIPTION
  * EXEMPLE
  * {SLAVE ADDRESS , FUNCTION CODE , 0x00 , START ADDRESS , 0x00 , NUMBER OF COILS}
 */
 
 /*
 * FUNCTION CODE
 * 5 WRITE SINGLE COIL = 0x05
 * 6 WRITE SINGLE REGISTER  = 0x06
 * 3 READ HOLDING REGISTERS = 0x03
 * 4 READ INPUT REGISTERS = 0x04
 */
 /*
  * BASE 16 AND FUNCTION WRITE DESCRIPTION
  * EXEMPLE
  * {SLAVE ADDRESS , FUNCTION CODE , 0x00 , START ADDRESS}
 */
 
uint8_t MESSAGE[8] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A};
/*----------------FUNCTION VARIABLE FOR CALCULATOR----------------*/

SoftwareSerial SOFT_WARESERIAL =  SoftwareSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  SOFT_WARESERIAL.begin(9600);
}
void loop() {
       transmit_message();
       /*----------------GET FUNCTION FOR LIST DATA----------------*/
       //result_list();
       /*
        *IF DATA SHOW INTEGER VARIABLE int DATA[SIZE DATA LIST];
        *IF DATA SHOW FLOAT VARIABLE float DATA[SIZE DATA LIST];
       ----------------GET FUNCTION FOR LIST DATA----------------*/
       
       /*----------------FOR GET FLOAT DATA----------------*/
     
       float DATA[8];
       for(int i = 0;i < 8;i++)DATA[i] = raw_buffer_to_float_mid_little_endian_cdab_result_select(i);
       Serial.print("TEMPERATURE ");
       Serial.print(DATA[0]);
       Serial.println(" C");
       Serial.print("HUMIDITY ");
       Serial.print(DATA[1]);
       Serial.println(" %");
       
        /*----------------FOR GET INTEGER DATA----------------*/
       //int DATA[10];
       //for(int i = 0;i < 10;i++)DATA[i] = raw_buffer_to_number_result_select(i);
       //Serial.print("VOLTAGE ");
       //Serial.print(DATA[0]*0.1);
       //Serial.println(" V");
       //Serial.print("CURRENT ");
       //Serial.print(DATA[1]*0.001);
       //Serial.println(" A");
       //Serial.print("POWER ");
       //Serial.print(DATA[3]*0.1);
       //Serial.println(" W");
       //Serial.print("ENERGY ");
       //Serial.print(DATA[5]*0.1);
       //Serial.println(" Wh");
       //Serial.print("FREQUENCY ");
       //Serial.print(DATA[7]*0.1);
       //Serial.println(" Hz");
       //Serial.print("POWER FACTOR ");
       //Serial.println(DATA[8]*0.01);
       Serial.println();
       delay(3000);
}

void transmit_message(){
      add_modbus_rtu_crc(MESSAGE,6);
      SOFT_WARESERIAL.write(MESSAGE,8);
      SOFT_WARESERIAL.flush(); 
      SOFT_WARESERIAL_AVAILABLE = SOFT_WARESERIAL.available();
      if(SOFT_WARESERIAL_AVAILABLE > 0){
        while(SOFT_WARESERIAL.read() != -1);
        CMD_LENGTH = SOFT_WARESERIAL.readBytes(BUFF,SOFT_WARESERIAL_AVAILABLE);
      }
}
/*----------------FUNCTION FOR LIST DATA----------------*/
void result_list(){
  raw_buffer_to_number_result_list();
  Serial.println();
  raw_buffer_to_float_mid_little_endian_cdab_result_list();
  
  }
void raw_buffer_to_number_result_list(){
        int i_2 = 0;
        Serial.println("---------------INTEGER---------------");
        for(int i = 3;i < BUFF[2]+2;i++){
          String STR1 = String((uint32_t)BUFF[i], HEX);
          String STR2 = String((uint32_t)BUFF[i+1], HEX);
          if (STR1.length() == 1) {STR1 = "0"+STR1;}
          if (STR2.length() == 1) {STR2 = "0"+STR2;}
          Serial.print("BLOCK DATA [ ");Serial.print(i_2++);Serial.print(" ] = ");
          Serial.println(hex_to_dec(STR1+STR2));
           i++;
       } 
       Serial.println("###############INTEGER###############");
}
void raw_buffer_to_float_mid_little_endian_cdab_result_list(){
       uint16_t *REGS;
        int i_2 = 0;
         Serial.println("---------------FLOAT---------------");
        for(int i = 3;i < BUFF[2]+2;i++){
          String STR1 = String((uint32_t)BUFF[i], HEX);
          String STR2 = String((uint32_t)BUFF[i+1], HEX);
          String STR3 = String((uint32_t)BUFF[i+2], HEX);
          String STR4 = String((uint32_t)BUFF[i+3], HEX);
          if (STR1.length() == 1) {STR1 = "0"+STR1;}
          if (STR2.length() == 1) {STR2 = "0"+STR2;}
          if (STR3.length() == 1) {STR3 = "0"+STR3;}
          if (STR4.length() == 1) {STR4 = "0"+STR4;}
          uint16_t _REGS[4];
          _REGS[0] = strtoul((("0x"+STR3).c_str()), NULL, 0);
          _REGS[1] = strtoul((("0x"+STR4).c_str()), NULL, 0);
          _REGS[2] = strtoul((("0x"+STR1).c_str()), NULL, 0);
          _REGS[3] = strtoul((("0x"+STR2).c_str()), NULL, 0);
          Serial.print("BLOCK DATA [ ");Serial.print(i_2++);Serial.print(" ] = ");
          Serial.println(float_mid_little_endian_cdab(_REGS));
           i= i+3;
        } 
        Serial.println("###############FLOAT###############");
}
/*----------------FUNCTION FOR GET BUFFER TO DEC----------------*/
int raw_buffer_to_number_result_select(int DATA_POINT){
        int DATA[hex_to_dec(String(MESSAGE[5]))*2];
        int i_2 = 0;
        for(int i = 3;i < BUFF[2]+2;i++){
          String STR1 = String((uint32_t)BUFF[i], HEX);
          String STR2 = String((uint32_t)BUFF[i+1], HEX);
          if (STR1.length() == 1) {STR1 = "0"+STR1;}
          if (STR2.length() == 1) {STR2 = "0"+STR2;}
          DATA[i_2] = hex_to_dec(STR1+STR2);
          if(DATA_POINT == i_2)return DATA[DATA_POINT];
          i++;
          i_2++;
       } 
}
/*----------------FUNCTION FOR GET TO FLOAT MID LITTLE ENDIAN CDAB----------------*/
float raw_buffer_to_float_mid_little_endian_cdab_result_select(int DATA_POINT){
      float FLOAT_MID_LITTLE_ENDIAN_CDAB[hex_to_dec(String(MESSAGE[5]))*2];
      uint16_t *REGS;
        int i_2 = 0;
        for(int i = 3;i < BUFF[2]+2;i++){
          String STR1 = String((uint32_t)BUFF[i], HEX);
          String STR2 = String((uint32_t)BUFF[i+1], HEX);
          String STR3 = String((uint32_t)BUFF[i+2], HEX);
          String STR4 = String((uint32_t)BUFF[i+3], HEX);
          if (STR1.length() == 1) {STR1 = "0"+STR1;}
          if (STR2.length() == 1) {STR2 = "0"+STR2;}
          if (STR3.length() == 1) {STR3 = "0"+STR3;}
          if (STR4.length() == 1) {STR4 = "0"+STR4;}
          uint16_t _REGS[4];
          _REGS[0] = strtoul((("0x"+STR3).c_str()), NULL, 0);
          _REGS[1] = strtoul((("0x"+STR4).c_str()), NULL, 0);
          _REGS[2] = strtoul((("0x"+STR1).c_str()), NULL, 0);
          _REGS[3] = strtoul((("0x"+STR2).c_str()), NULL, 0);
          FLOAT_MID_LITTLE_ENDIAN_CDAB[i_2] = float_mid_little_endian_cdab(_REGS);
          if(DATA_POINT == i_2)return FLOAT_MID_LITTLE_ENDIAN_CDAB[DATA_POINT];
           i= i+3;
           i_2++;
        } 
}
/*----------------FUNCTION FOR CALCULATOR FLOAT - MID-LITTLE ENDIAN (CDAB)----------------*/
float float_mid_little_endian_cdab(uint16_t *REGS)
{
    float DIVIDE_ABOVE_AND_ADD_1,FINAL_23_BITS,DIVIDE_ABOVE,MANTISSA_SIGNIFICAND,SIGN_X_MANTISSA_X_2A_EXPONENT,SIGN,EXPONENT,SUBTRACT_127;
    uint32_t HEX800000 = 0x800000;
    uint32_t HEX400000 = 0x400000;
    SIGN_X_MANTISSA_X_2A_EXPONENT = 0;
    SIGN = first_bit(REGS[0]);
    NEXT8BITS= next_8_bits(REGS[0],REGS[1]);
    SUBTRACT_127 = subtract_127(NEXT8BITS);
    EXPONENT = pow(2,SUBTRACT_127);
    final_23_bits(REGS[1],REGS[2],REGS[3]);
    FINAL_23_BITS = BIN2DEC_RIGHT_E11_7*256*256+BIN2DEC_F11*256+BIN2DEC_G11;
    DIVIDE_ABOVE_AND_ADD_1 = FINAL_23_BITS/HEX800000+1;
    MANTISSA_SIGNIFICAND = DIVIDE_ABOVE_AND_ADD_1;
    DIVIDE_ABOVE = FINAL_23_BITS/HEX400000;
    SIGN_X_MANTISSA_X_2A_EXPONENT = SIGN*MANTISSA_SIGNIFICAND*EXPONENT;
    if(SIGN_X_MANTISSA_X_2A_EXPONENT<0)SIGN_X_MANTISSA_X_2A_EXPONENT = SIGN_X_MANTISSA_X_2A_EXPONENT*-1;
    return SIGN_X_MANTISSA_X_2A_EXPONENT;
}
int first_bit(uint16_t FIRST_BIT){
  String FIRST = String(FIRST_BIT,BIN);
  for(int i = String(FIRST_BIT,BIN).length();i < 8;i++){FIRST = "0"+FIRST;}
  char CHAR_BUFF[8];
  FIRST.toCharArray(CHAR_BUFF, 8);
  if(CHAR_BUFF[0] == 0 || CHAR_BUFF[0] == '0' || CHAR_BUFF[0] == "0")return 1;else return -1;
}
long next_8_bits(uint16_t BIT_1,uint16_t BIT_2){
  String _1 = String(BIT_1,BIN);
  String _2 = String(BIT_2,BIN);
  String _NEXT8BITS = "";
  char CHAR_BUFF_1[256],CHAR_BUFF_2[256],INPUT_BINARY_STRING[16];
   for(int i = String(BIT_1,BIN).length();i < 8;i++){_1 = "0"+_1;}
   for(int i = String(BIT_2,BIN).length();i < 8;i++){_2 = "0"+_2;}
  _1.toCharArray(CHAR_BUFF_1, 16);
  _2.toCharArray(CHAR_BUFF_2, 16);
  for(int i = 1;i <= 8;i++){_NEXT8BITS = _NEXT8BITS+String(CHAR_BUFF_1[i]);}
  for(int i = 0;i < 1;i++){_NEXT8BITS = _NEXT8BITS+String(CHAR_BUFF_2[i]);}
   _NEXT8BITS.toCharArray(INPUT_BINARY_STRING, 16);
  return strtol(INPUT_BINARY_STRING, NULL, 2);
}
float subtract_127(float SUBTRACT){
  return SUBTRACT-127*1.00;
}
void final_23_bits(uint16_t BIT_2,uint16_t BIT_3,uint16_t BIT_4){
  String _2 = String(BIT_2,BIN);
  _BIN2DEC_F11 = String(BIT_3,BIN);
  _BIN2DEC_G11 = String(BIT_4,BIN);
  String _5,_6;
  char CHAR_BUFF[256];
   for(int i = String(BIT_2,BIN).length();i < 8;i++){_2 = "0"+_2;}
   for(int i = String(BIT_3,BIN).length();i < 8;i++){_BIN2DEC_F11 = "0"+_BIN2DEC_F11;}
   for(int i = String(BIT_4,BIN).length();i < 8;i++){_BIN2DEC_G11 = "0"+_BIN2DEC_G11;}
   _5 = String(_2)+String(_BIN2DEC_F11)+String(_BIN2DEC_G11);
   _5.toCharArray(CHAR_BUFF, 256);
   for(int i = 1;i < 8;i++){_6 = _6+String(CHAR_BUFF[i]);}
   _BIN2DEC_RIGHT_E11_7 = _6;
   for(int i = 1;i < 24;i++){_FINAL_23_BITS = _FINAL_23_BITS+String(CHAR_BUFF[i]);}
   char INPUT_BINARY_STRING_2[16],INPUT_BINARY_STRING_3[16],INPUT_BINARY_STRING_4[16];
   _BIN2DEC_RIGHT_E11_7.toCharArray(INPUT_BINARY_STRING_2, 16);
   BIN2DEC_RIGHT_E11_7 = strtol(INPUT_BINARY_STRING_2, NULL, 2);
   _BIN2DEC_F11.toCharArray(INPUT_BINARY_STRING_3, 16);
   BIN2DEC_F11 = strtol(INPUT_BINARY_STRING_3, NULL, 2);
   _BIN2DEC_G11.toCharArray(INPUT_BINARY_STRING_4, 16);
   BIN2DEC_G11 = strtol(INPUT_BINARY_STRING_4, NULL, 2);
}

/*----------------FUNCTION FOR CALCULATOR MODBUS RTU CRC 16----------------*/
void add_modbus_rtu_crc(uint8_t* BUF, int LEN){
  unsigned int CRC_ = 0xFFFF;
  for (int POS = 0; POS < LEN; POS++) {
    CRC_ ^= (unsigned int)BUF[POS];          // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((CRC_ & 0x0001) != 0) {      // If the LSB is set
        CRC_ >>= 1;                    // Shift right and XOR 0xA001
        CRC_ ^= 0xA001;
      }else                            // Else LSB is not set
        CRC_ >>= 1;                    // Just shift right
    }
  }
  long CRC = CRC_;
  MESSAGE[LEN] = CRC & 255;
  MESSAGE[LEN + 1] = (CRC & long(255*256))>>8;
}

/*----------------FUNCTION FOR CALCULATOR HEX TO DEC----------------*/
unsigned int hex_to_dec(String HEX_STR) {
  unsigned int DEC_VALUE = 0;
  int NEXT_INT;
  for (int i = 0; i < HEX_STR.length(); i++) {
    NEXT_INT = int(HEX_STR.charAt(i));
    if (NEXT_INT >= 48 && NEXT_INT <= 57) NEXT_INT = map(NEXT_INT, 48, 57, 0, 9);
    if (NEXT_INT >= 65 && NEXT_INT <= 70) NEXT_INT = map(NEXT_INT, 65, 70, 10, 15);
    if (NEXT_INT >= 97 && NEXT_INT <= 102) NEXT_INT = map(NEXT_INT, 97, 102, 10, 15);
    NEXT_INT = constrain(NEXT_INT, 0, 15);
    DEC_VALUE = (DEC_VALUE * 16) + NEXT_INT;
  }
  return DEC_VALUE;
}

/*----------------FUNCTION FOR CHECK DATA TYPE----------------*/
void check_types(String DATA) { Serial.println("DATA = String"); }
void check_types(int DATA) { Serial.println("DATA = int"); }
void check_types(char *DATA) { Serial.println("DATA = char*"); }
void check_types(float DATA) { Serial.println("DATA = float"); }
void check_types(bool DATA) { Serial.println("DATA = bool"); }
