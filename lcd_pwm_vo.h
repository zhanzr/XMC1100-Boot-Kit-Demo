#pragma once

#include <XMC1100.h>
#include <xmc_gpio.h>
#include <xmc_ccu4.h>
#include <xmc_scu.h>


typedef enum GLOBAL_CCU4_STATUS
{
  GLOBAL_CCU4_STATUS_SUCCESS = 0U, /**< Status success */
  GLOBAL_CCU4_STATUS_FAILURE /**< Status failure */
} GLOBAL_CCU4_STATUS_t;

/**
 * This saves the context of the GLOBAL_CCU4 APP.
 */
typedef struct GLOBAL_CCU4
{
  const uint32_t module_frequency; /**< fccu frequency */
  const XMC_SCU_CCU_TRIGGER_t syncstart_trigger_msk; /**< Mask to start the timers synchronously */
  XMC_CCU4_MODULE_t* const module_ptr;   /**< reference to module handle */
  XMC_CCU4_SLICE_MCMS_ACTION_t const mcs_action; /**< Shadow transfer of selected values in multi-channel mode */
  bool  is_initialized; /**< Indicates initialized state of particular instance of the APP */
} GLOBAL_CCU4_t;

#define PWM_CCU4_MAX_TIMER_COUNT  (65535U)

#define PWM_CCU4_DUTY_FULL_SCALE    (10000U) /*100% * 100*/
#define PWM_CCU4_DUTY_SCALE         (100U)   /*100*/

#define PWM_CCU4_SYM_DUTY_MAX       (10000U) /*duty Max*/
#define PWM_CCU4_SYM_DUTY_MIN       (0U)     /*duty Min*/

/**********************************************************************************************************************
* ENUMS
**********************************************************************************************************************/

 /**
  * @ingroup PWM_CCU4_enumerations
  * @{
  */

/**
 *   @brief The type identifies the APP status.
 */
typedef enum PWM_CCU4_STATUS
{
  /**
  * STATUS SUCCESS
  */
  PWM_CCU4_STATUS_SUCCESS = 0U,

  /**
  * STATUS FAILURE
  */
  PWM_CCU4_STATUS_FAILURE = 1U,

  /**
  * STATUS ALREADY INITIALIZED
  */
  PWM_CCU4_STATUS_ALREADY_INITIALIZED = 2U
} PWM_CCU4_STATUS_t;


/**
 * @brief The type identifies APP state.
 */
typedef enum PWM_CCU4_STATE
{
  /**
   * default state after power on reset
   * PWM_CCU4 APP is in uninitialized mode. The corresponding CCU4 timer is not configured.
   * PWM pulses is not generated.
   */
  PWM_CCU4_STATE_UNINITIALIZED = 0U,

  /**
   * PWM_CCU4 APP is in initialized mode. The corresponding CCU4 timer is configured.
   * The corresponding CCU4 timer is not started(not running).
   */
  PWM_CCU4_STATE_INITIALIZED = 1U,

  /**
   * PWM_CCU4 APP is in running mode. The corresponding CCU4 timer is running.
   * Trigger signal for any of the configured Interrupt or service request in the CCU4 timer is triggered.
   */
  PWM_CCU4_STATE_RUNNING = 2U,

  /**
   * PWM_CCU4 APP is in stopped mode. The corresponding CCU4 timer is stopped.
   * Trigger signal for any of the configured Interrupt or service request in the CCU4 timer is not triggered.
   */
  PWM_CCU4_STATE_STOPPED = 3U

} PWM_CCU4_STATE_t;

/**
 * @}
 */

/**********************************************************************************************************************
* DATA STRUCTURES
**********************************************************************************************************************/
/**
  * @ingroup PWM_CCU4_datastructures
  * @{
  */
/**
 * @brief Configuration parameters of the PWM_CCU4 APP
 */
