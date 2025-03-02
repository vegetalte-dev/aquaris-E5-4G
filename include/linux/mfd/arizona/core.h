/*
 * Arizona MFD internals
 *
 * Copyright 2012 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _WM_ARIZONA_CORE_H
#define _WM_ARIZONA_CORE_H

#include <linux/interrupt.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/mfd/arizona/pdata.h>

#define ARIZONA_MAX_CORE_SUPPLIES 2

enum arizona_type {
	WM5102 = 1,
	WM5110 = 2,
	WM8997 = 3,
	WM8280 = 4,
	WM8998 = 5,
	WM1814 = 6,
};

#define ARIZONA_IRQ_GP1                    0
#define ARIZONA_IRQ_GP2                    1
#define ARIZONA_IRQ_GP3                    2
#define ARIZONA_IRQ_GP4                    3
#define ARIZONA_IRQ_GP5_FALL               4
#define ARIZONA_IRQ_GP5_RISE               5
#define ARIZONA_IRQ_JD_FALL                6
#define ARIZONA_IRQ_JD_RISE                7
#define ARIZONA_IRQ_DSP1_RAM_RDY           8
#define ARIZONA_IRQ_DSP2_RAM_RDY           9
#define ARIZONA_IRQ_DSP3_RAM_RDY          10
#define ARIZONA_IRQ_DSP4_RAM_RDY          11
#define ARIZONA_IRQ_DSP_IRQ1              12
#define ARIZONA_IRQ_DSP_IRQ2              13
#define ARIZONA_IRQ_DSP_IRQ3              14
#define ARIZONA_IRQ_DSP_IRQ4              15
#define ARIZONA_IRQ_DSP_IRQ5              16
#define ARIZONA_IRQ_DSP_IRQ6              17
#define ARIZONA_IRQ_DSP_IRQ7              18
#define ARIZONA_IRQ_DSP_IRQ8              19
#define ARIZONA_IRQ_SPK_OVERHEAT_WARN     20
#define ARIZONA_IRQ_SPK_OVERHEAT          21
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
#define ARIZONA_IRQ_SPK_SHUTDOWN_WARN     20
#endif
#define ARIZONA_IRQ_MICDET                22
#define ARIZONA_IRQ_HPDET                 23
#define ARIZONA_IRQ_WSEQ_DONE             24
#define ARIZONA_IRQ_DRC2_SIG_DET          25
#define ARIZONA_IRQ_DRC1_SIG_DET          26
#define ARIZONA_IRQ_ASRC2_LOCK            27
#define ARIZONA_IRQ_ASRC1_LOCK            28
#define ARIZONA_IRQ_UNDERCLOCKED          29
#define ARIZONA_IRQ_OVERCLOCKED           30
#define ARIZONA_IRQ_FLL2_LOCK             31
#define ARIZONA_IRQ_FLL1_LOCK             32
#define ARIZONA_IRQ_CLKGEN_ERR            33
#define ARIZONA_IRQ_CLKGEN_ERR_ASYNC      34
#define ARIZONA_IRQ_ASRC_CFG_ERR          35
#define ARIZONA_IRQ_AIF3_ERR              36
#define ARIZONA_IRQ_AIF2_ERR              37
#define ARIZONA_IRQ_AIF1_ERR              38
#define ARIZONA_IRQ_CTRLIF_ERR            39
#define ARIZONA_IRQ_MIXER_DROPPED_SAMPLES 40
#define ARIZONA_IRQ_ASYNC_CLK_ENA_LOW     41
#define ARIZONA_IRQ_SYSCLK_ENA_LOW        42
#define ARIZONA_IRQ_ISRC1_CFG_ERR         43
#define ARIZONA_IRQ_ISRC2_CFG_ERR         44
#define ARIZONA_IRQ_BOOT_DONE             45
#define ARIZONA_IRQ_DCS_DAC_DONE          46
#define ARIZONA_IRQ_DCS_HP_DONE           47
#define ARIZONA_IRQ_FLL2_CLOCK_OK         48
#define ARIZONA_IRQ_FLL1_CLOCK_OK         49
#define ARIZONA_IRQ_MICD_CLAMP_RISE	  50
#define ARIZONA_IRQ_MICD_CLAMP_FALL	  51
#define ARIZONA_IRQ_HP3R_DONE             52
#define ARIZONA_IRQ_HP3L_DONE             53
#define ARIZONA_IRQ_HP2R_DONE             54
#define ARIZONA_IRQ_HP2L_DONE             55
#define ARIZONA_IRQ_HP1R_DONE             56
#define ARIZONA_IRQ_HP1L_DONE             57
#define ARIZONA_IRQ_ISRC3_CFG_ERR         58
#define ARIZONA_IRQ_DSP_SHARED_WR_COLL    59
#define ARIZONA_IRQ_SPK_SHUTDOWN          60
#define ARIZONA_IRQ_SPK1R_SHORT           61
#define ARIZONA_IRQ_SPK1L_SHORT           62
#define ARIZONA_IRQ_HP3R_SC_NEG           63
#define ARIZONA_IRQ_HP3R_SC_POS           64
#define ARIZONA_IRQ_HP3L_SC_NEG           65
#define ARIZONA_IRQ_HP3L_SC_POS           66
#define ARIZONA_IRQ_HP2R_SC_NEG           67
#define ARIZONA_IRQ_HP2R_SC_POS           68
#define ARIZONA_IRQ_HP2L_SC_NEG           69
#define ARIZONA_IRQ_HP2L_SC_POS           70
#define ARIZONA_IRQ_HP1R_SC_NEG           71
#define ARIZONA_IRQ_HP1R_SC_POS           72
#define ARIZONA_IRQ_HP1L_SC_NEG           73
#define ARIZONA_IRQ_HP1L_SC_POS           74
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
#define ARIZONA_IRQ_FLL3_LOCK             75 
#define ARIZONA_IRQ_FLL3_CLOCK_OK         76 

#define ARIZONA_NUM_IRQ                   77  
#else
#define ARIZONA_NUM_IRQ                   75
#endif

#define ARIZONA_HP_SHORT_IMPEDANCE        4
struct snd_soc_dapm_context;

struct arizona {
	struct regmap *regmap;
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
	struct regmap *regmap_32bit;
#endif

	
	struct device *dev;

	enum arizona_type type;
	unsigned int rev;

	int num_core_supplies;
	struct regulator_bulk_data core_supplies[ARIZONA_MAX_CORE_SUPPLIES];
	struct regulator *dcvdd;

	struct arizona_pdata pdata;

	unsigned int external_dcvdd:1;

	unsigned int irq_sem;
	int irq;
	struct irq_domain *virq;
	struct regmap_irq_chip_data *aod_irq_chip;
	struct regmap_irq_chip_data *irq_chip;
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
	bool hpdet_magic; 
#endif
	bool hpdet_clamp;
	unsigned int hp_ena;

	unsigned int hp_impedance;

	struct mutex clk_lock;
	int clk32k_ref;

	struct mutex subsys_max_lock;
	unsigned int subsys_max_rq;
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
	bool subsys_max_cached;	
#endif

	struct snd_soc_dapm_context *dapm;

	struct mutex reg_setting_lock;

	int tdm_width[ARIZONA_MAX_AIF];
	int tdm_slots[ARIZONA_MAX_AIF];

	uint16_t out_comp_coeff;
	uint8_t out_comp_enabled;
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
	bool bypass_cache; 
#endif
};

#define ARIZONA_DVFS_SR1_RQ          0x00000001
#define ARIZONA_DVFS_SR2_RQ          0x00000002
#define ARIZONA_DVFS_SR3_RQ          0x00000004
#define ARIZONA_DVFS_ASR1_RQ         0x00000010
#define ARIZONA_DVFS_ASR2_RQ         0x00000020
#define ARIZONA_DVFS_ADSP1_RQ        0x00010000

int arizona_clk32k_enable(struct arizona *arizona);
int arizona_clk32k_disable(struct arizona *arizona);
int arizona_dvfs_up(struct arizona *arizona, unsigned int mask);
int arizona_dvfs_down(struct arizona *arizona, unsigned int mask);

int arizona_request_irq(struct arizona *arizona, int irq, char *name,
			irq_handler_t handler, void *data);
void arizona_free_irq(struct arizona *arizona, int irq, void *data);
int arizona_set_irq_wake(struct arizona *arizona, int irq, int on);

int wm5102_patch(struct arizona *arizona);
int florida_patch(struct arizona *arizona);
int wm8997_patch(struct arizona *arizona);
int wm8998_patch(struct arizona *arizona);
#if defined(CONFIG_AUDIO_CODEC_FLORIDA)
int wm8285_patch(struct arizona *arizona); 
int cs47l24_patch(struct arizona *arizona); 
int wm5110_patch(struct arizona *arizona); 
#endif

extern int arizona_of_get_named_gpio(struct arizona *arizona, const char *prop,
				     bool mandatory);
extern int arizona_of_read_u32_array(struct arizona *arizona, const char *prop,
				     bool mandatory, u32 *data, size_t num);
extern int arizona_of_read_u32(struct arizona *arizona, const char* prop,
			       bool mandatory, u32 *data);

extern void arizona_florida_mute_analog(struct arizona* arizona,
					unsigned int mute);
extern void arizona_florida_clear_input(struct arizona *arizona);

#endif
