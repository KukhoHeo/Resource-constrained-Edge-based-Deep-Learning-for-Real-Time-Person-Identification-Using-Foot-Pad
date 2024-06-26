/*
 * Copyright (c) 2020 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBIDRV_NVMEM_H_
#define UBIDRV_NVMEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * @file nvmem.h
 *
 * @brief non-volatile memory device driver API
 *
 * 비휘발성 메모리 디바이스 드라이버 API를 정의합니다.
 */

#include <ubinos_config.h>
#include <ubinos/type.h>


/*!
 * 비휘발성 메모리의 한 페이지를 지우는 함수
 *
 * @param   addr        대상 주소
 *
 * @param   size        지울 길이
 *
 * @return  오류 번호
 */
ubi_err_t nvmem_erase(uint8_t *addr, size_t size);

/*!
 * 비휘발성 메모리에 자료를 갱신하는 함수
 *
 * @param   addr        대상 주소
 *
 * @param   buf         자료가 저장되어 있는 버퍼 주소
 *
 * @param   size        자료 길이
 *
 * @return  오류 번호
 */
ubi_err_t nvmem_update(uint8_t *addr, const uint8_t *buf, size_t size);

/*!
 * 비휘발성 메모리에서 자료를 읽는 함수
 *
 * @param   addr        대상 주소
 *
 * @param   buf         읽은 자료를 저장할 버퍼 주소
 *
 * @param   size        자료 길이
 *
 * @return  오류 번호
 */
ubi_err_t nvmem_read(const uint8_t *addr, uint8_t *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* UBIDRV_NVMEM_H_ */

