/*
 *  Planex MZK-W300NH board support
 *
 *  Copyright (C) 2008-2009 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <asm/mach-ar71xx/ar71xx.h>

#include "machtype.h"
#include "devices.h"
#include "dev-m25p80.h"
#include "dev-ar9xxx-wmac.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"

#define MZK_W300NH_GPIO_LED_STATUS	1
#define MZK_W300NH_GPIO_LED_WPS		3
#define MZK_W300NH_GPIO_LED_WLAN	6
#define MZK_W300NH_GPIO_LED_AP_GREEN	15
#define MZK_W300NH_GPIO_LED_AP_AMBER	16

#define MZK_W300NH_GPIO_BTN_APROUTER	5
#define MZK_W300NH_GPIO_BTN_WPS		12
#define MZK_W300NH_GPIO_BTN_RESET	21

#define MZK_W300NH_KEYS_POLL_INTERVAL	20	/* msecs */
#define MZK_W300NH_KEYS_DEBOUNCE_INTERVAL (3 * MZK_W300NH_KEYS_POLL_INTERVAL)

static struct gpio_led mzk_w300nh_leds_gpio[] __initdata = {
	{
		.name		= "planex:green:status",
		.gpio		= MZK_W300NH_GPIO_LED_STATUS,
		.active_low	= 1,
	}, {
		.name		= "planex:blue:wps",
		.gpio		= MZK_W300NH_GPIO_LED_WPS,
		.active_low	= 1,
	}, {
		.name		= "planex:green:wlan",
		.gpio		= MZK_W300NH_GPIO_LED_WLAN,
		.active_low	= 1,
	}, {
		.name		= "planex:green:aprouter",
		.gpio		= MZK_W300NH_GPIO_LED_AP_GREEN,
	}, {
		.name		= "planex:amber:aprouter",
		.gpio		= MZK_W300NH_GPIO_LED_AP_AMBER,
	}
};

static struct gpio_keys_button mzk_w300nh_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MZK_W300NH_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= MZK_W300NH_GPIO_BTN_RESET,
		.active_low	= 1,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = MZK_W300NH_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= MZK_W300NH_GPIO_BTN_WPS,
		.active_low	= 1,
	}, {
		.desc		= "aprouter",
		.type		= EV_KEY,
		.code		= BTN_2,
		.debounce_interval = MZK_W300NH_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= MZK_W300NH_GPIO_BTN_APROUTER,
		.active_low	= 0,
	}
};

#define MZK_W300NH_WAN_PHYMASK	BIT(4)
#define MZK_W300NH_MDIO_MASK	(~MZK_W300NH_WAN_PHYMASK)

static void __init mzk_w300nh_setup(void)
{
	u8 *eeprom = (u8 *) KSEG1ADDR(0x1fff1000);

	ar71xx_add_device_mdio(0, MZK_W300NH_MDIO_MASK);

	ar71xx_init_mac(ar71xx_eth0_data.mac_addr, eeprom, 0);
	ar71xx_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RMII;
	ar71xx_eth0_data.speed = SPEED_100;
	ar71xx_eth0_data.duplex = DUPLEX_FULL;
	ar71xx_eth0_data.has_ar8216 = 1;

	ar71xx_init_mac(ar71xx_eth1_data.mac_addr, eeprom, 1);
	ar71xx_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_RMII;
	ar71xx_eth1_data.phy_mask = MZK_W300NH_WAN_PHYMASK;

	ar71xx_add_device_eth(0);
	ar71xx_add_device_eth(1);

	ar71xx_add_device_m25p80(NULL);

	ar71xx_add_device_leds_gpio(-1, ARRAY_SIZE(mzk_w300nh_leds_gpio),
					mzk_w300nh_leds_gpio);

	ar71xx_register_gpio_keys_polled(-1, MZK_W300NH_KEYS_POLL_INTERVAL,
					 ARRAY_SIZE(mzk_w300nh_gpio_keys),
					 mzk_w300nh_gpio_keys);
	ar9xxx_add_device_wmac(eeprom, NULL);
}

MIPS_MACHINE(AR71XX_MACH_MZK_W300NH, "MZK-W300NH", "Planex MZK-W300NH",
	     mzk_w300nh_setup);
