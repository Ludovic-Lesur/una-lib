/*
 * una.c
 *
 *  Created on: 06 dec. 2024
 *      Author: Ludo
 */

#include "una.h"

#ifndef UNA_LIB_DISABLE_FLAGS_FILE
#include "una_lib_flags.h"
#endif
#include "maths.h"
#include "types.h"

#ifndef UNA_LIB_DISABLE

/*** UNA local macros ***/

#define UNA_SIGN_SIZE_BITS                      1

#define UNA_TIME_UNIT_SIZE_BITS                 2
#define UNA_TIME_VALUE_SIZE_BITS                6

#define UNA_TEMPERATURE_VALUE_SIZE_BITS         11

#define UNA_VOLTAGE_UNIT_SIZE_BITS              1
#define UNA_VOLTAGE_VALUE_SIZE_BITS             15

#define UNA_CURRENT_UNIT_SIZE_BITS              2
#define UNA_CURRENT_VALUE_SIZE_BITS             14

#define UNA_ELECTRICAL_POWER_UNIT_SIZE_BITS     2
#define UNA_ELECTRICAL_POWER_VALUE_SIZE_BITS    13

#define UNA_ELECTRICAL_ENERGY_UNIT_SIZE_BITS    2
#define UNA_ELECTRICAL_ENERGY_VALUE_SIZE_BITS   13

#define UNA_POWER_FACTOR_VALUE_SIZE_BITS        7

#define UNA_SECONDS_PER_MINUTE                  60
#define UNA_MINUTES_PER_HOUR                    60
#define UNA_HOURS_PER_DAY                       24

#define UNA_MV_PER_DV                           100

#define UNA_UA_PER_DMA                          100
#define UNA_DMA_PER_MA                          10
#define UNA_MA_PER_DA                           100

#define UNA_MW_MVA_PER_DW_DVA                   100
#define UNA_DW_DVA_PER_W_VA                     10
#define UNA_W_VA_PER_DAW_DAVA                   10

#define UNA_MWH_MVAH_PER_DWH_DVAH               100
#define UNA_DWH_DVAH_PER_WH_VAH                 10
#define UNA_WH_VAH_PER_DAWH_DAVAH               10

#define UNA_RF_POWER_OFFSET                     174

#define UNA_YEAR_OFFSET                         2000

/*** UNA local structures ***/

/*******************************************************************/
typedef enum {
    UNA_SIGN_POSITIVE = 0,
    UNA_SIGN_NEGATIVE
} UNA_sign_t;

/*******************************************************************/
typedef enum {
    UNA_TIME_UNIT_SECOND = 0,
    UNA_TIME_UNIT_MINUTE,
    UNA_TIME_UNIT_HOUR,
    UNA_TIME_UNIT_DAY
} UNA_time_unit_t;

/*******************************************************************/
typedef enum {
    UNA_VOLTAGE_UNIT_MV = 0,
    UNA_VOLTAGE_UNIT_DV
} UNA_voltage_unit_t;

/*******************************************************************/
typedef enum {
    UNA_CURRENT_UNIT_UA = 0,
    UNA_CURRENT_UNIT_DMA,
    UNA_CURRENT_UNIT_MA,
    UNA_CURRENT_UNIT_DA
} UNA_current_unit_t;

/*******************************************************************/
typedef enum {
    UNA_ELECTRICAL_POWER_UNIT_MW_MVA = 0,
    UNA_ELECTRICAL_POWER_UNIT_DW_DVA,
    UNA_ELECTRICAL_POWER_UNIT_W_VA,
    UNA_ELECTRICAL_POWER_UNIT_DAW_DAVA
} UNA_electrical_power_unit_t;