typedef struct PWM_CCU4_ConfigType
{
    const    bool                                   start_control;            /**<Enables starting of timer after initialization*/
    const    uint16_t                               period_value;             /**<Period register value. Determines the frequency*/
    const    uint16_t                               compare_value;            /**<Channel 1 compare register value. Determines the duty cycle*/

    const    bool                                   int_per_match;            /**<Enables event service request generation when timer value equals to period register */
    const    bool                                   int_cmp_match_up;         /**<Enables event service request generation when timer is counting up and equals to channel 1 compare register*/
    const    bool                                   int_cmp_match_down;       /**<Enables event service request generation when timer is counting down and equals to channel 1 compare register*/
    const    bool                                   int_one_match_down;       /**<Enables event service request generation when timer is counting down and equals 1*/
    const    bool                                   int_e0;                   /**<Enables event service request generation by external event 0 signal based on the trigger edge selection */
    const    bool                                   int_e1;                   /**<Enables event service request generation by external event 1 signal based on the trigger edge selection */
    const    bool                                   int_e2;                   /**<Enables event service request generation by external event 2 signal based on the trigger edge selection */

    const    XMC_CCU4_SLICE_SR_ID_t                 sr_per_match;             /**<Service request node to which  period match event is forwarded*/
    const    XMC_CCU4_SLICE_SR_ID_t                 sr_cmp_match_up;          /**<Service request node to which  channel 1 compare match while timer counting up event is forwarded*/
    const    XMC_CCU4_SLICE_SR_ID_t                 sr_cmp_match_down;        /**<Service request node to which  channel 1 compare match while timer counting down event is forwarded*/
    const    XMC_CCU4_SLICE_SR_ID_t                 sr_one_match_down;        /**<Service request node to which  timer one match event is forwarded*/
    const    XMC_CCU4_SLICE_SR_ID_t                 sr_e0;                    /**<Service request node to which  event 0 is forwarded*/
    const    XMC_CCU4_SLICE_SR_ID_t                 sr_e1;                    /**<Service request node to which  event 1 is forwarded*/
    const    XMC_CCU4_SLICE_SR_ID_t                 sr_e2;                    /**<Service request node to which  event 2 is forwarded*/

    const    XMC_CCU4_SLICE_EVENT_CONFIG_t *const     event0_config_ptr;      /**<Points to the variable containing event 0 configuration*/
    const    XMC_CCU4_SLICE_EVENT_CONFIG_t *const     event1_config_ptr;      /**<Points to the variable containing event 1 configuration*/
    const    XMC_CCU4_SLICE_EVENT_CONFIG_t *const     event2_config_ptr;      /**<Points to the variable containing event 2 configuration*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_start_event;        /**<Defines to which event external start signal is connected*/
    const    XMC_CCU4_SLICE_START_MODE_t              ext_start_mode;         /**<Defines mode of starting the timer*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_stop_event;         /**<Defines to which event external stop signal is connected*/
    const    XMC_CCU4_SLICE_END_MODE_t                ext_stop_mode;          /**<Defines mode of stopping the timer*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_count_dir_event;    /**<Defines to which event external count direction signal is connected*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_gate_event;         /**<Defines to which event external gating signal is connected*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_count_event;        /**<Defines to which event external count signal is connected*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_load_event;         /**<Defines to which event external load signal is connected*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_mod_event;          /**<Defines to which event external modulation signal is connected*/
    const    XMC_CCU4_SLICE_MODULATION_MODE_t         ext_mod_mode;           /**<Defines mode of external modulation*/
    const    bool                                     ext_mod_sync;           /**<Defines mode of synchronization for external modulation*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_override_edge_event;/**<Defines to which event external edge override signal is connected*/

    const    XMC_CCU4_SLICE_EVENT_t                   ext_override_level_event;/**<Defines to which event external level override signal is connected*/

    const    bool                                     ext_trap_enable;         /**<Enables the trap*/
    const    XMC_CCU4_SLICE_EVENT_t                   ext_trap_event;          /**<Defines to which event external trap signal is connected*/
    const    bool                                     ext_trap_sync;           /**<Defines mode of synchronization*/
    const    XMC_CCU4_SLICE_TRAP_EXIT_MODE_t          ext_trap_exit;           /**<Defines mode of exiting trap state*/

    const    XMC_CCU4_MULTI_CHANNEL_SHADOW_TRANSFER_t mcm_shadow_txfr_mode;    /**<Defines the mode of shadow transfer in multi channel mode operation*/

#if (UC_SERIES == XMC14)/*below feature available in XMC14xx devices */
    const    XMC_CCU4_SLICE_SHADOW_TRANSFER_MODE_t    shadow_transfer_mode;        /**<Defines the timer value(s) at which shadow transfer trigger is generated*/
    const    uint32_t                                 immediate_write;             /**<Defines the registers that are enabled for immediate shadow transfer*/
    const    uint32_t                                 automatic_shadow_transfer;   /**<Defines the registers that are enabled for automatic shadow transfer*/
    const    bool                                     cascaded_shadow_txfr_enable; /**<Enables cascade of shadow transfer in timer concatenate mode*/
#endif

    const    XMC_CCU4_SLICE_COMPARE_CONFIG_t *const ccu4_cc4_slice_timer_ptr;  /**<Points to the variable CCU4 timer initialization*/

    const    bool                                   gpio_ch_out_enable;        /**<Enables GPIO initialization for channel 1 direct output*/
             XMC_GPIO_PORT_t   *const               gpio_ch_out_ptr;           /**<Points to PORT BASE address*/
    const    uint8_t                                gpio_ch_out_pin;           /**<Pin number in the selected PORT*/
    const    XMC_GPIO_CONFIG_t *const               gpio_ch_out_config_ptr;    /**<Points to the variable containing GPIO configuration*/

             GLOBAL_CCU4_t      *const              global_ccu4_handle;        /**<Points to GLOBAL_CCU4 APP handle*/

} PWM_CCU4_CONFIG_t;

