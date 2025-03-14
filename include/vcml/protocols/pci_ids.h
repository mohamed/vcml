/******************************************************************************
 *                                                                            *
 * Copyright 2021 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_PROTOCOLS_PCI_IDS_H
#define VCML_PROTOCOLS_PCI_IDS_H

#include "vcml/core/types.h"

namespace vcml {

enum pci_device_class : u32 {
    PCI_CLASS_STORAGE_SCSI = 0x010000,
    PCI_CLASS_STORAGE_IDE = 0x010100,
    PCI_CLASS_STORAGE_FLOPPY = 0x010200,
    PCI_CLASS_STORAGE_IPI = 0x010300,
    PCI_CLASS_STORAGE_RAID = 0x010400,
    PCI_CLASS_STORAGE_ATA = 0x010500,
    PCI_CLASS_STORAGE_SATA = 0x010600,
    PCI_CLASS_STORAGE_SAS = 0x010700,
    PCI_CLASS_STORAGE_EXPRESS = 0x010800,
    PCI_CLASS_STORAGE_OTHER = 0x018000,

    PCI_CLASS_NETWORK_ETHERNET = 0x020000,
    PCI_CLASS_NETWORK_TOKEN_RING = 0x020100,
    PCI_CLASS_NETWORK_FDDI = 0x020200,
    PCI_CLASS_NETWORK_ATM = 0x020300,
    PCI_CLASS_NETWORK_ISDN = 0x020400,
    PCI_CLASS_NETWORK_WORLDFIP = 0x020500,
    PCI_CLASS_NETWORK_PICMG214 = 0x020600,
    PCI_CLASS_NETWORK_OTHER = 0x028000,

    PCI_CLASS_DISPLAY_VGA = 0x030000,
    PCI_CLASS_DISPLAY_XGA = 0x030100,
    PCI_CLASS_DISPLAY_3D = 0x030200,
    PCI_CLASS_DISPLAY_OTHER = 0x038000,

    PCI_CLASS_MULTIMEDIA_VIDEO = 0x040000,
    PCI_CLASS_MULTIMEDIA_AUDIO = 0x040100,
    PCI_CLASS_MULTIMEDIA_PHONE = 0x040200,
    PCI_CLASS_MULTIMEDIA_OTHER = 0x048000,

    PCI_CLASS_MEMORY_RAM = 0x050000,
    PCI_CLASS_MEMORY_FLASH = 0x050100,
    PCI_CLASS_MEMORY_OTHER = 0x058000,

    PCI_CLASS_BRIDGE_HOST = 0x060000,
    PCI_CLASS_BRIDGE_ISA = 0x060100,
    PCI_CLASS_BRIDGE_EISA = 0x060200,
    PCI_CLASS_BRIDGE_MC = 0x060300,
    PCI_CLASS_BRIDGE_PCI = 0x060400,
    PCI_CLASS_BRIDGE_PCMCIA = 0x060500,
    PCI_CLASS_BRIDGE_NUBUS = 0x060600,
    PCI_CLASS_BRIDGE_CARDBUS = 0x060700,
    PCI_CLASS_BRIDGE_RACEWAY = 0x060800,
    PCI_CLASS_BRIDGE_PCI_SEMITP = 0x060900,
    PCI_CLASS_BRIDGE_IB_PCI = 0x060a00,
    PCI_CLASS_BRIDGE_OTHER = 0x068000,

    PCI_CLASS_COMM_SERIAL = 0x070000,
    PCI_CLASS_COMM_PARALLEL = 0x070100,
    PCI_CLASS_COMM_MULTISERIAL = 0x070200,
    PCI_CLASS_COMM_MODEM = 0x070300,
    PCI_CLASS_COMM_GPIB = 0x070400,
    PCI_CLASS_COMM_SC = 0x070500,
    PCI_CLASS_COMM_OTHER = 0x078000,

    PCI_CLASS_SYSTEM_PIC = 0x080000,
    PCI_CLASS_SYSTEM_PIC_IOAPIC = 0x080010,
    PCI_CLASS_SYSTEM_PIC_IOXAPIC = 0x080020,
    PCI_CLASS_SYSTEM_DMA = 0x080100,
    PCI_CLASS_SYSTEM_TIMER = 0x080200,
    PCI_CLASS_SYSTEM_RTC = 0x080300,
    PCI_CLASS_SYSTEM_PCI_HOTPLUG = 0x080400,
    PCI_CLASS_SYSTEM_SDHCI = 0x080500,
    PCI_CLASS_SYSTEM_OTHER = 0x088000,

    PCI_CLASS_INPUT_KEYBOARD = 0x090000,
    PCI_CLASS_INPUT_PEN = 0x090100,
    PCI_CLASS_INPUT_MOUSE = 0x090200,
    PCI_CLASS_INPUT_SCANNER = 0x090300,
    PCI_CLASS_INPUT_GAMEPORT = 0x090400,
    PCI_CLASS_INPUT_OTHER = 0x098000,

    PCI_CLASS_DOCKING_GENERIC = 0x0a0000,
    PCI_CLASS_DOCKING_OTHER = 0x0a8000,

    PCI_CLASS_PROCESSOR_PENTIUM = 0x0b0200,
    PCI_CLASS_PROCESSOR_POWERPC = 0x0b2000,
    PCI_CLASS_PROCESSOR_MIPS = 0x0b3000,
    PCI_CLASS_PROCESSOR_CO = 0x0b4000,

    PCI_CLASS_SERIAL_FIREWIRE = 0x0c0000,
    PCI_CLASS_SERIAL_ACCESS = 0x0c0100,
    PCI_CLASS_SERIAL_SSA = 0x0c0200,
    PCI_CLASS_SERIAL_USB = 0x0c0300,
    PCI_CLASS_SERIAL_USB_UHCI = 0x0c0300,
    PCI_CLASS_SERIAL_USB_OHCI = 0x0c0310,
    PCI_CLASS_SERIAL_USB_EHCI = 0x0c0320,
    PCI_CLASS_SERIAL_USB_XHCI = 0x0c0330,
    PCI_CLASS_SERIAL_USB_UNKNOWN = 0x0c0380,
    PCI_CLASS_SERIAL_USB_DEVICE = 0x0c03fe,
    PCI_CLASS_SERIAL_FIBER = 0x0c0400,
    PCI_CLASS_SERIAL_SMBUS = 0x0c0500,
    PCI_CLASS_SERIAL_IB = 0x0c0600,
    PCI_CLASS_SERIAL_IPMI = 0x0c0700,
    PCI_CLASS_SERIAL_SERCOS = 0x0c0800,
    PCI_CLASS_SERIAL_CANBUS = 0x0c0900,

    PCI_CLASS_WIRELESS_IRDA = 0x0d0000,
    PCI_CLASS_WIRELESS_CIR = 0x0d0100,
    PCI_CLASS_WIRELESS_RF_CONTROLLER = 0x0d1000,
    PCI_CLASS_WIRELESS_BLUETOOTH = 0x0d1100,
    PCI_CLASS_WIRELESS_BROADBAND = 0x0d1200,
    PCI_CLASS_WIRELESS_OTHER = 0x0d8000,

    PCI_CLASS_SATELLITE_TV = 0x0f0000,
    PCI_CLASS_SATELLITE_AUDIO = 0x0f0100,
    PCI_CLASS_SATELLITE_VOICE = 0x0f0300,
    PCI_CLASS_SATELLITE_DATA = 0x0f0400,

    PCI_CLASS_CRYPT_NETWORK = 0x100000,
    PCI_CLASS_CRYPT_ENTERTAINMENT = 0x100100,
    PCI_CLASS_CRYPT_OTHER = 0x108000,

    PCI_CLASS_SP_DPIO = 0x110000,
    PCI_CLASS_SP_PERF = 0x110100,
    PCI_CLASS_SP_SYNCH = 0x111000,
    PCI_CLASS_SP_MANAGEMENT = 0x112000,
    PCI_CLASS_SP_OTHER = 0x118000,

    PCI_CLASS_OTHERS = 0xff0000,
};

constexpr u8 pci_dev_class(u32 cls) {
    return cls >> 16;
}
constexpr u8 pci_sub_class(u32 cls) {
    return cls >> 8;
}
constexpr u8 pci_reg_iface(u32 cls) {
    return cls;
}

constexpr u32 pci_class_code(u8 clazz, u8 subclass, u8 prog, u8 rev) {
    return (u32)clazz << 24 | (u32)subclass << 16 | (u32)prog << 8 | rev;
}

constexpr u32 pci_class_code(u32 code, u8 rev) {
    return code << 8 | rev;
}

enum pci_vendor_device_ids : u32 {
    PCI_VENDOR_QUMRANET = 0x1af4,
    PCI_DEVICE_VIRTIO = 0x1040,
};

} // namespace vcml

#endif