/*******************************************************************/
typedef enum {
    UNA_ELECTRICAL_ENERGY_UNIT_MWH_MVAH = 0,
    UNA_ELECTRICAL_ENERGY_UNIT_DWH_DVAH,
    UNA_ELECTRICAL_ENERGY_UNIT_WH_VAH,
    UNA_ELECTRICAL_ENERGY_UNIT_DAWH_DAVAH
} UNA_electrical_energy_unit_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_TIME_VALUE_SIZE_BITS;
        UNA_time_unit_t unit : UNA_TIME_UNIT_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_time_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_TEMPERATURE_VALUE_SIZE_BITS;
        UNA_sign_t sign : UNA_SIGN_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_temperature_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_VOLTAGE_VALUE_SIZE_BITS;
        UNA_voltage_unit_t unit : UNA_VOLTAGE_UNIT_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_voltage_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_CURRENT_VALUE_SIZE_BITS;
        UNA_current_unit_t unit : UNA_CURRENT_UNIT_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_current_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_ELECTRICAL_POWER_VALUE_SIZE_BITS;
        UNA_electrical_power_unit_t unit : UNA_ELECTRICAL_POWER_UNIT_SIZE_BITS;
        UNA_sign_t sign : UNA_SIGN_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_electrical_power_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_ELECTRICAL_ENERGY_VALUE_SIZE_BITS;
        UNA_electrical_energy_unit_t unit : UNA_ELECTRICAL_ENERGY_UNIT_SIZE_BITS;
        UNA_sign_t sign : UNA_SIGN_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_electrical_energy_t;

/*******************************************************************/
typedef union {
    uint32_t representation;
    struct {
        unsigned value : UNA_POWER_FACTOR_VALUE_SIZE_BITS;
        UNA_sign_t sign : UNA_SIGN_SIZE_BITS;
    } __attribute__((scalar_storage_order("little-endian"))) __attribute__((packed));
} UNA_power_factor_t;

/*** UNA global variables ***/

#ifdef UNA_LIB_USE_BOARD_NAME
const char_t* const UNA_BOARD_NAME[UNA_BOARD_ID_LAST] = { "LVRM", "BPSM", "DDRM", "UHFM", "GPSM", "SM", "RS485-BRIDGE", "RRM", "DMM", "MPMCM", "R4S8CR", "BCM" };
#endif

/*** UNA functions ***/

/*******************************************************************/
void UNA_reset_node_list(UNA_node_list_t* node_list) {
    // Local variables.
    uint8_t idx = 0;
    // Check parameter.
    if (node_list != NULL) {
        for (idx = 0; idx < UNA_NODE_ADDRESS_LAST; idx++) {
            node_list->list[idx].board_id = UNA_BOARD_ID_ERROR;
            node_list->list[idx].address = UNA_NODE_ADDRESS_ERROR;
        }
        node_list->count = 0;
    }
}

/*******************************************************************/
uint32_t UNA_convert_seconds(int32_t time_seconds) {
    // Local variables.
    UNA_time_t una_time;
    uint32_t value = 0;
    // Negative values not supported.
    MATH_abs(time_seconds, value, uint32_t);
    // Select unit.
    if (value < (0b1 << UNA_TIME_VALUE_SIZE_BITS)) {
        una_time.unit = UNA_TIME_UNIT_SECOND;
    }
    else {
        value /= UNA_SECONDS_PER_MINUTE;
        if (value < (0b1 << UNA_TIME_VALUE_SIZE_BITS)) {
            una_time.unit = UNA_TIME_UNIT_MINUTE;
        }
        else {
            value /= UNA_MINUTES_PER_HOUR;
            if (value < (0b1 << UNA_TIME_VALUE_SIZE_BITS)) {
                una_time.unit = UNA_TIME_UNIT_HOUR;
            }
            else {
                value /= UNA_HOURS_PER_DAY;
                una_time.unit = UNA_TIME_UNIT_DAY;
            }
        }
    }
    una_time.value = value;
    return (una_time.representation);
}

