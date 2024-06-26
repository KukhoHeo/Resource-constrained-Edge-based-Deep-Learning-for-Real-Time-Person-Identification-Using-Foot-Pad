if(INCLUDE__STM32CUBEF2)

	get_filename_component(_tmp_source_dir "${STM32CUBEF2__BASE_DIR}" ABSOLUTE)

	include_directories(${_tmp_source_dir}/Drivers/CMSIS/Include)
	include_directories(${_tmp_source_dir}/Drivers/CMSIS/Device/ST/STM32F2xx/Include)
	include_directories(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Inc)
	include_directories(${_tmp_source_dir}/Drivers/BSP/Components)
	include_directories(${_tmp_source_dir}/Drivers/BSP/Components/Common)
	include_directories(${_tmp_source_dir}/Utilities)
	include_directories(${_tmp_source_dir}/Utilities/Fonts)
	include_directories(${_tmp_source_dir}/Utilities/Log)

	if(STM32CUBEF2__USE_HAL_DRIVER)
	
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_adc.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_adc_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_can.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cortex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_crc.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cryp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dac.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dac_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dcmi.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma.c)
		set_source_files_properties(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma.c PROPERTIES COMPILE_FLAGS -O3)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma_ex.c)
		set_source_files_properties(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma_ex.c PROPERTIES COMPILE_FLAGS -O3)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_eth.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_exti.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_gpio.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_i2c.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_i2s.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_iwdg.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_nand.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_nor.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pwr.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pwr_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rng.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_sd.c)
		set_source_files_properties(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_sd.c PROPERTIES COMPILE_FLAGS -O3)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_spi.c)
		set_source_files_properties(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_spi.c PROPERTIES COMPILE_FLAGS -O0)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rtc.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rtc_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_sram.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim_ex.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_uart.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_usart.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_wwdg.c)
		
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_fsmc.c)
		set_source_files_properties(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_fsmc.c PROPERTIES COMPILE_FLAGS -O3)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_sdmmc.c)
		set_source_files_properties(${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_sdmmc.c PROPERTIES COMPILE_FLAGS -O3)
		
		if(UBINOS__BSP__BOARD_MODEL STREQUAL "STM3221GEVAL")
		
			include_directories(${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL)
			
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval.c)
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_io.c)
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_lcd.c)
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_sd.c)
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_ts.c)
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_camera.c)
			
		elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "NUCLEOF207ZG")
		
			include_directories(${_tmp_source_dir}/Drivers/BSP/STM32F2xx_Nucleo_144)
			
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM32F2xx_Nucleo_144/stm32f2xx_nucleo_144.c)
			
		else()
		
			message(FATAL_ERROR "Unsupported UBINOS__BSP__BOARD_MODEL")
			
		endif()
	
	endif(STM32CUBEF2__USE_HAL_DRIVER)
	
	if(STM32CUBEF2__USE_FULL_LL_DRIVER)
	
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_ll_utils.c)
	
	endif(STM32CUBEF2__USE_FULL_LL_DRIVER)

	if(STM32CUBEF2__USE_FREERTOS)
	
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/include)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3)
	
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/list.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/queue.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/timers.c)
	
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c)
	
	endif(STM32CUBEF2__USE_FREERTOS)

	if(STM32CUBEF2__USE_LWIP)
	
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/include)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/LwIP/system)

		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/def.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/dns.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/inet_chksum.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/init.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/ip.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/mem.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/memp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/netif.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/pbuf.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/raw.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/stats.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/sys.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/tcp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/tcp_in.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/tcp_out.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/timeouts.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/core/udp.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/api_lib.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/api_msg.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/err.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/netbuf.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/netdb.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/netifapi.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/sockets.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/api/tcpip.c)

		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/system/OS/sys_arch.c)

		if(STM32CUBEF2__USE_LWIP_ETHERNET)
		
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/netif/ethernet.c)
		
		endif(STM32CUBEF2__USE_LWIP_ETHERNET)

		if(STM32CUBEF2__USE_LWIP_HTTP)
		
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/apps/httpd/fs.c)
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/LwIP/src/apps/httpd/httpd.c)
		
		endif(STM32CUBEF2__USE_LWIP_HTTP)
	
	endif(STM32CUBEF2__USE_LWIP)

	if(STM32CUBEF2__USE_LCD)
	
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/Components/ili9320/ili9320.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/Components/ili9325/ili9325.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/Components/ov2640/ov2640.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/Components/stmpe811/stmpe811.c)

		if(UBINOS__BSP__BOARD_MODEL STREQUAL "STM3221GEVAL")
		
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_lcd.c)
		
		else()
		
			message(FATAL_ERROR "Unsupported UBINOS__BSP__BOARD_MODEL")
		
		endif()


		if(STM32CUBEF2__USE_LCD_LOG)

			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Utilities/Log/lcd_log.c)
		
		endif(STM32CUBEF2__USE_LCD_LOG)
	
	endif(STM32CUBEF2__USE_LCD)

	if(STM32CUBEF2__USE_SRAM)
		
		if(UBINOS__BSP__BOARD_MODEL STREQUAL "STM3221GEVAL")
		
			set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Drivers/BSP/STM322xG_EVAL/stm322xg_eval_sram.c)
		
		else()
		
			message(FATAL_ERROR "Unsupported UBINOS__BSP__BOARD_MODEL")
		
		endif()
		
	endif(STM32CUBEF2__USE_SRAM)

	if(STM32CUBEF2__USE_FATFS)
		
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FatFs/src)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FatFs/src/drivers)
		
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FatFs/src/diskio.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FatFs/src/ff.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c)
		
	endif(STM32CUBEF2__USE_FATFS)

	if(STM32CUBEF2__USE_FREERTOS)
	
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS)
		include_directories(${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/include)
		
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/list.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/queue.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/timers.c)
		
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c)
		set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c)
	
	endif(STM32CUBEF2__USE_FREERTOS)

	if(INCLUDE__UBINOS__UBIDRV)

		if(UBINOS__UBIDRV__INCLUDE_NVMEM)

		endif(UBINOS__UBIDRV__INCLUDE_NVMEM)

	endif(INCLUDE__UBINOS__UBIDRV)

endif(INCLUDE__STM32CUBEF2)

