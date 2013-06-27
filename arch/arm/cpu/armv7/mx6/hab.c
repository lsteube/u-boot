/*
 * Copyright (C) 2010-2013 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#if defined(CONFIG_SECURE_BOOT)
#include <asm/arch/hab.h>

#ifdef CONFIG_SECURE_BOOT
/* -------- start of HAB API updates ------------*/
#define hab_rvt_report_event ((hab_rvt_report_event_t *)HAB_RVT_REPORT_EVENT)
#define hab_rvt_report_status ((hab_rvt_report_status_t *)HAB_RVT_REPORT_STATUS)
#define hab_rvt_authenticate_image \
	((hab_rvt_authenticate_image_t *)HAB_RVT_AUTHENTICATE_IMAGE)
#define hab_rvt_entry ((hab_rvt_entry_t *) HAB_RVT_ENTRY)
#define hab_rvt_exit ((hab_rvt_exit_t *) HAB_RVT_EXIT)
#define hab_rvt_clock_init HAB_RVT_CLOCK_INIT


bool is_hab_enabled(void)
{
        struct ocotp_regs *ocotp = (struct ocotp_regs *)OCOTP_BASE_ADDR;
        struct fuse_bank *bank = &ocotp->bank[0];
        struct fuse_bank0_regs *fuse =
                        (struct fuse_bank0_regs *)bank->fuse_regs;
        uint32_t reg = readl(&fuse->cfg5);

        return (reg & 0x2) == 0x2;
}


void display_event(uint8_t *event_data, size_t bytes)
{
	uint32_t i;

	if ((event_data) && (bytes > 0)) {
		for (i = 0; i < bytes; i++) {
			if (i == 0)
				printf("\t0x%02x", event_data[i]);
			else if ((i % 8) == 0)
				printf("\n\t0x%02x", event_data[i]);
			else
				printf(" 0x%02x", event_data[i]);
		}
	}
}

int get_hab_status(void)
{
	uint32_t index = 0; /* Loop index */
	uint8_t event_data[128]; /* Event data buffer */
	size_t bytes = sizeof(event_data); /* Event size in bytes */
	hab_config_t config = 0;
	hab_state_t state = 0;

	if (is_hab_enabled())
		printf("\nSecure boot enabled\n");
	else
		printf("\nSecure boot disabled\n");

	/* Check HAB status */
	if (hab_rvt_report_status(&config, &state) != HAB_SUCCESS) {
		printf("\nHAB Configuration: 0x%02x, HAB State: 0x%02x\n",
			config, state);

		/* Display HAB Error events */
		while (hab_rvt_report_event(HAB_FAILURE, index, event_data,
				&bytes) == HAB_SUCCESS) {
			printf("\n");
			printf("--------- HAB Event %d -----------------\n",
					index + 1);
			printf("event data:\n");
			display_event(event_data, bytes);
			printf("\n");
			bytes = sizeof(event_data);
			index++;
		}
	}
	/* Display message if no HAB events are found */
	else {
		printf("\nHAB Configuration: 0x%02x, HAB State: 0x%02x\n",
			config, state);
		printf("No HAB Events Found!\n\n");
	}
	return 0;
}

int do_hab_status(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if ((argc != 1)) {
		cmd_usage(cmdtp);
		return 1;
	}

	get_hab_status();

	return 0;
}

U_BOOT_CMD(
	hab_status, CONFIG_SYS_MAXARGS, 1, do_hab_status,
	"display HAB status",
	""
);

#endif

#endif