/*******************************************************************/
int32_t UNA_get_seconds(uint32_t una_time) {
    // Local variables.
    int32_t time_seconds = 0;
    uint32_t local_una_time = una_time;
    uint32_t value = 0;
    UNA_time_unit_t unit = UNA_TIME_UNIT_SECOND;
    // Parse fields.
    unit = (((UNA_time_t*) &local_una_time)->unit);
    value = (uint32_t) (((UNA_time_t*) &local_una_time)->value);
    // Compute seconds.
    switch (unit) {
    case UNA_TIME_UNIT_SECOND:
        time_seconds = (int32_t) value;
        break;
    case UNA_TIME_UNIT_MINUTE:
        time_seconds = (int32_t) (UNA_SECONDS_PER_MINUTE * value);
        break;
    case UNA_TIME_UNIT_HOUR:
        time_seconds = (int32_t) (UNA_MINUTES_PER_HOUR * UNA_SECONDS_PER_MINUTE * value);
        break;
    default:
        time_seconds = (int32_t) (UNA_HOURS_PER_DAY * UNA_MINUTES_PER_HOUR * UNA_SECONDS_PER_MINUTE * value);
        break;
    }
    return time_seconds;
}

/*******************************************************************/
uint32_t UNA_convert_year(int32_t year) {
    return ((uint32_t) (year - UNA_YEAR_OFFSET));
}

/*******************************************************************/
int32_t UNA_get_year(uint32_t una_year) {
    return ((int32_t) (una_year + UNA_YEAR_OFFSET));
}

/*******************************************************************/
uint32_t UNA_convert_tenth_degrees(int32_t temperature_tenth_degrees) {
    // Local variables.
    uint32_t una_temperature = 0;
    // DINFox representation is equivalent to signed magnitude.
    MATH_integer_to_signed_magnitude(temperature_tenth_degrees, UNA_TEMPERATURE_VALUE_SIZE_BITS, &una_temperature);
    return una_temperature;
}

/*******************************************************************/
int32_t UNA_get_tenth_degrees(uint32_t una_temperature) {
    // Local variables.
    int32_t temperature_tenth_degrees = 0;
    uint32_t local_una_temperature = una_temperature;
    UNA_sign_t sign = UNA_SIGN_POSITIVE;
    uint32_t value = 0;
    // Parse fields.
    sign = (((UNA_temperature_t*) &local_una_temperature)->sign);
    value = (uint32_t) (((UNA_temperature_t*) &local_una_temperature)->value);
    // Check sign.
    temperature_tenth_degrees = (sign == UNA_SIGN_POSITIVE) ? ((int32_t) value) : ((-1) * ((int32_t) value));
    return temperature_tenth_degrees;
}

/*******************************************************************/
uint32_t UNA_convert_mv(int32_t voltage_mv) {
    // Local variables.
    UNA_voltage_t una_voltage;
    uint32_t value = 0;
    // Negative values not supported.
    MATH_abs(voltage_mv, value, uint32_t);
    // Select format.
    if (voltage_mv < (0b1 << UNA_VOLTAGE_VALUE_SIZE_BITS)) {
        una_voltage.unit = UNA_VOLTAGE_UNIT_MV;
        una_voltage.value = value;
    }
    else {
        una_voltage.unit = UNA_VOLTAGE_UNIT_DV;
        una_voltage.value = (value / UNA_MV_PER_DV);
    }
    return (una_voltage.representation);
}

/*******************************************************************/
int32_t UNA_get_mv(uint32_t una_voltage) {
    // Local variables.
    int32_t voltage_mv = 0;
    uint32_t local_una_voltage = una_voltage;
    uint32_t value = 0;
    UNA_voltage_unit_t unit = UNA_VOLTAGE_UNIT_MV;
    // Parse fields.
    unit = (((UNA_voltage_t*) &local_una_voltage)->unit);
    value = (uint32_t) (((UNA_voltage_t*) &local_una_voltage)->value);
    // Compute mV.
    voltage_mv = (unit == UNA_VOLTAGE_UNIT_MV) ? ((int32_t) value) : ((UNA_MV_PER_DV * (int32_t) value));
    return voltage_mv;
}

