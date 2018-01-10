//Some constants
const float maxExposure = 200000;

const float PI = 3.14159265359;
const float RAD = 1.0;
const float DEG = PI / 180.0;

const float STERADIAN = 1.0;

const float METER = 1.0;
const float KILOMETER = 1000.0 * METER;
const float M2 = METER * METER;
const float M3 = METER * METER * METER;

const float NANOMETER = 1.0; // In Wavelengths

const float LUMEN = 1.0;

const float WATT = 1.0;
const float WATT_PER_SQUARE_METER = WATT / M2;
const float WATT_PER_SQUARE_METER_PER_SR = WATT / (M2 * STERADIAN);
const float WATT_PER_SQUARE_METER_PER_NM = WATT / (M2 * NANOMETER);
const float WATT_PER_SQUARE_METER_PER_SR_PER_NM = WATT / (M2 * STERADIAN * NANOMETER);
const float WATT_PER_CUBIC_METER_PER_SR_PER_NM = WATT / (M3 * STERADIAN * NANOMETER);
const float CANDELA = LUMEN / STERADIAN;
const float KILOCANDELA = 1000.0 * CANDELA;
const float CD_PER_SQUARE_METER = CANDELA / M2;
const float KCD_PER_SQUARE_METER = KILOCANDELA / M2;