/*
 * una.h
 *
 *  Created on: 06 dec. 2024
 *      Author: Ludo
 */

#ifndef __UNA_H__
#define __UNA_H__

#ifndef UNA_LIB_DISABLE_FLAGS_FILE
#include "una_lib_flags.h"
#endif
#include "math.h"
#include "types.h"

#ifndef UNA_LIB_DISABLE

/*** UNA macros ***/

#define UNA_REGISTER_SIZE_BITS              MATH_U32_SIZE_BITS
#define UNA_REGISTER_SIZE_BYTES             MATH_U32_SIZE_BYTES

#define UNA_REGISTER_MASK_NONE              0
#define UNA_REGISTER_MASK_ALL               MATH_U32_MASK

#define UNA_TIME_ERROR_VALUE                0xFF
#define UNA_TEMPERATURE_ERROR_VALUE         0x7F
#define UNA_HUMIDITY_ERROR_VALUE            0xFF
#define UNA_VOLTAGE_ERROR_VALUE             0xFFFF
#define UNA_CURRENT_ERROR_VALUE             0xFFFF
#define UNA_ELECTRICAL_POWER_ERROR_VALUE    0x7FFF
#define UNA_ELECTRICAL_ENERGY_ERROR_VALUE   0x7FFF
#define UNA_POWER_FACTOR_ERROR_VALUE        0x7F
#define UNA_RF_POWER_ERROR_VALUE            0xFF
#define UNA_MAINS_FREQUENCY_ERROR_VALUE     0xFFFF

#define UNA_NODE_ADDRESS_RANGE_LVRM         8
#define UNA_NODE_ADDRESS_RANGE_DDRM         8
#define UNA_NODE_ADDRESS_RANGE_RRM          8

/*** UNA structures ***/

/*!******************************************************************
 * \enum UNA_board_id_t
 * \brief UNA board identifiers list.
 *******************************************************************/
typedef enum {
    UNA_BOARD_ID_LVRM = 0,
    UNA_BOARD_ID_BPSM,
    UNA_BOARD_ID_DDRM,
    UNA_BOARD_ID_UHFM,
    UNA_BOARD_ID_GPSM,
    UNA_BOARD_ID_SM,
    UNA_BOARD_ID_DIM,
    UNA_BOARD_ID_RRM,
    UNA_BOARD_ID_DMM,
    UNA_BOARD_ID_MPMCM,
    UNA_BOARD_ID_R4S8CR,
    UNA_BOARD_ID_LAST,
    UNA_BOARD_ID_ERROR = 0xFF
} UNA_board_id_t;

/*!******************************************************************
 * \enum UNA_address_t
 * \brief DINFox boards address range.
 *******************************************************************/
typedef enum {
    UNA_NODE_ADDRESS_MASTER = 0x00,
    UNA_NODE_ADDRESS_DIM,
    UNA_NODE_ADDRESS_BPSM,
    UNA_NODE_ADDRESS_UHFM,
    UNA_NODE_ADDRESS_GPSM,
    UNA_NODE_ADDRESS_SM,
    UNA_NODE_ADDRESS_MPMCM,
    UNA_NODE_ADDRESS_LVRM_START = 0x20,
    UNA_NODE_ADDRESS_LVRM_END = (UNA_NODE_ADDRESS_LVRM_START + UNA_NODE_ADDRESS_RANGE_LVRM - 1),
    UNA_NODE_ADDRESS_DDRM_START,
    UNA_NODE_ADDRESS_DDRM_END = (UNA_NODE_ADDRESS_DDRM_START + UNA_NODE_ADDRESS_RANGE_DDRM - 1),
    UNA_NODE_ADDRESS_RRM_START,
    UNA_NODE_ADDRESS_RRM_END = (UNA_NODE_ADDRESS_RRM_START + UNA_NODE_ADDRESS_RANGE_RRM - 1),
    UNA_NODE_ADDRESS_R4S8CR_START = 0x70,
    UNA_NODE_ADDRESS_R4S8CR_END = (UNA_NODE_ADDRESS_R4S8CR_START + 14),
    UNA_NODE_ADDRESS_LAST,
    UNA_NODE_ADDRESS_ERROR = 0xFF
} UNA_node_address_t;