/*******************************************************************/
uint32_t UNA_convert_ua(int32_t current_ua) {
    // Local variables.
    UNA_current_t una_current;
    uint32_t value = 0;
    // Negative values not supported.
    MATH_abs(current_ua, value, uint32_t);
    // Select unit.
    if (value < (0b1 << UNA_CURRENT_VALUE_SIZE_BITS)) {
        una_current.unit = UNA_CURRENT_UNIT_UA;
    }
    else {
        value /= UNA_UA_PER_DMA;
        if (value < (0b1 << UNA_CURRENT_VALUE_SIZE_BITS)) {
            una_current.unit = UNA_CURRENT_UNIT_DMA;
        }
        else {
            value /= UNA_DMA_PER_MA;
            if (value < (0b1 << UNA_CURRENT_VALUE_SIZE_BITS)) {
                una_current.unit = UNA_CURRENT_UNIT_MA;
            }
            else {
                value /= UNA_MA_PER_DA;
                una_current.unit = UNA_CURRENT_UNIT_DA;
            }
        }
    }
    una_current.value = value;
    return (una_current.representation);
}

/*******************************************************************/
int32_t UNA_get_ua(uint32_t una_current) {
    // Local variables.
    int32_t current_ua = 0;
    uint32_t local_una_current = una_current;
    uint32_t value = 0;
    UNA_current_unit_t unit = UNA_CURRENT_UNIT_UA;
    // Parse fields.
    unit = (((UNA_current_t*) &local_una_current)->unit);
    value = (uint32_t) (((UNA_current_t*) &local_una_current)->value);
    // Compute seconds.
    switch (unit) {
    case UNA_CURRENT_UNIT_UA:
        current_ua = (int32_t) value;
        break;
    case UNA_CURRENT_UNIT_DMA:
        current_ua = (int32_t) (UNA_UA_PER_DMA * value);
        break;
    case UNA_CURRENT_UNIT_MA:
        current_ua = (int32_t) (UNA_UA_PER_DMA * UNA_DMA_PER_MA * value);
        break;
    default:
        current_ua = (int32_t) (UNA_UA_PER_DMA * UNA_DMA_PER_MA * UNA_MA_PER_DA * value);
        break;
    }
    return current_ua;
}

/*******************************************************************/
uint32_t UNA_convert_mw_mva(int32_t electrical_power_mw_mva) {
    // Local variables.
    UNA_electrical_power_t una_electrical_power;
    uint32_t absolute_value = 0;
    // Read absolute value.
    MATH_abs(electrical_power_mw_mva, absolute_value, uint32_t);
    // Select sign.
    una_electrical_power.sign = (electrical_power_mw_mva < 0) ? UNA_SIGN_NEGATIVE : UNA_SIGN_POSITIVE;
    // Select unit.
    if (absolute_value < (0b1 << UNA_ELECTRICAL_POWER_VALUE_SIZE_BITS)) {
        una_electrical_power.unit = UNA_ELECTRICAL_POWER_UNIT_MW_MVA;
    }
    else {
        absolute_value /= UNA_MW_MVA_PER_DW_DVA;
        if (absolute_value < (0b1 << UNA_ELECTRICAL_POWER_VALUE_SIZE_BITS)) {
            una_electrical_power.unit = UNA_ELECTRICAL_POWER_UNIT_DW_DVA;
        }
        else {
            absolute_value /= UNA_DW_DVA_PER_W_VA;
            if (absolute_value < (0b1 << UNA_ELECTRICAL_POWER_VALUE_SIZE_BITS)) {
                una_electrical_power.unit = UNA_ELECTRICAL_POWER_UNIT_W_VA;
            }
            else {
                absolute_value /= UNA_W_VA_PER_DAW_DAVA;
                una_electrical_power.unit = UNA_ELECTRICAL_POWER_UNIT_DAW_DAVA;
            }
        }
    }
    una_electrical_power.value = absolute_value;
    return (una_electrical_power.representation);
}

