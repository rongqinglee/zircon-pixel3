// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define HAS_DEVICE_TREE 1
#define CLUSTER(power) \
    { \
        .entity_type = ZBI_TOPOLOGY_ENTITY_CLUSTER, \
        .parent_index = ZBI_TOPOLOGY_NO_PARENT, \
        .entity = { \
            .cluster = { \
                .performance_class = power, \
            } \
        } \
    }
#define PROCESSOR(index, parent, theflags) \
    { \
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR, \
        .parent_index = parent, \
        .entity = { \
            .processor = { \
                .logical_ids = {index}, \
                .logical_id_count = 1, \
                .flags = theflags, \
                .architecture = ZBI_TOPOLOGY_ARCH_ARM, \
                .architecture_info = { \
                    .arm = { \
                        .cluster_1_id = index, \
                        .cpu_id = 0, \
                        .gic_id = index, \
                    } \
                } \
            } \
        } \
    }

static const zbi_topology_node_t topology_config[] = {
    CLUSTER(0), //0
    PROCESSOR(0, 0, ZBI_TOPOLOGY_PROCESSOR_PRIMARY),
/* multiprocessor doesn't work
    PROCESSOR(1, 0, 0),
    PROCESSOR(2, 0, 0),
    PROCESSOR(3, 0, 0),
    CLUSTER(1), // 5
    PROCESSOR(4, 5, 0),
    PROCESSOR(5, 5, 0),
    PROCESSOR(6, 5, 0),
    PROCESSOR(7, 5, 0),
*/
};

static const zbi_mem_range_t mem_config[] = {
    {
        .type = ZBI_MEM_RANGE_RAM,
	.paddr = 0x80000000,
        .length = 0x100000000, // 4GB
    },
    {
        .type = ZBI_MEM_RANGE_PERIPHERAL,
        .paddr = 0x00000000,
        .length = 0x80000000,
    },
};

static const zbi_nvram_t nvram_config = {
    // pstore alternate
    .base = 0xa1a10000,
    .length = 0x200000,
};

static const dcfg_arm_gicv3_driver_t gicv3_driver = {
    .mmio_phys = 0x17a00000,
    .gicd_offset = 0x00000,
    .gicr_offset = 0x60000,
    .gicr_stride = 0x20000,
    .ipi_base = 9,
};

static const dcfg_arm_psci_driver_t psci_driver = {
    .use_hvc = false,
};

static const dcfg_arm_generic_timer_driver_t timer_driver = {
    .irq_phys = 16 + 2, // GIC_PPI 2
    .irq_virt = 16 + 3, // GIC_PPI 3
};

static const zbi_platform_id_t platform_id = {
    .vid = PDEV_VID_GOOGLE,
    .pid = PDEV_PID_CROSSHATCH,
    .board_name = "crosshatch",
};

static void append_board_boot_item(zbi_header_t* bootdata) {
    // add CPU configuration
    append_boot_item(bootdata, ZBI_TYPE_CPU_TOPOLOGY, sizeof(zbi_topology_node_t),
                    &topology_config,
                    sizeof(zbi_topology_node_t) * countof(topology_config));

    // add memory configuration
    append_boot_item(bootdata, ZBI_TYPE_MEM_CONFIG, 0, &mem_config,
                    sizeof(zbi_mem_range_t) * countof(mem_config));

    // append nvram config. Needed since otherwise Zircon defaults to 0x0?
    append_boot_item(bootdata, ZBI_TYPE_NVRAM, 0, &nvram_config,
                    sizeof(zbi_nvram_t));

    // add kernel drivers
    append_boot_item(bootdata, ZBI_TYPE_KERNEL_DRIVER, KDRV_ARM_GIC_V3, &gicv3_driver,
                    sizeof(gicv3_driver));
    append_boot_item(bootdata, ZBI_TYPE_KERNEL_DRIVER, KDRV_ARM_PSCI, &psci_driver,
                    sizeof(psci_driver));
    append_boot_item(bootdata, ZBI_TYPE_KERNEL_DRIVER, KDRV_ARM_GENERIC_TIMER, &timer_driver,
                    sizeof(timer_driver));

    // add platform ID
    append_boot_item(bootdata, ZBI_TYPE_PLATFORM_ID, 0, &platform_id, sizeof(platform_id));
}
