// imu.h - header for ISM330DHCX
// mostly swiped from BZ example code

// Address of ISM330DHCX 6DOF IMU
// 0x6A if SA0 = low, 0x6B if SA0 = high
// (SA0 pin on sparkfun board is pulled high by default)
const uint8_t ACC_ADDR= 0x6B;
const uint8_t ACC_WHO_AM_I_ADDR = 0x0F;
const uint8_t ACC_WHO_AM_I_VAL = 0x6B;
const uint8_t ACC_CTRL3_C_ADDR = 0x12; 
const uint8_t ACC_CTRL3_C_VAL = 0x03; // software reset
const uint8_t ACC_CTRL1_XL_ADDR = 0x10;
const uint8_t ACC_CTRL1_XL_VAL = 0b00011000; //accel ODR (12.5Hz) and FS (+-4g)
const uint8_t ACC_CTRL2_G_VAL = 0b00010000; //gyro ODR (12.5Hz) and FS (+-250dps)
const uint8_t ACC_OUT_TEMP_L_ADDR = 0x20; //start of output registers
const uint8_t ACC_CTRL5_C_ADDR = 0x14;

// scaling factors and offsets
const float ACC_ACCEL_SCF = 4.0/(2<<14);
const float ACC_GYRO_SCF = 250.0/(2<<14);
const float ACC_TEMP_SCF = 1.0/256;
const float ACC_TEMP_OFFSET = 25.0;

// function declarations
int acc_set_self_test(int8_t st_val);
int read_acc_raw(uint8_t res_buf[14]);
int read_acc(int16_t res_buf[7]);
int init_acc();