/*******************************************************************/
int32_t UNA_get_mw_mva(uint32_t una_electrical_power) {
    // Local variables.
    int32_t electrical_power_mw_mva = 0;
    int32_t absolute_value = 0;
    int32_t sign_multiplicator = 0;
    uint32_t local_una_electrical_power = una_electrical_power;
    UNA_electrical_power_unit_t unit = UNA_ELECTRICAL_POWER_UNIT_MW_MVA;
    UNA_sign_t sign = UNA_SIGN_POSITIVE;
    // Parse fields.
    sign = ((UNA_electrical_power_t*) &local_una_electrical_power)->sign;
    unit = ((UNA_electrical_power_t*) &local_una_electrical_power)->unit;
    absolute_value = (int32_t) ((UNA_electrical_power_t*) &local_una_electrical_power)->value;
    // Compute multiplicator.
    sign_multiplicator = (sign == UNA_SIGN_NEGATIVE) ? (-1) : (1);
    // Compute seconds.
    switch (unit) {
    case UNA_ELECTRICAL_POWER_UNIT_MW_MVA:
        electrical_power_mw_mva = (absolute_value * sign_multiplicator);
        break;
    case UNA_ELECTRICAL_POWER_UNIT_DW_DVA:
        electrical_power_mw_mva = (UNA_MW_MVA_PER_DW_DVA * absolute_value * sign_multiplicator);
        break;
    case UNA_ELECTRICAL_POWER_UNIT_W_VA:
        electrical_power_mw_mva = (UNA_MW_MVA_PER_DW_DVA * UNA_DW_DVA_PER_W_VA * absolute_value * sign_multiplicator);
        break;
    default:
        electrical_power_mw_mva = (UNA_MW_MVA_PER_DW_DVA * UNA_DW_DVA_PER_W_VA * UNA_W_VA_PER_DAW_DAVA * absolute_value * sign_multiplicator);
        break;
    }
    return electrical_power_mw_mva;
}

/*******************************************************************/
uint32_t UNA_convert_mwh_mvah(int32_t electrical_energy_mwh_mvah) {
    // Local variables.
    UNA_electrical_energy_t una_electrical_energy;
    uint32_t absolute_value = 0;
    // Read absolute value.
    MATH_abs(electrical_energy_mwh_mvah, absolute_value, uint32_t);
    // Select sign.
    una_electrical_energy.sign = (electrical_energy_mwh_mvah < 0) ? UNA_SIGN_NEGATIVE : UNA_SIGN_POSITIVE;
    // Select unit.
    if (absolute_value < (0b1 << UNA_ELECTRICAL_ENERGY_VALUE_SIZE_BITS)) {
        una_electrical_energy.unit = UNA_ELECTRICAL_ENERGY_UNIT_MWH_MVAH;
    }
    else {
        absolute_value /= UNA_MWH_MVAH_PER_DWH_DVAH;
        if (absolute_value < (0b1 << UNA_ELECTRICAL_ENERGY_VALUE_SIZE_BITS)) {
            una_electrical_energy.unit = UNA_ELECTRICAL_ENERGY_UNIT_DWH_DVAH;
        }
        else {
            absolute_value /= UNA_DWH_DVAH_PER_WH_VAH;
            if (absolute_value < (0b1 << UNA_ELECTRICAL_ENERGY_VALUE_SIZE_BITS)) {
                una_electrical_energy.unit = UNA_ELECTRICAL_ENERGY_UNIT_WH_VAH;
            }
            else {
                absolute_value /= UNA_WH_VAH_PER_DAWH_DAVAH;
                una_electrical_energy.unit = UNA_ELECTRICAL_ENERGY_UNIT_DAWH_DAVAH;
            }
        }
    }
    una_electrical_energy.value = absolute_value;
    return (una_electrical_energy.representation);
}

