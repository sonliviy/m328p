#ifndef NRF24_H
#define NRF24_H
#include <avr/io.h>
#include <stdio.h>


//#ifndef NRF_PORT
//#error "NOT DEFINED PORT FOR CE"
//#endif

#define NRF_DDR DDRC
#define NRF_PORT PORTC
#define NRF_CE 0

/**
 * @brief Инициализация GPIO и регистров NRF, очистка внутренних буферов
 *  Дефолтные настройки: канал 76, скорость 2M, усиление 0dbm
 *  Динамический размер пакета включен, контрольная сумма включена
 * @return Флаг прогресса
 */
uint8_t nrf24_init(void);
uint8_t nrf24_chipcheckout(void);
void    nrf24_setup_rf(uint8_t bitrate, uint8_t dbm);
void    nrf24_setup_retr(uint8_t retr);
void    nrf24_setup_ch(uint8_t ch);
uint8_t nrf24_powerup(void);
/**
 * @brief Создает прослушивающий пайп
 * @param n Номер пайпа
 * @param addr Адрес пайпа
 * @param size Максимальный размер передаваемых данных за раз, байт
 * @param en_aa Включение подтверждения приема (режим AA)
 * @param dynpd Включение динамического размера передаваемых данных (только для AA режима)
 */
void nrf24_createpipe(uint8_t n, uint8_t *addr, uint8_t size, uint8_t en_aa, uint8_t dynpd);
/**
 * @brief nrf24_openpipe
 * @param n
 */
void nrf24_disablepipe(uint8_t n);
void nrf24_enablepipe(uint8_t n);
uint8_t nrf24_rxcheckout(void);
/**
 * @brief Копирует данные из верхнего доступного буфера
 * @param pbuffer
 * @return Длина скопированных данных, 0 - если данных нет
 */
uint8_t nrf24_receive(uint8_t * pbuffer);
void nrf24_startlistening(void);
void nrf24_stoplistening(void);
/**
 * @brief nrf24_transmit
 * @param addr
 * @param payload
 * @param size
 * @param en_aa
 * @return Флаг результата передачи
 */
uint8_t nrf24_transmit(uint8_t *addr, uint8_t *payload, uint8_t size, uint8_t en_aa);
#endif // NRF24_H
