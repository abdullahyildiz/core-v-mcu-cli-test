/*==========================================================
 * Copyright 2020 QuickLogic Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *==========================================================*/

/*==========================================================
 *
 *    File   : main.c
 *    Purpose: 
 *                                                          
 *=========================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "libs/cli/include/cli.h"
#include "periph-tasks/include/write_uart_task.h"
#include "libs/utils/include/dbg_uart.h"
//#include "qlsh_commands.h"

// Sub menus
const struct cli_cmd_entry uart1_tests[];
const struct cli_cmd_entry mem_tests[];

// UART functions
static void uart1_tx(const struct cli_cmd_entry *pEntry);

// MEM functions
static void mem_print_start(const struct cli_cmd_entry *pEntry);
static void mem_check(const struct cli_cmd_entry *pEntry);
static void mem_peek(const struct cli_cmd_entry *pEntry);
static void mem_poke(const struct cli_cmd_entry *pEntry);

// Main menu
const struct cli_cmd_entry my_main_menu[] = {
  CLI_CMD_SUBMENU( "uart1", uart1_tests, "commands for uart1" ),
  CLI_CMD_SUBMENU( "mem", mem_tests, "commands for memory" ),
  CLI_CMD_TERMINATE()
};

// UART1 menu
const struct cli_cmd_entry uart1_tests[] =
{
  CLI_CMD_SIMPLE( "tx", uart1_tx, "<string>: write <string> to uart1" ),
  CLI_CMD_TERMINATE()
};

// mem menu
const struct cli_cmd_entry mem_tests[] =
{
  CLI_CMD_SIMPLE( "start", 	mem_print_start,   	"print start of unused memory" ),
  CLI_CMD_SIMPLE( "check", 	mem_check,         	"print start of unused memory" ),
  CLI_CMD_SIMPLE( "peek", 	mem_peek,         	"0xaddr -- print memory location " ),
  CLI_CMD_SIMPLE( "poke",   mem_poke,         	"0xaddr 0xvalue -- write value to addr" ),
  CLI_CMD_TERMINATE()
};

// UART functions
static void uart1_tx(const struct cli_cmd_entry *pEntry)
{
  char*  pzArg = NULL;
    (void)pEntry;
    // Add functionality here
    while (CLI_peek_next_arg() != NULL) {
      if (pzArg != NULL) {
    	  udma_uart_writeraw(1, 2, " ");
      }
      CLI_string_ptr_required("string", &pzArg);
      udma_uart_writeraw(1, strlen(pzArg), pzArg);
    }
    udma_uart_writeraw(1, 2, "\r\n");
    dbg_str("<<DONE>>");
    return;
}

// MEM functions
static void mem_print_start(const struct cli_cmd_entry *pEntry)
{
    (void)pEntry;
    // Add functionality here
    extern char __l2_shared_end;
    dbg_str_hex32("l2_shared_end", (uint32_t)(&__l2_shared_end));
    dbg_str("<<DONE>>");
}

static void mem_check(const struct cli_cmd_entry *pEntry)
{
    (void)pEntry;
    // Add functionality here
    bool  fPassed = true;
    extern char __l2_shared_end;
    uint32_t*  pl;
    for (pl = (uint32_t*)(&__l2_shared_end); (uint32_t)pl < 0x1c080000; pl++) {
      *pl = (uint32_t)pl;
    }

    // pl=0x1c070000; *pl = 76;  // Enable to force and error

    for (pl = (uint32_t*)(&__l2_shared_end); (uint32_t)pl < 0x1c080000; pl++) {
    if (*pl != (uint32_t)pl) {
      dbg_str_hex32("mem check fail at", (uint32_t)pl);
      dbg_str_hex32("read back        ", *pl);
      fPassed = false;
      break;
    }
  }
    if (fPassed) {
      dbg_str("<<PASSED>>");
    } else {
      dbg_str("<<FAILED>>");
    }
}

static void mem_peek(const struct cli_cmd_entry *pEntry)
{
    (void)pEntry;
    // Add functionality here
    uint32_t	xValue;
    uint32_t*	pAddr;

    CLI_uint32_required( "addr", &pAddr );
    xValue = *pAddr;
    dbg_str_hex32("value", xValue);
    dbg_str("<<DONE>>");
}

static void mem_poke(const struct cli_cmd_entry *pEntry)
{
    (void)pEntry;
    // Add functionality here
    uint32_t	xValue;
    uint32_t*	pAddr;

    CLI_uint32_required( "addr", &pAddr );
    CLI_uint32_required( "value", &xValue);
    *pAddr = xValue;
    dbg_str("<<DONE>>");
}