/*!******************************************************************
 * \struct UNA_node_t
 * \brief Node descriptor.
 *******************************************************************/
typedef struct {
    UNA_node_address_t address;
    UNA_board_id_t board_id;
} UNA_node_t;

/*!******************************************************************
 * \enum NODE_list_t
 * \brief Node list type.
 *******************************************************************/
typedef struct {
    UNA_node_t list[UNA_NODE_ADDRESS_LAST];
    uint8_t count;
} UNA_node_list_t;

/*!******************************************************************
 * \enum UNA_register_access_t
 * \brief UNA register accesses.
 *******************************************************************/
typedef enum {
    UNA_REGISTER_ACCESS_READ_ONLY = 0,
    UNA_REGISTER_ACCESS_READ_WRITE,
    UNA_REGISTER_ACCESS_LAST
} UNA_register_access_t;

/*!******************************************************************
 * \enum UNA_reply_type_t
 * \brief Node reply type.
 *******************************************************************/
typedef enum {
    UNA_ACCESS_TYPE_READ = 0b0,
    UNA_ACCESS_TYPE_WRITE = 0b1
} UNA_access_type_t;

/*!******************************************************************
 * \enum UNA_reply_type_t
 * \brief Node reply type.
 *******************************************************************/
typedef enum {
    UNA_REPLY_TYPE_NONE = 0,
    UNA_REPLY_TYPE_OK,
    UNA_REPLY_TYPE_VALUE,
    UNA_REPLY_TYPE_LAST
} UNA_reply_type_t;

/*!******************************************************************
 * \struct UNA_reply_parameters_t
 * \brief Node reply parameters.
 *******************************************************************/
typedef struct {
    UNA_reply_type_t type;
    uint32_t timeout_ms;
} UNA_reply_parameters_t;

/*!******************************************************************
 * \struct UNA_command_parameters_t
 * \brief Node command parameters.
 *******************************************************************/
typedef struct {
    UNA_node_address_t node_addr;
    char_t* command;
} UNA_command_parameters_t;

/*!******************************************************************
 * \struct UNA_access_parameters_t
 * \brief Node access parameters.
 *******************************************************************/
typedef struct {
    UNA_node_address_t node_addr;
    uint8_t reg_addr;
    UNA_reply_parameters_t reply_params;
} UNA_access_parameters_t;

/*!******************************************************************
 * \union UNA_access_status_t
 * \brief Node access status.
 *******************************************************************/
typedef union {
    struct {
        UNA_access_type_t type :1;
        unsigned flags :7;
    } __attribute__((scalar_storage_order("big-endian")))__attribute__((packed));
    struct {
        UNA_access_type_t type_bis :1;
        unsigned unused :3;
        unsigned sequence_timeout :1;
        unsigned reply_timeout :1;
        unsigned parser_error :1;
        unsigned error_received :1;
    } __attribute__((scalar_storage_order("big-endian")))__attribute__((packed));
    uint8_t all;
} UNA_access_status_t;

/*!******************************************************************
 * \type UNA_bit_representation_t
 * \brief UNA bit representation type.
 *******************************************************************/
typedef enum {
    UNA_BIT_0 = 0b00,
    UNA_BIT_1 = 0b01,
    UNA_BIT_FORCED_HARDWARE = 0b10,
    UNA_BIT_ERROR = 0b11,
} UNA_bit_representation_t;

/*!******************************************************************
 * \type UNA_time_representation_t
 * \brief UNA time representation type.
 *******************************************************************/
typedef uint8_t UNA_time_representation_t;

/*!******************************************************************
 * \type UNA_temperature_representation_t
 * \brief UNA temperature representation type.
 *******************************************************************/
typedef uint8_t UNA_temperature_representation_t;

