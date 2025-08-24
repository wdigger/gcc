/* Common hooks for pdp11.
   Copyright (C) 1994-2024 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"

/* Implement TARGET_HANDLE_OPTION.  */

static bool
pdp11_handle_option (struct gcc_options *opts,
		     struct gcc_options *opts_set ATTRIBUTE_UNUSED,
		     const struct cl_decoded_option *decoded,
		     location_t loc ATTRIBUTE_UNUSED)
{
  size_t code = decoded->opt_index;

  switch (code)
    {
    case OPT_m10:
      opts->x_target_flags &= ~(MASK_FPU | MASK_AC0 | MASK_SPLIT);
      opts->x_pdp11_model = OPTION_MASK_M11_10;
      return true;

    case OPT_m40:
      opts->x_target_flags &= ~(MASK_FPU | MASK_AC0 | MASK_SPLIT);
      opts->x_pdp11_model = OPTION_MASK_M11_40;
      return true;

    case OPT_mbm1g:
      opts->x_target_flags &= ~(MASK_FPU | MASK_AC0 | MASK_SPLIT);
      opts->x_pdp11_model = OPTION_MASK_1801BM1G;
      return true;

//    case OPT_mbm1:
    case OPT_mbm1a:
      opts->x_target_flags &= ~(MASK_FPU | MASK_AC0 | MASK_SPLIT);
      opts->x_pdp11_model = OPTION_MASK_1801BM1A;
      return true;

    case OPT_mbm2:
      opts->x_target_flags &= ~(MASK_FPU | MASK_AC0 | MASK_SPLIT);
      opts->x_pdp11_model = OPTION_MASK_1801BM2;
      return true;

    case OPT_m45:
      opts->x_target_flags |= (MASK_FPU | MASK_AC0 | MASK_SPLIT);
      opts->x_pdp11_model = OPTION_MASK_M11_45;
      return true;

    case OPT_mfpu:
      opts->x_pdp11_model &= ~(OPTION_MASK_M11_40 | OPTION_MASK_1801BM1A | OPTION_MASK_1801BM1G | OPTION_MASK_1801BM2);
      opts->x_pdp11_model |= OPTION_MASK_M11_45;
      return true;
      
    case OPT_msoft_float:
      opts->x_target_flags &= ~MASK_AC0;
      return true;

    case OPT_msplit:
      opts->x_pdp11_model &= ~(OPTION_MASK_M11_40 |  OPTION_MASK_1801BM1A | OPTION_MASK_1801BM1G | OPTION_MASK_1801BM2);
      opts->x_pdp11_model |= OPTION_MASK_M11_45;
      return true;

    case OPT_munix_asm:
    case OPT_mgnu_asm:
      targetm_common.have_named_sections = false;
      return true;

    case OPT_mdec_asm:
      targetm_common.have_named_sections = true;
      return true;

    default:
      return true;
    }
}

/* Implement TARGET_OPTION_INIT_STRUCT.  */

static void
pdp11_option_init_struct (struct gcc_options *opts)
{
  opts->x_flag_finite_math_only = 0;
  opts->x_flag_trapping_math = 0;
  opts->x_flag_signaling_nans = 0;
}

#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS \
  (MASK_FPU | TARGET_UNIX_ASM_DEFAULT)
#undef TARGET_DEFAULT_TARGET_CPU_MODEL
#define TARGET_DEFAULT_TARGET_CPU_MODEL \
  (OPTION_MASK_M11_45)
#undef TARGET_HANDLE_OPTION
#define TARGET_HANDLE_OPTION pdp11_handle_option
#undef TARGET_OPTION_INIT_STRUCT
#define TARGET_OPTION_INIT_STRUCT pdp11_option_init_struct

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
