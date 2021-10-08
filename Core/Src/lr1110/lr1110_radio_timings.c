/**
 * @file      lr1110_radio_timings.c
 *
 * @brief     LR1110 timing helper functions implementation
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr1110_radio_timings.h"
#include "lr1110_radio.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/**
 * @brief Time in microsecond taken by the chip to process the Rx done interrupt
 */
#define RX_DONE_IRQ_PROCESSING_TIME_IN_US 74

/**
 * @brief Time in microsecond taken by the chip to process the Tx done interrupt
 */
#define TX_DONE_IRQ_PROCESSING_TIME_IN_US 111

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/**
 * @brief Get the power amplifier ramp time for a given power amplifier ramp time parameter
 *
 * @param [in] ramp_time Power amplifier ramp time parameter
 *
 * @returns Ramp time in microsecond
 */
static uint32_t lr1110_radio_timings_get_pa_ramp_time_in_us( const lr1110_radio_ramp_time_t ramp_time );

/**
 * @brief Get the LoRa reception input delay
 *
 * @param [in] bw LoRa bandwidth
 *
 * @returns LoRa reception input delay in microsecond
 */
static uint32_t lr1110_radio_timings_get_lora_rx_input_delay_in_us( lr1110_radio_lora_bw_t bw );

/**
 * @brief Get the LoRa symbol time
 *
 * @param [in] bw LoRa bandwidth
 * @param [in] sf LoRa spreading factor
 *
 * @returns LoRa symbol time in microsecond
 */
static uint32_t lr1110_radio_timings_get_lora_symb_time_in_us( const lr1110_radio_lora_sf_t sf,
                                                               const lr1110_radio_lora_bw_t bw );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

uint32_t lr1110_radio_timings_get_delay_between_last_bit_sent_and_rx_done_in_us(
    const lr1110_radio_modulation_param_lora_t* mod_params )
{
    return lr1110_radio_timings_get_lora_rx_input_delay_in_us( mod_params->bandwidth ) +
           2 * lr1110_radio_timings_get_lora_symb_time_in_us( mod_params->spreading_factor, mod_params->bandwidth ) +
           RX_DONE_IRQ_PROCESSING_TIME_IN_US;
}

uint32_t lr1110_radio_timings_get_delay_between_last_bit_sent_and_tx_done_in_us(
    const lr1110_radio_ramp_time_t ramp_time )
{
    return lr1110_radio_timings_get_pa_ramp_time_in_us( ramp_time ) + TX_DONE_IRQ_PROCESSING_TIME_IN_US;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static uint32_t lr1110_radio_timings_get_pa_ramp_time_in_us( const lr1110_radio_ramp_time_t ramp_time )
{
    switch( ramp_time )
    {
    case LR1110_RADIO_RAMP_TIME_10U:
    {
        return 10;
    }
    case LR1110_RADIO_RAMP_TIME_20U:
    {
        return 20;
    }
    case LR1110_RADIO_RAMP_TIME_40U:
    {
        return 40;
    }
    case LR1110_RADIO_RAMP_TIME_80U:
    {
        return 80;
    }
    case LR1110_RADIO_RAMP_TIME_200U:
    {
        return 200;
    }
    case LR1110_RADIO_RAMP_TIME_800U:
    {
        return 800;
    }
    case LR1110_RADIO_RAMP_TIME_1700U:
    {
        return 1700;
    }
    case LR1110_RADIO_RAMP_TIME_3400U:
    {
        return 3400;
    }
    default:
        return 0;
    }
}

static uint32_t lr1110_radio_timings_get_lora_rx_input_delay_in_us( lr1110_radio_lora_bw_t bw )
{
    switch( bw )
    {
    case LR1110_RADIO_LORA_BW500:
    {
        return 16;
    }
    case LR1110_RADIO_LORA_BW250:
    {
        return 31;
    }
    case LR1110_RADIO_LORA_BW125:
    {
        return 57;
    }
    default:
    {
        return 0;
    }
    }
}

static uint32_t lr1110_radio_timings_get_lora_symb_time_in_us( const lr1110_radio_lora_sf_t sf,
                                                               const lr1110_radio_lora_bw_t bw )
{
    return ( 1 << ( uint8_t ) sf ) * 1000000 / lr1110_radio_get_lora_bw_in_hz( bw );
}

/* --- EOF ------------------------------------------------------------------ */