/*!******************************************************************
 * \type UNA_voltage_representation_t
 * \brief UNA voltage representation type.
 *******************************************************************/
typedef uint16_t UNA_voltage_representation_t;

/*!******************************************************************
 * \type UNA_current_representation_t
 * \brief UNA current representation type.
 *******************************************************************/
typedef uint16_t UNA_current_representation_t;

/*!******************************************************************
 * \type UNA_electrical_power_representation_t
 * \brief UNA electrical power representation type.
 *******************************************************************/
typedef uint16_t UNA_electrical_power_representation_t;

/*!******************************************************************
 * \type UNA_electrical_energy_representation_t
 * \brief UNA electrical energy representation type.
 *******************************************************************/
typedef uint16_t UNA_electrical_energy_representation_t;

/*!******************************************************************
 * \type UNA_power_factor_representation_t
 * \brief UNA power factor representation type.
 *******************************************************************/
typedef uint8_t UNA_power_factor_representation_t;

/*!******************************************************************
 * \type UNA_rf_power_representation_t
 * \brief UNA RF power representation type.
 *******************************************************************/
typedef uint8_t UNA_rf_power_representation_t;

/*!******************************************************************
 * \type UNA_year_representation_t
 * \brief UNA year representation type.
 *******************************************************************/
typedef uint8_t UNA_year_representation_t;

/*** UNA global variables ***/

#ifdef UNA_LIB_BOARD_NAME_ENABLE
extern const char_t* UNA_BOARD_NAME[UNA_BOARD_ID_LAST];
#endif

/*** UNA functions ***/

/*!******************************************************************
 * \fn void UNA_reset_node_list(UNA_node_list_t* node_list)
 * \brief Reset a nodes list.
 * \param[in]   node_list: List to reset.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void UNA_reset_node_list(UNA_node_list_t* node_list);

/*!******************************************************************
 * \fn UNA_time_representation_t UNA_convert_seconds(uint32_t time_seconds)
 * \brief Convert a time to UNA representation.
 * \param[in]   time_seconds: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_time_representation_t UNA_convert_seconds(uint32_t time_seconds);

/*!******************************************************************
 * \fn uint32_t UNA_get_seconds(UNA_time_representation_t una_time)
 * \brief Convert a UNA representation to time.
 * \param[in]   una_time: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted time value in seconds.
 *******************************************************************/
uint32_t UNA_get_seconds(UNA_time_representation_t una_time);

/*!******************************************************************
 * \fn UNA_temperature_representation_t UNA_convert_degrees(int8_t temperature_degrees)
 * \brief Convert a temperature to UNA representation.
 * \param[in]   temperature_degrees: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_temperature_representation_t UNA_convert_degrees(int8_t temperature_degrees);

/*!******************************************************************
 * \fn int8_t UNA_get_degrees(UNA_temperature_representation_t una_temperature)
 * \brief Convert a UNA representation to temperature.
 * \param[in]   una_temperature: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted temperature value in degrees.
 *******************************************************************/
int8_t UNA_get_degrees(UNA_temperature_representation_t una_temperature);

/*!******************************************************************
 * \fn UNA_voltage_representation_t UNA_convert_mv(uint32_t voltage_mv)
 * \brief Convert a voltage to UNA representation.
 * \param[in]   voltage_mv: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_voltage_representation_t UNA_convert_mv(uint32_t voltage_mv);

/*!******************************************************************
 * \fn uint32_t UNA_get_mv(UNA_voltage_representation_t una_voltage)
 * \brief Convert a UNA representation to voltage.
 * \param[in]   una_voltage: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted voltage value in mV.
 *******************************************************************/
uint32_t UNA_get_mv(UNA_voltage_representation_t una_voltage);

/*!******************************************************************
 * \fn UNA_current_representation_t UNA_convert_ua(uint32_t current_ua)
 * \brief Convert a current to UNA representation.
 * \param[in]   voltage_mv: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_current_representation_t UNA_convert_ua(uint32_t current_ua);

/*!******************************************************************
 * \fn uint32_t UNA_get_ua(UNA_current_representation_t una_current)
 * \brief Convert a UNA representation to current.
 * \param[in]   una_current: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted current value in uA.
 *******************************************************************/
