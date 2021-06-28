
void send_poweron(CC1101_t *cc1101);
void send_poweron_2(CC1101_t *cc1101);
void send_hormann_frame(CC1101_t *cc1101, uint8_t *buf, uint8_t len);
void send_hormann_frame_2(CC1101_t *cc1101, uint8_t *buf, uint8_t len, uint8_t repeat);

void hermann_power_on();
void hermann_send_frame();
