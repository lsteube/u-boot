/*
 * Copyright (c) 2011 The Chromium OS Authors.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __SANDBOX_ASM_IO_H
#define __SANDBOX_ASM_IO_H

/*
 * Given a physical address and a length, return a virtual address
 * that can be used to access the memory range with the caching
 * properties specified by "flags".
 */
#define MAP_NOCACHE	(0)
#define MAP_WRCOMBINE	(0)
#define MAP_WRBACK	(0)
#define MAP_WRTHROUGH	(0)

void *map_physmem(phys_addr_t paddr, unsigned long len, unsigned long flags);

/*
 * Take down a mapping set up by map_physmem().
 */
static inline void unmap_physmem(void *vaddr, unsigned long flags)
{

}

/* For sandbox, we want addresses to point into our RAM buffer */
static inline void *map_sysmem(phys_addr_t paddr, unsigned long len)
{
	return map_physmem(paddr, len, MAP_WRBACK);
}

static inline void unmap_sysmem(const void *vaddr)
{
}

/* Map from a pointer to our RAM buffer */
phys_addr_t map_to_sysmem(void *ptr);

#endif
