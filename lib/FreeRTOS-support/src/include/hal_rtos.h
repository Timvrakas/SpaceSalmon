/**
 * \file
 *
 * \brief RTOS HAL API declaration.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
#ifndef _HAL_RTOS_H_INCLUDED
#define _HAL_RTOS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "rtos_port.h"
#include "stdbool.h"
#include "sam.h"

#ifndef is_in_isr
inline bool is_in_isr(void)
{
	return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
}
#endif

/* Semaphore */
#ifdef SEMAPHORE_ENABLED

/**
 * \brief Semaphore max count declaration
 */
#define SEMAPHORE_MAX_COUNT 1

#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
/**
 * \brief Semaphore initialization
 * it used as count Semaphore, The max count
 * value is defined by SEMAPHORE_MAX_COUNT
 *
 * \param[in] sem The pointer to a sem_t instance
 * \param[in] count Initializing semaphore number
 *            it should not exceed the max value which
 *            is defined by SEMAPHORE_MAX_COUNT
 *
 * \return The result of initialization
 * return ERR_NONE for success
 * return <0 for ERR
 */
int32_t sem_init(sem_t *sem, uint32_t count);
#endif

/**
 * Initialize a new semaphore using provided static memory
 * added by timv
 */
sem_handle_t sem_init_static(sem_buf_t *memory, uint32_t count);

/**
 * \brief Semaphore up
 *
 * \param[in] sem The pointer to a sem_t instance
 *
 * \return The result of up operation
 * return ERR_NONE for success
 * return <0 for ERR
 */
int32_t sem_up(sem_t *sem);

/**
 * \brief Semaphore down, may suspend the caller thread
 *
 * \param[in] sem The pointer to a sem_t instance
 * \param[in] timeout Timeout in ticks
 *
 * \return The result of down operation
 * \retval ERR_NONE for success
 * \retval <0 for ERR
 */
int32_t sem_down(sem_t *sem, uint32_t timeout);

/**
 * \brief Semaphore deinitialization
 *
 * \param[in] sem The pointer to a sem_t instance
 *
 * \return The result of deinitialize semaphore
 * \retval ERR_NONE for success
 * \retval <0 for ERR
 */
int32_t sem_deinit(sem_t *sem);

#endif

#ifdef MUTEX_ENABLED

int32_t mutex_init();
mutex_handle_t mutex_init_static(mutex_buf_t *memory);
void mutex_take(mutex_handle_t, uint32_t timeout);
void mutex_give(mutex_handle_t);

#endif

/* os lock */

#if !defined(os_lock) || !defined(os_unlock)
#undef os_lock
#undef os_unlock

//#include "hal_atomic.h" //this created circular dependency issues

static volatile hal_atomic_t interrupt_flags;
static int                   lock_nesting;

#define os_lock()                                                                                                      \
	do {                                                                                                               \
		if (lock_nesting == 0) {                                                                                       \
			atomic_enter_critical(&interrupt_flags);                                                                   \
		}                                                                                                              \
                                                                                                                       \
		lock_nesting++;                                                                                                \
	} while (0)

#define os_unlock()                                                                                                    \
	do {                                                                                                               \
		lock_nesting--;                                                                                                \
                                                                                                                       \
		if (lock_nesting == 0) {                                                                                       \
			atomic_leave_critical(&interrupt_flags);                                                                   \
		}                                                                                                              \
	} while (0)

#endif

/* os sleep */
#if !defined(os_sleep)
#warning "No sleep method is provided by RTOS"
#endif
#ifdef __cplusplus
}
#endif

#endif
