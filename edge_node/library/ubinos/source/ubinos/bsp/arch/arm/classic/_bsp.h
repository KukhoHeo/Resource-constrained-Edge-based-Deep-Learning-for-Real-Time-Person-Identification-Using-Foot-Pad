/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BSP_H_
#define _BSP_H_

/**
 * @file _bsp.h
 *
 * @brief bsp 컴포넌트 내부 인터페이스
 *
 * BSP 컴포넌트 내부 인터페이스를 정의합니다.
 */

#ifdef	__cplusplus
extern "C" {
#endif

#include <ubinos/bsp.h>
#include <ubinos/bsp_ubik.h>
#include <ubinos/bsp/arch.h>

#ifndef __ASSEMBLY__

void bsp_spurious_exception_handler(void);

#else

#endif /* __ASSEMBLY__ */

#ifdef	__cplusplus
}
#endif

#endif /* _BSP_H_ */