uint32_t UNA_get_ua(UNA_current_representation_t una_current);

/*!******************************************************************
 * \fn UNA_electrical_power_representation_t UNA_convert_mw_mva(int32_t electrical_power_mw_mva)
 * \brief Convert an electrical power to UNA representation.
 * \param[in]   electrical_power_mw_mva: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_electrical_power_representation_t UNA_convert_mw_mva(int32_t electrical_power_mw_mva);

/*!******************************************************************
 * \fn int32_t UNA_get_mw_mva(UNA_electrical_power_representation_t una_electrical_power)
 * \brief Convert a UNA representation to electrical power.
 * \param[in]   una_electrical_power: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted electrical power value in mW or mVA.
 *******************************************************************/
int32_t UNA_get_mw_mva(UNA_electrical_power_representation_t una_electrical_power);

/*!******************************************************************
 * \fn UNA_electrical_energy_representation_t UNA_convert_mwh_mvah(int32_t electrical_energy_mwh_mvah)
 * \brief Convert an electrical energy to UNA representation.
 * \param[in]   electrical_energy_mwh_mvah: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_electrical_energy_representation_t UNA_convert_mwh_mvah(int32_t electrical_energy_mwh_mvah);

/*!******************************************************************
 * \fn int32_t UNA_get_mwh_mvah(UNA_electrical_energy_representation_t una_electrical_energy)
 * \brief Convert a UNA representation to electrical energy.
 * \param[in]   una_electrical_energy: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted electrical energy value in mWh or mVAh.
 *******************************************************************/
int32_t UNA_get_mwh_mvah(UNA_electrical_energy_representation_t una_electrical_energy);

/*!******************************************************************
 * \fn UNA_power_factor_representation_t UNA_convert_power_factor(int32_t power_factor)
 * \brief Convert an power factor to UNA representation.
 * \param[in]   power_factor: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_power_factor_representation_t UNA_convert_power_factor(int32_t power_factor);

/*!******************************************************************
 * \fn int32_t UNA_get_power_factor(UNA_power_factor_representation_t una_power_factor)
 * \brief Convert a UNA representation to electrical power.
 * \param[in]   una_electrical_power: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted power factor.
 *******************************************************************/
int32_t UNA_get_power_factor(UNA_power_factor_representation_t una_power_factor);

/*!******************************************************************
 * \fn UNA_rf_power_representation_t UNA_convert_dbm(int16_t rf_power_dbm)
 * \brief Convert an RF power to UNA representation.
 * \param[in]   rf_power_dbm: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_rf_power_representation_t UNA_convert_dbm(int16_t rf_power_dbm);

/*!******************************************************************
 * \fn int16_t UNA_get_dbm(UNA_rf_power_representation_t una_rf_power)
 * \brief Convert a UNA representation to current.
 * \param[in]   una_rf_power: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted RF power value in dBm.
 *******************************************************************/
int16_t UNA_get_dbm(UNA_rf_power_representation_t una_rf_power);

/*!******************************************************************
 * \fn UNA_year_representation_t UNA_convert_year(uint16_t year)
 * \brief Convert a year to UNA representation.
 * \param[in]   year: Value to convert
 * \param[out]  none
 * \retval      UNA representation.
 *******************************************************************/
UNA_year_representation_t UNA_convert_year(uint16_t year);

/*!******************************************************************
 * \fn uint16_t UNA_get_year(UNA_year_representation_t una_year)
 * \brief Convert a UNA representation to year.
 * \param[in]   una_year: UNA representation to convert.
 * \param[out]  none
 * \retval      Converted year value.
 *******************************************************************/
uint16_t UNA_get_year(UNA_year_representation_t una_year);

#endif /* UNA_LIB_DISABLE */

#endif /* __UNA_H__ */