/*******************************************************************/
int32_t UNA_get_mwh_mvah(uint32_t una_electrical_energy) {
    // Local variables.
    int32_t electrical_energy_mwh_mvah = 0;
    int32_t absolute_value = 0;
    int32_t sign_multiplicator = 0;
    uint32_t local_una_electrical_energy = una_electrical_energy;
    UNA_electrical_energy_unit_t unit = UNA_ELECTRICAL_ENERGY_UNIT_MWH_MVAH;
    UNA_sign_t sign = UNA_SIGN_POSITIVE;
    // Parse fields.
    sign = ((UNA_electrical_energy_t*) &local_una_electrical_energy)->sign;
    unit = ((UNA_electrical_energy_t*) &local_una_electrical_energy)->unit;
    absolute_value = (int32_t) ((UNA_electrical_energy_t*) &local_una_electrical_energy)->value;
    // Compute multiplicator.
    sign_multiplicator = (sign == UNA_SIGN_NEGATIVE) ? (-1) : (1);
    // Compute seconds.
    switch (unit) {
    case UNA_ELECTRICAL_ENERGY_UNIT_MWH_MVAH:
        electrical_energy_mwh_mvah = (absolute_value * sign_multiplicator);
        break;
    case UNA_ELECTRICAL_ENERGY_UNIT_DWH_DVAH:
        electrical_energy_mwh_mvah = (UNA_MWH_MVAH_PER_DWH_DVAH * absolute_value * sign_multiplicator);
        break;
    case UNA_ELECTRICAL_ENERGY_UNIT_WH_VAH:
        electrical_energy_mwh_mvah = (UNA_MWH_MVAH_PER_DWH_DVAH * UNA_DWH_DVAH_PER_WH_VAH * absolute_value * sign_multiplicator);
        break;
    default:
        electrical_energy_mwh_mvah = (UNA_MWH_MVAH_PER_DWH_DVAH * UNA_DWH_DVAH_PER_WH_VAH * UNA_WH_VAH_PER_DAWH_DAVAH * absolute_value * sign_multiplicator);
        break;
    }
    return electrical_energy_mwh_mvah;
}

/*******************************************************************/
uint32_t UNA_convert_power_factor(int32_t power_factor) {
    // Local variables.
    uint32_t una_power_factor = 0;
    // DINFox representation is equivalent to signed magnitude
    MATH_integer_to_signed_magnitude(power_factor, UNA_POWER_FACTOR_VALUE_SIZE_BITS, &una_power_factor);
    return ((uint32_t) una_power_factor);
}

/*******************************************************************/
int32_t UNA_get_power_factor(uint32_t una_power_factor) {
    // Local variables.
    int32_t power_factor = 0;
    uint32_t value = 0;
    uint32_t local_una_power_factor = una_power_factor;
    UNA_sign_t sign = UNA_SIGN_POSITIVE;
    // Parse fields.
    sign = (((UNA_power_factor_t*) &local_una_power_factor)->sign);
    value = (uint32_t) (((UNA_power_factor_t*) &local_una_power_factor)->value);
    // Check sign.
    power_factor = (sign == UNA_SIGN_POSITIVE) ? ((int32_t) value) : ((-1) * ((int32_t) value));
    return power_factor;
}

/*******************************************************************/
uint32_t UNA_convert_dbm(int32_t rf_power_dbm) {
    return ((uint32_t) (rf_power_dbm + UNA_RF_POWER_OFFSET));
}

/*******************************************************************/
int32_t UNA_get_dbm(uint32_t una_rf_power) {
    return ((int32_t) (una_rf_power - UNA_RF_POWER_OFFSET));
}

#endif /* UNA_LIB_DISABLE */
