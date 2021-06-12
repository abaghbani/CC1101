#ifndef TIMER_DRIVER_H__
#define TIMER_DRIVER_H__

void timer_init(nrfx_timer_t *TIMER, uint32_t time_us);
void timer_send_duration(bool state, uint16_t n_tick);
void timer_send_data(uint8_t *data, uint16_t len);

#endif // TIMER_DRIVER_H__
