# Description

This repository contains the common definitions of the **Unified Node Access (UNA)** protocol.

# Dependencies

The driver relies on:

* An external `types.h` header file defining the **standard C types** of the targeted MCU.
* The **embedded utility functions** defined in the [embedded-utils](https://github.com/Ludovic-Lesur/embedded-utils) repository.

Here is the versions compatibility table:

| **una-lib** | **embedded-utils** |
|:---:|:---:|
| [sw2.1](https://github.com/Ludovic-Lesur/una-lib/releases/tag/sw2.1) | >= [sw7.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw7.0) |
| [sw2.0](https://github.com/Ludovic-Lesur/una-lib/releases/tag/sw2.0) | [sw6.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw6.0) to [sw6.2](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw6.2) |
| [sw1.0](https://github.com/Ludovic-Lesur/una-lib/releases/tag/sw1.0) | [sw6.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw6.0) to [sw6.2](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw6.2) |

# Compilation flags

| **Flag name** | **Value** | **Description** |
|:---:|:---:|:---:|
| `UNA_LIB_DISABLE_FLAGS_FILE` | `defined` / `undefined` | Disable the `una_lib_flags.h` header file inclusion when compilation flags are given in the project settings or by command line. |
| `UNA_LIB_DISABLE` | `defined` / `undefined` | Disable the UNA library. |
| `UNA_LIB_USE_BOARD_NAME` | `defined` / `undefined` | Enable or disable the UNA boards name list. |