/**
 * @brief Initialization parameters of the PWM_CCU4 APP
 */
typedef struct PWM_CCU4_HandleType
{
  const PWM_CCU4_CONFIG_t      *const  config_ptr;                  /**<Points to the variable containing PWM_CCU4 APP configuration*/
        XMC_CCU4_MODULE_t      *const  ccu4_module_ptr;             /**<Points to CCU4 global register base address*/
        XMC_CCU4_SLICE_t       *const  ccu4_slice_ptr;              /**<Points to CCU4 slice register base address*/
  const uint8_t                        kernel_number;               /**<CCU4 Kernel number*/
  const uint8_t                        slice_number;                /**<CCU4 slice number*/
  const uint32_t                       shadow_txfr_msk;             /**<Mask for enabling shadow transfer of period and compare registers*/
  const uint32_t                       dither_shadow_txfr_msk;      /**<Mask for enabling shadow transfer of dither registers*/
  const uint32_t                       prescaler_shadow_txfr_msk;   /**<Mask for enabling shadow transfer of floating prescaler registers*/

        PWM_CCU4_STATE_t               state;                       /**<Defines the current state of the PWM_CCU4 APP*/
        uint32_t                       frequency_tclk;              /**<Defines the operating frequency of the CCU4 slice*/
        uint32_t                       sym_duty;                    /**<Defines the channel 1 duty cycle in symmetric mode*/
} PWM_CCU4_t;

//TODO: to be removed or relocated
PWM_CCU4_STATUS_t PWM_CCU4_Start(PWM_CCU4_t* handle_ptr);
void PWM_CCU4_lInit_Interrupt(PWM_CCU4_t* handle_ptr);
void PWM_CCU4_lConfigure_Events(PWM_CCU4_t* handle_ptr);

void LCD_PWM_VO_Init(void);
